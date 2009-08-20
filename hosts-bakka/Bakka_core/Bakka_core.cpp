// Bakka_core.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Bakka_core.h"
#include "IniFile.h"
#include "MyInternetSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;


CString g_strHostFile;
CString g_strHostBackupFile;

CString g_strTitle, g_strRestoreBtnName;
CString g_strButtonName1, g_strButtonName2, g_strButtonName3, g_strButtonName4;
CString g_strButtonHost1, g_strButtonHost2, g_strButtonHost3, g_strButtonHost4;
CString g_strButtonFile1, g_strButtonFile2, g_strButtonFile3, g_strButtonFile4;

CIniFile iniFile(_T(".\\Bakka.ini"), 1024);
void ShowStatus(CString str)
{
	WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE),str,str.GetLength(),NULL,NULL);
}
BOOL ReadHostsAndRename(CStringArray* hosts) {

	CStdioFile f;
	CString tmp;
	BOOL bDontRead = FALSE;
	int i = 0;

	if( !f.Open( g_strHostFile, CFile::modeRead | CFile::modeNoTruncate , NULL ) ) {
		ShowStatus(_T("ERROR: Can't find windows hosts file"));
		return FALSE;
	}

	while(f.ReadString(tmp)) {
		i++;

		if (tmp.Find(BAKKA_START) != LB_ERR) {
			bDontRead = TRUE;
		}

		if (!bDontRead) {
			hosts->Add(tmp + _T("\n"));
		}

		if (tmp.Find(BAKKA_END) != LB_ERR)
		{
			bDontRead = FALSE;
		}
	}

	f.Close();

	// if you can find a backup file delete it.
	try {
		f.Remove(g_strHostBackupFile);
	} catch(CFileException* pEx) {
		pEx->Delete();
#ifdef _DEBUG
		afxDump << g_strHostBackupFile << " does not exist.\n";
#endif
	}

	try {
		f.Rename(g_strHostFile, g_strHostBackupFile);
	} catch(CFileException* pEx) {
		ShowStatus(L"ERROR: You don't have enough privileges to change Windows hosts");
		pEx->Delete();
		return FALSE;
	}

	return TRUE;
}
BOOL DnsFlushResolverCache()
{
	BOOL (WINAPI *DoDnsFlushResolverCache)();
	*(FARPROC *)&DoDnsFlushResolverCache = GetProcAddress(LoadLibrary(L"dnsapi.dll"), "DnsFlushResolverCache");
	if(!DoDnsFlushResolverCache) return FALSE;
	return DoDnsFlushResolverCache();
}
void DummyAndDone(int p) {

}
void RestoreHosts(CStringArray* hosts) {
	CStdioFile f;
	if( f.Open( g_strHostFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText, NULL ) ) {
		for (int j = 0; j < hosts->GetSize(); j++) {
			f.WriteString(hosts->GetAt(j));
		}
	}
}

