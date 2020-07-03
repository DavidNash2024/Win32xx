///////////////////////////////
// View.cpp

#include "stdafx.h"
#include "resource.h"
#include "BrowserApp.h"
#include "Mainfrm.h"
#include "View.h"
#include "DispatchSink.h"


CView::CView() : m_pInetExplorer(NULL), m_pSourceUnk(0), m_pSinkUnk(0), 
                 m_adviseCookie(0)
{
    // Initialise OLE
    OleInitialize(NULL);
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

}

void CView::Navigate(LPCTSTR str)
{
    // Navigate to the web page.
    VARIANT vars[4];
    memset(vars, 0, sizeof(vars));
    CComBSTR BStrURL(str);
    m_pInetExplorer->Navigate(BStrURL, vars, vars+1, vars+2, vars+3);
    SysFreeString(BStrURL);
}

int CView::OnCreate(CREATESTRUCT& cs)
{
    UNREFERENCED_PARAMETER(cs);

    // Set the window's icon
    SetIconSmall(IDW_MAIN);
    SetIconLarge(IDW_MAIN);

    // Use ATL to create the ActiveX control, initializes it, and hosts it in the specified window.
    LRESULT hr = AtlAxCreateControlEx(TtoOLE(_T("about:blank")), *this, NULL, NULL, &m_pSourceUnk, IID_NULL);
    if (SUCCEEDED(hr))
    {
        // Set the IWebBrowser2 pointer.
        m_pSourceUnk->QueryInterface (IID_IWebBrowser2, (LPVOID *) &m_pInetExplorer);

        // Create sink object using CDispatchSink. 
        CComObject<CDispatchSink> *pSinkClass = NULL;
        hr = CComObject<CDispatchSink>::CreateInstance (&pSinkClass);
        if (SUCCEEDED(hr))
        {
            hr = pSinkClass->QueryInterface(IID_IUnknown, (LPVOID*)&m_pSinkUnk);
            if (SUCCEEDED(hr))
            {
                // Connect the event sink to our browser.
                hr = AtlAdvise(m_pSourceUnk, m_pSinkUnk, DIID_DWebBrowserEvents2, &m_adviseCookie);
            }
        }
    }

    if (FAILED(hr))
        Trace("*** Error ***  Failed to create browser and connect event sink\n\n");

    return 0;
}

void CView::OnDestroy()
{
    GetIWebBrowser2()->Stop();
    AtlUnadvise(m_pSourceUnk, DIID_DWebBrowserEvents2, m_adviseCookie);
}

void CView::OnInitialUpdate()
{
    // OnInitial is called after the window is created.
    // Tasks which are to done after the window is created go here.

    // Suppress Java script errors.
    GetIWebBrowser2()->put_Silent(VARIANT_TRUE);

    TRACE(_T("CView::OnInitialUpdate\n"));
}

void CView::PreCreate(CREATESTRUCT& cs)
{
    // Set some optional parameters for the window
    cs.style = WS_HSCROLL| WS_VSCROLL | WS_VISIBLE | WS_CHILD;
    cs.dwExStyle = WS_EX_CLIENTEDGE;        // Extended style
}

