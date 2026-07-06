# Win32 Disk Imager

A Windows utility for reading and writing raw disk images (.img) to SD cards and USB memory devices.

Original version developed by Justin Davis <tuxdavis@gmail.com>. 
Maintained by the ImageWriter developers (http://sourceforge.net/projects/win32diskimager) by  gruemaster, tuxinator2009

## Features

- **Write** — flash an image file to a removable device
- **Read** — capture a device to an image file
- **Verify** — compare an image file against a device (byte-for-byte)
- **Hash** — generate MD5, SHA1, or SHA256 checksums of image files
- **Partition-aware read** — option to read only allocated partitions instead of the entire device
- **USB hotplug** — automatically detects when devices are inserted or removed
- **Drag and drop** — drop image files onto the file path field
- **Command-line** — pass an image file path as an argument to pre-fill the field
- **Persistent settings** — remembers the last used folder between sessions
- **Multi-language** — translations for Spanish, Italian, Polish, Dutch, German, French, Chinese (Simplified/Traditional), and Tamil

## Requirements

- Windows 7 or later
- Administrator privileges (required for raw disk access)
- **Qt 6.11.1** with MinGW 13.1.0 (for building from source)

## Quick Start

### Running the pre-built version

1. Open the `Release/` folder
2. Double-click `Win32DiskImager.exe`
3. Approve the UAC elevation prompt
4. Select an image file and a target device
5. Click Read, Write, or Verify

### Building from source

```bat
compile.bat
```

This runs `qmake` and `mingw32-make` from the `src/` directory. The built executable appears in `Release/`.

To build manually:

```bat
set PATH=C:\Qt\6.11.1\mingw_64\bin;C:\Qt\Tools\mingw1310_64\bin;%PATH%
cd src
qmake
mingw32-make
```

## Project Structure

```
Win32DiskImager/
├── src/                    # Source code
│   ├── main.cpp            # Entry point
│   ├── mainwindow.cpp/h    # Main window, UI logic, disk operations
│   ├── mainwindow.ui       # Qt Designer UI layout
│   ├── disk.cpp/h          # Low-level Win32 disk I/O wrappers
│   ├── droppablelineedit.cpp/h  # Drag-and-drop file path widget
│   ├── elapsedtimer.cpp/h  # Status bar timer widget
│   ├── DiskImager.pro      # qmake project file
│   ├── DiskImager.rc       # Windows resource file
│   ├── DiskImager.manifest # UAC manifest (requireAdministrator)
│   ├── resource.h          # Windows resource header
│   ├── gui_icons.qrc       # GUI icon resources
│   ├── translations.qrc    # Translation resources
│   └── lang/               # Translation files (.ts)
├── Release/                # Pre-built release binaries
├── CHANGELOG.md            # Version history
├── compile.bat             # Build script
├── GPL-2                   # GPL v2 license
├── LGPL-2.1                # LGPL v2.1 license (Qt runtime)
└── License.txt             # Combined license information
```

## How It Works

The application uses Windows raw disk access APIs (`CreateFile`, `DeviceIoControl`, `ReadFile`, `WriteFile`) to read and write sectors directly to physical devices. The typical operation flow is:

1. Open a handle to the target volume
2. Lock the volume (`FSCTL_LOCK_VOLUME`)
3. Dismount the volume (`FSCTL_DISMOUNT_VOLUME`)
4. Open a handle to the physical disk (`\\.\PhysicalDriveN`)
5. Read/write sectors in 1024-sector chunks
6. Unlock and close all handles

The device list shows only removable drives and USB/SD/MMC fixed drives, preventing accidental writes to internal disks.

## Known Limitations

- No CD-ROM writing support
- No USB floppy support
- No image compression (raw .img only)
- No reformat capabilities

## License

Licensed under the GNU General Public License v2. See `GPL-2` for the full text.

Qt runtime libraries are licensed under the GNU Lesser General Public License v2.1. See `LGPL-2.1`.

## Credits

Originally developed by Justin Davis. Maintained by the ImageWriter developers on SourceForge.
