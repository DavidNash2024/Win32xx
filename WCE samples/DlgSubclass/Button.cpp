/////////////////////////////
// Button.cpp

#include "DlgSubclassApp.h"
#include "Button.h"

LRESULT CButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CMyDialog& dialog = GetSubApp().GetDialog();
	switch (uMsg)
	{
	case WM_MOUSEMOVE:
		TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
		dialog.SetStatic(TEXT("WM_MOUSEMOVE"));
		break;
	case WM_SETCURSOR:
		TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
		dialog.SetStatic(TEXT("WM_SETCURSOR"));
		break;
	case WM_LBUTTONDOWN:
		TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
		dialog.SetStatic(TEXT("WM_LBUTTONDOWN"));
		break;
	case WM_LBUTTONUP:
		TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
		dialog.SetStatic(TEXT("WM_LBUTTONUP"));
		break;

	default:
		TRACE("CButton::WndProc - Unspecified Message\n");
		break;
	}

	// Pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}
