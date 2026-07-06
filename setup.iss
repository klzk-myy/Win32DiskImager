; Win32 Disk Imager - Inno Setup Script
; Updated for Qt 6.11.1 / MinGW 13.1.0
#define MyAppSourceDir AddBackslash(SourcePath) + "Release"
#define MyAppExeName "Win32DiskImager.exe"
#define MyAppExeFile AddBackslash(MyAppSourceDir) + MyAppExeName
#define MyAppName "Win32 Disk Imager"
#define MyAppVersion "1.1.0"
#define MyAppPublisher "ImageWriter Developers"
#define MyAppURL "http://win32diskimager.sourceforge.net"

[Setup]
AppId={{3DFFA293-DF2C-4B23-92E5-3433BDC310E1}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf32}\ImageWriter
DefaultGroupName=Image Writer
LicenseFile=License.txt
OutputBaseFilename={#MyAppName}-setup-{#MyAppVersion}
SetupIconFile=src\images\setup.ico
Compression=lzma2
SolidCompression=yes
PrivilegesRequired=admin
MinVersion=6.1

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main executable
Source: "Release\Win32DiskImager.exe"; DestDir: "{app}"; Flags: ignoreversion

; Qt runtime DLLs
Source: "Release\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\Qt6Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\Qt6Svg.dll"; DestDir: "{app}"; Flags: ignoreversion

; MinGW runtime
Source: "Release\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "Release\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion

; Qt plugins
Source: "Release\platforms\*.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "Release\styles\*.dll"; DestDir: "{app}\styles"; Flags: ignoreversion
Source: "Release\imageformats\*.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "Release\generic\*.dll"; DestDir: "{app}\generic"; Flags: ignoreversion
Source: "Release\iconengines\*.dll"; DestDir: "{app}\iconengines"; Flags: ignoreversion
Source: "Release\networkinformation\*.dll"; DestDir: "{app}\networkinformation"; Flags: ignoreversion
Source: "Release\tls\*.dll"; DestDir: "{app}\tls"; Flags: ignoreversion

; Documentation
Source: "CHANGELOG.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "GPL-2"; DestDir: "{app}"; Flags: ignoreversion
Source: "LGPL-2.1"; DestDir: "{app}"; Flags: ignoreversion
Source: "README.md"; DestDir: "{app}"; Flags: ignoreversion isreadme

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent runascurrentuser
