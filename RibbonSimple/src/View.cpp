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

	if (m_Ribbon.CreateRibbon(this))
		TRACE(_T("Ribbon Created Succesfully\n"));
	else
		TRACE(_T("Failed to create ribbon\n"));

}

void CView::OnDestroy()
{
	m_Ribbon.DestroyRibbon();

	// End the application when the window is destroyed
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitialUpdate is called after the window is created.
	// Tasks which are to be done after the window is created go here.

	TRACE(_T("OnInitialUpdate\n"));
}

void CView::OnPaint(HDC hDC)
{
	// OnPaint is called automatically whenever a part of the
	// window needs to be repainted.

	// Centre some text in our view window
	CRect r = GetClientRect();
	r.top += GetRibbonHeight();
	::DrawText(hDC, _T("Simple Ribon Demo"), -1, &r, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CView::OnSize()
{
	CRect r = GetClientRect();
	r.top += GetRibbonHeight();
	InvalidateRect(&r);
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

HRESULT CView::RibbonOnViewChanged(UINT viewId, UINT typeId, void* pView, UINT verb, INT uReasonCode)
{
		UNREFERENCED_PARAMETER(viewId);
		UNREFERENCED_PARAMETER(uReasonCode);

		HRESULT hr = E_NOTIMPL;

		// Checks to see if the view that was changed was a Ribbon view.
		if (UI_VIEWTYPE_RIBBON == typeId)
		{
			switch (verb)
			{           
				// The view was newly created.
			case UI_VIEWVERB_CREATE:
				hr = S_OK;
				break;

				// The view has been resized.  For the Ribbon view, the application should
				// call GetHeight to determine the height of the ribbon.
			case UI_VIEWVERB_SIZE:
				{
					IUIRibbon* pRibbon = NULL;
					UINT uRibbonHeight;

					hr = ((IUnknown*)pView)->QueryInterface(IID_PPV_ARGS(&pRibbon));
					if (SUCCEEDED(hr))
					{
						// Call to the framework to determine the desired height of the Ribbon.
						hr = pRibbon->GetHeight(&uRibbonHeight);
						m_uRibbonHeight = uRibbonHeight;
						pRibbon->Release();
						// Use the ribbon height to position controls in the client area of the window.
					}
				}
				break;
				// The view was destroyed.
			case UI_VIEWVERB_DESTROY:
				hr = S_OK;
				break;
			}
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
	case WM_SIZE:
		OnSize();
		break;	// and also do default processing for this message
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

