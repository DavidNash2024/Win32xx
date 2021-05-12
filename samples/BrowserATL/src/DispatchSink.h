/////////////////////////////
//  DispatchSink.h
//

#ifndef __DISPATCHSINK_H__
#define __DISPATCHSINK_H__


#include <atlbase.h>            // ATL support
#include <atlcom.h>             // ATL COM support
#include <atlhost.h>            // ATL ActiveX support
#include <exdispid.h>           // ATL IDispatchID helper
#include <exdisp.h>             // ATL WebBrowser2 helper


////////////////////////////////////////////////////
// Creates an ATL class for IDispatch. This is used
// to sink events from IWebbrowser2 via invoke.
class ATL_NO_VTABLE CDispatchSink: public IDispatch,
    public CComObjectRootEx<CComSingleThreadModel>
{
public:
BEGIN_COM_MAP(CDispatchSink)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, IDispatch)
END_COM_MAP()
    // Process events from the web browser.
    virtual HRESULT __stdcall Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
                                     DISPPARAMS* pdispparams, VARIANT* pvarResult,
                                     EXCEPINFO* pexcepinfo, UINT* puArgErr);

    // These must be present, even though they are not implemented.
    virtual HRESULT __stdcall GetTypeInfoCount(UINT*)
    {
         return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetTypeInfo(UINT, LCID, ITypeInfo**)
    {
        return E_NOTIMPL;
    }
    virtual HRESULT __stdcall GetIDsOfNames(REFIID, LPOLESTR*, UINT,
                                            LCID, DISPID*)
    {
        return E_NOTIMPL;
    }
};

#endif  // __DISPATCHSINK_H__
