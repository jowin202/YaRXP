#include "eventdslserializer.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QRegularExpression>
#include <QStringList>
#include <QMap>

// ============================================================
//  Helpers
// ============================================================

QString EventDslSerializer::dslEscape(const QString &s)
{
    QString r;
    r.reserve(s.size() + 8);
    for (QChar c : s) {
        if      (c == '\\') r += "\\\\";
        else if (c == '"')  r += "\\\"";
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\t') r += "\\t";
        else                r += c;
    }
    return r;
}

QString EventDslSerializer::dslUnescape(const QString &s)
{
    QString r;
    r.reserve(s.size());
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            QChar n = s[++i];
            if      (n == 'n')  r += '\n';
            else if (n == 'r')  r += '\r';
            else if (n == 't')  r += '\t';
            else if (n == '"')  r += '"';
            else if (n == '\\') r += '\\';
            else { r += '\\'; r += n; }
        } else {
            r += s[i];
        }
    }
    return r;
}

static QString esc(const QString &s) { return EventDslSerializer::dslEscape(s); }

static QString audioStr(const QJsonObject &af)
{
    return "\"" + esc(af["@name"].toString()) + "\", "
           + QString::number(af["@volume"].toInt()) + ", "
           + QString::number(af["@pitch"].toInt());
}

static QString toneStr(const QJsonObject &t)
{
    return "r=" + QString::number(t["r"].toInt())
         + ", g=" + QString::number(t["g"].toInt())
         + ", b=" + QString::number(t["b"].toInt())
         + ", gray=" + QString::number(t["alpha_gray"].toInt());
}

static const char *const DIRS6[] = {"?","?","down","left","right","up","?","?","up"};
static const char *const BLEND3[] = {"normal","add","sub"};

static QString dirName(int d)
{
    return (d >= 0 && d <= 8) ? DIRS6[d] : QString::number(d);
}

static QString whoStr(int id)
{
    if (id == -1) return "player";
    if (id ==  0) return "this";
    return "event(" + QString::number(id) + ")";
}

// Conditional-branch condition → DSL string
static QString serializeCondition(const QJsonArray &p)
{
    static const char *const OPS[] = {"==",">=","<=",">","<","!="};
    int type = p[0].toInt();
    switch (type) {
    case 0: // switch
        return QString("switch(%1) == %2").arg(p[1].toInt()).arg(p[2].toInt()==0?"ON":"OFF");
    case 1: { // variable
        int opIdx = p.size() > 4 ? p[4].toInt() : 0;
        QString op  = opIdx < 6 ? OPS[opIdx] : "==";
        QString rhs = p[2].toInt()==0
            ? QString::number(p[3].toInt())
            : "variable(" + QString::number(p[3].toInt()) + ")";
        return "variable(" + QString::number(p[1].toInt()) + ") " + op + " " + rhs;
    }
    case 2: // self switch
        return QString("self_switch(%1) == %2").arg(p[1].toString()).arg(p[2].toInt()==0?"ON":"OFF");
    case 3: { // timer
        QString op = p[2].toInt()==1 ? "<=" : ">=";
        return "timer " + op + " " + QString::number(p[1].toInt()) + "sec";
    }
    case 4: { // actor
        QString a = "actor(" + QString::number(p[1].toInt()) + ")";
        int sub = p[2].toInt();
        if (sub==0) return a + " in_party";
        if (sub==1) return a + " name == \"" + esc(p[3].toString()) + "\"";
        if (sub==2) return a + " learned(" + QString::number(p[3].toInt()) + ")";
        if (sub==3) return a + " weapon(" + QString::number(p[3].toInt()) + ")";
        if (sub==4) return a + " armor(" + QString::number(p[3].toInt()) + ")";
        if (sub==5) return a + " state(" + QString::number(p[3].toInt()) + ")";
        return a + " ?";
    }
    case 5: { // enemy
        QString e = "enemy(" + QString::number(p[1].toInt()+1) + ")";
        return p[2].toInt()==0 ? e+" appeared" : e+" state("+QString::number(p[3].toInt())+")";
    }
    case 6: // character facing
        return "character(" + whoStr(p[1].toInt()) + ") facing(" + dirName(p[2].toInt()) + ")";
    case 7:
        return "gold " + QString(p[2].toInt()==1?"<=":">=") + " " + QString::number(p[1].toInt());
    case 8:  return "item("   + QString::number(p[1].toInt()) + ") in_inventory";
    case 9:  return "weapon(" + QString::number(p[1].toInt()) + ") in_inventory";
    case 10: return "armor("  + QString::number(p[1].toInt()) + ") in_inventory";
    case 11: return "button(" + QString::number(p[1].toInt()) + ") pressed";
    case 12: return "script(\"" + esc(p[1].toString()) + "\")";
    default: return "?";
    }
}

static QString varRhs(const QJsonArray &p, int srcIdx, int val1Idx, int val2Idx)
{
    static const char *const STATS[] =
        {"Level","EXP","HP","SP","MaxHP","MaxSP","STR","DEX","AGI","INT","ATK","PDEF","MDEF","EVA"};
    static const char *const MAPV[] =
        {"x","y","dir","screen_x","screen_y","terrain_tag"};
    static const char *const OTHER[] =
        {"map_id","party_size","gold","steps","play_time","timer","save_count"};
    int src = p[srcIdx].toInt();
    int v1  = p.size() > val1Idx ? p[val1Idx].toInt() : 0;
    int v2  = p.size() > val2Idx ? p[val2Idx].toInt() : 0;
    switch (src) {
    case 0: return QString::number(v1);
    case 1: return "variable(" + QString::number(v1) + ")";
    case 2: return "random(" + QString::number(v1) + ", " + QString::number(v2) + ")";
    case 3: return "item_count(" + QString::number(v1) + ")";
    case 4: return "actor_stat(" + QString::number(v1) + ", " + (v2<14?STATS[v2]:"?") + ")";
    case 5: return "enemy_stat(" + QString::number(v1+1) + ", " + (v2+2<6?STATS[v2+2]:"?") + ")";
    case 6: return "char_prop(" + whoStr(v1) + ", " + (v2<6?MAPV[v2]:"?") + ")";
    case 7: return "other(" + (v1<7?OTHER[v1]:QString::number(v1)) + ")";
    default: return "?";
    }
}

static QString rawLine(const QString &pfx, int code, const QJsonArray &p)
{
    return pfx + "raw(" + QString::number(code) + ", "
           + QString::fromUtf8(QJsonDocument(p).toJson(QJsonDocument::Compact)) + ")\n";
}

// ============================================================
//  toScript – serializer
// ============================================================

