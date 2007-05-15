///////////////////////////////
// View.cpp

#include "resource.h"
#include "View.h"


// Note: This code requires the following libraries to be linked:
//  comsupp.lib (for ConvertStringToBSTR support)
//  atl.lib		(assuming you want to link ATL staticly)
//


// Global CAtlDummyMoldule object
//CView::CAtlDummyModule g_MyModule;

// Definitions for the CView class
CView::CView() : m_pInetExplorer(NULL)
{
	AtlAxWinInit();			// Initialise ATL
	OleInitialize(NULL);	// Initialise OLE
}

CView::~CView()
{
	if (m_pInetExplorer)
		m_pInetExplorer->Release();
	
	OleUninitialize();
}

void CView::Navigate(LPCTSTR str)
{
	// Navigate to the web page
	VARIANT vars[4];
	memset(vars,0,sizeof(vars));
	BSTR BStrURL = _com_util::ConvertStringToBSTR(str);
	HRESULT hrie = m_pInetExplorer->Navigate(BStrURL,vars,vars+1,vars+2,vars+3);
	SysFreeString(BStrURL);
}

void CView::OnCreate()
{
	// Set the window's icon
	SetIconSmall(IDW_MAIN);
	SetIconLarge(IDW_MAIN);

    LPUNKNOWN spUnk=NULL;
	USES_CONVERSION;

	// Use ATL to create the ActiveX control, initializes it, and hosts it in the specified window
	LRESULT hr = AtlAxCreateControlEx(T2OLE("about:blank"), m_hWnd, NULL, NULL, &spUnk, IID_NULL);
    
	if (SUCCEEDED(hr))
	{
		// Set the IWebBrowser2 pointer
		spUnk->QueryInterface (IID_IWebBrowser2, (LPVOID *) &m_pInetExplorer);
		spUnk->Release();
	} 
	else 
		DebugErrMsg(TEXT("Failed to create browser control"));

	TRACE("OnCreate");
}

void CView::OnDestroy()
{
	// Post a message to end the application
	::PostQuitMessage(0);
}

void CView::OnInitialUpdate()
{
	// OnInitial is called after the window is created.
	// Tasks which are to done after the window is created go here.

	TRACE("CView::OnInitialUpdate");
}

void CView::PreCreate(CREATESTRUCT& cs)
{
	// Set some optional parameters for the window
	cs.style = WS_HSCROLL| WS_VSCROLL | WS_VISIBLE | WS_CHILD;
	cs.dwExStyle = WS_EX_CLIENTEDGE;		// Extended style
	cs.lpszClass = TEXT("Win32++ Window");	// Window class name
}

LRESULT CView::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// This function is our message procedure. We process the messages for
	// the view window here, and pass the unprocessed messages to
	// CWnd::WndProc for default processing.

	switch(uMsg)
	{
	case WM_DESTROY:
		OnDestroy();
		return 0;	
	}

	// Pass unprocessed messages to CWin::WndProc
	return CWnd::WndProc(hwnd, uMsg, wParam, lParam);
}
