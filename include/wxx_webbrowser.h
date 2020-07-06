// Win32++   Version 8.7.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2020  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////

#ifndef _WIN32XX_WEBBROWSER_H_
#define _WIN32XX_WEBBROWSER_H_

#include "wxx_appcore0.h"
#include <exdisp.h>
#include <ocidl.h>

#ifdef _MSC_VER
  #pragma warning ( push )
  #pragma warning ( disable : 4091 )  // temporarily disable C4091 warning
#endif // _MSC_VER

#include <shlobj.h>

#ifdef _MSC_VER
  #pragma warning ( pop )
#endif // _MSC_VER

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( push )
#pragma warning ( disable : 26812 )       // enum type is unscoped. 
#endif // (_MSC_VER) && (_MSC_VER >= 1400)


namespace Win32xx
{
    ///////////////////////////////////////////////////
    // Declaration of the CAXHost class
    // This class implements a COM object which hosts an ActiveX control,
    // such as internet explorer.
    class CAXHost : public IOleInPlaceFrame, public IOleClientSite, 
                    public IOleInPlaceSite, public IOleControlSite, 
                    public IDispatch
    {
    public:
        CAXHost();
        ~CAXHost();

        // CAXHost methods
        STDMETHODIMP Activate();
        STDMETHODIMP CreateControl(BSTR clsidName, void** ppUnk);
        STDMETHODIMP CreateControl(REFCLSID clsid, void** ppUnk);
        STDMETHODIMP Remove();
        STDMETHODIMP SetParent(HWND hWndParent);
        STDMETHODIMP SetLocation(int x, int y, int width, int height);
        STDMETHODIMP SetVisible(BOOL isVisible);
        STDMETHODIMP SetStatusWindow(HWND status);
        STDMETHODIMP TranslateKey(MSG msg);

        // IUnknown Methods
        STDMETHODIMP QueryInterface(REFIID riid, void** ppObject);
        STDMETHODIMP_(ULONG) AddRef();
        STDMETHODIMP_(ULONG) Release();

        // IOleClientSite Methods
        STDMETHODIMP SaveObject();
        STDMETHODIMP GetMoniker(DWORD assign, DWORD whichMoniker, LPMONIKER* ppMk);
        STDMETHODIMP GetContainer(LPOLECONTAINER* ppContainer);
        STDMETHODIMP ShowObject();
        STDMETHODIMP OnShowWindow(BOOL show);
        STDMETHODIMP RequestNewObjectLayout();

        // IOleWindow Methods
        STDMETHODIMP GetWindow(HWND* pHwnd);
        STDMETHODIMP ContextSensitiveHelp(BOOL enterMode);

        // IOleInPlaceSite Methods
        STDMETHODIMP CanInPlaceActivate();
        STDMETHODIMP OnInPlaceActivate();
        STDMETHODIMP OnUIActivate();
        STDMETHODIMP GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT pRect, LPRECT pClipRect, LPOLEINPLACEFRAMEINFO pFrameInfo);
        STDMETHODIMP Scroll(SIZE scrollExtent);
        STDMETHODIMP OnUIDeactivate(BOOL undoable);
        STDMETHODIMP OnInPlaceDeactivate();
        STDMETHODIMP DiscardUndoState();
        STDMETHODIMP DeactivateAndUndo();
        STDMETHODIMP OnPosRectChange(LPCRECT pRect);

