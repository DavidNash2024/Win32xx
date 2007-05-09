/////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// Note: This code requires the following libraries to be linked:
//  comsupp.lib (for ConvertStringToBSTR support)
//  atl.lib		(assuming you want to link ATL staticly)
//
// For Visual Studio 2005
//  It seems compiling with ALT staticly is not possible 
//  (or at least more trouble than its worth) so you may need to also
//  ship the ATL80.dll.   Sigh... 


#include <exdisp.h>				// WebBrowser2 support
#include <atlbase.h>			// ATL support
#include <comutil.h>			// For ConvertStringToBSTR support


#include "..\Win32++\Wincore.h"	


class CView : public CWnd
{
public:
	CView();
	virtual ~CView();

protected:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	IWebBrowser2* m_pInetExplorer;
};

#endif
