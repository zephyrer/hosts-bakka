// BakkaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "afxinet.h"
#include "Bakka.h"
#include "BakkaDlg.h"
#include "MyInternetSession.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_strHostFile;
CString g_strHostBackupFile;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CBakkaDlg dialog




CBakkaDlg::CBakkaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBakkaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBakkaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBakkaDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CBakkaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CBakkaDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CBakkaDlg message handlers

BOOL CBakkaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CProgressCtrl * m_ctlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_ctlProgress->SetRange(0, 100);
	m_ctlProgress->SetStep(5);

	// Get System32 Directory
	CString path;
	LPTSTR p = path.GetBuffer(MAX_PATH);
	HRESULT hr = ::SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, SHGFP_TYPE_CURRENT, p);
	if(SUCCEEDED(hr))
	{ /* succeeded */
		path.ReleaseBuffer();

#ifdef _DEBUG
		afxDump << "System32 Path:  " << path << "\n";
#endif

		path += HOST_FILE_POSTFIX;
		g_strHostFile = path;

		path += HOST_BFILE_POSTFIX;
		g_strHostBackupFile = path;

	} /* succeeded */
	else
	{ /* failed */
		path.ReleaseBuffer();

		ShowStatus(L"ERROR: Can't find windows system directory");

		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	} /* failed */

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBakkaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBakkaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBakkaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CBakkaDlg::ReadHostsAndRename(CStringArray* hosts) {

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

// When you click "Bakka!"
void CBakkaDlg::OnBnClickedOk()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	InitEditCtrlAndProgress();

	DWORD Status;
	CStringArray hosts;

	if (ReadHostsAndRename(&hosts)) {
		StepIt();
		CMyInternetSession *Session = NULL;
		Session = new CMyInternetSession(NULL);
		Status = Session->GetAndSetHosts(HOST_SERVER, HOST_PORT, HOST_SERVER_FILE, &hosts);

		if (Status == HTTP_STATUS_OK) {
			DummyAndDone(1);
		} else {
			RestoreHosts(&hosts);
		}
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);
}

// When you click "Restore"
void CBakkaDlg::OnBnClickedButton1()
{
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	InitEditCtrlAndProgress();

	ShowStatus(L"Restoring Windows hosts to factory settings");
	
	CStringArray hosts;
	if (ReadHostsAndRename(&hosts)) {
		StepIt();
		RestoreHosts(&hosts);
		StepIt();
		DummyAndDone(5);
	}

	GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
}

void CBakkaDlg::ShowEditBox(LPCTSTR str) {
	CEdit * ce;
	ce = (CEdit*) GetDlgItem(IDC_EDIT1);

	int length = ce->GetWindowTextLength();
	ce->SetSel(length, length);
	ce->ReplaceSel(str);
}

void CBakkaDlg::ShowStatus(LPCTSTR str) {
	CStatic * cs;
	cs = (CStatic*) GetDlgItem(IDC_STATIC);

	cs->SetWindowTextW(str);
}

void CBakkaDlg::StepIt() {
	CProgressCtrl * m_ctlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_ctlProgress->StepIt();
}

void CBakkaDlg::StepIt(DWORD pos) {
	CProgressCtrl * m_ctlProgress = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_ctlProgress->SetPos(pos);
}

void CBakkaDlg::InitEditCtrlAndProgress() {
	StepIt(0);

	CEdit * ce;
	ce = (CEdit*) GetDlgItem(IDC_EDIT1);
	int length = ce->GetWindowTextLength();
	ce->SetSel(0, length);
	ce->ReplaceSel(L"");

	StepIt();
}

void CBakkaDlg::DummyAndDone(int p) {
		for (int i = 0;i <= p;i++) {
			StepIt();
			Sleep(100);
		}

		StepIt(100);
		ShowStatus(L"Done");
}

void CBakkaDlg::RestoreHosts(CStringArray* hosts) {
		CStdioFile f;
		if( f.Open( g_strHostFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText, NULL ) ) {
			for (int j = 0; j < hosts->GetSize(); j++) {
				f.WriteString(hosts->GetAt(j));
			}
		}
}