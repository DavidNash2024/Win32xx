/////////////////////////////
// EventSink.cpp
//

#include "stdafx.h"
#include "EventSink.h"
#include "MainFrm.h"

//////////////////////////////////
// CEventSink function definitions
//

// Not implemented. Increments a reference count.
STDMETHODIMP_(ULONG) CEventSink::AddRef()
{
    // Always 1. Automatic deletion isn't required.
    return 1;
}

// Assigns a pointer to ppvObject for the specified interface.
STDMETHODIMP CEventSink::QueryInterface(REFIID riid, void** ppvObject)
{
    if (!ppvObject)
        return E_POINTER;

    if (IsEqualIID(riid, IID_IDispatch))
        *ppvObject = static_cast<IDispatch *>(this);
    else if (IsEqualIID(riid, IID_IUnknown))
        *ppvObject = static_cast<IUnknown*>(this);
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    return S_OK;
}

// Not implemented. Retrieves an array of DISPID.
STDMETHODIMP CEventSink::GetIDsOfNames(REFIID riid, OLECHAR** rgszNames, unsigned int cNames, LCID lcid, DISPID* rgdispid)
{
    UNREFERENCED_PARAMETER((IID)riid);          // IID cast required for the MinGW compiler
    UNREFERENCED_PARAMETER(rgszNames);
    UNREFERENCED_PARAMETER(cNames);
    UNREFERENCED_PARAMETER(lcid);
    *rgdispid = DISPID_UNKNOWN;
    return DISP_E_UNKNOWNNAME;
}

// Not implemented. Retrieves the type information for an object.
STDMETHODIMP CEventSink::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo)
{
    UNREFERENCED_PARAMETER(itinfo);
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(pptinfo);
    return E_NOTIMPL;
}

// Not implemented. Retrieves the number of type information interfaces that an object provides.
STDMETHODIMP CEventSink::GetTypeInfoCount(unsigned int* pctinfo)
{
    UNREFERENCED_PARAMETER(pctinfo);
    return E_NOTIMPL;
}

// Sends a window message to the frame when a browser event occurs.
STDMETHODIMP CEventSink::Invoke(DISPID dispid, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pvarResult, EXCEPINFO* pexecinfo, unsigned int* puArgErr)
{
    UNREFERENCED_PARAMETER((IID)riid);          // IID cast required for the MinGW compiler
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(wFlags);
    UNREFERENCED_PARAMETER(pvarResult);
    UNREFERENCED_PARAMETER(pexecinfo);
    UNREFERENCED_PARAMETER(puArgErr);

    if (!pDispParams)
        return E_INVALIDARG;

    HWND frame = GetApp()->GetMainWnd();

    switch (dispid)
    {
    case DISPID_BEFORENAVIGATE2:
        ::SendMessage(frame, UWM_BEFORENAVIGATE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_COMMANDSTATECHANGE:
        ::SendMessage(frame, UWM_COMMANDSTATECHANGE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_DOCUMENTCOMPLETE:
        ::SendMessage(frame, UWM_DOCUMENTCOMPLETE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_DOWNLOADBEGIN:
        ::SendMessage(frame, UWM_DOWNLOADBEGIN, (LPARAM)pDispParams, 0);
        break;
    case DISPID_DOWNLOADCOMPLETE:
        ::SendMessage(frame, UWM_DOWNLOADCOMPLETE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_NAVIGATECOMPLETE2:
        ::SendMessage(frame, UWM_NAVIGATECOMPLETE2, (LPARAM)pDispParams, 0);
        break;
    case DISPID_PROGRESSCHANGE:
        ::SendMessage(frame, UWM_PROGRESSCHANGE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_PROPERTYCHANGE:
        ::SendMessage(frame, UWM_PROPERTYCHANGE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_STATUSTEXTCHANGE:
        ::SendMessage(frame, UWM_STATUSTEXTCHANGE, (LPARAM)pDispParams, 0);
        break;
    case DISPID_NEWWINDOW2:
        ::SendMessage(frame, UWM_NEWWINDOW2, (LPARAM)pDispParams, 0);
        break;
    case DISPID_TITLECHANGE:
        ::SendMessage(frame, UWM_TITLECHANGE, (LPARAM)pDispParams, 0);
        break;
    }

    return S_OK;
}

// Not implemented. Decrements a reference count.
STDMETHODIMP_(ULONG) CEventSink::Release()
{
    // Always 1. Automatic deletion isn't required.
    return 1;
}

