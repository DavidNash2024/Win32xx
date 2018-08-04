///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT resID) : CDialog(resID)
{
    if (GetComCtlVersion() < 471)
        ::MessageBox(NULL,  _T("Date Time control not supported"), _T(""), MB_OK );
}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDC_BUTTONSET:
        {
            SYSTEMTIME month = m_monthCal.GetCurSel();      
            SYSTEMTIME all   = m_dateTime.GetTime();
            
            all.wDay = month.wDay;
            all.wMonth = month.wMonth;
            all.wYear = month.wYear;
        
            if (!SetLocalTime(&all))
            {
                MessageBox(_T("Failed to set the time\nRun app as Administrator"), _T("Set Time Failed"), MB_OK);
            }
            return TRUE;
        }

    case IDC_BUTTONCANCEL:
        {
            // Set timer for 1000 miliseconds
            SetTimer(ID_TIMER, 1000, 0);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL CMyDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach the dialog items to the CWnd objects
    AttachItem(IDC_DATETIMEPICKER1, m_dateTime);
    AttachItem(IDC_MONTHCALENDAR1, m_monthCal);

    // Set timer for 1000 miliseconds
    SetTimer(ID_TIMER, 1000, 0);

    return TRUE;
}

LRESULT CMyDialog::OnNotify(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(wparam);

    switch (((LPNMHDR)lparam)->code)
    {
    case DTN_DATETIMECHANGE:
        // Stop the timer when the DataeTime's time is changed  
        KillTimer(ID_TIMER);
        break;
    case NM_KILLFOCUS:
        // Set timer for 1000 miliseconds
        SetTimer(ID_TIMER, 1000, 0);
        break;
    }

    return 0;
}

void CMyDialog::OnOK()
{
    // This function suppresses the default OnOK
}

