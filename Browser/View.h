/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// Note: This code requires the following libraries to be linked:
//  comsupp.lib (for ConvertStringToBSTR support)
//  atl.lib		(assuming you want to link ATL staticly)
//


#include <exdisp.h>				// WebBrowser2 support
#include <exdispid.h>

#include <atlbase.h>			// ATL support

// For VS Studio 6 (ATL v3.0) and below
#if defined (_MSC_VER) && _MSC_VER <= 1200
  extern CComModule _Module;
  typedef long LONG_PTR;
#else
  struct CAtlDummyModule : CAtlModule
  {
	CAtlDummyModule() {memset(&m_libid, 1, sizeof(GUID));}
	virtual HRESULT AddCommonRGSReplacements(IRegistrarBase*) { return S_OK; }
  };
#endif

#include <atlcom.h>
#include <atlhost.h>

#include <comutil.h>			// For ConvertStringToBSTR support
#include "../Win32++/Wincore.h"



class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual IWebBrowser2* GetIWebBrowser2() {return m_pInetExplorer;}
	virtual void Navigate(LPCTSTR str);

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	IWebBrowser2* m_pInetExplorer;
	LPUNKNOWN m_pSourceUnk;
	LPUNKNOWN m_pSinkUnk;
	DWORD m_dwCustCookie;
};


// An ATL class to the sink events from IWebbrowser2
// It uses multiple inheritance, and message map macros
class ATL_NO_VTABLE CDispatchSink: public IDispatch,
	public CComObjectRootEx<CComSingleThreadModel>
{
public:
// ATL message map macros. These expose interfaces on an object to a client through QueryInterface
BEGIN_COM_MAP(CDispatchSink)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, IDispatch)
END_COM_MAP()

	virtual HRESULT __stdcall Invoke(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
	                                 DISPPARAMS* pdispparams, VARIANT* pvarResult,
									 EXCEPINFO* pexcepinfo, UINT* puArgErr);

	// These must be present, even though they are not implemented
	virtual HRESULT __stdcall GetTypeInfoCount(UINT* pctinfo)
	                                     {return E_NOTIMPL;}
	virtual HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	                                     {return E_NOTIMPL;}
	virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
		                                 {return E_NOTIMPL;}
};


#endif
