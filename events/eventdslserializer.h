#ifndef EVENTDSLSERIALIZER_H
#define EVENTDSLSERIALIZER_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

/*
 * EventDslSerializer – bidirectional converter between the internal
 * QJsonArray event-command list and a human-readable DSL.
 *
 * Example output:
 *
 *   show_text("Hello World")
 *   if switch(1) == ON:
 *       show_text("Switch is on!")
 *   else:
 *       self_switch(A) = ON
 *   endif
 *   script:
 *       $game_switches[2] = true
 *   end_script
 *   wait(30)
 *
 * Indentation: 4 spaces per @indent level.
 * Unrecognised / complex commands fall back to:
 *   raw(CODE, [p1, p2, ...])   ← valid JSON array for parameters
 */
class EventDslSerializer
{
public:
    static QString toScript(const QJsonArray &commands);
    static QJsonArray fromScript(const QString &text,
                                 bool *ok       = nullptr,
                                 QString *errMsg = nullptr);

    static QString dslEscape(const QString &s);
    static QString dslUnescape(const QString &s);
};

#endif // EVENTDSLSERIALIZER_H
