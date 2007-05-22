///////////////////////////////////////
// MyDialog.cpp


#include "PerfApp.h"
#include "MyDialog.h"
#include "resource.h"



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
	::SetDlgItemText(GetHwnd(), IDC_WINDOWS,  TEXT("100"));
	::SetDlgItemText(GetHwnd(), IDC_MESSAGES, TEXT("1000000"));

	return true;
}

void CMyDialog::OnOK()
// This function is called when the OK button is hit
{
	// Get the number of test windows to create 
	int nWindows = ::GetDlgItemInt(GetHwnd(), IDC_WINDOWS, NULL, FALSE);
	
	// Get the number of test messages to send
	int nTestMessages = ::GetDlgItemInt(GetHwnd(), IDC_MESSAGES, NULL, FALSE);

	// Get a reference to the CMainWindow object
	CMainWindow& MainWnd = ((CPerformanceApp*)GetApp())->GetMainWnd();
	
	MainWnd.SetTestMessages(nTestMessages);
	MainWnd.CreateTestWindows(nWindows);

	// End the dialog
	EndDialog(IDOK);
}