QString EventDslSerializer::toScript(const QJsonArray &commands)
{
    static const char *const STAT_OPS[] = {"=","+=","-=","*=","/=","%="};
    static const char *const MOVE_NAMES[] = {
        "end","move_down","move_left","move_right","move_up",
        "move_lower_left","move_lower_right","move_upper_left","move_upper_right",
        "move_random","move_toward_player","move_away_from_player",
        "step_forward","step_backward","jump","wait",
        "turn_down","turn_left","turn_right","turn_up",
        "turn_90_right","turn_90_left","turn_180","turn_90_random","turn_random",
        "turn_toward_player","turn_away_from_player",
        "switch_on","switch_off","change_speed","change_freq",
        "move_anim_on","move_anim_off","stop_anim_on","stop_anim_off",
        "dir_fix_on","dir_fix_off","through_on","through_off",
        "always_top_on","always_top_off",
        "change_graphic","change_opacity","change_blend","play_se","script"
    };

    QString out;
    out.reserve(commands.size() * 48);
    int i = 0;
    while (i < commands.size()) {
        QJsonObject cmd = commands[i].toObject();
        int code        = cmd["@code"].toInt();
        int ind         = cmd["@indent"].toInt();
        QJsonArray p    = cmd["@parameters"].toArray();
        QString pfx     = QString(ind * 4, ' ');

        switch (code) {

        // ── end marker ──────────────────────────────────────
        case 0: i++; break;

        // ── Text (101 + 401 continuations) ──────────────────
        case 101: {
            QStringList lines;
            lines += p[0].toString();
            i++;
            while (i < commands.size() && commands[i].toObject()["@code"].toInt() == 401) {
                lines += commands[i].toObject()["@parameters"].toArray()[0].toString();
                i++;
            }
            if (lines.size() == 1) {
                out += pfx + "show_text(\"" + esc(lines[0]) + "\")\n";
            } else {
                out += pfx + "show_text:\n";
                for (const QString &l : lines)
                    out += pfx + "    " + l + "\n";
                out += pfx + "end_show_text\n";
            }
            break;
        }
        case 401: i++; break; // orphaned continuation

        // ── Comment (108 + 408) ─────────────────────────────
        case 108: {
            QStringList lines;
            lines += p[0].toString();
            i++;
            while (i < commands.size() && commands[i].toObject()["@code"].toInt() == 408) {
                lines += commands[i].toObject()["@parameters"].toArray()[0].toString();
                i++;
            }
            for (const QString &l : lines)
                out += pfx + "# " + l + "\n";
            break;
        }
        case 408: i++; break;

        // ── Script (355 + 655) ──────────────────────────────
        case 355: {
            QStringList lines;
            lines += p[0].toString();
            i++;
            while (i < commands.size() && commands[i].toObject()["@code"].toInt() == 655) {
                lines += commands[i].toObject()["@parameters"].toArray()[0].toString();
                i++;
            }
            out += pfx + "script:\n";
            for (const QString &l : lines)
                out += pfx + "    " + l + "\n";
            out += pfx + "end_script\n";
            break;
        }
        case 655: i++; break;

        // ── Wait ────────────────────────────────────────────
        case 106: out += pfx + "wait(" + QString::number(p[0].toInt()) + ")\n"; i++; break;

        // ── Conditional branch ──────────────────────────────
        case 111: out += pfx + "if " + serializeCondition(p) + ":\n"; i++; break;
        case 411: out += pfx + "else:\n";  i++; break;
        case 412: out += pfx + "endif\n"; i++; break;

        // ── Loop ────────────────────────────────────────────
        case 112: out += pfx + "loop:\n";     i++; break;
        case 113: out += pfx + "break\n";     i++; break;
        case 413: out += pfx + "end_loop\n";  i++; break;

        // ── Flow ────────────────────────────────────────────
        case 115: out += pfx + "exit_event\n";  i++; break;
        case 116: out += pfx + "erase_event\n"; i++; break;
        case 117: out += pfx + "call_common_event(" + QString::number(p[0].toInt()) + ")\n"; i++; break;
        case 118: out += pfx + "label(\"" + esc(p[0].toString()) + "\")\n";    i++; break;
        case 119: out += pfx + "jump(\"" + esc(p[0].toString()) + "\")\n";     i++; break;

        // ── Switches / Variables ─────────────────────────────
        case 121: {
            QString range = p[0].toInt() == p[1].toInt()
                ? QString::number(p[0].toInt())
                : QString("%1..%2").arg(p[0].toInt()).arg(p[1].toInt());
            out += pfx + "switch(" + range + ") = " + (p[2].toInt()==0?"ON":"OFF") + "\n";
            i++; break;
        }
        case 122: {
            QString range = p[0].toInt() == p[1].toInt()
                ? QString::number(p[0].toInt())
                : QString("%1..%2").arg(p[0].toInt()).arg(p[1].toInt());
            int opIdx = p[2].toInt();
            QString op = opIdx < 6 ? STAT_OPS[opIdx] : "=";
            out += pfx + "variable(" + range + ") " + op + " " + varRhs(p,3,4,5) + "\n";
            i++; break;
        }
        case 123:
            out += pfx + "self_switch(" + p[0].toString() + ") = " + (p[1].toInt()==0?"ON":"OFF") + "\n";
            i++; break;
        case 124:
            if (p[0].toInt()==1)
                out += pfx + "timer(stop)\n";
            else
                out += pfx + "timer(start, " + QString::number(p[1].toInt()) + ")\n";
            i++; break;

        // ── Gold / Items ─────────────────────────────────────
        case 125: {
            QString op  = p[0].toInt()==0 ? "+=" : "-=";
            QString rhs = p[1].toInt()==0 ? QString::number(p[2].toInt()) : "variable("+QString::number(p[2].toInt())+")";
            out += pfx + "gold " + op + " " + rhs + "\n"; i++; break;
        }
        case 126: case 127: case 128: {
            static const char *const IT[] = {"item","weapon","armor"};
            QString op  = p[1].toInt()==0 ? "+=" : "-=";
            QString rhs = p[2].toInt()==0 ? QString::number(p[3].toInt()) : "variable("+QString::number(p[3].toInt())+")";
            out += pfx + IT[code-126] + "(" + QString::number(p[0].toInt()) + ") " + op + " " + rhs + "\n";
            i++; break;
        }
        case 129:
            if (p[1].toInt()==0)
                out += pfx + "party_add(" + QString::number(p[0].toInt()) + (p[2].toInt()==1?", initialize":"") + ")\n";
            else
                out += pfx + "party_remove(" + QString::number(p[0].toInt()) + ")\n";
            i++; break;

        // ── Scene control ────────────────────────────────────
        case 131: out += pfx + "windowskin(\"" + esc(p[0].toString()) + "\")\n"; i++; break;
        case 132: case 133:
            out += pfx + (code==132?"battle_bgm":"battle_me")
                   + "(" + audioStr(p[0].toObject()) + ")\n"; i++; break;
        case 134: out += pfx + "save_access(" + (p[0].toInt()==0?"enable":"disable") + ")\n"; i++; break;
        case 135: out += pfx + "menu_access(" + (p[0].toInt()==0?"enable":"disable") + ")\n"; i++; break;
        case 136: out += pfx + "encounter(" +   (p[0].toInt()==0?"enable":"disable") + ")\n"; i++; break;

        // ── Transfer ─────────────────────────────────────────
        case 201: {
            bool useVar = p[0].toInt()!=0;
            auto coord = [&](int idx) -> QString {
                return useVar ? "variable("+QString::number(p[idx].toInt())+")" : QString::number(p[idx].toInt());
            };
            out += pfx + "transfer(map=" + (useVar?"variable("+QString::number(p[1].toInt())+")":QString::number(p[1].toInt()))
                   + ", x=" + coord(2) + ", y=" + coord(3);
            if (p[4].toInt()!=0) out += ", dir=" + dirName(p[4].toInt());
            if (p[5].toInt()==1) out += ", no_fade";
            out += ")\n"; i++; break;
        }

        // ── Map effects ──────────────────────────────────────
        case 203: out += pfx + "scroll_map(" + dirName(p[0].toInt()) + ", " + QString::number(p[1].toInt()) + ", " + QString::number(p[2].toInt()) + ")\n"; i++; break;
        case 207: out += pfx + "show_animation(" + whoStr(p[0].toInt()) + ", anim=" + QString::number(p[1].toInt()) + ")\n"; i++; break;
        case 208: out += pfx + "transparent(" + (p[0].toInt()==0?"on":"off") + ")\n"; i++; break;
        case 210: out += pfx + "wait_for_move\n"; i++; break;

        // ── Move route (209 + 509 decorators) ───────────────
        case 209: {
            QJsonObject mr = p[1].toObject();
            QString opts;
            if (mr["@repeat"].toBool())    opts += ", repeat";
            if (mr["@skippable"].toBool()) opts += ", skip";
            out += pfx + "move_route(" + whoStr(p[0].toInt()) + opts + "):\n";
            i++;
            while (i < commands.size() && commands[i].toObject()["@code"].toInt() == 509) {
                QJsonObject mc  = commands[i].toObject()["@parameters"].toArray()[0].toObject();
                int mc_code     = mc["@code"].toInt();
                QJsonArray mc_p = mc["@parameters"].toArray();
                QString mline   = pfx + "    ";
                if (mc_code >= 0 && mc_code <= 45) {
                    mline += MOVE_NAMES[mc_code];
                    if (mc_code == 14)
                        mline += "(" + QString::number(mc_p[0].toInt()) + ", " + QString::number(mc_p[1].toInt()) + ")";
                    else if (mc_code==15||mc_code==29||mc_code==30||mc_code==42)
                        mline += "(" + QString::number(mc_p[0].toInt()) + ")";
                    else if (mc_code==27||mc_code==28)
                        mline += "(" + QString::number(mc_p[0].toInt()) + ")";
                    else if (mc_code==41)
                        mline += "(\""+esc(mc_p[0].toString())+"\", "+QString::number(mc_p[1].toInt())+", "+QString::number(mc_p[2].toInt())+", "+QString::number(mc_p[3].toInt())+")";
                    else if (mc_code==43)
                        mline += "(" + QString(mc_p[0].toInt()<3?BLEND3[mc_p[0].toInt()]:"?") + ")";
                    else if (mc_code==44)
                        mline += "(" + audioStr(mc_p[0].toObject()) + ")";
                    else if (mc_code==45)
                        mline += "(\"" + esc(mc_p[0].toString()) + "\")";
                } else {
                    mline += "raw_move(" + QString::number(mc_code) + ")";
                }
                out += mline + "\n";
                i++;
            }
            out += pfx + "end_move_route\n";
            break;
        }
        case 509: i++; break;

        // ── Transitions / Screen ─────────────────────────────
        case 221: out += pfx + "prepare_transition\n"; i++; break;
        case 222: out += pfx + "transition(\"" + esc(p[0].toString()) + "\")\n"; i++; break;
        case 223: case 205: {
            out += pfx + (code==223?"screen_tone":"fog_tone")
                   + "(" + toneStr(p[0].toObject()) + ", dur=" + QString::number(p[1].toInt()) + ")\n";
            i++; break;
        }
        case 224: {
            QJsonObject c = p[0].toObject();
            out += pfx + "screen_flash(r="+QString::number(c["r"].toInt())+", g="+QString::number(c["g"].toInt())
                   +", b="+QString::number(c["b"].toInt())+", alpha="+QString::number(c["alpha_gray"].toInt())
                   +", dur="+QString::number(p[1].toInt())+")\n";
            i++; break;
        }
        case 225: out += pfx + "screen_shake(power="+QString::number(p[0].toInt())+", speed="+QString::number(p[1].toInt())+", dur="+QString::number(p[2].toInt())+")\n"; i++; break;

        // ── Pictures ─────────────────────────────────────────
        case 233: out += pfx + "rotate_picture("+QString::number(p[0].toInt())+", "+QString::number(p[1].toInt())+")\n"; i++; break;
        case 234: {
            QJsonObject t = p[1].toObject();
            out += pfx + "picture_tone("+QString::number(p[0].toInt())+", "+toneStr(t)+", dur="+QString::number(p[2].toInt())+")\n";
            i++; break;
        }
        case 235: out += pfx + "erase_picture("+QString::number(p[0].toInt())+")\n"; i++; break;
        case 206: out += pfx + "fog_opacity("+QString::number(p[0].toInt())+", dur="+QString::number(p[1].toInt())+")\n"; i++; break;

        // ── Weather ──────────────────────────────────────────
        case 236: {
            static const char *const WX[] = {"none","rain","storm","snow"};
            int w = p[0].toInt();
            out += pfx + "weather(" + (w<4?WX[w]:"none");
            if (w!=0) out += ", power="+QString::number(p[1].toInt());
            out += ", dur="+QString::number(p[2].toInt())+")\n";
            i++; break;
        }

        // ── Audio ────────────────────────────────────────────
        case 241: out += pfx + "play_bgm("+audioStr(p[0].toObject())+")\n"; i++; break;
        case 242: out += pfx + "fade_bgm("+QString::number(p[0].toInt())+")\n"; i++; break;
        case 245: out += pfx + "play_bgs("+audioStr(p[0].toObject())+")\n"; i++; break;
        case 246: out += pfx + "fade_bgs("+QString::number(p[0].toInt())+")\n"; i++; break;
        case 247: out += pfx + "memorize_bgm_bgs\n"; i++; break;
        case 248: out += pfx + "restore_bgm_bgs\n"; i++; break;
        case 249: out += pfx + "play_me("+audioStr(p[0].toObject())+")\n"; i++; break;
        case 250: out += pfx + "play_se("+audioStr(p[0].toObject())+")\n"; i++; break;
        case 251: out += pfx + "stop_se\n"; i++; break;

        // ── Show choices (102/402/403/404) ───────────────────
        case 102: {
            QJsonArray ch = p[0].toArray();
            out += pfx + "show_choices(";
            for (int j = 0; j < ch.size(); j++) {
                if (j) out += ", ";
                out += "\"" + esc(ch[j].toString()) + "\"";
            }
            out += ", cancel=" + QString::number(p[1].toInt()) + "):\n";
            i++; break;
        }
        case 402: out += pfx + "when(\"" + esc(p[1].toString()) + "\"):\n"; i++; break;
        case 403: out += pfx + "when_cancel:\n"; i++; break;
        case 404: out += pfx + "end_choices\n"; i++; break;

        // ── Battle processing (301/601/602/603/604) ──────────
        case 301: {
            out += pfx + "battle(troop="+QString::number(p[0].toInt());
            if (p[1].toBool()) out += ", escape";
            if (p[2].toBool()) out += ", lose";
            out += "):\n"; i++; break;
        }
        case 601: out += pfx + "win:\n";       i++; break;
        case 602: out += pfx + "escape:\n";    i++; break;
        case 603: out += pfx + "lose:\n";      i++; break;
        case 604: out += pfx + "end_battle\n"; i++; break;

        // ── Shop (302 + 605) ─────────────────────────────────
        case 302: {
            static const char *const IT[] = {"item","weapon","armor"};
            out += pfx + "shop:\n";
            auto shopLine = [&](const QJsonArray &sp) {
                out += pfx + "    " + IT[qBound(0,sp[0].toInt(),2)] + "(" + QString::number(sp[1].toInt());
                if (sp[2].toInt()==1) out += ", price="+QString::number(sp[3].toInt());
                out += ")\n";
            };
            shopLine(p);
            i++;
            while (i < commands.size() && commands[i].toObject()["@code"].toInt() == 605) {
                shopLine(commands[i].toObject()["@parameters"].toArray());
                i++;
            }
            out += pfx + "end_shop\n";
            break;
        }
        case 605: i++; break;

        // ── Actor stats ──────────────────────────────────────
        case 311: case 312: case 315: case 316: {
            QString fn = (code==311?"change_hp":(code==312?"change_sp":(code==315?"change_exp":"change_level")));
            QString who = p[0].toInt()==0 ? "party" : "actor("+QString::number(p[0].toInt())+")";
            QString op  = p[1].toInt()==0 ? "+" : "-";
            QString rhs = p[2].toInt()==0 ? QString::number(p[3].toInt()) : "variable("+QString::number(p[3].toInt())+")";
            out += pfx + fn + "(" + who + ", " + op + rhs + ")\n"; i++; break;
        }
        case 314: {
            QString who = p[0].toInt()==0 ? "party" : "actor("+QString::number(p[0].toInt())+")";
            out += pfx + "recover_all(" + who + ")\n"; i++; break;
        }
        case 334: {
            QString who = p[0].toInt()==-1 ? "troop" : "enemy("+QString::number(p[0].toInt()+1)+")";
            out += pfx + "enemy_recover(" + who + ")\n"; i++; break;
        }

        // ── Change actor name ────────────────────────────────
        case 320:
            out += pfx + "change_actor_name(actor(" + QString::number(p[0].toInt())
                   + "), \"" + esc(p[1].toString()) + "\")\n";
            i++; break;

        // ── Force action ─────────────────────────────────────
        case 339: {
            QString subject = p[0].toInt()==0
                ? "enemy(" + QString::number(p[1].toInt()) + ")"
                : "actor(" + QString::number(p[1].toInt()) + ")";
            QString action = p[2].toInt()==0
                ? "basic(" + QString::number(p[3].toInt()) + ")"
                : "skill(" + QString::number(p[3].toInt()) + ")";
            QString seq = p[5].toInt()==1 ? "now" : "normal";
            out += pfx + "force_action(" + subject + ", " + action
                   + ", target=" + QString::number(p[4].toInt()) + ", " + seq + ")\n";
            i++; break;
        }

        // ── Simple no-arg battle commands ────────────────────
        case 340: out += pfx + "abort_battle\n";     i++; break;
        case 351: out += pfx + "call_menu\n";        i++; break;
        case 352: out += pfx + "call_save\n";        i++; break;
        case 353: out += pfx + "game_over\n";        i++; break;
        case 354: out += pfx + "return_to_title\n";  i++; break;

        // ── Misc ─────────────────────────────────────────────
        case 103:
            out += pfx + "input_number(var="+QString::number(p[0].toInt())+", digits="+QString::number(p[1].toInt())+")\n";
            i++; break;
        case 104: {
            static const char *const POS[] = {"top","middle","bottom"};
            int pos2 = qBound(0,p[0].toInt(),2);
            out += pfx + "text_options(pos="+QString(POS[pos2])
                   +", window="+(p[1].toInt()==0?"show":"hide")+")\n";
            i++; break;
        }
        case 105: out += pfx + "button_input(var="+QString::number(p[0].toInt())+")\n"; i++; break;

        // ── Fallback: raw ─────────────────────────────────────
        default: out += rawLine(pfx, code, p); i++; break;
        }
    }
    return out;
}

