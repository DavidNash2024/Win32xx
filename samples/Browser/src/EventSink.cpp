/////////////////////////////
// EventSink.cpp
//

#include "stdafx.h"
#include "EventSink.h"
#include "UserMessages.h"

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
STDMETHODIMP CEventSink::GetIDsOfNames(REFIID, OLECHAR**, unsigned int, LCID, DISPID* rgdispid)
{
    if (rgdispid != NULL)
        *rgdispid = DISPID_UNKNOWN;

    return DISP_E_UNKNOWNNAME;
}

// Not implemented. Retrieves the type information for an object.
STDMETHODIMP CEventSink::GetTypeInfo(unsigned int, LCID, ITypeInfo**)
{
    return E_NOTIMPL;
}

// Not implemented. Retrieves the number of type information interfaces that an object provides.
STDMETHODIMP CEventSink::GetTypeInfoCount(unsigned int*)
{
    return E_NOTIMPL;
}

// Sends a window message to the frame when a browser event occurs.
STDMETHODIMP CEventSink::Invoke(DISPID dispid, REFIID, LCID, WORD, DISPPARAMS* pDispParams, VARIANT*, EXCEPINFO*, unsigned int*)
{
    if (!pDispParams)
        return E_INVALIDARG;

    HWND frame = GetApp()->GetMainWnd();

    switch (dispid)
    {
    case DISPID_BEFORENAVIGATE2:
        ::SendMessage(frame, UWM_BEFORENAVIGATE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_COMMANDSTATECHANGE:
        ::SendMessage(frame, UWM_COMMANDSTATECHANGE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_DOCUMENTCOMPLETE:
        ::SendMessage(frame, UWM_DOCUMENTCOMPLETE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_DOWNLOADBEGIN:
        ::SendMessage(frame, UWM_DOWNLOADBEGIN, (WPARAM)pDispParams, 0);
        break;
    case DISPID_DOWNLOADCOMPLETE:
        ::SendMessage(frame, UWM_DOWNLOADCOMPLETE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_NAVIGATECOMPLETE2:
        ::SendMessage(frame, UWM_NAVIGATECOMPLETE2, (WPARAM)pDispParams, 0);
        break;
    case DISPID_PROGRESSCHANGE:
        ::SendMessage(frame, UWM_PROGRESSCHANGE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_PROPERTYCHANGE:
        ::SendMessage(frame, UWM_PROPERTYCHANGE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_STATUSTEXTCHANGE:
        ::SendMessage(frame, UWM_STATUSTEXTCHANGE, (WPARAM)pDispParams, 0);
        break;
    case DISPID_NEWWINDOW2:
        ::SendMessage(frame, UWM_NEWWINDOW2, (WPARAM)pDispParams, 0);
        break;
    case DISPID_TITLECHANGE:
        ::SendMessage(frame, UWM_TITLECHANGE, (WPARAM)pDispParams, 0);
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

