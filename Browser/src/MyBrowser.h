
#include "WebBrowser.h"


///////////////////////////////////////////////
// About the CEventSink class.
//  The CEventSink implements the event sink for our browser. An Event Sink
//  provides a mechanism to respond to events triggered by an ActiveX control.
//  The ActiveX browser is the event source, and CMyBrowser is the event sink.
class CEventSink : public IDispatch
{
public:
	CEventSink();
	void SetSink(CWebBrowser* pSink) {m_pSink = pSink;}

	// IUnknown Methods
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	// IDispatch Methods
	STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, unsigned int cNames, LCID lcid, DISPID* rgdispid);
	STDMETHODIMP GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo);
	STDMETHODIMP GetTypeInfoCount(unsigned int* pctinfo);
	STDMETHODIMP Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexecinfo, unsigned int* puArgErr);

private:
	ULONG        m_cRefs;		// ref count
	CWebBrowser* m_pSink;		// Send the notifications here
};



//////////////////////////////////////////////////////////////
// CMyBrowser extends the capabilities provided by CWebBrowser
//  by adding a Status bar, and support for an Event Sink.
class CMyBrowser : public CWebBrowser
{
public:
	CMyBrowser() { 	m_EventSink.SetSink(this); }
	virtual ~CMyBrowser() {}

	virtual void ConnectEvents();
	virtual IConnectionPoint* GetConnectionPoint(REFIID riid);

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void OnSize(int width, int height);

private:
	CEventSink		m_EventSink;		// Routes event notifications from IWebBrowser
	CStatusbar		m_Statusbar;		// The status bar window
	DWORD			m_eventCookie;		// Token that uniquely identifies this connection

};

