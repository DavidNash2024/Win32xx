/////////////////////////////
// Button.cpp

#include "stdafx.h"
#include "DialogDemoApp.h"
#include "MyButton.h"

void CMyButton::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_MOUSEMOVE Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_MOUSEMOVE"));
}

void CMyButton::OnNCHitTest(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_NCHITTEST Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_NCHITTEST"));
}

void CMyButton::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_SETCURSOR Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_SETCURSOR"));
}

void CMyButton::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_LBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONDOWN"));
}

void CMyButton::OnLButtonUp(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_LBUTTONUP Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_LBUTTONUP"));
}

void CMyButton::OnRButtonDown(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	TRACE("CButton::WndProc - WM_RBUTTONDOWN Message\n");
	CMyDialog* pDialog = GetDlgApp()->GetDialog();
	pDialog->SetStatic(_T("WM_RBUTTONDOWN"));
}

LRESULT CMyButton::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEMOVE:		OnMouseMove(wParam, lParam);	break;
	case WM_NCHITTEST:		OnNCHitTest(wParam, lParam);	break;
	case WM_SETCURSOR:		OnSetCursor(wParam, lParam);	break;
	case WM_LBUTTONDOWN:	OnLButtonDown(wParam, lParam);	break;
	case WM_LBUTTONUP:		OnLButtonUp(wParam, lParam);	break;
	case WM_RBUTTONDOWN:	OnRButtonDown(wParam, lParam);	break;
	default:
	//	TRACE("CButton::WndProc - Unspecified Message\n");
		break;
	}
	
	return WndProcDefault(uMsg, wParam, lParam);
}