/****** MAIN ******/
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		
		/************************************************************************/
		/* 초기화                                                                     */
		/************************************************************************/
		// Get System32 Directory
		CString path;
		LPTSTR p = path.GetBuffer(MAX_PATH);
		HRESULT hr = ::SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, p);
		if(SUCCEEDED(hr))
		{ /* succeeded */
			path.ReleaseBuffer();

			path += HOST_FILE_POSTFIX;
			g_strHostFile = path;

			path += HOST_BFILE_POSTFIX;
			g_strHostBackupFile = path;

		} /* succeeded */
		else
		{ /* failed */
			path.ReleaseBuffer();

			_tprintf(_T("ERROR: Can't find windows system directory"));
			return 1;
		} /* failed */

		// Read INI Information
		iniFile.GetString(L"global", L"title", g_strTitle, L"");
		if (g_strTitle == "") {
			g_strTitle = "Bakka";
		} else {
			CString t_strTitle = g_strTitle;
			g_strTitle = "Bakka - ";
			g_strTitle += t_strTitle;
		}

		iniFile.GetString(L"global", L"restore_btn_name", g_strRestoreBtnName, L"");
		
		iniFile.GetString(L"button_1", L"name", g_strButtonName1, L"");
		iniFile.GetString(L"button_1", L"host", g_strButtonHost1, L"");
		iniFile.GetString(L"button_1", L"file", g_strButtonFile1, L"");
		
		iniFile.GetString(L"button_2", L"name", g_strButtonName2, L"");
		iniFile.GetString(L"button_2", L"host", g_strButtonHost2, L"");
		iniFile.GetString(L"button_2", L"file", g_strButtonFile2, L"");
		
		iniFile.GetString(L"button_3", L"name", g_strButtonName3, L"");
		iniFile.GetString(L"button_3", L"host", g_strButtonHost3, L"");
		iniFile.GetString(L"button_3", L"file", g_strButtonFile3, L"");
		
		iniFile.GetString(L"button_4", L"name", g_strButtonName4, L"");
		iniFile.GetString(L"button_4", L"host", g_strButtonHost4, L"");
		iniFile.GetString(L"button_4", L"file", g_strButtonFile4, L"");
		

		// 옵션체크
		/************************************************************************/
		/* 옵션의 종류                                                          */
		/************************************************************************/
		/**
		* ex) bakka_core.exe BTN1
		* BTN1,BTN2,BTN3,BTN4,RESTORE
		* 
		*/


		if( argc < 2 )
		{
			_tprintf(_T("usage : bakka_core.exe [BTN1,BTN2,BTN3,BTN4,RESTORE]\n"));
			return 0;
		}

		CString act = argv[1];

		// 다운로드시 프로그레스 출력
		// @DOWNBEGIN
		// 0
		// ..
		// 100
		// @DOWNEND

		if( act == "BTN1")
		{
			DWORD Status;
			CStringArray hosts;

			if (ReadHostsAndRename(&hosts)) {
				
				CMyInternetSession *Session = NULL;
				Session = new CMyInternetSession(NULL);
				Status = Session->GetAndSetHosts(g_strButtonHost1, 80, g_strButtonFile1, &hosts);

				if (Status == HTTP_STATUS_OK) {
					DummyAndDone(1);
				} else {
					RestoreHosts(&hosts);
				}
			}

			DnsFlushResolverCache();
		}
		else if( act == "BTN2")
		{
			DWORD Status;
			CStringArray hosts;

			if (ReadHostsAndRename(&hosts)) {
				
				CMyInternetSession *Session = NULL;
				Session = new CMyInternetSession(NULL);
				Status = Session->GetAndSetHosts(g_strButtonHost2, 80, g_strButtonFile2, &hosts);

				if (Status == HTTP_STATUS_OK) {
					DummyAndDone(1);
				} else {
					RestoreHosts(&hosts);
				}
			}

			DnsFlushResolverCache();
		}
		else if( act == "BTN3")
		{
			DWORD Status;
			CStringArray hosts;

			if (ReadHostsAndRename(&hosts)) {
				
				CMyInternetSession *Session = NULL;
				Session = new CMyInternetSession(NULL);
				Status = Session->GetAndSetHosts(g_strButtonHost3, 80, g_strButtonFile3, &hosts);

				if (Status == HTTP_STATUS_OK) {
					DummyAndDone(1);
				} else {
					RestoreHosts(&hosts);
				}
			}

			DnsFlushResolverCache();
		}
		else if( act == "BTN4")
		{

			DWORD Status;
			CStringArray hosts;

			if (ReadHostsAndRename(&hosts)) {
				
				CMyInternetSession *Session = NULL;
				Session = new CMyInternetSession(NULL);
				Status = Session->GetAndSetHosts(g_strButtonHost4, 80, g_strButtonFile4, &hosts);

				if (Status == HTTP_STATUS_OK) {
					DummyAndDone(1);
				} else {
					RestoreHosts(&hosts);
				}
			}

			DnsFlushResolverCache();

		}
		else if( act == "RESTORE")
		{
			ShowStatus(L"Restoring Windows hosts to factory settings");

			CStringArray hosts;
			if (ReadHostsAndRename(&hosts)) {
				
				RestoreHosts(&hosts);
				
				DummyAndDone(5);
			}

			DnsFlushResolverCache();
		}
		// HOST내용출력
		// @HOSTSBEGIN
		// ...
		// @END
	}

	return nRetCode;
}
