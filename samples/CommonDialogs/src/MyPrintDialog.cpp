/* (26-Mar-2025)                                           (MyPrintDialog.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the MyPrintDialog and MyPageSetup
    classes for applications using the Win32++ Windows interface classes. The
    MyPrintDialog class derives from the CPrintDialog and the MyPageSetup
    class derives from CPageSetupDialog.

*******************************************************************************/

#include "stdafx.h"
#include "MyPrintDialog.h"

BOOL MyPrintDialog::OnInitDialog()
{
    SetWindowText(m_sPDTitle);
    HWND hbtn = FindWindowEx(*this, nullptr, L"Button", L"OK");
    if (hbtn != nullptr)
        ::SetWindowText(hbtn, L"&Print");
    return TRUE;
}

void MyPrintDialog::OnCancel()
{
    ::MessageBox(nullptr, L"Print job cancelled.", L"Information",
        MB_OK | MB_TASKMODAL | MB_ICONINFORMATION);
}

// Override this member method to perform special processing when the printer
// box is initialized. Return TRUE.
BOOL MyPageSetup::OnInitDialog()
{
    SetWindowText(m_PSDTitle);
    return TRUE;
}

// Override this member to perform any special processing to reset the printer
// to its incoming state. For now just announce that the dialog was cancelled.
void MyPageSetup::OnCancel()
{
    ::MessageBox(nullptr, L"Page setup cancelled. ",
    L"Information", MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
}

// Record the title of the page setup dialog box after an object of this class
// is constructed, but before DoModal() is invoked.
void MyPageSetup::SetBoxTitle(LPCWSTR title)
{
    m_PSDTitle = title;
}