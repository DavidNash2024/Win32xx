/////////////////////////////
// EventSink.h
//

#ifndef EVENTSINK_H
#define EVENTSINK_H

#include "exdispid.h"

// Some possible IDispatch events
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


/////////////////////////////////////////////////////////////////
// CEventSink implements the event sink for our browser. An Event
// Sink provides a mechanism to respond to events triggered by an
// ActiveX control. The ActiveX browser is the event source, and
// CEventSink is the event sink.
class CEventSink : public IDispatch
{
public:
    virtual ~CEventSink() {}

    // IUnknown Methods
    virtual STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject);
    virtual STDMETHODIMP_(ULONG) AddRef();
    virtual STDMETHODIMP_(ULONG) Release();

    // IDispatch Methods
    virtual STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, unsigned int cNames, LCID lcid, DISPID* rgdispid);
    virtual STDMETHODIMP GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo);
    virtual STDMETHODIMP GetTypeInfoCount(unsigned int* pctinfo);
    virtual STDMETHODIMP Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexecinfo, unsigned int* puArgErr);
};

#endif // EVENTSINK_H
