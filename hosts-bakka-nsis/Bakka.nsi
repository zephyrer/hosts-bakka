;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  
  ;Name and file
  Name "Bakka"
  OutFile "Bakka Setup 1.0.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Daum\Bakka"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bakka" ""
  
  ;Request application privileges for Windows Vista
  RequestExecutionLevel user
  
  BrandingText "Daum Bakka 1.0"

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
  !define MUI_FINISHPAGE_RUN_TEXT "Bakka ½ÇÇà"
  
  !insertmacro MUI_PAGE_FINISH  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "Korean"

;--------------------------------
;Installer Sections

Section "Bakka" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE...
  File /a "Bakka.exe"
  
  CreateDirectory "$SMPROGRAMS\Daum"
  CreateShortCut "$SMPROGRAMS\Daum\Bakka.lnk" "$INSTDIR\Bakka.exe"
  
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
  LangString DESC_SecDummy ${LANG_KOREAN} "A test section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Bakka.exe"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$SMPROGRAMS\Daum\Bakka.lnk"
  Delete "$SMPROGRAMS\Daum\Bakka.lnk"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Bakka"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka"

SectionEnd