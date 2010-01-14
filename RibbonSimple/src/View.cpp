///////////////////////////////
// View.cpp

#include "View.h"
#include "resource.h"


// Definitions for the CView class
void CView::OnCreate()
{
	// OnCreate is called automatically during window creation when a
	// WM_CREATE message received.

	// Tasks such as setting the icon, creating child windows, or anything
	// associated with creating windows are normally performed here.

	// Set the window's icon
	SetIconSmall(IDI_SMALL);
	SetIconLarge(IDI_SIMPLERIBBON);

	TRACE(_T("OnCreate\n"));
}

void CView::OnDestroy()
{
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

LRESULT CView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (!m_Ribbon.InitializeFramework(m_hWnd)) 
            return -1;

        break;
    case WM_DESTROY:
		m_Ribbon.DestroyFramework();
        PostQuitMessage(0);
        break;
    default:
		return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
	
	return WndProcDefault(uMsg, wParam, lParam);
}

