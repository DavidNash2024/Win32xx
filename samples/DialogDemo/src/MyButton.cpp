/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "MyButton.h"

LRESULT CMyButton::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_MOUSEMOVE"));
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_NCHITTEST"));
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_SETCURSOR"));
	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONDOWN"));

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONUP"));

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_RBUTTONDOWN"));

	return FinalWindowProc(uMsg, wParam, lParam);
}

LRESULT CMyButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:		return OnMouseMove(uMsg, wParam, lParam);
	case WM_NCHITTEST:		return OnNCHitTest(uMsg, wParam, lParam);
	case WM_SETCURSOR:		return OnSetCursor(uMsg, wParam, lParam);
	case WM_LBUTTONDOWN:	return OnLButtonDown(uMsg, wParam, lParam);
	case WM_LBUTTONUP:		return OnLButtonUp(uMsg, wParam, lParam);
	case WM_RBUTTONDOWN:	return OnRButtonDown(uMsg, wParam, lParam);
	default:
	//	TRACE("CButton::WndProc - Unspecified Message\n");
		break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
