/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "PerfApp.h"
#include "resource.h"

/////////////////////////////////
// CMyDialog function definitions
//

// Constructor.
CMyDialog::CMyDialog(int resID) : CDialog(resID)
{
}

// Destructor.
CMyDialog::~CMyDialog()
{
}

// Process the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {

    //  switch (msg)
    //  {
    //  Additional messages to be handled go here
    //  }

        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Put the initial values in the edit boxes
    SetDlgItemText(IDC_WINDOWS,  _T("10"));
    SetDlgItemText(IDC_MESSAGES, _T("1000000"));

    return TRUE;
}

// Called when the OK button or Enter key is pressed.
void CMyDialog::OnOK()
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