// ============================================================
//  Parser helpers
// ============================================================

static int lineIndent(const QString &line)
{
    int n = 0;
    for (QChar c : line) { if (c==' ') n++; else break; }
    return n;
}

// Extract a quoted string starting at pos (pointing to '"'), advance pos
static QString extractQuoted(const QString &s, int &pos)
{
    if (pos >= s.size() || s[pos] != '"') return QString();
    pos++; // skip opening "
    QString r;
    while (pos < s.size() && s[pos] != '"') {
        if (s[pos]=='\\' && pos+1<s.size()) {
            QChar n = s[++pos];
            if      (n=='n')  r+='\n';
            else if (n=='r')  r+='\r';
            else if (n=='t')  r+='\t';
            else if (n=='"')  r+='"';
            else if (n=='\\') r+='\\';
            else { r+='\\'; r+=n; }
        } else {
            r += s[pos];
        }
        pos++;
    }
    if (pos < s.size()) pos++; // skip closing "
    return r;
}

// Simple tokenizer: split "a, b, key=value" respecting quoted strings
struct Token {
    QString key;   // empty = positional
    QString value; // raw (without quotes stripped for strings)
    bool isString = false;
    QString strVal;
    int    intVal  = 0;
    bool   boolVal = false;
};

static QList<Token> tokenizeArgs(const QString &s)
{
    QList<Token> result;
    int pos = 0;
    auto skip = [&]() { while (pos<s.size() && s[pos].isSpace()) pos++; };

    while (pos < s.size()) {
        skip();
        if (pos >= s.size()) break;

        Token tok;
        // Check if it starts with a key= pattern
        // find possible '=' before ',' or '"'
        {
            int p2 = pos;
            while (p2 < s.size() && s[p2] != ',' && s[p2] != '=' && s[p2] != '"') p2++;
            if (p2 < s.size() && s[p2] == '=') {
                tok.key = s.mid(pos, p2-pos).trimmed();
                pos = p2+1;
                skip();
            }
        }

        if (pos < s.size() && s[pos] == '"') {
            tok.strVal  = extractQuoted(s, pos);
            tok.isString = true;
            tok.value    = tok.strVal;
        } else {
            int start = pos;
            while (pos < s.size() && s[pos] != ',') pos++;
            tok.value = s.mid(start, pos-start).trimmed();
            bool ok;
            tok.intVal = tok.value.toInt(&ok);
            if (!ok) {
                if (tok.value=="true")  { tok.boolVal=true; }
                else if (tok.value=="false") { tok.boolVal=false; }
            }
            tok.strVal = tok.value;
        }
        result.append(tok);
        skip();
        if (pos < s.size() && s[pos] == ',') pos++;
    }
    return result;
}

