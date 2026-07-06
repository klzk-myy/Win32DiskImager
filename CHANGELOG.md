# Changelog

## 1.1.0 — Qt 6 Port (2026-07-05)

Ported from Qt 5.7 / MinGW 5.3 to Qt 6.11.1 / MinGW 13.1.0. Fixed 28 issues total.

### Breaking Changes
- Build requires Qt 6.11.1+ and MinGW 13.1.0+ (update `compile.bat` PATH)
- Administrator privileges required at runtime (UAC elevation prompt on launch)

### Bug Fixes
- **Singleton recursion crash** — `MainWindow` constructor called `getLogicalDrives()` which triggered `getInstance()` before `instance` was set, causing infinite `new MainWindow()` → stack overflow. Fixed by setting `instance = this` at top of constructor.
- **`DBTF_NET` filter inverted** — USB hotplug detection only processed network drives, not removable devices. Changed `if(DBTF_NET)` to `if(!(lpdbv->dbcv_flags & DBTF_NET))`.
- **`sectorData2` uninitialized** — Destructor checked `!= NULL` on an uninitialized pointer. Added `sectorData2 = NULL` in constructor.
- **Invalid `LPCWSTR` cast** — `getHandleOnFile(LPCWSTR(text.data()))` reinterpreted `char*` as `wchar_t*`. Replaced with `text.toStdWString().c_str()`.
- **`ReadFile`/`WriteFile` 64-bit truncation** — 64-bit sector counts silently truncated to 32-bit `DWORD`. Added explicit clamping with `bytesToRead`/`bytesToWrite` variables.
- **`getDeviceID` garbage on error** — `VOLUME_DISK_EXTENTS` struct used uninitialized if `IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS` failed. Zero-initialized with `= {}`.
- **Progress bar int overflow** — `setRange(0, (int)numsectors)` overflows for images >2.1B sectors (~1TB). Capped at `INT_MAX` with proportional scaling.
- **MBR read NULL dereference** — `on_bRead_clicked` read partition table without checking if `readSectorDataFromHandle` returned NULL. Added error dialog + cleanup.
- **Error 122 (insufficient buffer)** — `GetDisksProperty` called `IOCTL_STORAGE_CHECK_VERIFY2` with NULL buffers, triggering `ERROR_INSUFFICIENT_BUFFER` dialog on every drive. Removed the broken fallback call.
- **Hash blocks UI ("Not Responding")** — `QCryptographicHash::addData(&file)` read entire file synchronously. Now reads in 1MB chunks with `QApplication::processEvents()` between each.
- **Hash on unopenable file** — `file.open()` return was ignored; hash generated from invalid handle. Now checks return and shows error message.
- **`closeEvent` UX** — Always called `event->ignore()` even when user confirmed exit. Now accepts immediately when idle, defers only during active operations.
- **Space check assumes drive letter** — `myFile.left(3)` broke on UNC paths. Now uses `QFileInfo(myFile).absolutePath()`.

### Qt 6 API Migrations
- `QTime` → `QElapsedTimer` for elapsed timer (deprecated in Qt 5.14, removed in Qt 6)
- `QRegExp` → `QRegularExpression` for bracket removal in device text
- `Qt::AA_UseDesktopOpenGL` removed (Qt 6 handles OpenGL backend automatically)
- `nativeEvent` third parameter `long*` → `qintptr*` (64-bit ABI change)
- `QString::fromUtf16(const ushort*)` → `QString::fromWCharArray()` (15 call sites)
- `QFileDialog::setConfirmOverwrite(false)` → `setOption(QFileDialog::DontConfirmOverwrite)`
- `myHomeDir == NULL` → `myHomeDir.isEmpty()` (ambiguous operator overload)
- `tr(msg.str().c_str())` dangling pointer → `QString::fromStdString(msg.str())`

### Consistency Improvements
- All `CreateFile` calls converted to `CreateFileW` (wide string API)
- `afxres.h` replaced with `winres.h` (MinGW-compatible)
- `qulonglong` → `quint64` (Qt 6 preferred alias)
- "permision" typo fixed to "permission"

### Style Improvements
- `NULL` and bare `0` → `nullptr` throughout codebase
- Broad `#include <QtWidgets>` replaced with specific headers in all files
- Removed unused `#include <QDirIterator>` and unused `cbBytesReturned` variable

---

## 1.0.0
- Verify Image function
- SHA1 and SHA256 checksums
- Read Only Allocated Partitions
- Save last opened folder
- Additional language translations
- USB Floppy detection disabled
- Innosetup updated for Windows 10
- Qt/MinGW updated for Windows 10

## 0.9.5
- Updated copyright headers
- Converted build to Qt 5.2 / MinGW 4.8
- Added Italian translation
- Innosetup installer started

## 0.9
- Custom file dialog window

## 0.8
- Drag and drop support
- DiskImagesDir environment variable
- MD5 copy button
- Improved startup Downloads directory search

## 0.7
- Default directory path to Downloads
- Administrator privileges for Vista/7
- Translation support
- Version info in main window
