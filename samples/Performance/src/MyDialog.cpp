///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "PerfApp.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

CMyDialog::~CMyDialog()
{
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
//  switch (msg)
//  {
        //Additional messages to be handled go here
//  }

    return DialogProcDefault(msg, wparam, lparam);
}


BOOL CMyDialog::OnInitDialog()
{
    // Put the initial values in the edit boxes
    SetDlgItemText(IDC_WINDOWS,  _T("10"));
    SetDlgItemText(IDC_MESSAGES, _T("1000000"));

    return TRUE;
}

void CMyDialog::OnOK()
// This function is called when the OK button is hit
{
    // Get the number of test windows to create 
    // Note 1: A Windows limit of 10000 handles per process imposes a practical limit of aprox 1000 test windows.
    //         Refer to: http://support.microsoft.com/kb/327699
    // Note 2: Creating (or destroying) more than say 200 windows may temporarily stress the Explorer process.
    int nWindows = MIN(1000, GetDlgItemInt(IDC_WINDOWS, FALSE));
    
    // Get the number of test messages to send
    int nTestMessages = GetDlgItemInt(IDC_MESSAGES, FALSE);

    // Get a pointer to the CMainWindow object
    CMainWindow& MainWnd = GetPerfApp()->GetMainWnd();
    
    MainWnd.SetTestMessages(nTestMessages);
    MainWnd.CreateTestWindows(nWindows);

    // End the dialog
    EndDialog(IDOK);
}

