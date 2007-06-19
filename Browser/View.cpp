///////////////////////////////
// View.cpp

#include "resource.h"
#include "Mainfrm.h"
#include "View.h"


// Note: This code requires the following libraries to be linked:
//  comsupp.lib (for ConvertStringToBSTR support)
//  atl.lib		(assuming you want to link ATL staticly)
//


#if defined (_MSC_VER) && _MSC_VER <= 1200
  CComModule _Module; // For VS Studio 6 (ATL v3.0) and below
#else
  CAtlDummyModule _Module; //  for ATL v7 and above
#endif


// Definitions for the CView class
CView::CView() : m_pInetExplorer(NULL)
{

// For VS Studio 6 (ATL v3.0) and below
#if defined (_MSC_VER) && _MSC_VER <= 1200
	_Module.Init(NULL, GetApp()->GetInstanceHandle());
#endif

	AtlAxWinInit();			// Initialise ATL
	OleInitialize(NULL);	// Initialise OLE
}

CView::~CView()
{
	if (m_pSourceUnk)
		m_pSourceUnk->Release();

	if (m_pSinkUnk)
		m_pSinkUnk->Release();

	if (m_pInetExplorer)
		m_pInetExplorer->Release();

	OleUninitialize();

#if defined (_MSC_VER) && _MSC_VER <= 1200
	_Module.Term();
#endif
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

    m_pSourceUnk = NULL;
	USES_CONVERSION;

	// Use ATL to create the ActiveX control, initializes it, and hosts it in the specified window
	LRESULT hr = AtlAxCreateControlEx(T2OLE("about:blank"), m_hWnd, NULL, NULL, &m_pSourceUnk, IID_NULL);

	if (SUCCEEDED(hr))
	{
		// Set the IWebBrowser2 pointer
		m_pSourceUnk->QueryInterface (IID_IWebBrowser2, (LPVOID *) &m_pInetExplorer);
	}
	else
		DebugErrMsg(_T("Failed to create browser control"));

	// Create sink object.  CMySink is a CComObjectRootEx-derived class
	// that implements the event interface methods.
	CComObject<CDispatchSink> *pSinkClass = NULL;
	CComObject<CDispatchSink>::CreateInstance (&pSinkClass);
	hr = pSinkClass->QueryInterface (IID_IUnknown, (LPVOID*)&m_pSinkUnk);
	_ASSERT (SUCCEEDED (hr));

	hr = AtlAdvise (m_pSourceUnk, m_pSinkUnk, DIID_DWebBrowserEvents2, &m_dwCustCookie);
	_ASSERT (SUCCEEDED (hr));
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
	cs.lpszClass = _T("Win32++ Window");	// Window class name
}

LRESULT CView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	return CWnd::WndProc(hWnd, uMsg, wParam, lParam);
}


STDMETHODIMP CDispatchSink::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
                               DISPPARAMS* pDispParams, VARIANT* pvarResult,
                               EXCEPINFO*  pExcepInfo,  UINT* puArgErr)
{
	if (!pDispParams)
		return E_INVALIDARG;

	CMainFrame* pMainFrame = (CMainFrame*)GetApp()->GetFrame();

	switch (dispidMember)
	{

	case DISPID_BEFORENAVIGATE2:
		pMainFrame->OnBeforeNavigate(pDispParams);
		break;

	case DISPID_COMMANDSTATECHANGE:
		pMainFrame->OnCommandStateChange(pDispParams);
		break;

	case DISPID_DOCUMENTCOMPLETE:
		pMainFrame->OnDocumentComplete(pDispParams);
		break;

	case DISPID_DOWNLOADBEGIN:
		pMainFrame->OnDownloadBegin(pDispParams);
		break;

	case DISPID_DOWNLOADCOMPLETE:
		pMainFrame->OnDownloadBegin(pDispParams);
		break;

	case DISPID_NAVIGATECOMPLETE2:
		pMainFrame->OnNavigateComplete2(pDispParams);
	   break;

	case DISPID_PROGRESSCHANGE:
		pMainFrame->OnProgressChange(pDispParams);
	   break;

	case DISPID_PROPERTYCHANGE:
		pMainFrame->OnPropertyChange(pDispParams);
	   break;

	case DISPID_STATUSTEXTCHANGE:
		pMainFrame->OnStatusTextChange(pDispParams);
		break;

	case DISPID_NEWWINDOW2:
		pMainFrame->OnNewWindow2(pDispParams);
		break;

	case DISPID_TITLECHANGE:
		pMainFrame->OnTitleChange(pDispParams);
	   break;
   }

   return S_OK;
}


