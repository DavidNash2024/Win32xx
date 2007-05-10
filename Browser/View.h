/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// Note: This code requires the following libraries to be linked:
//  comsupp.lib (for ConvertStringToBSTR support)
//  atl.lib		(assuming you want to link ATL staticly)
//



#include <exdisp.h>				// WebBrowser2 support
#include <atlbase.h>			// ATL support
#include <comutil.h>			// For ConvertStringToBSTR support
#include "..\Win32++\Wincore.h"	

//Required for static linking of ATL
#include <atlhost.h>
#ifdef _ATL_STATIC_REGISTRY
  #include <statreg.h>
#endif



class CView : public CWnd
{
public:
	CView();
	virtual ~CView();
	
	// ATL expects a module class to be used, especially when it is 
	// linked as a static library. This satisfies that requirement.
	struct CAtlDummyModule : ATL::CAtlModule
	{
		CAtlDummyModule() {memset(&m_libid, 1, sizeof(GUID));}
		virtual HRESULT AddCommonRGSReplacements(IRegistrarBase*) { return S_OK; }
	};

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	IWebBrowser2* m_pInetExplorer;
	CAtlDummyModule m_MyModule;
};

#endif
