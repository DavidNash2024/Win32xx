
#include "MyPropertySheet.h"
#include "Mainfrm.h"
#include "resource.h"


CButtonPage::CButtonPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

LRESULT CButtonPage::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		PropSheet_Changed(GetParent(hWnd), hWnd);
		break;

	default:
		break;
	}

	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

CComboPage::CComboPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) : CPropertyPage(nIDTemplate, szTitle)
{
}

LRESULT CComboPage::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	// on any command notification, tell the property sheet to enable the Apply button
	case WM_COMMAND:
		PropSheet_Changed(GetParent(hWnd), hWnd);
		break;

	default:
		break;
	}

	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption /*=NULL*/, HWND hwndParent /* = NULL*/) : CPropertySheet(pszCaption, hwndParent)
{
	m_PSH.pszIcon          = MAKEINTRESOURCE(IDI_BACKCOLOR);
	m_PSH.dwFlags          = PSH_PROPSHEETPAGE | PSH_USEICONID  | PSH_USECALLBACK;
}

LRESULT CMyPropertySheet::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(hWnd, uMsg, wParam, lParam);	
}

