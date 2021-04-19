unit Keyman.Configuration.System.TIPMaintenance;

interface

uses
  System.Classes,

  KeymanAPI_TLB;

type
  TTIPMaintenance = class
  public
    /// <summary>Install a TIP for the current user (user context)</summary>
    class function DoInstall(const KeyboardID, BCP47Tag: string): Boolean;

    /// <summary>Install a TIP for the current user (user context)</summary>
    class function InstallTip(LangID: Integer; const KeyboardID, BCP47Tag, KeyboardToRemove: string): Boolean;

    /// <summary>Install TIPs for packages for the current user (user context)</summary>
    class function InstallTipsForPackages(Packages: TStrings): Boolean; static;

    /// <summary>Install TIP for a package for the current user (user context)</summary>
    class function InstallTipForPackage(const PackageFilename, BCP47Tag: string): Boolean; static;

    /// <summary>Register TIP for the local machine (elevated context)</summary>
    class function DoRegister(const KeyboardID, BCP47Tag: string): Boolean;

    /// <summary>Register TIP for the local machine (elevated context)</summary>
    class function RegisterTip(LangID: Integer; const KeyboardID, BCP47Tag: string): Boolean;

    /// <summary>Helper function to get default BCP47 tag for an installed keyboard</summary>
    class function GetFirstLanguage(Keyboard: IKeymanKeyboardInstalled): string; overload;
    class function GetFirstLanguage(Keyboard: IKeymanKeyboardFile): string; overload;

    /// <summary>Get the BCP47 tag for the user's default language</summary>
    class function GetUserDefaultLanguage: string; static;
  private
    class function GetKeyboardLanguage(const KeyboardID,
      BCP47Tag: string): IKeymanKeyboardLanguageInstalled; static;
  end;

implementation

uses
  System.SysUtils,
  System.Win.Registry,
  Winapi.Windows,

  Keyman.System.LanguageCodeUtils,

  BCP47Tag,
  glossary,
  kmint,
  RegistryKeys,
  utilkmshell,
  utilexecute,
  utilsystem;

{ TTIPMaintenance }

class function TTIPMaintenance.InstallTipsForPackages(Packages: TStrings): Boolean;
var
  i: Integer;
begin
  Result := True;
  for i := 0 to Packages.Count - 1 do
    // We'll attempt every package but return failure if any of them have issues
    Result := InstallTipForPackage(Packages.Names[i], Packages.ValueFromIndex[i]) and Result;
end;

class function TTIPMaintenance.InstallTipForPackage(const PackageFilename, BCP47Tag: string): Boolean;
var
  PackageID: string;
  pack: IKeymanPackageInstalled;
begin
  // This function has a known limitation: if a package contains more than one keyboard,
  // then the BCP47 association will be made only for the first keyboard. This is
  // considered an acceptable limitation at this time

  PackageID := ChangeFileExt(ExtractFileName(PackageFilename), '');
  pack := kmcom.Packages[PackageID];
  if not Assigned(pack) then
    Exit(False);

  if pack.Keyboards.Count = 0 then
    Exit(False);

  if BCP47Tag = ''
    then Result := DoInstall(pack.Keyboards[0].ID, GetFirstLanguage(pack.Keyboards[0] as IKeymanKeyboardInstalled))
    else Result := DoInstall(pack.Keyboards[0].ID, BCP47Tag);
end;

class function TTIPMaintenance.InstallTip(LangID: Integer; const KeyboardID, BCP47Tag,
  KeyboardToRemove: string): Boolean;
var
  lang: IKeymanKeyboardLanguageInstalled;
begin
  lang := GetKeyboardLanguage(KeyboardID, BCP47Tag);
  if lang = nil then
    Exit(False);

  // TODO: can this fail?
  (lang as IKeymanKeyboardLanguageInstalled2).InstallTip(LangID, KeyboardToRemove);
  Result := True;
end;

class function TTIPMaintenance.RegisterTip(LangID: Integer; const KeyboardID,
  BCP47Tag: string): Boolean;
var
  lang: IKeymanKeyboardLanguageInstalled;
begin
  lang := GetKeyboardLanguage(KeyboardID, BCP47Tag);
  if lang = nil then
    Exit(False);

  // TODO: can this fail?
  (lang as IKeymanKeyboardLanguageInstalled2).RegisterTip(LangID);
  Result := True;
end;

class function TTIPMaintenance.DoInstall(const KeyboardID,
  BCP47Tag: string): Boolean;
var
  lang: IKeymanKeyboardLanguageInstalled;
  RegistrationRequired: WordBool;
  TemporaryKeyboardID: WideString;
  LangID: Integer;
  childExitCode: Cardinal;
