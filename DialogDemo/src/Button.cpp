/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "Button.h"

LRESULT CButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMyDialog& dialog = ((CDlgSubclassApp*) GetApp())->GetDialog();
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
		dialog.SetStatic("WM_MOUSEMOVE");
		break;
	case WM_NCHITTEST:
		TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
		dialog.SetStatic("WM_NCHITTEST");
		break;
	case WM_SETCURSOR:
		TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
		dialog.SetStatic("WM_SETCURSOR");
		break;
	case WM_LBUTTONDOWN:
		TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
		dialog.SetStatic("WM_LBUTTONDOWN");
		break;
	case WM_LBUTTONUP:
		TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
		dialog.SetStatic("WM_LBUTTONUP");
		break;	

	default:
//		TRACE("CButton::WndProc - Unspecified Message");
		break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
