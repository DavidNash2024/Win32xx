///////////////////////////////
// View.cpp

#include "View.h"
#include "resource.h"
#include "RibbonUI.h"

// Definitions for the CView class

void CView::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	TRACE(_T("OnCreate\n"));
	
	// Set the window's icon
	SetIconSmall(IDI_SMALL);
	SetIconLarge(IDI_SIMPLERIBBON);

	if (m_RibbonApp.CreateRibbon(this))
		TRACE(_T("Ribbon Created Succesfully\n"));
	else
		TRACE(_T("Failed to create ribbon\n"));

}

void CView::OnDestroy()
{
	m_RibbonApp.DestroyRibbon();

	// End the application when the window is destroyed
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	TRACE(_T("OnInitialUpdate\n"));
}

void CView::PreCreate(CREATESTRUCT& cs)
{
	// This function will be called automatically by Create. It provides an
	// opportunity to set various window parameters prior to window creation.
	// You are not required to set these parameters, any paramters which
	// aren't specified are set to reasonable defaults.

	// Set some optional parameters for the window
	cs.dwExStyle = WS_EX_CLIENTEDGE;		// Extended style
	cs.lpszClass = _T("View Window");		// Window Class
	cs.x = 50;								// top x
	cs.y = 50;								// top y
	cs.cx = 400;							// width
	cs.cy = 300;							// height
	cs.lpszName = LoadString(IDS_APP_TITLE);// Window title
}

//HRESULT CView::OnRibbonExecute(UINT nCmdID, UI_EXECUTIONVERB verb, __in_opt const PROPERTYKEY* key, __in_opt const PROPVARIANT* ppropvarValue, 
//											  __in_opt IUISimplePropertySet* pCommandExecutionProperties)
HRESULT CView::RibbonExecute(UINT nCmdID, UINT verb, LPCVOID key, LPCVOID ppropvarValue, LPVOID pCommandExecutionProperties)
{
	UNREFERENCED_PARAMETER(pCommandExecutionProperties);
	UNREFERENCED_PARAMETER(ppropvarValue);
	UNREFERENCED_PARAMETER(key);
	UNREFERENCED_PARAMETER(verb);

	// Use the following casts for void pointers:
	//   const PROPERTYKEY* key
	//   const PROPVARIANT* ppropvarValue
	//   IUISimplePropertySet* pCommandExecutionProperties

	HRESULT hr = S_OK;
	switch(nCmdID)
	{
	case cmdButton1:
		TRACE(_T("Button 1\n"));
		break;
	case cmdButton2:
		TRACE(_T("Button 2\n"));
		break;
	case cmdButton3:
		TRACE(_T("Button 3\n"));
		break;
	case cmdButton4:
		TRACE(_T("Button 4\n"));
		break;
	case cmdButton5:
		TRACE(_T("Button 5\n"));
		break;
	case cmdButton6:
		TRACE(_T("Button 6\n"));
		break;
	case cmdToggleButton1:
		TRACE(_T("Toggle Button 1\n"));
		break;
	case cmdToggleButton2:
		TRACE(_T("Toggle Button 2\n"));
		break;
	case IDC_CMD_EXIT:
		TRACE(_T("Exit button\n"));
		break;
	default:
		TRACE(_T("Unknown button\n"));
		break;
	}

	return hr; 
}

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		OnDestroy();
		break;
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

