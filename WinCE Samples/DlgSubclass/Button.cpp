/////////////////////////////
// Button.cpp

#include "DlgSubclassApp.h"
#include "Button.h"

LRESULT CButton::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMyDialog& dialog = ((CDlgSubclassApp*) GetApp())->GetDialog();
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
//		TRACE(TEXT("CButton::WndProc - WM_MOUSEMOVE Message"));
		dialog.SetStatic(TEXT("WM_MOUSEMOVE"));
		break;
	case WM_SETCURSOR:
//		TRACE(TEXT("CButton::WndProc - WM_SETCURSOR Message"));
		dialog.SetStatic(TEXT("WM_SETCURSOR"));
		break;
	case WM_LBUTTONDOWN:
//		TRACE(TEXT("CButton::WndProc - WM_LBUTTONDOWN Message"));
		dialog.SetStatic(TEXT("WM_LBUTTONDOWN"));
		break;
	case WM_LBUTTONUP:
//		TRACE(TEXT("CButton::WndProc - WM_LBUTTONUP Message"));
		dialog.SetStatic(TEXT("WM_LBUTTONUP"));
		break;

	default:
//		TRACE("CButton::WndProc - Unspecified Message");
		break;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(hwnd, uMsg, wParam, lParam);
}