begin
  lang := GetKeyboardLanguage(KeyboardID, (kmcom as IKeymanBCP47Canonicalization).GetCanonicalTag(BCP47Tag));
  if lang = nil then
    // The keyboard was not found
    Exit(False);

  if lang.IsInstalled then
    // After canonicalization, we may find the language is already installed
    Exit(True);

  TemporaryKeyboardID := '';
  LangID := 0;
  RegistrationRequired := False;

  if not (lang as IKeymanKeyboardLanguageInstalled2).FindInstallationLangID(LangID, TemporaryKeyboardID, RegistrationRequired, kifInstallTransientLanguage) then
  begin
    // We were not able to find a TIP, perhaps all transient TIPs have been used
    Exit(False);
  end;

  if RegistrationRequired then
  begin
    // This calls back into TTIPMaintenance.RegisterTip
    if WaitForElevatedConfiguration(0, '-register-tip '+IntToHex(LangID,4)+' "'+KeyboardID+'" "'+lang.BCP47Code+'"') <> 0 then
      Exit(False);
  end;

  // This calls back into TTIPMaintenance.InstallTip
  if not TUtilExecute.WaitForProcess('"'+ParamStr(0)+'" -install-tip '+IntToHex(LangID,4)+' "'+KeyboardID+'" "'+lang.BCP47Code+'" "'+TemporaryKeyboardID+'"',
      GetCurrentDir, childExitCode) or
    (childExitCode <> 0) then
  begin
    kmcom.Refresh;
    Exit(False);
  end;

  kmcom.Refresh;
  Result := True;
end;

function GetDefaultHKL: HKL;
begin
  if not SystemParametersInfo(SPI_GETDEFAULTINPUTLANG, 0, @Result, 0) then
    Result := 0;
end;

class function TTIPMaintenance.DoRegister(const KeyboardID,
  BCP47Tag: string): Boolean;
var
  lang: IKeymanKeyboardLanguageInstalled;
  LangID: Integer;
  TemporaryKeyboardID: WideString;
  RegistrationRequired: WordBool;
begin
  lang := GetKeyboardLanguage(KeyboardID, (kmcom as IKeymanBCP47Canonicalization).GetCanonicalTag(BCP47Tag));
  if lang = nil then
    // The keyboard was not found
    Exit(False);

  if lang.IsInstalled or (lang as IKeymanKeyboardLanguageInstalled2).IsRegistered then
    // After canonicalization, we may find the language is already installed
    Exit(True);

  TemporaryKeyboardID := '';
  LangID := 0;
  RegistrationRequired := False;

  if (lang as IKeymanKeyboardLanguageInstalled2).FindInstallationLangID(LangID, TemporaryKeyboardID, RegistrationRequired, 0) then
  begin
    Result := not RegistrationRequired or RegisterTip(LangID, KeyboardID, lang.BCP47Code);
  end
  else
    Result := False;
end;

class function TTIPMaintenance.GetUserDefaultLanguage: string;
var
  r: TRegistry;
  tags: TStringList;
  v: string;
  keys: TStringList;
  key: string;
begin
  // Fallback result
  Result := TLanguageCodeUtils.TranslateWindowsLanguagesToBCP47(HKLToLanguageID(GetDefaultHKL));

  // For Win10, look in CPL/International/UserProfile
  r := TRegistry.Create;
  try
    if not r.OpenKeyReadOnly(SRegKey_ControlPanelInternationalUserProfile) then
      Exit;
    if r.ValueExists(SRegValue_CPIUP_InputMethodOverride) then
    begin
      // Lookup the override input method BCP 47 tag
      v := r.ReadString(SRegValue_CPIUP_InputMethodOverride);
      keys := TStringList.Create;
      try
        r.GetKeyNames(keys);
        for key in keys do
        begin
          if r.OpenKeyReadOnly('\' + SRegKey_ControlPanelInternationalUserProfile + '\' + key) and
            r.ValueExists(v) then
          begin
            Result := key;
            Break;
          end;
        end;
      finally
        keys.Free;
      end;
    end
    else if r.ValueExists(SRegValue_CPIUP_Languages) then
    begin
      // The first tag is the default language tag
      tags := TStringList.Create;
      try
        r.ReadMultiString(SRegValue_CPIUP_Languages, tags);
        if tags.Count > 0 then
        begin
          Result := tags[0].Trim;
        end;
      finally
        tags.Free;
      end;
    end;
  finally
    r.Free;
  end;
end;

class function TTIPMaintenance.GetFirstLanguage(Keyboard: IKeymanKeyboardFile): string;
begin
  if Keyboard.Languages.Count > 0
    then Result := Keyboard.Languages[0].BCP47Code
    else Result := GetUserDefaultLanguage;
end;

class function TTIPMaintenance.GetFirstLanguage(Keyboard: IKeymanKeyboardInstalled): string;
begin
  if Keyboard.Languages.Count > 0
    then Result := Keyboard.Languages[0].BCP47Code
    else Result := GetUserDefaultLanguage;
end;

class function TTIPMaintenance.GetKeyboardLanguage(const KeyboardID,
  BCP47Tag: string): IKeymanKeyboardLanguageInstalled;
var
  keyboard: IKeymanKeyboardInstalled;
  i: Integer;
begin
  keyboard := kmcom.Keyboards[KeyboardID];
  if keyboard = nil then
    Exit(nil);

  for i := 0 to keyboard.Languages.Count - 1 do
    if SameText(keyboard.Languages[i].BCP47Code, BCP47Tag) then
      Exit(keyboard.Languages[i]);

  Result := (keyboard.Languages as IKeymanKeyboardLanguagesInstalled2).Add(BCP47Tag);
end;

end.
