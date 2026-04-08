# Yet Another RPGMaker XP (YaRXP)

[![Windows Build](https://github.com/jowin202/YaRXP/actions/workflows/windows-build.yml/badge.svg)](https://github.com/jowin202/YaRXP/actions/workflows/windows-build.yml)
[![Linux Build](https://github.com/jowin202/YaRXP/actions/workflows/linux-build.yml/badge.svg)](https://github.com/jowin202/YaRXP/actions/workflows/linux-build.yml)
[![macOS Build](https://github.com/jowin202/YaRXP/actions/workflows/macos-build.yml/badge.svg)](https://github.com/jowin202/YaRXP/actions/workflows/macos-build.yml)

YaRXP is a cross-platform, open-source editor for **RPG Maker XP** projects, built with C++ and the Qt framework. It allows you to open, edit, and save RPG Maker XP game projects without needing the original RPG Maker XP application.

---

## Table of Contents

- [Features](#features)
- [Architecture Overview](#architecture-overview)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [RPG Maker XP Data Format](#rpg-maker-xp-data-format)
- [Export](#export)
- [Import](#import)
- [Contributing](#contributing)
- [License](#license)

---

## Features

### Map Editor
- Visual tile-based map editor with three independent layers
- Multiple editing modes: pen (draw), selection (copy/paste/delete), and event placement
- Tileset viewer with multi-tile selection support
- Autotile support with automatic border calculation
- Map manipulation tools: shift, extend, and crop
- Undo/redo support for all map operations
- Zoom in/out for detailed editing

### Data Editors
Full editors for all RPG Maker XP database entries:

| Category | Description |
|---|---|
| **Actors** | Character stats, level curves, equipment slots, graphics |
| **Classes** | Character classes with skill learning schedules |
| **Skills** | Battle and menu skills with effect parameters |
| **Items** | Consumable and key items |
| **Weapons** | Weapon definitions with stat modifiers |
| **Armors** | Armor and accessories with stat modifiers |
| **Enemies** | Enemy stats, AI actions, and loot |
| **Troops** | Battle groups and encounter conditions |
| **States** | Status conditions (poison, sleep, etc.) |
| **Animations** | Frame-by-frame battle animation editor |
| **Tilesets** | Tileset passability and terrain tag configuration |
| **Common Events** | Reusable event scripts |
| **System** | Global game settings (title, currency, starting position, etc.) |
| **Scripts** | Script editor with syntax highlighting |

### Event System
- Over 100 individual event command types, each with a dedicated editor dialog
- Color-coded event list display for quick readability
- Conditional branches, loops, and page switching
- Full coverage of RPG Maker XP's event command set

### Map Management
- Hierarchical map tree with drag-and-drop reordering
- Map properties dialog (name, tileset, size, BGM, fog, panorama)
- Context menus for quick map operations (add, delete, duplicate, copy/paste)

### Import & Export
- **Import:** Decrypt and extract RGSSAD archives (the encrypted project format used by some RPG Maker XP distributions)
- **Export:** Convert maps and tilesets to [Godot Engine](https://godotengine.org/) format

---

## Architecture Overview

YaRXP follows a layered, MVC-inspired architecture:

```
┌────────────────────────────────────────────────────┐
│                 Qt Application                     │
│              MainWindow / Menus / Toolbars         │
└──────────────┬────────────────┬────────────────────┘
               │                │
   ┌───────────▼──────┐  ┌──────▼───────────┐
   │    Map Editor    │  │   Data Editors   │
   │  (viewer/)       │  │   (editors/)     │
   │  MapView         │  │   DataEditor     │
   │  TilesetView     │  │   14 sub-editors │
   └───────────┬──────┘  └──────┬───────────┘
               │                │
               └───────┬────────┘
                       │
          ┌────────────▼────────────┐
          │          RPGDB          │
          │   Central Database      │
          │  (RXIO2/rpgdb.h/.cpp)   │
          └────────────┬────────────┘
                       │
      ┌────────────────┼──────────────────┐
      │                │                  │
 ┌────▼─────┐   ┌───────▼──────┐   ┌──────▼────┐
 │  Parser  │   │  Controllers │   │  Factory  │
 │  Writer  │   │  (RPGEditor  │   │  (object  │
 │  RGSSAD  │   │   RPGMap     │   │  creation)│
 │  (I/O)   │   │   RPGMapInfo)│   └───────────┘
 └──────────┘   └──────────────┘
```

### Core Layers

| Layer | Location | Responsibility |
|---|---|---|
| **UI** | `mainwindow.*`, `viewer/`, `editors/`, `events/`, `dialogs/` | All Qt widgets, rendering, and user interaction |
| **Controllers** | `RXIO2/rpg*controller.*` | Bridge between UI and data; handle selection, modification, signals |
| **Database** | `RXIO2/rpgdb.*` | Central store for all project data; loads/saves all `.rxdata` files |
| **I/O** | `RXIO2/parser.*`, `RXIO2/writer.*`, `RXIO2/iorgssad.*` | Binary `.rxdata` parsing/writing, RGSSAD decryption |
| **Factory** | `RXIO2/factory.*` | Creates new game objects with correct default values |

### Key Components

- **`RPGDB`** — The central database object. Reads all RPG Maker data files from disk into JSON (`QJsonDocument`), exposes them to controllers and editors, and writes them back on save.
- **`RPGEditorController`** — Manages the connection between the data editor widgets and the underlying JSON data. Handles list selection, field changes, and apply/cancel logic.
- **`RPGMapController`** — Controls map rendering and tile manipulation. Converts raw tile data to `MapTile` graphics items and manages layer drawing and event overlays.
- **`MapView`** — The main map editing canvas, implemented as a `QGraphicsView`. Handles all mouse-based drawing operations, tile painting across layers, and undo integration.
- **`EventCommandDialog`** — The root dialog for placing event commands. Spawns one of 100+ individual command dialogs based on the selected command type.

---

## Prerequisites

- **Qt 6.5.0** or later (Qt Widgets, Qt Multimedia, Qt Core, Qt GUI)
- **qmake** (included with Qt)
- A C++11-capable compiler:
  - Windows: MSVC 2019 or later
  - macOS: Clang (Xcode Command Line Tools)
  - Linux: GCC 7+ or Clang 6+

---

## Building

### 1. Clone the repository

```bash
git clone https://github.com/jowin202/YaRXP.git
cd YaRXP
```

### 2. Generate build files

```bash
qmake YaRXP.pro
```

### 3. Compile

**Linux / macOS:**
```bash
make
```

**Windows (MSVC):**
```bash
msbuild YaRXP.vcxproj -property:Configuration=Release
```

Build output is placed in:
- `build/debug/YaRXP` (debug build)
- `build/release/YaRXP` (release build)

### 4. Deploy runtime dependencies (Windows only)

After a Windows build, run Qt's deployment tool to copy the required DLLs next to the executable:

```bash
windeployqt.exe build\release\YaRXP.exe
```

### CI/CD

All builds are triggered **manually** via `workflow_dispatch` in GitHub Actions. There are three workflows:

| Workflow | File | Runner | Output |
|---|---|---|---|
| Windows | `windows-build.yml` | `windows-latest` | `.exe` + Qt DLLs (zip) |
| Linux | `linux-build.yml` | `ubuntu-latest` | binary (zip) |
| macOS Intel | `macos-build.yml` | `macos-13` (x86_64) | `.dmg` |
| macOS ARM | `macos-build.yml` | `macos-latest` (Apple Silicon) | `.dmg` |

All pipelines install Qt 6.5.0 via `aqtinstall`, build in Release mode, upload artifacts, and publish a pre-release tag. A GitLab CI pipeline (`.gitlab-ci.yml`) is also available for the Windows build.

---

## Usage

### Opening a Project

1. Launch YaRXP.
2. Go to **File → Open Project** and select a `Game.rxproj` file, or select the project root directory.
3. The map tree populates on the left. Click a map to open it in the map editor.

### Editing Maps

- **Layer selection:** Use the layer toolbar to switch between Layers 1, 2, and 3.
- **Pen tool:** Select tiles in the tileset panel (right side), then click or drag on the map to paint.
- **Multi-tile selection:** In the tileset panel, click and drag to select a rectangular region of tiles and paint them all at once.
- **Selection tool:** Draw a selection rectangle on the map to copy, paste, or delete a region.
- **Event tool:** Click on the map to place or edit map events.
- **Undo/Redo:** `Ctrl+Z` / `Ctrl+Y` (or toolbar buttons).

### Editing Database Entries

1. Go to **Data → [category]** (e.g., Actors, Items, Skills).
2. Select an entry from the list on the left.
3. Modify fields in the right panel.
4. Click **Apply** to save changes or **OK** to close and save.

### Managing Maps

- Right-click a map in the map tree to **Add**, **Delete**, **Duplicate**, or change **Properties**.
- Drag maps in the tree to reorder them or change their parent.

### Saving

- **File → Save** (`Ctrl+S`) writes all modified `.rxdata` files back to the project's `Data/` directory.

---

## Project Structure

```
YaRXP/
├── YaRXP.pro               # qmake project file
├── main.cpp                # Entry point
├── mainwindow.h/.cpp       # Main application window
├── maptreewidget.h/.cpp    # Hierarchical map tree widget
│
├── RXIO2/                  # Core data layer
│   ├── rpgdb.h/.cpp        # Central database (load/save all project data)
│   ├── rpgeditorcontroller.*  # Data editor controller
│   ├── rpgmapcontroller.*     # Map rendering controller
│   ├── rpgmapinfocontroller.* # Map metadata controller
│   ├── rpgeventlistcontroller.* # Event list controller
│   ├── factory.*           # Object factory (default value creation)
│   ├── parser.*            # .rxdata binary format parser → JSON
│   ├── writer.*            # JSON → .rxdata binary format writer
│   ├── iorgssad.*          # RGSSAD archive decryption
│   ├── fileopener.*        # File/directory discovery helpers
│   ├── autotileset.*       # Autotile layout and rendering helpers
│   └── searchthread.*      # Background search thread
│
├── viewer/                 # Map editor UI
│   ├── mapview.h/.cpp      # Main map editing canvas (QGraphicsView)
│   ├── maptile.h/.cpp      # Individual tile graphics item
│   ├── tilesetview.h/.cpp  # Tileset selector panel
│   ├── tilesetrectangle.*  # Selection rectangle in tileset
│   ├── mapselectrectangle.* # Selection rectangle on map
│   └── undodb.*            # Undo/redo stack
│
├── editors/                # Data editors (one sub-folder per category)
│   ├── dataeditor.h/.cpp   # Master editor container (tabs + controller setup)
│   ├── actors/             # Actor editor widgets
│   ├── classes/            # Class editor widgets
│   ├── skills/             # Skill editor widgets
│   ├── items/              # Item editor widgets
│   ├── weapons/            # Weapon editor widgets
│   ├── armors/             # Armor editor widgets
│   ├── enemies/            # Enemy editor widgets
│   ├── troops/             # Troop editor widgets
│   ├── states/             # State editor widgets
│   ├── animations/         # Animation editor widgets
│   ├── tilesets/           # Tileset editor widgets
│   ├── commonevents/       # Common event editor widgets
│   ├── system/             # System settings editor
│   └── scripts/            # Script editor with syntax highlighting
│
├── events/                 # Event system UI
│   ├── eventdialog.*       # Event container dialog
│   ├── eventpage.*         # Event page (conditions + commands)
│   ├── eventlistitem.*     # Colored event command list entry
│   ├── eventcommanddialog.* # Command picker dialog (100+ commands)
│   └── commands/           # Individual command editor dialogs (~110 files)
│
├── dialogs/                # Reusable dialog components
│   ├── audiodialog.*       # BGM/SFX file picker
│   ├── imagedialog.*       # Graphic file picker
│   ├── listdialog.*        # Generic list selector
│   ├── switchwidget.*      # Switch selector widget
│   ├── variablewidget.*    # Variable selector widget
│   └── ...
│
├── export/                 # Export pipelines
│   └── godotexporter.*     # Godot Engine map/tileset exporter
│
└── import/                 # Import pipelines
    ├── importdialog.*      # RGSSAD import workflow UI
    ├── importscreenshotdialog.* # Screenshot-based tileset import
    └── tilesetcompare.*    # Tileset comparison logic
```

---

## RPG Maker XP Data Format

RPG Maker XP stores game data as serialized Ruby objects in binary `.rxdata` files. YaRXP includes a custom **parser** and **writer** that convert between this binary format and JSON (`QJsonDocument`) for in-memory manipulation.

### Directory Layout of an RPG Maker XP Project

```
MyGame/
├── Game.rxproj             # Project marker file
├── Data/
│   ├── Actors.rxdata
│   ├── Classes.rxdata
│   ├── Skills.rxdata
│   ├── Items.rxdata
│   ├── Weapons.rxdata
│   ├── Armors.rxdata
│   ├── Enemies.rxdata
│   ├── Troops.rxdata
│   ├── States.rxdata
│   ├── Animations.rxdata
│   ├── Tilesets.rxdata
│   ├── CommonEvents.rxdata
│   ├── System.rxdata
│   ├── Scripts.rxdata
│   ├── MapInfos.rxdata     # Map hierarchy and metadata
│   ├── Map001.rxdata
│   ├── Map002.rxdata
│   └── ...
└── Graphics/
    ├── Tilesets/
    ├── Autotiles/
    ├── Characters/
    ├── Battlers/
    ├── Battlebacks/
    ├── Animations/
    ├── Icons/
    ├── Pictures/
    ├── Windowskins/
    ├── Transitions/
    ├── Panoramas/
    ├── Fogs/
    ├── Titles/
    └── Gameovers/
```

### RGSSAD Archives

Some distributed RPG Maker XP projects ship as a single `.rgssad` encrypted archive file instead of loose files. YaRXP can decrypt and extract these archives via **File → Import from RGSSAD**, making the project editable. Decryption uses the standard RGSSAD XOR key (`0xDEADCAFE`).

---

## Export

### Godot Engine Export

YaRXP can export maps and tilesets to [Godot Engine](https://godotengine.org/) format via **File → Export → Godot**. The exporter:

- Converts RPG Maker tile IDs to Godot tile indices
- Handles autotile conversion
- Creates Godot-compatible directory structure
- Exports each map layer separately

---

## Import

### RGSSAD Archive Import

Use **File → Import** to open the import dialog. Select an `.rgssad` file and a destination directory. YaRXP will decrypt the archive and extract all project files so they can be loaded as a regular project.

### Screenshot-Based Tileset Import

YaRXP also includes a screenshot-based tileset importer that can analyze a screenshot of a game to assist with mapping tileset graphics to the correct tile positions.

---

## Contributing

Contributions are welcome. Please open an issue or pull request on [GitHub](https://github.com/jowin202/YaRXP).

When contributing code, please follow the existing conventions:
- Snake_case for method names
- Qt signal/slot architecture for cross-component communication
- Keep the PBS directory structure separate from the main editor code

---

## License

This project is licensed under the **GNU General Public License v3.0**. See [LICENSE](LICENSE) for the full text.
