#ifndef EVENTSINK_H
#define EVENTSINK_H

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


class CMainFrame;   // Forward declaration

///////////////////////////////////////////////
// About the CEventSink class.
//  The CEventSink implements the event sink for our browser. An Event Sink
//  provides a mechanism to respond to events triggered by an ActiveX control.
//  The ActiveX browser is the event source, and CMainFrame is the event sink.
class CEventSink : public IDispatch
{
public:
    CEventSink();
    void SetSink(CMainFrame* pSink) {m_pSink = pSink;}

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
    ULONG       m_refcount;    // ref count
    CMainFrame* m_pSink;    // Send the notifications here
};

#endif // EVENTSINK_H
