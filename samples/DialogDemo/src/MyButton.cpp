/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "MyButton.h"

LRESULT CMyButton::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_MOUSEMOVE"));
	return 0L;
}

LRESULT CMyButton::OnNCHitTest(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_NCHITTEST"));

	// Pass message on for default processing
	return FinalWindowProc(WM_NCHITTEST, wParam, lParam);
}

LRESULT CMyButton::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_SETCURSOR"));
	return 0L;
}

LRESULT CMyButton::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONDOWN"));
	return 0L;
}

LRESULT CMyButton::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONUP"));
	return 0L;
}

LRESULT CMyButton::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_RBUTTONDOWN"));
	return 0L;
}

LRESULT CMyButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:		return OnMouseMove(wParam, lParam);
	case WM_NCHITTEST:		return OnNCHitTest(wParam, lParam);
	case WM_SETCURSOR:		return OnSetCursor(wParam, lParam);
	case WM_LBUTTONDOWN:	return OnLButtonDown(wParam, lParam);
	case WM_LBUTTONUP:		return OnLButtonUp(wParam, lParam);
	case WM_RBUTTONDOWN:	return OnRButtonDown(wParam, lParam);
	default:
	//	TRACE("CButton::WndProc - Unspecified Message\n");
		break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