// Helper: get string between first '(' and last ')'
static QString innerParens(const QString &line)
{
    int a = line.indexOf('(');
    int b = line.lastIndexOf(')');
    if (a<0 || b<=a) return QString();
    return line.mid(a+1, b-a-1).trimmed();
}

static QJsonObject makeCmd(int code, int indent, const QJsonArray &params)
{
    QJsonObject o;
    o["@code"]       = code;
    o["@indent"]     = indent;
    o["@parameters"] = params;
    o["RXClass"]     = "RPG::EventCommand";
    return o;
}

static QJsonObject makeMoveCmd(int code, const QJsonArray &params)
{
    QJsonObject o;
    o["@code"]       = code;
    o["@parameters"] = params;
    o["RXClass"]     = "RPG::MoveCommand";
    return o;
}

static QJsonObject makeAudioFile(const QString &name, int vol, int pitch)
{
    QJsonObject o;
    o["@name"]   = name;
    o["@volume"] = vol;
    o["@pitch"]  = pitch;
    o["RXClass"] = "RPG::AudioFile";
    return o;
}

// Parse "r=N, g=N, b=N, gray=N" from a tone string (already inside parens)
static QJsonObject parseTone(const QList<Token> &toks, int startIdx)
{
    QJsonObject t;
    t["r"]=0; t["g"]=0; t["b"]=0; t["alpha_gray"]=0;
    t["RXClass"]="RPG::Color"; // actually Tone in real RMXP, but stored as color-like
    for (int i = startIdx; i < toks.size(); i++) {
        if (toks[i].key=="r")    t["r"]=toks[i].intVal;
        if (toks[i].key=="g")    t["g"]=toks[i].intVal;
        if (toks[i].key=="b")    t["b"]=toks[i].intVal;
        if (toks[i].key=="gray") t["alpha_gray"]=toks[i].intVal;
        if (toks[i].key=="alpha") t["alpha_gray"]=toks[i].intVal;
    }
    return t;
}

static int findNamedInt(const QList<Token> &toks, const QString &key, int def=0)
{
    for (const Token &t : toks) if (t.key==key) return t.intVal;
    return def;
}
static QString findNamedStr(const QList<Token> &toks, const QString &key, const QString &def={})
{
    for (const Token &t : toks) if (t.key==key) return t.strVal;
    return def;
}
static bool hasNamedKey(const QList<Token> &toks, const QString &key)
{
    for (const Token &t : toks) if (t.key==key) return true;
    return false;
}
static int posInt(const QList<Token> &toks, int idx, int def=0)
{
    int n = 0;
    for (const Token &t : toks) {
        if (t.key.isEmpty()) { if (n==idx) return t.intVal; n++; }
    }
    return def;
}
static QString posStr(const QList<Token> &toks, int idx, const QString &def={})
{
    int n = 0;
    for (const Token &t : toks) {
        if (t.key.isEmpty()) { if (n==idx) return t.strVal; n++; }
    }
    return def;
}

// Parse  "variable(N)"  → N, or plain int string → N
static int parseVarOrInt(const QString &s, bool &isVar)
{
    static QRegularExpression re(R"(^variable\((\d+)\)$)");
    auto m = re.match(s.trimmed());
    if (m.hasMatch()) { isVar=true; return m.captured(1).toInt(); }
    isVar=false;
    return s.trimmed().toInt();
}

// Parse move-route step name → code
static int moveNameToCode(const QString &name)
{
    static const QMap<QString,int> MAP = {
        {"end",0},{"move_down",1},{"move_left",2},{"move_right",3},{"move_up",4},
        {"move_lower_left",5},{"move_lower_right",6},{"move_upper_left",7},{"move_upper_right",8},
        {"move_random",9},{"move_toward_player",10},{"move_away_from_player",11},
        {"step_forward",12},{"step_backward",13},{"jump",14},{"wait",15},
        {"turn_down",16},{"turn_left",17},{"turn_right",18},{"turn_up",19},
        {"turn_90_right",20},{"turn_90_left",21},{"turn_180",22},
        {"turn_90_random",23},{"turn_random",24},
        {"turn_toward_player",25},{"turn_away_from_player",26},
        {"switch_on",27},{"switch_off",28},{"change_speed",29},{"change_freq",30},
        {"move_anim_on",31},{"move_anim_off",32},{"stop_anim_on",33},{"stop_anim_off",34},
        {"dir_fix_on",35},{"dir_fix_off",36},{"through_on",37},{"through_off",38},
        {"always_top_on",39},{"always_top_off",40},
        {"change_graphic",41},{"change_opacity",42},{"change_blend",43},
        {"play_se",44},{"script",45}
    };
    return MAP.value(name, -1);
}

