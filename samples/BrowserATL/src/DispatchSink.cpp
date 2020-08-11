/////////////////////////////
// DispatchSink.cpp
//

#include "stdafx.h"
#include "DispatchSink.h"
#include "BrowserApp.h"

/////////////////////////////////////
// CDispatchSink function definitions
//

// Sends a window message to the frame when a browser event occurs.
STDMETHODIMP CDispatchSink::Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
                               DISPPARAMS* pDispParams, VARIANT* pvarResult,
                               EXCEPINFO*  pExcepInfo,  UINT* puArgErr)
{
    UNREFERENCED_PARAMETER(riid);
    UNREFERENCED_PARAMETER(lcid);
    UNREFERENCED_PARAMETER(wFlags);
    UNREFERENCED_PARAMETER(pDispParams);
    UNREFERENCED_PARAMETER(pvarResult);
    UNREFERENCED_PARAMETER(pExcepInfo);
    UNREFERENCED_PARAMETER(puArgErr);

    if (!pDispParams)
        return E_INVALIDARG;

    HWND frame = GetApp()->GetMainWnd();

    switch (dispidMember)
    {
    case DISPID_BEFORENAVIGATE2:
        ::SendMessage(frame, UWM_BEFORENAVIGATE2, (LPARAM)pDispParams, 0);
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