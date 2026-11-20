; Instalador de FlatCAM para Windows (Inno Setup 6). Lo llama tools/package_windows.ps1 con
; /DAppVersion=<versión> /DSourceDir=<carpeta armada> /DOutputDir=<salida>.
#ifndef AppVersion
  #define AppVersion "0.0.0"
#endif

[Setup]
AppId={{6E0B7C1A-3D52-4F7E-9A61-2C8B0F4D9E17}
AppName=FlatCAM
AppVersion={#AppVersion}
AppVerName=FlatCAM {#AppVersion}
AppPublisher=dnasta
AppPublisherURL=https://github.com/dnasta/flatcam-cpp
AppSupportURL=https://github.com/dnasta/flatcam-cpp/issues
DefaultDirName={autopf}\FlatCAM
DefaultGroupName=FlatCAM
DisableProgramGroupPage=yes
LicenseFile={#SourceDir}\LICENSE
OutputDir={#OutputDir}
OutputBaseFilename=FlatCAM-{#AppVersion}-Windows-x64-setup
SetupIconFile={#SourcePath}\..\..\src\gui\resources\images\flatcam_icon256.ico
UninstallDisplayIcon={app}\FlatCAM.exe
Compression=lzma2/max
SolidCompression=yes
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0.17763
PrivilegesRequiredOverridesAllowed=dialog
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "german"; MessagesFile: "compiler:Languages\German.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "turkish"; MessagesFile: "compiler:Languages\Turkish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\FlatCAM"; Filename: "{app}\FlatCAM.exe"
Name: "{autodesktop}\FlatCAM"; Filename: "{app}\FlatCAM.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\FlatCAM.exe"; Description: "{cm:LaunchProgram,FlatCAM}"; Flags: nowait postinstall skipifsilent
