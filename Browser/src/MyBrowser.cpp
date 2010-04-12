
#include "stdafx.h"
#include "MyBrowser.h"

// The following can also be found in exdispid.h (except MinGW)
#define DISPID_STATUSTEXTCHANGE   102
#define DISPID_DOWNLOADCOMPLETE   104
#define DISPID_COMMANDSTATECHANGE 105
#define DISPID_DOWNLOADBEGIN      106
#define DISPID_PROGRESSCHANGE     108
#define DISPID_PROPERTYCHANGE     112
#define DISPID_TITLECHANGE        113
#define DISPID_BEFORENAVIGATE2    250
#define DISPID_NEWWINDOW2         251
#define DISPID_NAVIGATECOMPLETE2  252
#define DISPID_DOCUMENTCOMPLETE   259

CEventSink::CEventSink() : m_cRefs(1), m_pSink(NULL)
{
}

STDMETHODIMP_(ULONG) CEventSink::AddRef()
{
	return ++m_cRefs;
}

STDMETHODIMP CEventSink::QueryInterface(REFIID riid, void** ppvObject)
{
	if (!ppvObject)
		return E_POINTER;

	if (IsEqualIID(riid, IID_IDispatch))
		*ppvObject = (IDispatch *)this;
	else if (IsEqualIID(riid, IID_IUnknown))
		*ppvObject = this;
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

STDMETHODIMP CEventSink::GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, unsigned int cNames, LCID lcid, DISPID* rgdispid)
{
	UNREFERENCED_PARAMETER((IID)riid);			// IID cast required for the MinGW compiler
	UNREFERENCED_PARAMETER(rgszNames);
	UNREFERENCED_PARAMETER(cNames);
	UNREFERENCED_PARAMETER(lcid);
    *rgdispid = DISPID_UNKNOWN;
    return DISP_E_UNKNOWNNAME;
}

STDMETHODIMP CEventSink::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo)
{
	UNREFERENCED_PARAMETER(itinfo);
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(pptinfo);
    return E_NOTIMPL;
}

STDMETHODIMP CEventSink::GetTypeInfoCount(unsigned int* pctinfo)
{
	UNREFERENCED_PARAMETER(pctinfo);
    return E_NOTIMPL;
}

STDMETHODIMP CEventSink::Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexecinfo, unsigned int* puArgErr)
{
	UNREFERENCED_PARAMETER((IID)riid);			// IID cast required for the MinGW compiler
	UNREFERENCED_PARAMETER(lcid);
	UNREFERENCED_PARAMETER(wFlags);
	UNREFERENCED_PARAMETER(pdispparams);
	UNREFERENCED_PARAMETER(pvarResult);
	UNREFERENCED_PARAMETER(pexecinfo);
	UNREFERENCED_PARAMETER(puArgErr);

	if (!m_pSink)
		return S_OK;

	switch (dispid)
	{
	    // Just some of the possible events ...
		case DISPID_BEFORENAVIGATE2:
			break;

		case DISPID_COMMANDSTATECHANGE:
    //        m_pSink->eventCommandStateChange(pdispparams->rgvarg[1].lVal, pdispparams->rgvarg[0].boolVal);
            break;

        case DISPID_DOCUMENTCOMPLETE:
            break;

        case DISPID_DOWNLOADBEGIN:
     //       m_pSink->eventDownloadBegin();
            break;

        case DISPID_DOWNLOADCOMPLETE:
      //      m_pSink->eventDownloadComplete();
            break;

        case DISPID_NAVIGATECOMPLETE2:
            break;

        case DISPID_NEWWINDOW2:
            break;

        case DISPID_PROGRESSCHANGE:
            break;

		case DISPID_PROPERTYCHANGE:
       //     m_pSink->eventPropertyChange(pdispparams->rgvarg[0].bstrVal);
            break;

        case DISPID_STATUSTEXTCHANGE:
    //        m_pSink->eventStatusTextChange(pdispparams->rgvarg[0].bstrVal);
            break;

        case DISPID_TITLECHANGE:
      //      m_pSink->eventTitleChange(pdispparams->rgvarg[0].bstrVal);
            break;
    }

    return S_OK;
}

STDMETHODIMP_(ULONG) CEventSink::Release()
{
	return --m_cRefs;
}


void CMyBrowser::ConnectEvents()
{
	IUnknown* pUnk = GetAXContainer().GetUnknown();
	if(!pUnk)
		return;

	IConnectionPoint* pcp;
	pcp = GetConnectionPoint(DIID_DWebBrowserEvents2);
	if(!pcp)
		return;

	pcp->Advise(&m_EventSink, &m_eventCookie);
	pcp->Release();
	pUnk->Release();
}

IConnectionPoint* CMyBrowser::GetConnectionPoint(REFIID riid)
{
	IUnknown *pUnk = GetAXContainer().GetUnknown();
	if(!pUnk)
		return NULL;

	IConnectionPointContainer* pcpc;
	IConnectionPoint* pcp = NULL;

	HRESULT hr = pUnk->QueryInterface(IID_IConnectionPointContainer, (void**)&pcpc);
	if(SUCCEEDED(hr))
	{
		pcpc->FindConnectionPoint(riid, &pcp);
		pcpc->Release();
	}

	pUnk->Release();

	return pcp;
}

void CMyBrowser::OnCreate()
{
	CWebBrowser::OnCreate();

	// Add the status bar to our window
	m_Statusbar.Create(m_hWnd);
	GetAXContainer().SetStatusWindow(m_Statusbar);

	// Enable the Event Sink
	ConnectEvents();
}

void CMyBrowser::OnInitialUpdate()
{
	Navigate(_T("http://sourceforge.net/projects/win32-framework/"));
}

void CMyBrowser::OnSize(int width, int height)
{
	// move the statusbar.
	CRect rcStatus = m_Statusbar.GetWindowRect();
	int statusHeight = rcStatus.Height();
	m_Statusbar.MoveWindow(0, height - statusHeight, width, statusHeight, TRUE);

	// reposition the container
	GetAXContainer().SetLocation(0, 0, width, height-statusHeight);
}


