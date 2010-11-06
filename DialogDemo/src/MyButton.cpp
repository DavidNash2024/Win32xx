/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "MyButton.h"

LRESULT CMyButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMyDialog& dialog = ((CDialogDemoApp*) GetApp())->GetDialog();
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE(_T("CButton::WndProc - WM_MOUSEMOVE Message\n"));
		dialog.SetStatic(_T("WM_MOUSEMOVE"));
		break;
	case WM_NCHITTEST:
		TRACE(_T("CButton::WndProc - WM_NCHITTEST Message\n"));
		dialog.SetStatic(_T("WM_NCHITTEST"));
		break;
	case WM_SETCURSOR:
		TRACE(_T("CButton::WndProc - WM_SETCURSOR Message\n"));
		dialog.SetStatic(_T("WM_SETCURSOR"));
		break;
	case WM_LBUTTONDOWN:
		TRACE(_T("CButton::WndProc - WM_LBUTTONDOWN Message\n"));
		dialog.SetStatic(_T("WM_LBUTTONDOWN"));
		break;
	case WM_LBUTTONUP:
		TRACE(_T("CButton::WndProc - WM_LBUTTONUP Message\n"));
		dialog.SetStatic(_T("WM_LBUTTONUP"));
		break;
	case WM_RBUTTONDOWN:
		TRACE(_T("CButton::WndProc - WM_RBUTTONDOWN Message\n"));
		dialog.SetStatic(_T("WM_RBUTTONDOWN"));
		break;

	default:
//		TRACE("CButton::WndProc - Unspecified Message");
		break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