// Parse condition string for `if` → QJsonArray params for code 111
static bool parseCondition(const QString &cond, QJsonArray &params)
{
    static const QMap<QString,int> OPS = {{"==",0},{">=",1},{"<=",2},{">",3},{"<",4},{"!=",5}};

    // switch(N) == ON/OFF
    {
        static QRegularExpression re(R"(^switch\((\d+)\)\s*==\s*(ON|OFF)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            params = {0, m.captured(1).toInt(), (m.captured(2)=="ON"?0:1)};
            return true;
        }
    }
    // variable(N) OP value|variable(M)
    {
        static QRegularExpression re(R"(^variable\((\d+)\)\s*(==|>=|<=|>|<|!=)\s*(.+)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            int varId = m.captured(1).toInt();
            int opIdx = OPS.value(m.captured(2).trimmed(), 0);
            QString rhs = m.captured(3).trimmed();
            bool isVar;
            int val = parseVarOrInt(rhs, isVar);
            params = {1, varId, isVar?1:0, val, opIdx};
            return true;
        }
    }
    // self_switch(A) == ON/OFF
    {
        static QRegularExpression re(R"(^self_switch\(([ABCD])\)\s*==\s*(ON|OFF)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            params = {2, m.captured(1), (m.captured(2)=="ON"?0:1)};
            return true;
        }
    }
    // timer OP Nsec
    {
        static QRegularExpression re(R"(^timer\s*(>=|<=)\s*(\d+)sec$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            int op   = m.captured(1)==">=" ? 0 : 1;
            int secs = m.captured(2).toInt();
            params = {3, secs, op};
            return true;
        }
    }
    // actor(N) in_party
    {
        static QRegularExpression re(R"(^actor\((\d+)\)\s+in_party$)");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={4,m.captured(1).toInt(),0,0}; return true; }
    }
    // actor(N) name == "X"
    {
        static QRegularExpression re(R"x(^actor\((\d+)\)\s+name\s*==\s*"(.*)"$)x");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={4,m.captured(1).toInt(),1,EventDslSerializer::dslUnescape(m.captured(2))}; return true; }
    }
    // actor(N) learned/weapon/armor/state (M)
    {
        static QRegularExpression re(R"(^actor\((\d+)\)\s+(learned|weapon|armor|state)\((\d+)\)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            static const QMap<QString,int> SUB={{"learned",2},{"weapon",3},{"armor",4},{"state",5}};
            params={4,m.captured(1).toInt(),SUB[m.captured(2)],m.captured(3).toInt()};
            return true;
        }
    }
    // enemy(N) appeared
    {
        static QRegularExpression re(R"(^enemy\((\d+)\)\s+appeared$)");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={5,m.captured(1).toInt()-1,0,0}; return true; }
    }
    // enemy(N) state(M)
    {
        static QRegularExpression re(R"(^enemy\((\d+)\)\s+state\((\d+)\)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={5,m.captured(1).toInt()-1,1,m.captured(2).toInt()}; return true; }
    }
    // character(who) facing(dir)
    {
        static QRegularExpression re(R"(^character\((\w+|\w+\(\w+\))\)\s+facing\((\w+)\)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            QString who = m.captured(1).trimmed();
            int evId = who=="player"?-1:(who=="this"?0:who.toInt());
            static const QMap<QString,int> DIRMAP={{"down",2},{"left",4},{"right",6},{"up",8}};
            int dir = DIRMAP.value(m.captured(2).toLower(), 0);
            params={6,evId,dir}; return true;
        }
    }
    // gold OP N
    {
        static QRegularExpression re(R"(^gold\s*(>=|<=)\s*(\d+)$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            params={7,m.captured(2).toInt(),(m.captured(1)==">="?0:1)}; return true;
        }
    }
    // item/weapon/armor (N) in_inventory
    {
        static QRegularExpression re(R"(^(item|weapon|armor)\((\d+)\)\s+in_inventory$)");
        auto m = re.match(cond);
        if (m.hasMatch()) {
            static const QMap<QString,int> T={{"item",8},{"weapon",9},{"armor",10}};
            params={T[m.captured(1)],m.captured(2).toInt()}; return true;
        }
    }
    // button(N) pressed
    {
        static QRegularExpression re(R"(^button\((\d+)\)\s+pressed$)");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={11,m.captured(1).toInt()}; return true; }
    }
    // script("expr")
    {
        static QRegularExpression re(R"x(^script\("(.*)"\)$)x");
        auto m = re.match(cond);
        if (m.hasMatch()) { params={12,EventDslSerializer::dslUnescape(m.captured(1))}; return true; }
    }
    return false;
}

// ============================================================
//  fromScript – parser
// ============================================================

