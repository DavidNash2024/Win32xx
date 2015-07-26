///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID) : CDialog(nResID)
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

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_TIMER:	
		if (wParam == ID_TIMER)
		{
			// Update the time displayed every second
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			m_DateTime.SetTime(GDT_VALID, &lt);

			return 0;
		}
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CMyDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTONSET:
		{
			SYSTEMTIME month, all;
			
			m_MonthCal.GetCurSel(&month);
			m_DateTime.GetTime(&all);
			
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
	AttachItem(IDC_DATETIMEPICKER1, m_DateTime);
	AttachItem(IDC_MONTHCALENDAR1, m_MonthCal);

	// Set timer for 1000 miliseconds
	SetTimer(ID_TIMER, 1000, 0);

	return true;
}

LRESULT CMyDialog::OnNotify(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	switch (((LPNMHDR)lParam)->code)
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

	return 0L;
}

void CMyDialog::OnOK()
{
	// This function suppresses the default OnOK
}

