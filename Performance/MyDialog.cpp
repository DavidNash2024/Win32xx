///////////////////////////////////////
// MyDialog.cpp

//#include "tchar.h"
#include "PerfApp.h"
#include "MyDialog.h"
#include "resource.h"


// Define _tstoi  -  TCHAR to int
#ifdef  _UNICODE
#define _tstoi      _wtoi
#else
#define _tstoi      atoi
#endif


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	//Use the dialogframe default message handling for remaining messages
	return CDialog::DialogProc(hwnd, uMsg, wParam, lParam);
}


BOOL CMyDialog::OnInitDialog()
{
	// Put the initial values in the edit boxes
	::SetDlgItemText(GetHwnd(), IDC_THREADS,  TEXT("100"));
	::SetDlgItemText(GetHwnd(), IDC_MESSAGES, TEXT("1000000"));

	return true;
}

void CMyDialog::OnOK()
// This function is called when the OK button is hit
{
	TCHAR szText[80];
	
	// Get the number of threads to create 
	::GetDlgItemText(GetHwnd(), IDC_THREADS, szText, 80);
	int nWindows = _tstoi(szText);
	
	// Get the number of test messages to send
	::GetDlgItemText(GetHwnd(), IDC_MESSAGES, szText, 80);
	int nTestMessages = _tstoi(szText);

	// Get a reference to the CMainWindow object
	CMainWindow& MainWnd = ((CPerformanceApp*)GetApp())->GetMainWnd();
	
	MainWnd.SetTestMessages(nTestMessages);
	MainWnd.CreateTestWindows(nWindows);

	// End the dialog
	EndDialog(IDOK);
}