QJsonArray EventDslSerializer::fromScript(const QString &text, bool *ok, QString *errMsg)
{
    QJsonArray result;
    QStringList lines = text.split('\n');
    while (!lines.isEmpty() && lines.last().trimmed().isEmpty())
        lines.removeLast();

    int lineCount = lines.size();
    int i = 0;

    auto setErr = [&](const QString &msg) {
        if (ok)     *ok     = false;
        if (errMsg) *errMsg = msg;
    };

    // Advance past blank/empty lines only (comments are real commands).
    auto skipBlank = [&]() {
        while (i < lineCount && lines[i].trimmed().isEmpty())
            i++;
    };

    while (i < lineCount) {
        skipBlank();
        if (i >= lineCount) break;

        const QString &raw  = lines[i];
        const QString  line = raw.trimmed();
        int ind             = lineIndent(raw) / 4;

        // ── Comment (#) → code 108 + 408 continuations ──────
        if (line.startsWith('#')) {
            QString first = line.mid(1);
            if (!first.isEmpty() && first[0]==' ') first = first.mid(1);
            QJsonArray p0; p0.append(first);
            result.append(makeCmd(108, ind, p0));
            i++;
            while (i < lineCount) {
                const QString &r2 = lines[i];
                QString l2 = r2.trimmed();
                if (!l2.startsWith('#')) break;
                QString cont = l2.mid(1);
                if (!cont.isEmpty() && cont[0]==' ') cont = cont.mid(1);
                QJsonArray pc; pc.append(cont);
                result.append(makeCmd(408, lineIndent(r2)/4, pc));
                i++;
            }
            continue;
        }

        // ── show_text multiline block ────────────────────────
        if (line == "show_text:") {
            i++;
            QStringList textLines;
            while (i < lineCount) {
                QString l2 = lines[i].trimmed();
                if (l2 == "end_show_text") { i++; break; }
                if (lineIndent(lines[i]) < (ind+1)*4 && !lines[i].trimmed().isEmpty()) break;
                textLines += lines[i].mid((ind+1)*4); // strip the leading indent
                i++;
            }
            if (!textLines.isEmpty()) {
                QJsonArray p; p.append(textLines[0]);
                result.append(makeCmd(101, ind, p));
                for (int k=1; k<textLines.size(); k++) {
                    QJsonArray pc; pc.append(textLines[k]);
                    result.append(makeCmd(401, ind, pc));
                }
            }
            continue;
        }
        // ── show_text("...") single-line ────────────────────
        if (line.startsWith("show_text(\"")) {
            int pos = 10; // points to opening "
            QString t = extractQuoted(line, pos);
            QStringList sl = t.split('\n');
            {QJsonArray p; p.append(sl[0]); result.append(makeCmd(101,ind,p));}
            for (int k=1;k<sl.size();k++){QJsonArray p;p.append(sl[k]);result.append(makeCmd(401,ind,p));}
            i++; continue;
        }

        // ── comment  #  ──────────────────────────────────────
        // (handled by the leading # check above for full-line;
        //  but we also want inline `# text` that's been put as a command)
        // If a line starts with # it's already skipped above.

        // ── script block ─────────────────────────────────────
        if (line == "script:") {
            i++;
            QStringList sl;
            while (i < lineCount) {
                QString l2 = lines[i].trimmed();
                if (l2 == "end_script") { i++; break; }
                sl += lines[i].mid((ind+1)*4);
                i++;
            }
            if (!sl.isEmpty()) {
                {QJsonArray p; p.append(sl[0]); result.append(makeCmd(355,ind,p));}
                for (int k=1;k<sl.size();k++){QJsonArray p;p.append(sl[k]);result.append(makeCmd(655,ind,p));}
            }
            continue;
        }

        // ── wait(N) ──────────────────────────────────────────
        if (line.startsWith("wait(")) {
            QJsonArray p; p.append(innerParens(line).toInt());
            result.append(makeCmd(106,ind,p)); i++; continue;
        }

        // ── if condition: ────────────────────────────────────
        if (line.startsWith("if ") && line.endsWith(':')) {
            QString cond = line.mid(3, line.size()-4).trimmed();
            QJsonArray params;
            if (!parseCondition(cond, params)) {
                setErr(QString("Line %1: cannot parse if condition: '%2'").arg(i+1).arg(cond));
                return QJsonArray();
            }
            result.append(makeCmd(111,ind,params));
            result.append(makeCmd(0, ind+1, {}));
            i++; continue;
        }
        if (line == "else:") {
            result.append(makeCmd(411,ind,QJsonArray()));
            result.append(makeCmd(0, ind+1, {}));
            i++; continue;
        }
        if (line == "endif") {
            result.append(makeCmd(412,ind,QJsonArray())); i++; continue;
        }

        // ── loop ─────────────────────────────────────────────
        if (line == "loop:") {
            result.append(makeCmd(112,ind,QJsonArray()));
            result.append(makeCmd(0, ind+1, {}));
            i++; continue;
        }
        if (line == "break") {
            result.append(makeCmd(113,ind,QJsonArray())); i++; continue;
        }
        if (line == "end_loop") {
            result.append(makeCmd(413,ind,QJsonArray())); i++; continue;
        }

        // ── no-arg flow ──────────────────────────────────────
        if (line=="exit_event")    { result.append(makeCmd(115,ind,{})); i++; continue; }
        if (line=="erase_event")   { result.append(makeCmd(116,ind,{})); i++; continue; }
        if (line=="wait_for_move") { result.append(makeCmd(210,ind,{})); i++; continue; }
        if (line=="prepare_transition"){ result.append(makeCmd(221,ind,{})); i++; continue; }
        if (line=="memorize_bgm_bgs")  { result.append(makeCmd(247,ind,{})); i++; continue; }
        if (line=="restore_bgm_bgs")   { result.append(makeCmd(248,ind,{})); i++; continue; }
        if (line=="stop_se")       { result.append(makeCmd(251,ind,{})); i++; continue; }
        if (line=="abort_battle")  { result.append(makeCmd(340,ind,{})); i++; continue; }
        if (line=="call_menu")     { result.append(makeCmd(351,ind,{})); i++; continue; }
        if (line=="call_save")     { result.append(makeCmd(352,ind,{})); i++; continue; }
        if (line=="game_over")     { result.append(makeCmd(353,ind,{})); i++; continue; }
        if (line=="return_to_title"){ result.append(makeCmd(354,ind,{})); i++; continue; }

        // ── call_common_event(N) ─────────────────────────────
        if (line.startsWith("call_common_event(")) {
            QJsonArray p; p.append(innerParens(line).toInt());
            result.append(makeCmd(117,ind,p)); i++; continue;
        }

        // ── show_animation(who, anim=N) ──────────────────────
        if (line.startsWith("show_animation(")) {
            static QRegularExpression re(R"(^show_animation\((player|this|event\(\d+\)),\s*anim=(\d+)\)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                QString whoS = m.captured(1);
                int who = whoS=="player" ? -1
                        : whoS=="this"   ?  0
                        : innerParens(whoS).toInt();
                QJsonArray p; p << who << m.captured(2).toInt();
                result.append(makeCmd(207,ind,p)); i++; continue;
            }
        }

        // ── label / jump ─────────────────────────────────────
        if (line.startsWith("label(\"")) {
            int pos=6; QJsonArray p; p.append(extractQuoted(line,pos));
            result.append(makeCmd(118,ind,p)); i++; continue;
        }
        if (line.startsWith("jump(\"")) {
            int pos=5; QJsonArray p; p.append(extractQuoted(line,pos));
            result.append(makeCmd(119,ind,p)); i++; continue;
        }

        // ── switch(N|range) = ON/OFF ─────────────────────────
        {
            static QRegularExpression re(R"(^switch\((\d+)(?:\.\.(\d+))?\)\s*=\s*(ON|OFF)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                int a = m.captured(1).toInt();
                int b = m.captured(2).isEmpty() ? a : m.captured(2).toInt();
                int v = m.captured(3)=="ON" ? 0 : 1;
                QJsonArray p; p<<a<<b<<v;
                result.append(makeCmd(121,ind,p)); i++; continue;
            }
        }

        // ── variable(N|range) OP rhs ─────────────────────────
        {
            static QRegularExpression re(R"(^variable\((\d+)(?:\.\.(\d+))?\)\s*(=|\+=|-=|\*=|/=|%=)\s*(.+)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                static const QMap<QString,int> OPS={{"=",0},{"+=",1},{"-=",2},{"*=",3},{"/=",4},{"%=",5}};
                int a   = m.captured(1).toInt();
                int b   = m.captured(2).isEmpty() ? a : m.captured(2).toInt();
                int op  = OPS.value(m.captured(3),0);
                QString rhsStr = m.captured(4).trimmed();

                // Determine source type and values
                int srcType=0, v1=0, v2=0;
                static QRegularExpression reVar(R"(^variable\((\d+)\)$)");
                static QRegularExpression reRand(R"(^random\((\d+),\s*(\d+)\)$)");
                static QRegularExpression reItem(R"(^item_count\((\d+)\)$)");
                static QRegularExpression reActorStat(R"(^actor_stat\((\d+),\s*(\w+)\)$)");
                static const QMap<QString,int> STATS={{"Level",0},{"EXP",1},{"HP",2},{"SP",3},{"MaxHP",4},{"MaxSP",5},{"STR",6},{"DEX",7},{"AGI",8},{"INT",9},{"ATK",10},{"PDEF",11},{"MDEF",12},{"EVA",13}};
                static QRegularExpression reOther(R"(^other\((\w+)\)$)");
                static const QMap<QString,int> OTHM={{"map_id",0},{"party_size",1},{"gold",2},{"steps",3},{"play_time",4},{"timer",5},{"save_count",6}};

                auto mv = reVar.match(rhsStr);
                auto mr = reRand.match(rhsStr);
                auto mi = reItem.match(rhsStr);
                auto ms = reActorStat.match(rhsStr);
                auto mo = reOther.match(rhsStr);
                if (mv.hasMatch()) { srcType=1; v1=mv.captured(1).toInt(); }
                else if (mr.hasMatch()) { srcType=2; v1=mr.captured(1).toInt(); v2=mr.captured(2).toInt(); }
                else if (mi.hasMatch()) { srcType=3; v1=mi.captured(1).toInt(); }
                else if (ms.hasMatch()) { srcType=4; v1=ms.captured(1).toInt(); v2=STATS.value(ms.captured(2),0); }
                else if (mo.hasMatch()) { srcType=7; v1=OTHM.value(mo.captured(1),0); }
                else { srcType=0; v1=rhsStr.toInt(); }

                QJsonArray p; p<<a<<b<<op<<srcType<<v1<<v2;
                result.append(makeCmd(122,ind,p)); i++; continue;
            }
        }

        // ── self_switch(A) = ON/OFF ──────────────────────────
        {
            static QRegularExpression re(R"(^self_switch\(([ABCD])\)\s*=\s*(ON|OFF)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                QJsonArray p; p<<m.captured(1)<<(m.captured(2)=="ON"?0:1);
                result.append(makeCmd(123,ind,p)); i++; continue;
            }
        }

        // ── timer(start, N) / timer(stop) ────────────────────
        if (line.startsWith("timer(")) {
            QString inner = innerParens(line);
            QJsonArray p;
            if (inner.trimmed()=="stop") {
                p<<1<<0;
            } else {
                auto toks = tokenizeArgs(inner);
                int secs = posInt(toks,1,0);
                p<<0<<secs;
            }
            result.append(makeCmd(124,ind,p)); i++; continue;
        }

        // ── gold += / -= N|variable(N) ──────────────────────
        {
            static QRegularExpression re(R"(^gold\s*(\+=|-=)\s*(.+)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                int op = m.captured(1)=="+=" ? 0 : 1;
                bool isVar; int val = parseVarOrInt(m.captured(2).trimmed(), isVar);
                QJsonArray p; p<<op<<(isVar?1:0)<<val;
                result.append(makeCmd(125,ind,p)); i++; continue;
            }
        }

        // ── item/weapon/armor(N) += / -= rhs ─────────────────
        {
            static QRegularExpression re(R"(^(item|weapon|armor)\((\d+)\)\s*(\+=|-=)\s*(.+)$)");
            static const QMap<QString,int> TC={{"item",126},{"weapon",127},{"armor",128}};
            auto m = re.match(line);
            if (m.hasMatch()) {
                int c2   = TC[m.captured(1)];
                int id   = m.captured(2).toInt();
                int op   = m.captured(3)=="+=" ? 0 : 1;
                bool isVar; int val = parseVarOrInt(m.captured(4).trimmed(), isVar);
                QJsonArray p; p<<id<<op<<(isVar?1:0)<<val;
                result.append(makeCmd(c2,ind,p)); i++; continue;
            }
        }

        // ── party_add / party_remove ──────────────────────────
        if (line.startsWith("party_add(")) {
            auto toks = tokenizeArgs(innerParens(line));
            int actor = posInt(toks,0,0);
            int init  = hasNamedKey(toks,"initialize") ? 1 : 0;
            QJsonArray p; p<<actor<<0<<init;
            result.append(makeCmd(129,ind,p)); i++; continue;
        }
        if (line.startsWith("party_remove(")) {
            QJsonArray p; p<<innerParens(line).toInt()<<1<<0;
            result.append(makeCmd(129,ind,p)); i++; continue;
        }

        // ── transfer(…) ──────────────────────────────────────
        if (line.startsWith("transfer(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QString mapStr = findNamedStr(toks,"map","0");
            QString xStr   = findNamedStr(toks,"x","0");
            QString yStr   = findNamedStr(toks,"y","0");
            bool mapIsVar, xIsVar, yIsVar;
            int mapV = parseVarOrInt(mapStr,mapIsVar);
            int xV   = parseVarOrInt(xStr,xIsVar);
            int yV   = parseVarOrInt(yStr,yIsVar);
            bool useVar = mapIsVar||xIsVar||yIsVar;
            QString dirStr = findNamedStr(toks,"dir","");
            static const QMap<QString,int> DM={{"down",2},{"left",4},{"right",6},{"up",8}};
            int dir  = DM.value(dirStr,0);
            int fade = hasNamedKey(toks,"no_fade") ? 1 : 0;
            QJsonArray p; p<<(useVar?1:0)<<mapV<<xV<<yV<<dir<<fade;
            result.append(makeCmd(201,ind,p)); i++; continue;
        }

        // ── play_bgm/bgs/me/se("name", vol, pitch) ───────────
        {
            static QRegularExpression re(R"(^(play_bgm|play_bgs|play_me|play_se|battle_bgm|battle_me)\()");
            auto m = re.match(line);
            if (m.hasMatch()) {
                static const QMap<QString,int> CM={{"play_bgm",241},{"play_bgs",245},{"play_me",249},{"play_se",250},{"battle_bgm",132},{"battle_me",133}};
                int paren = line.indexOf('(');
                int pos   = paren+1;
                QString name = extractQuoted(line,pos);
                pos++; // skip comma+space
                auto rest = tokenizeArgs(line.mid(pos, line.lastIndexOf(')')-pos));
                int vol   = posInt(rest,0,100);
                int pitch = posInt(rest,1,100);
                QJsonArray p; p.append(makeAudioFile(name,vol,pitch));
                result.append(makeCmd(CM[m.captured(1)],ind,p)); i++; continue;
            }
        }

        // ── fade_bgm / fade_bgs ──────────────────────────────
        if (line.startsWith("fade_bgm(")) {
            QJsonArray p; p.append(innerParens(line).toInt());
            result.append(makeCmd(242,ind,p)); i++; continue;
        }
        if (line.startsWith("fade_bgs(")) {
            QJsonArray p; p.append(innerParens(line).toInt());
            result.append(makeCmd(246,ind,p)); i++; continue;
        }

        // ── transition("name") ───────────────────────────────
        if (line.startsWith("transition(\"")) {
            int pos=12; QJsonArray p; p.append(extractQuoted(line,pos));
            result.append(makeCmd(222,ind,p)); i++; continue;
        }

        // ── screen_tone / fog_tone ───────────────────────────
        {
            static QRegularExpression re(R"(^(screen_tone|fog_tone)\()");
            auto m = re.match(line);
            if (m.hasMatch()) {
                auto toks = tokenizeArgs(innerParens(line));
                QJsonObject tone = parseTone(toks,0);
                int dur = findNamedInt(toks,"dur",0);
                QJsonArray p; p.append(tone); p.append(dur);
                result.append(makeCmd(m.captured(1)=="screen_tone"?223:205,ind,p));
                i++; continue;
            }
        }

        // ── screen_flash ─────────────────────────────────────
        if (line.startsWith("screen_flash(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonObject c;
            c["r"]=findNamedInt(toks,"r",0);
            c["g"]=findNamedInt(toks,"g",0);
            c["b"]=findNamedInt(toks,"b",0);
            c["alpha_gray"]=findNamedInt(toks,"alpha",0);
            c["RXClass"]="RPG::Color";
            int dur=findNamedInt(toks,"dur",0);
            QJsonArray p; p.append(c); p.append(dur);
            result.append(makeCmd(224,ind,p)); i++; continue;
        }

        // ── screen_shake ─────────────────────────────────────
        if (line.startsWith("screen_shake(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonArray p;
            p<<findNamedInt(toks,"power",0)<<findNamedInt(toks,"speed",0)<<findNamedInt(toks,"dur",0);
            result.append(makeCmd(225,ind,p)); i++; continue;
        }

        // ── weather(none|rain|storm|snow, power=N, dur=N) ────
        if (line.startsWith("weather(")) {
            auto toks = tokenizeArgs(innerParens(line));
            static const QMap<QString,int> WM={{"none",0},{"rain",1},{"storm",2},{"snow",3}};
            int w   = WM.value(posStr(toks,0,"none"),0);
            int pwr = findNamedInt(toks,"power",5);
            int dur = findNamedInt(toks,"dur",0);
            QJsonArray p; p<<w<<pwr<<dur;
            result.append(makeCmd(236,ind,p)); i++; continue;
        }

        // ── show_choices(…): ─────────────────────────────────
        if (line.startsWith("show_choices(") && line.endsWith(':')) {
            QString inner = line.mid(13, line.size()-14); // strip show_choices( and ):
            auto toks = tokenizeArgs(inner);
            QJsonArray choices;
            for (const Token &t : toks)
                if (t.key.isEmpty() && t.isString) choices.append(t.strVal);
            int cancel = findNamedInt(toks,"cancel",0);
            QJsonArray p; p.append(choices); p.append(cancel);
            result.append(makeCmd(102,ind,p)); i++; continue;
        }
        if (line.startsWith("when(\"") && line.endsWith("):")) {
            int pos=5; QString label = extractQuoted(line,pos);
            // Find which choice index this corresponds to - we need it as p[0]
            // We search backwards for the last 102 at same indent
            int choiceIdx = 0;
            for (int k=result.size()-1; k>=0; k--) {
                int c2 = result[k].toObject()["@code"].toInt();
                if (c2==102) break;
                if (c2==402) choiceIdx++;
            }
            QJsonArray p; p<<choiceIdx<<label;
            result.append(makeCmd(402,ind,p));
            result.append(makeCmd(0, ind+1, {}));
            i++; continue;
        }
        if (line=="when_cancel:") {
            result.append(makeCmd(403,ind,{}));
            result.append(makeCmd(0, ind+1, {}));
            i++; continue;
        }
        if (line=="end_choices") {
            result.append(makeCmd(404,ind,{})); i++; continue;
        }

        // ── battle(troop=N, escape?, lose?): ─────────────────
        if (line.startsWith("battle(") && line.endsWith(':')) {
            auto toks = tokenizeArgs(line.mid(7, line.size()-8));
            QJsonArray p;
            p<<findNamedInt(toks,"troop",0)
             <<(hasNamedKey(toks,"escape")?true:false)
             <<(hasNamedKey(toks,"lose")?true:false);
            result.append(makeCmd(301,ind,p)); i++; continue;
        }
        if (line=="win:")    { result.append(makeCmd(601,ind,{})); result.append(makeCmd(0, ind+1, {})); i++; continue; }
        if (line=="escape:") { result.append(makeCmd(602,ind,{})); result.append(makeCmd(0, ind+1, {})); i++; continue; }
        if (line=="lose:")   { result.append(makeCmd(603,ind,{})); result.append(makeCmd(0, ind+1, {})); i++; continue; }
        if (line=="end_battle"){ result.append(makeCmd(604,ind,{})); i++; continue; }

        // ── move_route(who, opts?): ──────────────────────────
        if (line.startsWith("move_route(") && line.endsWith(':')) {
            auto toks = tokenizeArgs(line.mid(11, line.size()-13));
            QString who = posStr(toks,0,"this");
            int evId = who=="player" ? -1
                     : who=="this"   ?  0
                     : innerParens(who).toInt();
            bool repeat = hasNamedKey(toks,"repeat");
            bool skip   = hasNamedKey(toks,"skip");

            // Collect move steps
            i++;
            QJsonArray steps;
            while (i < lineCount) {
                QString l2 = lines[i].trimmed();
                if (l2 == "end_move_route") { i++; break; }
                if (l2.isEmpty()||l2.startsWith('#')) { i++; continue; }

                // Parse move step
                int paren = l2.indexOf('(');
                QString name = (paren<0 ? l2 : l2.left(paren)).trimmed();
                int code2 = moveNameToCode(name);
                QJsonArray sp;
                if (paren >= 0) {
                    QString inner2 = l2.mid(paren+1, l2.lastIndexOf(')')-paren-1);
                    if (code2==14) { // jump(x,y)
                        auto t2 = tokenizeArgs(inner2);
                        sp<<posInt(t2,0,0)<<posInt(t2,1,0);
                    } else if (code2==41) { // change_graphic
                        int pos2=0; QString cn=extractQuoted(inner2,pos2);
                        auto t2=tokenizeArgs(inner2.mid(pos2));
                        sp<<cn<<posInt(t2,0,0)<<QString()<<0; // char, hue, battle, hue
                    } else if (code2==44) { // play_se
                        int pos2=0; QString sn=extractQuoted(inner2,pos2);
                        auto t2=tokenizeArgs(inner2.mid(pos2));
                        sp<<makeAudioFile(sn,posInt(t2,0,100),posInt(t2,1,100));
                    } else if (code2==45) { // script
                        int pos2=0; sp<<extractQuoted(inner2,pos2);
                    } else {
                        sp<<innerParens(l2).toInt();
                    }
                }
                QJsonObject step = makeMoveCmd(code2 < 0 ? 0 : code2, sp);
                steps.append(step);

                // Create 509 display item
                QJsonArray p509; p509.append(step);
                result.append(makeCmd(209, ind, {})); // placeholder – will be fixed below
                // Actually we should build the 209 first and then 509s after.
                // Let's collect all steps and build at the end of the block.
                result.removeLast(); // remove placeholder
                i++;
            }

            // Empty terminator
            steps.append(makeMoveCmd(0, {}));

            // Build move route object
            QJsonObject mr;
            mr["@list"]      = steps;
            mr["@repeat"]    = repeat;
            mr["@skippable"] = skip;
            mr["RXClass"]    = "RPG::MoveRoute";

            QJsonArray p209; p209<<evId<<mr;
            result.append(makeCmd(209,ind,p209));

            // 509 display items (all steps except the terminator)
            for (int k=0; k<steps.size()-1; k++) {
                QJsonArray p509; p509.append(steps[k]);
                result.append(makeCmd(509,ind,p509));
            }
            continue;
        }
        if (line=="end_move_route") { i++; continue; } // orphaned

        // ── shop: ────────────────────────────────────────────
        if (line=="shop:") {
            i++;
            bool first = true;
            while (i < lineCount) {
                QString l2 = lines[i].trimmed();
                if (l2=="end_shop") { i++; break; }
                if (l2.isEmpty()||l2.startsWith('#')) { i++; continue; }
                static QRegularExpression re(R"(^(item|weapon|armor)\((\d+)(?:,\s*price=(\d+))?\)$)");
                static const QMap<QString,int> IT={{"item",0},{"weapon",1},{"armor",2}};
                auto m2 = re.match(l2);
                if (m2.hasMatch()) {
                    int t2 = IT[m2.captured(1)];
                    int id = m2.captured(2).toInt();
                    int usePrice = m2.captured(3).isEmpty()?0:1;
                    int price    = usePrice ? m2.captured(3).toInt() : 0;
                    QJsonArray sp; sp<<t2<<id<<usePrice<<price;
                    result.append(makeCmd(first?302:605, ind, sp));
                    first=false;
                }
                i++;
            }
            continue;
        }

        // ── erase_picture(N) ─────────────────────────────────
        if (line.startsWith("erase_picture(")) {
            QJsonArray p; p.append(innerParens(line).toInt());
            result.append(makeCmd(235,ind,p)); i++; continue;
        }

        // ── windowskin ───────────────────────────────────────
        if (line.startsWith("windowskin(\"")) {
            int pos=12; QJsonArray p; p.append(extractQuoted(line,pos));
            result.append(makeCmd(131,ind,p)); i++; continue;
        }

        // ── save_access / menu_access / encounter ─────────────
        {
            static QRegularExpression re(R"(^(save_access|menu_access|encounter)\((enable|disable)\)$)");
            static const QMap<QString,int> CM={{"save_access",134},{"menu_access",135},{"encounter",136}};
            auto m = re.match(line);
            if (m.hasMatch()) {
                QJsonArray p; p.append(m.captured(2)=="enable"?0:1);
                result.append(makeCmd(CM[m.captured(1)],ind,p)); i++; continue;
            }
        }

        // ── change_hp/sp/exp/level ───────────────────────────
        {
            static QRegularExpression re(R"(^(change_hp|change_sp|change_exp|change_level)\((party|actor\((\d+)\)),\s*([+-])(.+)\)$)");
            static const QMap<QString,int> CM={{"change_hp",311},{"change_sp",312},{"change_exp",315},{"change_level",316}};
            auto m = re.match(line);
            if (m.hasMatch()) {
                int who = m.captured(2)=="party" ? 0 : m.captured(3).toInt();
                int op  = m.captured(4)=="+" ? 0 : 1;
                bool isVar; int val=parseVarOrInt(m.captured(5).trimmed(),isVar);
                QJsonArray p; p<<who<<op<<(isVar?1:0)<<val;
                result.append(makeCmd(CM[m.captured(1)],ind,p)); i++; continue;
            }
        }

        // ── scroll_map(dir, dist, speed) ─────────────────────
        if (line.startsWith("scroll_map(")) {
            auto toks = tokenizeArgs(innerParens(line));
            static const QMap<QString,int> DM={{"down",2},{"left",4},{"right",6},{"up",8}};
            QJsonArray p; p<<DM.value(posStr(toks,0,"down"),2)<<posInt(toks,1,0)<<posInt(toks,2,0);
            result.append(makeCmd(203,ind,p)); i++; continue;
        }

        // ── transparent(on|off) ───────────────────────────────
        if (line.startsWith("transparent(")) {
            QJsonArray p; p.append(innerParens(line).trimmed()=="on"?0:1);
            result.append(makeCmd(208,ind,p)); i++; continue;
        }

        // ── rotate_picture(N, speed) ──────────────────────────
        if (line.startsWith("rotate_picture(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonArray p; p<<posInt(toks,0,0)<<posInt(toks,1,0);
            result.append(makeCmd(233,ind,p)); i++; continue;
        }

        // ── picture_tone(N, tone..., dur=N) ───────────────────
        if (line.startsWith("picture_tone(")) {
            auto toks = tokenizeArgs(innerParens(line));
            int num = posInt(toks,0,0);
            QJsonObject tone = parseTone(toks,1);
            int dur = findNamedInt(toks,"dur",0);
            QJsonArray p; p<<num<<tone<<dur;
            result.append(makeCmd(234,ind,p)); i++; continue;
        }

        // ── fog_opacity(N, dur=N) ────────────────────────────
        if (line.startsWith("fog_opacity(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonArray p; p<<posInt(toks,0,0)<<findNamedInt(toks,"dur",0);
            result.append(makeCmd(206,ind,p)); i++; continue;
        }

        // ── text_options(pos=X, window=show|hide) ────────────
        if (line.startsWith("text_options(")) {
            auto toks = tokenizeArgs(innerParens(line));
            static const QMap<QString,int> PM={{"top",0},{"middle",1},{"bottom",2}};
            QJsonArray p;
            p<<PM.value(findNamedStr(toks,"pos","top"),0)
             <<(findNamedStr(toks,"window","show")=="show"?0:1);
            result.append(makeCmd(104,ind,p)); i++; continue;
        }

        // ── input_number(var=N, digits=N) ────────────────────
        if (line.startsWith("input_number(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonArray p; p<<findNamedInt(toks,"var",0)<<findNamedInt(toks,"digits",0);
            result.append(makeCmd(103,ind,p)); i++; continue;
        }

        // ── button_input(var=N) ───────────────────────────────
        if (line.startsWith("button_input(")) {
            auto toks = tokenizeArgs(innerParens(line));
            QJsonArray p; p<<findNamedInt(toks,"var",0);
            result.append(makeCmd(105,ind,p)); i++; continue;
        }

        // ── recover_all(party|actor(N)) ───────────────────────
        if (line.startsWith("recover_all(")) {
            QString who = innerParens(line).trimmed();
            int id = who=="party" ? 0 : innerParens(who).toInt();
            QJsonArray p; p<<id;
            result.append(makeCmd(314,ind,p)); i++; continue;
        }

        // ── enemy_recover(troop|enemy(N)) ────────────────────
        if (line.startsWith("enemy_recover(")) {
            QString who = innerParens(line).trimmed();
            int id = who=="troop" ? -1 : innerParens(who).toInt()-1;
            QJsonArray p; p<<id;
            result.append(makeCmd(334,ind,p)); i++; continue;
        }

        // ── change_actor_name(actor(N), "name") ──────────────
        if (line.startsWith("change_actor_name(")) {
            static QRegularExpression re(R"x(^change_actor_name\(actor\((\d+)\),\s*"(.*)"\)$)x");
            auto m = re.match(line);
            if (m.hasMatch()) {
                QJsonArray p; p<<m.captured(1).toInt()<<dslUnescape(m.captured(2));
                result.append(makeCmd(320,ind,p)); i++; continue;
            }
        }

        // ── force_action(enemy(N)|actor(N), basic(N)|skill(N), target=N, now|normal) ──
        if (line.startsWith("force_action(")) {
            static QRegularExpression re(R"(^force_action\((enemy|actor)\((\d+)\),\s*(basic|skill)\((\d+)\),\s*target=(-?\d+),\s*(now|normal)\)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                int stype = m.captured(1)=="enemy" ? 0 : 1;
                int atype = m.captured(3)=="basic" ? 0 : 1;
                QJsonArray p;
                p<<stype<<m.captured(2).toInt()<<atype<<m.captured(4).toInt()
                 <<m.captured(5).toInt()<<(m.captured(6)=="now"?1:0);
                result.append(makeCmd(339,ind,p)); i++; continue;
            }
        }

        // ── raw(code, [...]) fallback ─────────────────────────
        {
            static QRegularExpression re(R"(^raw\((\d+),\s*(\[.*\])\)$)");
            auto m = re.match(line);
            if (m.hasMatch()) {
                int rawCode = m.captured(1).toInt();
                QJsonParseError err;
                QJsonDocument doc = QJsonDocument::fromJson(m.captured(2).toUtf8(), &err);
                if (err.error == QJsonParseError::NoError && doc.isArray()) {
                    result.append(makeCmd(rawCode, ind, doc.array()));
                    i++; continue;
                }
                setErr(QString("Line %1: invalid raw() JSON: %2").arg(i+1).arg(err.errorString()));
                return QJsonArray();
            }
        }

        // ── Unknown line ─────────────────────────────────────
        setErr(QString("Line %1: unknown statement: '%2'").arg(i+1).arg(line));
        return QJsonArray();
    }

    // End-of-list marker
    result.append(makeCmd(0, 0, {}));

    if (ok) *ok = true;
    return result;
}