        // IOleInPlaceUIWindow Methods
        STDMETHODIMP GetBorder(LPRECT pBorderRect);
        STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS pBorderWidths);
        STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS pBorderWidths);
        STDMETHODIMP SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pObjName);

        // IOleInPlaceFrame Methods
        STDMETHODIMP InsertMenus(HMENU shared, LPOLEMENUGROUPWIDTHS pMenuWidths);
        STDMETHODIMP SetMenu(HMENU shared, HOLEMENU holemenu, HWND activeObject);
        STDMETHODIMP RemoveMenus(HMENU shared);
        STDMETHODIMP SetStatusText(LPCOLESTR pStatusText);
        STDMETHODIMP EnableModeless(BOOL enable);
        STDMETHODIMP TranslateAccelerator(LPMSG pMsg, WORD id);

        // IOleControlSite Methods
        STDMETHODIMP OnControlInfoChanged();
        STDMETHODIMP LockInPlaceActive(BOOL lock);
        STDMETHODIMP GetExtendedControl(IDispatch** ppDisp);
        STDMETHODIMP TransformCoords(POINTL* pHimetric, POINTF* pContainer, DWORD flags);
        STDMETHODIMP TranslateAccelerator(LPMSG pMsg, DWORD modifiers);
        STDMETHODIMP OnFocus(BOOL gotFocus);
        STDMETHODIMP ShowPropertyFrame();

        // IDispatch Methods
        STDMETHODIMP GetIDsOfNames(REFIID riid, OLECHAR** pNames, unsigned int namesCount, LCID lcid, DISPID* pID);
        STDMETHODIMP GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo);
        STDMETHODIMP GetTypeInfoCount(unsigned int* pctinfo);
        STDMETHODIMP Invoke(DISPID dispID, REFIID riid, LCID lcid, WORD flags, DISPPARAMS* pParams, VARIANT* result, EXCEPINFO* pExecInfo, unsigned int* pArgErr);

    private:
         HWND        m_ax;          // window handle of the container
         HWND        m_status;      // status window handle
         IUnknown*   m_pUnk;        // IUnknown of contained object
         CRect       m_controlRect; // size of control
    };


    ///////////////////////////////////////////////
    // Declaration of the CWebBrowser class.
    // This class provides the functionality of a WebBrower, using the
    // IWebBrower2 interface.
    class CWebBrowser : public CWnd
    {
    public:
        CWebBrowser();
        virtual ~CWebBrowser();

        //Attributes
        LPDISPATCH GetApplication() const;
        BOOL    GetBusy() const;
        LPDISPATCH GetContainer() const;
        LPDISPATCH GetDocument() const;
        BOOL    GetFullScreen() const;
        long    GetHeight() const;
        IWebBrowser2* const GetIWebBrowser2() const { return m_pIWebBrowser2; }
        long    GetLeft() const;
        CString GetLocationName() const;
        CString GetLocationURL() const;
        BOOL    GetOffline() const;
        LPDISPATCH GetParent() const;
        VARIANT GetProperty(LPCTSTR pProperty) const;
        READYSTATE GetReadyState() const;
        BOOL    GetRegisterAsBrowser() const;
        BOOL    GetTheaterMode() const;
        long    GetTop() const;
        BOOL    GetTopLevelContainer() const;
        CString GetType() const;
        BOOL    GetVisible() const;
        long    GetWidth() const;
        HRESULT SetFullScreen(BOOL isFullScreen);
        HRESULT SetHeight(long height);
        HRESULT SetLeft(long leftEdge);
        HRESULT SetOffline(BOOL isOffline);
        HRESULT SetRegisterAsBrowser(BOOL isBrowser);
        HRESULT SetTheaterMode(BOOL isTheaterMode);
        HRESULT SetTop(long topEdge);
        HRESULT SetVisible(BOOL isVisible);
        HRESULT SetWidth(long width);

        // Operations
        HRESULT AddWebBrowserControl();
        HRESULT ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdExecOpt, VARIANT* pIn, VARIANT* pOut);
        HRESULT GoBack();
        HRESULT GoForward();
        HRESULT GoHome();
        HRESULT GoSearch();
        HRESULT Navigate(LPCTSTR pURL, DWORD flags = 0, LPCTSTR pTargetFrameName = NULL,
                        LPCTSTR pHeaders = NULL, LPVOID pPostData = NULL, DWORD postDataLen = 0);
        HRESULT Navigate2(LPITEMIDLIST pIDL, DWORD flags = 0, LPCTSTR pTargetFrameName = NULL);
        HRESULT Navigate2(LPCTSTR pURL, DWORD flags = 0, LPCTSTR pTargetFrameName = NULL,
                         LPCTSTR pHeaders = NULL, LPVOID pPostData = NULL, DWORD postDataLen = 0);
        HRESULT PutProperty(LPCTSTR pPropertyName, const VARIANT& value);
        HRESULT PutProperty(LPCTSTR pPropertyName, double value);
        HRESULT PutProperty(LPCTSTR pPropertyName, long value);
        HRESULT PutProperty(LPCTSTR pPropertyName, LPCTSTR pValue);
        HRESULT PutProperty(LPCTSTR pPropertyName, short value);
        HRESULT Refresh();
        HRESULT Refresh2(int evel);
        HRESULT Stop();

    protected:
        // Override these as required
        virtual void OnAttach();
        virtual int  OnCreate(CREATESTRUCT& cs);
        virtual void OnDestroy();
        virtual void OnSize(int width, int height);

        // Not intended to be overridden
        LRESULT WndProcDefault(UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        UINT    GetPidlLength(LPITEMIDLIST pidl);
        CAXHost  m_axHost;              // The ActiveX host
        IWebBrowser2* m_pIWebBrowser2;  // Interface to the ActiveX web browser control
    };

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

    /////////////////////////////////////////
    // Definitions for the CAXHost class
    //

    inline CAXHost::CAXHost() : m_ax(NULL), m_status(0), m_pUnk(NULL)
    {
        HRESULT hr;
        VERIFY(SUCCEEDED(hr = OleInitialize(NULL)));
    }

    inline CAXHost::~CAXHost()
    {
        if (m_pUnk)
            m_pUnk->Release();

        OleUninitialize();
    }

    inline STDMETHODIMP CAXHost::Activate()
    {
        HRESULT hr = E_FAIL;
        if (m_pUnk)
        {
            IOleObject* pObject;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&pObject))));
            if (pObject)
                VERIFY(SUCCEEDED(hr = pObject->DoVerb(OLEIVERB_UIACTIVATE, NULL, this, 0, m_ax, &m_controlRect)));

            pObject->Release();
        }

        return hr;
    }

    inline STDMETHODIMP CAXHost::CreateControl(BSTR clsidName, void** ppUnk)
    {
        CLSID   clsid;
        HRESULT hr;
        VERIFY(SUCCEEDED(hr = CLSIDFromString(clsidName, &clsid)));
        VERIFY(SUCCEEDED(hr = CreateControl(clsid, ppUnk)));

        return hr;
    }

    inline STDMETHODIMP CAXHost::CreateControl(REFCLSID clsid, void** ppUnk)
    {
        HRESULT hr = E_FAIL;

        VERIFY(SUCCEEDED(hr = CoCreateInstance(clsid, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER, IID_IUnknown, ppUnk)));
        m_pUnk = reinterpret_cast<IUnknown*>(*ppUnk);
        if (m_pUnk)
        {
            IOleObject* pObject;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&pObject))));
            if (pObject)
            {
                VERIFY(SUCCEEDED(hr = pObject->SetClientSite(this)));
                pObject->Release();

                IPersistStreamInit* ppsi;
                VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IPersistStreamInit, reinterpret_cast<void**>(&ppsi))));
                if (ppsi)
                {
                    VERIFY(SUCCEEDED(hr = ppsi->InitNew()));
                    ppsi->Release();
                }
            }
        }

        return hr;
    }

    inline STDMETHODIMP_(ULONG) CAXHost::AddRef()
    {
        // Automatic deletion is not required.
        return 1;
    }

    inline STDMETHODIMP CAXHost::CanInPlaceActivate()
    {
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::ContextSensitiveHelp(BOOL enterMode)
    {
        UNREFERENCED_PARAMETER(enterMode);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::DeactivateAndUndo()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::DiscardUndoState()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::EnableModeless(BOOL enable)
    {
        UNREFERENCED_PARAMETER(enable);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::GetBorder(LPRECT pBorderRect)
    {
        UNREFERENCED_PARAMETER(pBorderRect);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::GetContainer(LPOLECONTAINER* ppContainer)
    {
        UNREFERENCED_PARAMETER(ppContainer);
        return E_NOINTERFACE;
    }

    // Call Release on this pointer when it is no longer required.
    inline STDMETHODIMP CAXHost::GetExtendedControl(IDispatch** ppDisp)
    {
        if (ppDisp == NULL)
            return E_INVALIDARG;

        *ppDisp = static_cast<IDispatch*>(this);
        (*ppDisp)->AddRef();

        return S_OK;
    }

    inline STDMETHODIMP CAXHost::GetIDsOfNames(REFIID riid, OLECHAR** pNames, unsigned int namesCount, LCID lcid, DISPID* pID)
    {
        UNREFERENCED_PARAMETER((IID)riid);      // IID cast required for the MinGW compiler
        UNREFERENCED_PARAMETER(pNames);
        UNREFERENCED_PARAMETER(namesCount);
        UNREFERENCED_PARAMETER(lcid);

        *pID = DISPID_UNKNOWN;
        return DISP_E_UNKNOWNNAME;
    }

    inline STDMETHODIMP CAXHost::GetMoniker(DWORD assign, DWORD whichMoniker, LPMONIKER* ppMk)
    {
        UNREFERENCED_PARAMETER(assign);
        UNREFERENCED_PARAMETER(whichMoniker);
        UNREFERENCED_PARAMETER(ppMk);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::GetTypeInfo(unsigned int itinfo, LCID lcid, ITypeInfo** pptinfo)
    {
        UNREFERENCED_PARAMETER(itinfo);
        UNREFERENCED_PARAMETER(lcid);
        UNREFERENCED_PARAMETER(pptinfo);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::GetTypeInfoCount(unsigned int* pctinfo)
    {
        UNREFERENCED_PARAMETER(pctinfo);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::GetWindow(HWND* pHwnd)
    {
        if (!IsWindow(m_ax))
            return S_FALSE;

        *pHwnd = m_ax;
        return S_OK;
    }

    // Call Release on this pointer when it is no longer required.
    inline STDMETHODIMP CAXHost::GetWindowContext (IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppIIPUIWin,
                                      LPRECT pRect, LPRECT pClipRect, LPOLEINPLACEFRAMEINFO pFrameInfo)
    {
        *ppFrame = (IOleInPlaceFrame*)this;
        *ppIIPUIWin = NULL;

        RECT rect;
        GetClientRect(m_ax, &rect);
        pRect->left       = 0;
        pRect->top        = 0;
        pRect->right      = rect.right;
        pRect->bottom     = rect.bottom;

        CopyRect(pClipRect, pRect);

        pFrameInfo->cb             = sizeof(OLEINPLACEFRAMEINFO);
        pFrameInfo->fMDIApp        = FALSE;
        pFrameInfo->hwndFrame      = m_ax;
        pFrameInfo->haccel         = 0;
        pFrameInfo->cAccelEntries  = 0;

        (*ppFrame)->AddRef();
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::InsertMenus(HMENU shared, LPOLEMENUGROUPWIDTHS pMenuWidths)
    {
        UNREFERENCED_PARAMETER(shared);
        UNREFERENCED_PARAMETER(pMenuWidths);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::Invoke(DISPID dispID, REFIID riid, LCID lcid, WORD flags, DISPPARAMS* pParams, VARIANT* result, EXCEPINFO* pExecInfo, unsigned int* pArgErr)
    {
        UNREFERENCED_PARAMETER(dispID);
        UNREFERENCED_PARAMETER((IID)riid);      // IID cast required for the MinGW compiler
        UNREFERENCED_PARAMETER(lcid);
        UNREFERENCED_PARAMETER(flags);
        UNREFERENCED_PARAMETER(pParams);
        UNREFERENCED_PARAMETER(result);
        UNREFERENCED_PARAMETER(pExecInfo);
        UNREFERENCED_PARAMETER(pArgErr);
        return DISP_E_MEMBERNOTFOUND;
    }

    inline STDMETHODIMP CAXHost::LockInPlaceActive(BOOL lock)
    {
        UNREFERENCED_PARAMETER(lock);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::OnControlInfoChanged()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::OnFocus(BOOL gotFocus)
    {
        UNREFERENCED_PARAMETER(gotFocus);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::OnInPlaceActivate()
    {
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::OnInPlaceDeactivate()
    {
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::OnPosRectChange(LPCRECT pPosRect)
    {
        UNREFERENCED_PARAMETER(pPosRect);
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::OnShowWindow(BOOL show)
    {
        UNREFERENCED_PARAMETER(show);
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::OnUIActivate()
    {
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::OnUIDeactivate(BOOL undoable)
    {
        UNREFERENCED_PARAMETER(undoable);
        return E_NOTIMPL;
    }

    // Call Release on this pointer when it is no longer required.
    inline STDMETHODIMP CAXHost::QueryInterface(REFIID riid, void** ppObject)
    {
        if (!ppObject)
            return E_POINTER;

        if (IsEqualIID(riid, IID_IOleClientSite))
            *ppObject = static_cast<IOleClientSite*>(this);
        else if (IsEqualIID(riid, IID_IOleInPlaceSite))
            *ppObject = static_cast<IOleInPlaceSite*>(this);
        else if (IsEqualIID(riid, IID_IOleInPlaceFrame))
            *ppObject = static_cast<IOleInPlaceFrame*>(this);
        else if (IsEqualIID(riid, IID_IOleControlSite))
            *ppObject = static_cast<IOleControlSite*>(this);
        else if (IsEqualIID(riid, IID_IDispatch))
            *ppObject = static_cast<IDispatch*>(this);
        else if (IsEqualIID(riid, IID_IUnknown))
            *ppObject = static_cast<IUnknown*>(static_cast<IOleInPlaceFrame*>(this));
        else
        {
            *ppObject = NULL;
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
    }

    inline STDMETHODIMP_(ULONG) CAXHost::Release()
    {
        // Automatic deletion is not required.
        return 1;
    }

    inline STDMETHODIMP CAXHost::Remove()
    {
        HRESULT hr = E_FAIL;

        if (m_pUnk)
        {
            IOleInPlaceObject* pipo;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&pipo))));
            if (pipo)
            {
                VERIFY(SUCCEEDED(hr = pipo->UIDeactivate()));
                VERIFY(SUCCEEDED(hr = pipo->InPlaceDeactivate()));
                pipo->Release();
            }

            IOleObject* pObject;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&pObject))));
            if (pObject)
            {
                VERIFY(SUCCEEDED(hr = pObject->Close(OLECLOSE_NOSAVE)));
                VERIFY(SUCCEEDED(hr = pObject->SetClientSite(NULL)));
                pObject->Release();
            }
        }
        
        return hr;
    }

    inline STDMETHODIMP CAXHost::RemoveMenus(HMENU shared)
    {
        UNREFERENCED_PARAMETER(shared);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::RequestBorderSpace(LPCBORDERWIDTHS pBorderWidths)
    {
        UNREFERENCED_PARAMETER(pBorderWidths);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::RequestNewObjectLayout()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::SaveObject()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::Scroll(SIZE scrollExtent)
    {
        UNREFERENCED_PARAMETER(scrollExtent);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::SetActiveObject(IOleInPlaceActiveObject* pActiveObject, LPCOLESTR pObjName)
    {
        UNREFERENCED_PARAMETER(pActiveObject);
        UNREFERENCED_PARAMETER(pObjName);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::SetBorderSpace(LPCBORDERWIDTHS pBorderWidths)
    {
        UNREFERENCED_PARAMETER(pBorderWidths);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::SetLocation(int x, int y, int width, int height)
    {
        m_controlRect.SetRect(x, y, x + width, y + height);

        HRESULT hr = E_FAIL;
        if (m_pUnk)
        {
            IOleInPlaceObject* pipo;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&pipo))));
            if (pipo)
            {
                VERIFY(SUCCEEDED(hr = pipo->SetObjectRects(&m_controlRect, &m_controlRect)));
                pipo->Release();
            }
        }

        return hr;
    }

    inline STDMETHODIMP CAXHost::SetMenu(HMENU shared, HOLEMENU holemenu, HWND activeObject)
    {
        UNREFERENCED_PARAMETER(shared);
        UNREFERENCED_PARAMETER(holemenu);
        UNREFERENCED_PARAMETER(activeObject);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::SetParent(HWND hWndParent)
    {
        m_ax = hWndParent;
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::SetStatusText(LPCOLESTR pStatusText)
    {
        if (NULL == pStatusText)
            return E_POINTER;

        CString status(pStatusText);
        if (IsWindow(m_status))
            SendMessage(m_status, SB_SETTEXT, 0, (LPARAM)status.c_str());

        return S_OK;
    }

    inline STDMETHODIMP CAXHost::SetStatusWindow(HWND status)
    {
        m_status = status;
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::SetVisible(BOOL isVisible)
    {
        HRESULT hr = E_FAIL;
        if (m_pUnk)
        {
            IOleObject* pObject;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleObject, reinterpret_cast<void**>(&pObject))));
            if (pObject)
            {
                if (isVisible)
                {
                    VERIFY(SUCCEEDED(hr = pObject->DoVerb(OLEIVERB_INPLACEACTIVATE, NULL, this, 0, m_ax, &m_controlRect)));
                    VERIFY(SUCCEEDED(hr = pObject->DoVerb(OLEIVERB_SHOW, NULL, this, 0, m_ax, &m_controlRect)));
                }
                else
                    VERIFY(SUCCEEDED(hr = pObject->DoVerb(OLEIVERB_HIDE, NULL, this, 0, m_ax, NULL)));

                pObject->Release();
            }
        }

        return hr;
    }

    inline STDMETHODIMP CAXHost::ShowObject()
    {
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::ShowPropertyFrame()
    {
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::TransformCoords(POINTL* pHimetric, POINTF* pContainer, DWORD flags)
    {
        UNREFERENCED_PARAMETER(pHimetric);
        UNREFERENCED_PARAMETER(pContainer);
        UNREFERENCED_PARAMETER(flags);
        return E_NOTIMPL;
    }

    inline STDMETHODIMP CAXHost::TranslateAccelerator(LPMSG pMsg, WORD id)
    {
        UNREFERENCED_PARAMETER(pMsg);
        UNREFERENCED_PARAMETER(id);
        return S_OK;
    }

    inline STDMETHODIMP CAXHost::TranslateAccelerator(LPMSG pMsg, DWORD modifiers)
    {
        UNREFERENCED_PARAMETER(pMsg);
        UNREFERENCED_PARAMETER(modifiers);
        return S_FALSE;
    }

    inline STDMETHODIMP CAXHost::TranslateKey(MSG msg)
    {
        HRESULT hr = E_FAIL;
        if (m_pUnk)
        {
            IOleInPlaceActiveObject* pao;
            VERIFY(SUCCEEDED(hr = m_pUnk->QueryInterface(IID_IOleInPlaceActiveObject, reinterpret_cast<void**>(&pao))));
            if (pao)
            {
                VERIFY(SUCCEEDED(hr = pao->TranslateAccelerator(&msg)));
                pao->Release();
            }
        }

        return hr;
    }


    ////////////////////////////////////////
    // Definitions for the CWebBrowser class
    //

    inline CWebBrowser::CWebBrowser() : m_pIWebBrowser2(0)
    {
    }

    inline CWebBrowser::~CWebBrowser()
    {
        if (m_pIWebBrowser2)
            m_pIWebBrowser2->Release();
    }

    // Adds the IWebBrowser interface to the ActiveX host.
    // Refer to IID_IWebBrowser2 in the Windows API documentation for more information.
    inline HRESULT CWebBrowser::AddWebBrowserControl()
    {
        IUnknown* pUnk = NULL;
        VERIFY(SUCCEEDED(m_axHost.CreateControl(CLSID_WebBrowser, (void**)&pUnk)));
        VERIFY(SUCCEEDED(m_axHost.SetParent(*this)));
        VERIFY(SUCCEEDED(m_axHost.SetVisible(TRUE)));
        VERIFY(SUCCEEDED(m_axHost.Activate()));

        HRESULT hr = E_FAIL;
        if (pUnk)
        {
            // Store the pointer to the WebBrowser control
            VERIFY(SUCCEEDED(hr = pUnk->QueryInterface(IID_IWebBrowser2, reinterpret_cast<void**>(&m_pIWebBrowser2))));

            // Navigate to an empty page
            VERIFY(SUCCEEDED(hr = Navigate(_T("about:blank"))));
        }

        return hr;
    }

    // Called when the WebBrowser window's HWND is attached this object.
    inline void CWebBrowser::OnAttach()
    {
        if (m_pIWebBrowser2 == NULL)
            VERIFY(SUCCEEDED(AddWebBrowserControl()));
    }

    // Called when the web browser window is created.
    inline int CWebBrowser::OnCreate(CREATESTRUCT& cs)
    {
        UNREFERENCED_PARAMETER(cs);
        VERIFY(SUCCEEDED(AddWebBrowserControl()));
        return 0;
    }

    // Called when the window is destroyed.
    inline void CWebBrowser::OnDestroy()
    {
        VERIFY(SUCCEEDED(m_pIWebBrowser2->Stop()));
        VERIFY(SUCCEEDED(m_axHost.Remove()));
    }

    // Called when the window is resized.
    inline void CWebBrowser::OnSize(int width, int height)
    {
        // position the container
        VERIFY(SUCCEEDED(m_axHost.SetLocation(0, 0, width, height)));
    }

    // Provides default message processing for the web browser window.
    inline LRESULT CWebBrowser::WndProcDefault(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_SIZE:
            OnSize(LOWORD(lparam), HIWORD(lparam));
            break;
        }

        return CWnd::WndProcDefault(msg, wparam, lparam);
    }

    //////////////////////////////////////////////////
    // Wrappers for the IWebBrowser2 interface

    // Retrieves a pointer to the IDispatch interface for the the application that is hosting the WebBrowser Control.
    inline LPDISPATCH CWebBrowser::GetApplication() const
    {
        LPDISPATCH pDispatch = NULL;
        GetIWebBrowser2()->get_Application(&pDispatch);
        return pDispatch;
    }

    // Retrieves a value that indicates whether the object is engaged in a navigation or downloading operation.
    inline BOOL CWebBrowser::GetBusy() const
    {
        VARIANT_BOOL isBusy = VARIANT_FALSE;
        GetIWebBrowser2()->get_Busy(&isBusy);
        return (isBusy != 0);
    }

    // Retrieves a pointer to the IDispatch interface to a container. This property returns the same pointer
    // as GetParent.
    inline LPDISPATCH CWebBrowser::GetContainer() const
    {
        LPDISPATCH pDispatch = NULL;
        GetIWebBrowser2()->get_Container(&pDispatch);
        return pDispatch;
    }

    // Retrieves a pointer to the IDispatch interface of the active document object. Call QueryInterface on the
    // IDispatch received from this property get the Component Object Model (COM) interfaces IHTMLDocument, 
    // IHTMLDocument2, and IHTMLDocument3.
    inline LPDISPATCH CWebBrowser::GetDocument() const
    {
        LPDISPATCH Value = NULL;
        GetIWebBrowser2()->get_Document(&Value);
        return Value;
    }

    // Retrieves a value that indicates whether Internet Explorer is in full-screen mode or normal window mode.
    inline BOOL CWebBrowser::GetFullScreen() const
    {
        VARIANT_BOOL value = VARIANT_FALSE;
        GetIWebBrowser2()->get_FullScreen(&value);
        return (value != 0);
    }

    // Retrieves the height of the object.
    inline long CWebBrowser::GetHeight() const
    {
        long height = 0;
        GetIWebBrowser2()->get_Height(&height);
        return height;
    }

    // Retrieves the coordinate of the left edge of the object.
    inline long CWebBrowser::GetLeft() const
    {
        long left = 0;
        GetIWebBrowser2()->get_Left(&left);
        return left;
    }

    // Retrieves the path or title of the resource that is currently displayed.
    inline CString CWebBrowser::GetLocationName() const
    {
        BSTR bstr;
        GetIWebBrowser2()->get_LocationName(&bstr);
        CString str(bstr);
        SysFreeString(bstr);
        return str;
    }

    // Retrieves the URL of the resource that is currently displayed.
    inline CString CWebBrowser::GetLocationURL() const
    {
        BSTR bstr;
        GetIWebBrowser2()->get_LocationURL(&bstr);
        CString str(bstr);
        SysFreeString(bstr);
        return str;
    }

    // Retrieves a value that indicates whether the object is operating in offline mode.
    inline BOOL CWebBrowser::GetOffline() const
    {
        VARIANT_BOOL isOffLine = VARIANT_FALSE;
        GetIWebBrowser2()->get_Offline(&isOffLine);
        return (isOffLine != 0);
    }

    // Retrieves a pointer to the IDispatch interface of the object that is the 
    // container of the WebBrowser control. If the WebBrowser control is in a frame, 
    // this method returns the automation interface of the document object in the
    // containing window. Otherwise, it delegates to the top-level control, if there is one.
    inline LPDISPATCH CWebBrowser::GetParent() const
    {
        LPDISPATCH pDispatch = NULL;
        GetIWebBrowser2()->get_Parent(&pDispatch);
        return pDispatch;
    }

    // Retrieves the accumulated  length of the ITEMIDLIST.
    inline UINT CWebBrowser::GetPidlLength(LPITEMIDLIST pidl)
    {
        assert(pidl);
        UINT cbPidl = sizeof(pidl->mkid.cb);
        while (pidl && pidl->mkid.cb)
        {
            cbPidl += pidl->mkid.cb;

            // Walk to next item
            BYTE* ptr = reinterpret_cast<BYTE*>(pidl);
            ptr += pidl->mkid.cb;
            pidl = reinterpret_cast<LPITEMIDLIST>(ptr);
        }

        return cbPidl;
    }

    // Retrieves the ready state of the object.
    inline READYSTATE CWebBrowser::GetReadyState() const
    {
        READYSTATE rs = READYSTATE_UNINITIALIZED;
        GetIWebBrowser2()->get_ReadyState(&rs);
        return rs;
    }

    // Retrieves a value that indicates whether the object is registered as a top-level browser window.
    inline BOOL CWebBrowser::GetRegisterAsBrowser() const
    {
        VARIANT_BOOL isTopLevel = VARIANT_FALSE;
#if !defined(__BORLANDC__) || (__BORLANDC__ >= 0x600)
        GetIWebBrowser2()->get_RegisterAsBrowser(&isTopLevel);
#endif
        return (isTopLevel != 0);
    }

    // Retrieves the theater mode state of the object.
    inline BOOL CWebBrowser::GetTheaterMode() const
    {
        VARIANT_BOOL isTheater = VARIANT_FALSE;
        GetIWebBrowser2()->get_TheaterMode(&isTheater);
        return (isTheater != 0);
    }

    // Retrieves the coordinate of the top edge of the object.
    inline long CWebBrowser::GetTop() const
    {
        long top;
        GetIWebBrowser2()->get_Top(&top);
        return top;
    }

    //Returns TRUE  if the object is a top-level container.
    inline BOOL CWebBrowser::GetTopLevelContainer() const
    {
        VARIANT_BOOL isTop = VARIANT_FALSE;
        GetIWebBrowser2()->get_TopLevelContainer(&isTop);
        return (isTop != 0);
    }

    // Retrieves the user type name of the contained document object.
    inline CString CWebBrowser::GetType() const
    {
        BSTR bstr;
        GetIWebBrowser2()->get_Type(&bstr);
        CString str(bstr);
        SysFreeString(bstr);
        return str;
    }

    // Retrieves a value that indicates whether the object is visible or hidden.
    inline BOOL CWebBrowser::GetVisible() const
    {
        VARIANT_BOOL isVisible = VARIANT_FALSE;
        GetIWebBrowser2()->get_Visible(&isVisible);
        return (isVisible != 0);
    }

    // Retrieves the width of the object.
    inline long CWebBrowser::GetWidth() const
    {
        long width = 0;
        GetIWebBrowser2()->get_Width(&width);
        return width;
    }

    // Sets a value that indicates whether Internet Explorer is in full-screen mode or normal window mode.
    inline HRESULT CWebBrowser::SetFullScreen(BOOL isFullScreen)
    {
        VARIANT_BOOL isFS = isFullScreen ? VARIANT_TRUE : VARIANT_FALSE;
        return GetIWebBrowser2()->put_FullScreen(isFS);
    }

    // Sets the height of the object.
    inline HRESULT CWebBrowser::SetHeight(long height)
    {
        return GetIWebBrowser2()->put_Height(height);
    }

    // Sets the coordinate of the left edge of the object.
    inline HRESULT CWebBrowser::SetLeft(long leftEdge)
    {
        return GetIWebBrowser2()->put_Left(leftEdge);
    }

    // Sets a value that indicates whether the object is operating in offline mode.
    inline HRESULT CWebBrowser::SetOffline(BOOL isOffline)
    {
        VARIANT_BOOL isOL = isOffline ? VARIANT_TRUE : VARIANT_FALSE;
        return GetIWebBrowser2()->put_Offline(isOL);
    }

    // Sets a value that indicates whether the object is registered as a top-level browser window.
    inline HRESULT CWebBrowser::SetRegisterAsBrowser(BOOL isBrowser)
    {
        VARIANT_BOOL isB = isBrowser ? VARIANT_TRUE : VARIANT_FALSE;
        return GetIWebBrowser2()->put_RegisterAsBrowser(isB);
    }

    // Sets the theatre mode state of the object.
    inline HRESULT CWebBrowser::SetTheaterMode(BOOL isTheaterMode)
    {
        VARIANT_BOOL isTM = isTheaterMode ? VARIANT_TRUE : VARIANT_FALSE;
        return GetIWebBrowser2()->put_TheaterMode(isTM);
    }

    // Sets the coordinate of the top edge of the object.
    inline HRESULT CWebBrowser::SetTop(long topEdge)
    {
        return GetIWebBrowser2()->put_Top(topEdge);
    }

    // Sets a value that indicates whether the object is visible or hidden.
    inline HRESULT CWebBrowser::SetVisible(BOOL isVisible)
    {
        VARIANT_BOOL isV = isVisible ? VARIANT_TRUE : VARIANT_FALSE;
        return GetIWebBrowser2()->put_Visible(isV);
    }

    // Sets the width of the object.
    inline HRESULT CWebBrowser::SetWidth(long width)
    {
        return GetIWebBrowser2()->put_Width(width);
    }

    // Executes a command using the IOleCommandTarget interface.
    inline HRESULT CWebBrowser::ExecWB(OLECMDID cmdID, OLECMDEXECOPT cmdExecOpt, VARIANT* in, VARIANT* out)
    {
        return GetIWebBrowser2()->ExecWB(cmdID, cmdExecOpt, in, out);
    }

    // Gets the value associated with the specified property name.
    inline VARIANT CWebBrowser::GetProperty( LPCTSTR pProperty ) const
    {
        VARIANT v;
        GetIWebBrowser2()->GetProperty( TtoBSTR(pProperty), &v );
        return v;
    }

    // Navigates backward one item in the history list.
    inline HRESULT CWebBrowser::GoBack()
    {
        return GetIWebBrowser2()->GoBack();
    }

    // Navigates forward one item in the history list.
    inline HRESULT CWebBrowser::GoForward()
    {
        return GetIWebBrowser2()->GoForward();
    }

    // Navigates to the current home or start page.
    inline HRESULT CWebBrowser::GoHome()
    {
        return GetIWebBrowser2()->GoHome();
    }

    // Navigates to the current search page.
    inline HRESULT CWebBrowser::GoSearch()
    {
        return GetIWebBrowser2()->GoSearch();
    }

    // Navigates to a resource identified by a URL or to a file identified by a full path.
    inline HRESULT CWebBrowser::Navigate(LPCTSTR pURL,   DWORD flags /*= 0*/, LPCTSTR pTargetFrameName /*= NULL*/,
                    LPCTSTR pHeaders /*= NULL*/, LPVOID pPostData /*= NULL*/,   DWORD postDataLen /*= 0*/)
    {
        VARIANT flagsVariant;
        flagsVariant.vt = VT_I4;
        flagsVariant.lVal = flags;

        VARIANT targetVariant;
        targetVariant.vt = VT_BSTR;
        targetVariant.bstrVal = SysAllocString(TtoW(pTargetFrameName));

        SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, postDataLen);
        CopyMemory(psa->pvData, pPostData, postDataLen);
        VARIANT dataVariant;
        dataVariant.vt = VT_ARRAY|VT_UI1;
        dataVariant.parray = psa;

        VARIANT headersVariant;
        headersVariant.vt = VT_BSTR;
        headersVariant.bstrVal = SysAllocString(TtoW(pHeaders));
        BSTR url = SysAllocString(TtoW(pURL));
        HRESULT hr = E_FAIL;
        if (url)
            hr = GetIWebBrowser2()->Navigate(url, &flagsVariant, &targetVariant, &dataVariant, &headersVariant);

        VariantClear(&flagsVariant);
        VariantClear(&targetVariant);
        VariantClear(&dataVariant);
        VariantClear(&headersVariant);

        return hr;
    }

    // Navigates the browser to a location specified by a pointer to an item identifier list (PIDL) for an entity in the Microsoft Windows Shell namespace.
    inline HRESULT CWebBrowser::Navigate2(LPITEMIDLIST pIDL, DWORD flags /*= 0*/, LPCTSTR pTargetFrameName /*= NULL*/)
    {
        UINT cb = GetPidlLength(pIDL);
        LPSAFEARRAY pSA = SafeArrayCreateVector(VT_UI1, 0, cb);
        VARIANT pidlVariant;
        pidlVariant.vt = VT_ARRAY|VT_UI1;
        pidlVariant.parray = pSA;
        CopyMemory(pSA->pvData, pIDL, cb);

        VARIANT flagsVariant;
        flagsVariant.vt = VT_I4;
        flagsVariant.lVal = flags;

        VARIANT targetVariant;
        targetVariant.vt = VT_BSTR;
        targetVariant.bstrVal = SysAllocString(TtoW(pTargetFrameName));

        HRESULT hr = GetIWebBrowser2()->Navigate2(&pidlVariant, &flagsVariant, &targetVariant, 0, 0);

        VariantClear(&pidlVariant);
        VariantClear(&flagsVariant);
        VariantClear(&targetVariant);

        return hr;
    }

    // Navigates the browser to a location that is expressed as a URL.
    inline HRESULT CWebBrowser::Navigate2(LPCTSTR pURL, DWORD flags /*= 0*/, LPCTSTR pTargetFrameName /*= NULL*/,
                     LPCTSTR pHeaders /*= NULL*/,   LPVOID pPostData /*= NULL*/, DWORD postDataLen /*= 0*/)
    {
        VARIANT urlVariant;
        urlVariant.vt = VT_BSTR;
        urlVariant.bstrVal = SysAllocString(TtoW(pURL));

        VARIANT flagsVariant;
        flagsVariant.vt = VT_I4;
        flagsVariant.lVal = flags;

        VARIANT TargetVariant;
        TargetVariant.vt = VT_BSTR;
        TargetVariant.bstrVal = SysAllocString(TtoW(pTargetFrameName));

        // Store the pidl in a SafeArray, and assign the SafeArray to a VARIANT
        SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, postDataLen);
        CopyMemory(psa->pvData, pPostData, postDataLen);
        VARIANT dataVariant;
        dataVariant.vt = VT_ARRAY|VT_UI1;
        dataVariant.parray = psa;

        VARIANT headersVariant;
        headersVariant.vt = VT_BSTR;
        headersVariant.bstrVal = SysAllocString(TtoW(pHeaders));

        HRESULT hr = GetIWebBrowser2()->Navigate2(&urlVariant, &flagsVariant, &TargetVariant, &dataVariant, &headersVariant);

        VariantClear(&urlVariant);
        VariantClear(&flagsVariant);
        VariantClear(&TargetVariant);
        VariantClear(&dataVariant);
        VariantClear(&headersVariant);

        return hr;
    }

    // Sets the value of a property associated with the object.
    inline HRESULT CWebBrowser::PutProperty(LPCTSTR pPropertyName, const VARIANT& value)
    {
        return GetIWebBrowser2()->PutProperty(TtoBSTR(pPropertyName), value);
    }

    // Sets the value of a property associated with the object.
    inline HRESULT CWebBrowser::PutProperty(LPCTSTR pPropertyName, double value)
    {
        VARIANT v;
        v.vt = VT_I4;
        v.dblVal = value;
        HRESULT hr = GetIWebBrowser2()->PutProperty(TtoBSTR(pPropertyName), v);
        VariantClear(&v);
        return hr;
    }

    // Sets the value of a property associated with the object.
    inline HRESULT CWebBrowser::PutProperty(LPCTSTR pPropertyName, long value)
    {
        VARIANT v;
        v.vt = VT_I4;
        v.lVal= value;
        HRESULT hr = GetIWebBrowser2()->PutProperty(TtoBSTR(pPropertyName), v);
        VariantClear(&v);
        return hr;
    }

    // Sets the value of a property associated with the object.
    inline HRESULT CWebBrowser::PutProperty(LPCTSTR pPropertyName, LPCTSTR pValue)
    {
        VARIANT v;
        v.vt = VT_BSTR;
        v.bstrVal= SysAllocString(TtoW(pValue));
        HRESULT hr = GetIWebBrowser2()->PutProperty(TtoBSTR(pPropertyName), v);
        VariantClear(&v);
        return hr;
    }

    // Sets the value of a property associated with the object.
    inline HRESULT CWebBrowser::PutProperty(LPCTSTR pPropertyName, short value)
    {
        VARIANT v;
        v.vt = VT_I4;
        v.iVal = value;
        HRESULT hr = GetIWebBrowser2()->PutProperty(TtoBSTR(pPropertyName), v);
        VariantClear(&v);
        return hr;
    }

    // Reloads the file that is currently displayed in the object.
    inline HRESULT CWebBrowser::Refresh()
    {
        return GetIWebBrowser2()->Refresh();
    }

    // Reloads the file that is currently displayed with the specified refresh level.
    inline HRESULT CWebBrowser::Refresh2(int level)
    {
        VARIANT v;
        v.vt = VT_I4;
        v.intVal = level;
        HRESULT hr = GetIWebBrowser2()->Refresh2(&v);
        VariantClear(&v);
        return hr;
    }

    // Cancels a pending navigation or download, and stops dynamic page elements, such as background sounds and animations.
    inline HRESULT CWebBrowser::Stop()
    {
        return GetIWebBrowser2()->Stop();
    }

}

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( pop )
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

#endif  // _WIN32XX_WEBBROWSER_H_

