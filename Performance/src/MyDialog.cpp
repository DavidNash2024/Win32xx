///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "PerfApp.h"
#include "resource.h"



// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	return DialogProcDefault(uMsg, wParam, lParam);
}


BOOL CMyDialog::OnInitDialog()
{
	// Put the initial values in the edit boxes
	::SetDlgItemText(GetHwnd(), IDC_WINDOWS,  _T("10"));
	::SetDlgItemText(GetHwnd(), IDC_MESSAGES, _T("1000000"));

	return true;
}

void CMyDialog::OnOK()
// This function is called when the OK button is hit
{
	// Get the number of test windows to create 
	// Note 1: A Windows limit of 10000 handles per process imposes a practical limit of aprox 1000 test windows.
	//         Refer to: http://support.microsoft.com/kb/327699
	// Note 2: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
	int nWindows = MIN(1000, GetDlgItemInt(IDC_WINDOWS, NULL, FALSE));
	
	// Get the number of test messages to send
	int nTestMessages = GetDlgItemInt(IDC_MESSAGES, NULL, FALSE);

	// Get a reference to the CMainWindow object
	CMainWindow& MainWnd = ((CPerformanceApp*)GetApp())->GetMainWnd();
	
	MainWnd.SetTestMessages(nTestMessages);
	MainWnd.CreateTestWindows(nWindows);

	// End the dialog
	EndDialog(IDOK);
}

