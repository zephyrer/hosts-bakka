;--------------------------------
;Define Settings

  !define VERSION	1.2.1


;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  
  ;Name and file
  Name "Hosts Bakka"
  OutFile "Bakka_Setup_${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Bakka"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bakka" ""
  
  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
  
  BrandingText "Hosts Bakka ${VERSION}"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
  !define MUI_FINISHPAGE_RUN "$INSTDIR\Bakka.exe"
  !define MUI_FINISHPAGE_RUN_TEXT "Run Hosts Bakka"
  
  !insertmacro MUI_PAGE_FINISH  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Bakka" SecDummy

  SetOutPath "$INSTDIR"
  
  File /a "Bakka.exe"
  
  CreateDirectory "$SMPROGRAMS\Bakka"
  CreateShortCut "$SMPROGRAMS\Bakka\Bakka.lnk" "$INSTDIR\Bakka.exe"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Bakka" "" $INSTDIR
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka" \
	"DisplayName" "Bakka (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka" \
	"UninstallString" '"$INSTDIR\Uninstall.exe"'
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
;  LangString DESC_SecDummy ${LANG_ENGLISH} ""

  ;Assign language strings to sections
;  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
;    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
;  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Bakka.exe"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$SMPROGRAMS\Bakka\Bakka.lnk"
  Delete "$SMPROGRAMS\Bakka\Bakka.lnk"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Bakka"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka"

SectionEnd