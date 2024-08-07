program control_firefox_tsf;

uses
  System.SysUtils,
  Winapi.Windows,
  Keyman.System.SettingsManager in '..\..\global\delphi\general\Keyman.System.SettingsManager.pas',
  RegistryKeys in '..\..\..\..\common\windows\delphi\general\RegistryKeys.pas',
  KeymanVersion in '..\..\..\..\common\windows\delphi\general\KeymanVersion.pas',
  Keyman.System.Settings in '..\..\global\delphi\general\Keyman.System.Settings.pas';

{$R *.res}

var
  Settings: TKeymanSettings;
  Setting: TKeymanSetting;
  IsEnabled: Boolean;
  EnabledText: string;
const
  AppName = 'Mozilla Firefox';
  AppFilename = 'firefox.exe';
begin
  Settings := TKeymanSettings.Create;
  TKeymanSettingsManager.Load(Settings);
  Setting := Settings.Find(CustomKeymanSetting_TSFApp.ID+AppFilename);
  if not Assigned(Setting) then
  begin
    Setting := TKeymanSetting.CreateCustom_TSFApp(CustomKeymanSetting_TSFApp.ID+AppFilename);
    Setting.ValueInt := 1;
    Settings.Add(Setting);
  end;

  IsEnabled := Setting.ValueInt = 1;

  if IsEnabled
    then EnabledText := 'ENABLED'
    else EnabledText := 'DISABLED';

  case MessageBox(0, PChar(
    'This application updates the settings for Keyman integration with '+AppName+'. If you are experiencing '+
    'compatibility issues, you may wish to disable Text Services Framework integration.'#13#10#13#10+
    'Keyman Text Services Framework integration for '+AppName+' is currently '+EnabledText+'.'#13#10#13#10+
    'Select YES to ENABLE Keyman Text Services Framework integration for '+AppName+'.'#13#10+
    'Select NO to DISABLE Keyman Text Services Framework integration for '+AppName+'.'),
    'Keyman Support Assistant', MB_YESNOCANCEL) of
    IDYES: Setting.ValueInt := 1;
    IDNO:  Setting.ValueInt := 0;
    IDCANCEL: Exit;
    else Exit;
  end;

  TKeymanSettingsManager.Save(Settings, True);

  Settings.Free;
end.
