# Developer Guide

## Getting the Source

```bat
git clone <repository-url>
cd Win32DiskImager
```

### Contributing Changes

1. Create a fork of the repository
2. Clone your fork locally
3. Create a feature branch: `git checkout -b feature-name`
4. Make your changes and commit: `git commit -m "Description of changes"`
5. Push to your fork: `git push origin feature-name`
6. Open a pull request against the master branch

## Requirements

| Component | Version |
|-----------|---------|
| Qt | 6.11.1 |
| MinGW | 13.1.0 |
| Windows SDK | 7.0+ |
| OS | Windows 7+ |

## Installing Qt

1. Download the Qt Online Installer from https://www.qt.io/download-open-source
2. Run the installer and follow the prompts
3. During component selection:
   - Select **Qt 6.11.1** → **MinGW 13.1.0 64-bit**
   - Select **Tools** → **MinGW 13.1.0**
4. Default installation path: `C:\Qt`

## Building

### Quick build

```bat
compile.bat
```

### Manual build

```bat
set PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%
cd src
qmake
mingw32-make
```

The built executable and dependencies are output to the `Release/` directory.

### Clean build

```bat
cd src
mingw32-make clean
qmake
mingw32-make
```

## Debugging

Win32 Disk Imager requires administrator privileges, so Qt Creator must be run as administrator:

1. Right-click Qt Creator → Run as administrator
2. File → Open Project → browse to `src/DiskImager.pro`
3. Configure the project when prompted
4. Press F5 to start debugging

## Project Layout

| Path | Description |
|------|-------------|
| `src/main.cpp` | Application entry point |
| `src/mainwindow.cpp/h` | Main window, UI logic, disk operations |
| `src/disk.cpp/h` | Win32 API wrappers for disk I/O |
| `src/droppablelineedit.cpp/h` | Drag-and-drop file path widget |
| `src/elapsedtimer.cpp/h` | Status bar timer widget |
| `src/mainwindow.ui` | Qt Designer form |
| `src/DiskImager.pro` | qmake project file |
| `src/DiskImager.rc` | Windows resource file |
| `src/DiskImager.manifest` | UAC manifest |
| `src/lang/` | Translation source files (.ts) |
| `src/images/` | Application icons |

## Adding a Translation

1. Install Qt as described above
2. Open Qt Linguist from the Start Menu
3. File → Open → select `src/lang/diskimager_en.ts`
4. Edit → Translation File Settings → set target language
5. File → Save As → `src/lang/diskimager_??.ts` (use ISO 639-1 code)
6. Translate all strings, save the file
7. Add the language to `DiskImager.pro`:
   ```
   LANGUAGES += xx
   ```
8. Rebuild to compile the `.ts` file into a `.qm` binary

## Architecture Notes

- **Singleton pattern** — `MainWindow` uses `getInstance()` with `instance = this` at the top of the constructor to prevent recursion when `getInstance()` is called during construction (e.g., from error dialogs in `getLogicalDrives()`).
- **Raw disk access** — Uses Win32 `CreateFile`/`ReadFile`/`WriteFile` on `\\.\PhysicalDriveN` handles. Requires administrator privileges via UAC manifest.
- **Volume lock sequence** — `FSCTL_LOCK_VOLUME` → `FSCTL_DISMOUNT_VOLUME` → raw I/O → `FSCTL_UNLOCK_VOLUME`. This ensures exclusive access during read/write operations.
- **ANSI build** — `DEFINES -= UNICODE` in the `.pro` file. All file paths use `char*`/wide string conversion at API boundaries.
- **USB hotplug** — `nativeEvent` handles `WM_DEVICECHANGE` / `DBT_DEVICEARRIVAL` / `DBT_DEVICEREMOVECOMPLETE`. Filters out network drives via `!(dbcv_flags & DBTF_NET)`.
- **Progress bar** — Capped at `INT_MAX` with proportional scaling for images exceeding ~1TB (2.1B sectors).
