/////////////////////////////
// MyDialog.cpp
//

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"

//////////////////////////////////////
// Definitions for the CMyDialog class
//

// Constructor.
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Processes the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_TIMER:
            if (wparam == ID_TIMER)
            {
                // Update the time displayed every second
                SYSTEMTIME lt;
                GetLocalTime(&lt);
                m_dateTime.SetTime(lt);

                return 0;
            }
            break;

        default: break;
        }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}

// Called when a control sends a command notification (WM_COMMAND).
BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch(id)
    {
    case IDC_BUTTONCANCEL:  return OnButtonCancel();
    case IDC_BUTTONSET:     return OnButtonSet();

    default: return FALSE;
    }
}

BOOL CMyDialog::OnButtonCancel()
{
    TRACE(L"Cancel Button Pressed\n");
    OnCancel();
    return TRUE;
}

BOOL CMyDialog::OnButtonSet()
{
    SYSTEMTIME month = m_monthCal.GetCurSel();
    SYSTEMTIME all = m_dateTime.GetTime();

    all.wDay = month.wDay;
    all.wMonth = month.wMonth;
    all.wYear = month.wYear;

    if (!SetLocalTime(&all))
    {
        MessageBox(L"Failed to set the time\nRun app as Administrator",
            L"Set Time Failed", MB_OK);
    }
    return TRUE;
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the dialog items to the CWnd objects
    AttachItem(IDC_DATETIMEPICKER1, m_dateTime);
    AttachItem(IDC_MONTHCALENDAR1, m_monthCal);

    // Set timer for 1000 milliseconds
    SetTimer(ID_TIMER, 1000, 0);

    return TRUE;
}

// Called when a notification (WM_NOTIFY) is received.
LRESULT CMyDialog::OnNotify(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(wparam);

    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    case DTN_DATETIMECHANGE:   return KillTimer(ID_TIMER);
    case NM_KILLFOCUS:         return SetTimer(ID_TIMER, 1000, 0);

    default: return 0;
    }
}

// Called when the enter key is pressed.
void CMyDialog::OnOK()
{
    // This function suppresses the default OnOK
}

