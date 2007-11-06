/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H


#include "../Win32++/Wincore.h"
#include <exdisp.h>				// WebBrowser2 support
#include <exdispid.h>
#include <atlbase.h>			// ATL support
extern CComModule _Module;		// Used later to initialise ATL
#include <atlcom.h>				
#include <atlhost.h>			// ActiveX support


// Define LONG_PTR for Visual Studio 6
#if defined (_MSC_VER) && _MSC_VER <= 1200
  #define LONG_PTR LONG
#endif 


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	virtual IWebBrowser2* GetIWebBrowser2() {return m_pInetExplorer;}
	virtual void Navigate(LPCTSTR str);

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);

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
