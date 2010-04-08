/////////////////////////////////
// View.h


/////////////////////////////////////////////////////////////
// An important note for VS 2005 Express and VS 2008 Express users
//  To compile this program you will need to do the following
//
//  1) The SDK shipped with VS 2008 Express does not include ATL.
//      Users of VS2008 Express will need to download a Platform SDK.
//      Users of VS2005 Express need to download a Platform SDK for 
//      Win32 programming in any case.
//      Note that the more modern Windows SDK doesn't include ATL, so
//      we must use the older Plaform SDK, currently available at:
//      http://www.microsoft.com/msdownload/platformsdk/sdkupdate/
//
//     Add the include\atl path of Platform SDK to the set of
//      directories used for include files
//
//  2) Modify atlwin.h
//      Change SetChainEntry function at line 1725 and define
//      "int i" at the first line of the function body
//
//  3) Modify atlbase.h
//      Change AllocStdCallThunk and FreeStdCallThunk at line 287
//
//      Make these changes:
//
//		/* Comment these lines
//		PVOID __stdcall __AllocStdCallThunk(VOID);
//		VOID __stdcall __FreeStdCallThunk(PVOID);
//
//		#define AllocStdCallThunk() __AllocStdCallThunk()
//		#define FreeStdCallThunk(p) __FreeStdCallThunk(p)
//
//		#pragma comment(lib, "atlthunk.lib")
//		*/
//
//      and add these 2 lines:
//		#define AllocStdCallThunk() HeapAlloc(GetProcessHeap(), 0, sizeof(_stdcallthunk))
//		#define FreeStdCallThunk(p) HeapFree(GetProcessHeap(), 0, p)
//
//
//		This information was published by Baryon Lee, in the CodeProject article
//      titled "Using WTL with Visual C++ 2005 Express Edition"
//      URL:  http://www.codeproject.com/wtl/WTLExpress.asp
//
//      This information was tested on Platform SDK version Windows Server 2003 R2
/////////////////////////////////////////////////////////////////////////////////////////


#ifndef VIEW_H
#define VIEW_H


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
										{
											 UNREFERENCED_PARAMETER(pctinfo);
											 return E_NOTIMPL;
										}
	virtual HRESULT __stdcall GetTypeInfo(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
										{
											UNREFERENCED_PARAMETER(itinfo);
											UNREFERENCED_PARAMETER(lcid);
											UNREFERENCED_PARAMETER(pptinfo);
											return E_NOTIMPL;
										}
	virtual HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
										{
											UNREFERENCED_PARAMETER(riid);
											UNREFERENCED_PARAMETER(rgszNames);
											UNREFERENCED_PARAMETER(cNames);
											UNREFERENCED_PARAMETER(lcid);
											UNREFERENCED_PARAMETER(rgdispid);
											return E_NOTIMPL;
										}
};


#endif
