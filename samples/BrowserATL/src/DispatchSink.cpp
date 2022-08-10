/////////////////////////////
// DispatchSink.cpp
//

#include "stdafx.h"
#include "DispatchSink.h"
#include "BrowserApp.h"
#include "UserMessages.h"

/////////////////////////////////////
// CDispatchSink function definitions
//

// Sends a window message to the frame when a browser event occurs.
STDMETHODIMP CDispatchSink::Invoke(DISPID dispidMember, REFIID, LCID, WORD,
                               DISPPARAMS* pDispParams, VARIANT*, EXCEPINFO*, UINT*)
{
    if (!pDispParams)
        return E_INVALIDARG;

    HWND frame = GetApp()->GetMainWnd();

    switch (dispidMember)
    {
    case DISPID_BEFORENAVIGATE2:
        ::SendMessage(frame, UWM_BEFORENAVIGATE2, (WPARAM)pDispParams, 0);
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