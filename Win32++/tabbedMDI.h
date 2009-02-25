// Win32++  Version 6.4.1
// Released: 12th February, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2009  David Nash
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


////////////////////////////////////////////////////
// TabbedMDI.h - Declaration of the CTabbedMDI class 
//        


#ifndef TABBEDMDI_H
#define TABBEDMDI_H

#include "tab.h"

namespace Win32xx
{
	  ////////////////////////////////////////
	  // Declaration of the CTabbedMDI class
	  class CTabbedMDI : public CWnd
	  {
	  public:
	  	CTabbedMDI() {}
	  	virtual ~CTabbedMDI() {}
	  	virtual CWnd* AddMDIChild(CWnd* pWnd, LPCTSTR szTabText);
	  	virtual void RecalcLayout();
	  
	  protected:
	  	virtual HWND Create(HWND hWndParent);
	  	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	  
	  private:
	  	CTab m_Tab;
	  };
  
  
	////////////////////////////////////////
	// Definitions for the CTabbedMDI class
	inline CWnd* CTabbedMDI::AddMDIChild(CWnd* pWnd, LPCTSTR szTabText)
	{
		if (NULL == pWnd)
			throw CWinException(_T("Cannot add Null MDI Child"));
		
		m_Tab.AddTabPage(pWnd, szTabText);
		if (!m_Tab.IsWindow())
		{
			m_Tab.Create(m_hWnd);
			RecalcLayout();
		}

		return pWnd;
	}

	inline HWND CTabbedMDI::Create(HWND hWndParent /* = NULL*/)
	{
		CLIENTCREATESTRUCT clientcreate ;
		clientcreate.hWindowMenu  = m_hWnd;
		clientcreate.idFirstChild = IDW_FIRSTCHILD ;
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | MDIS_ALLCHILDSTYLES;

		// Create the view window
		if (!CreateEx(0, _T("MDICLient"), _T(""),
			dwStyle, 0, 0, 0, 0, hWndParent, NULL, (PSTR) &clientcreate))
				throw CWinException(_T("CMDIClient::Create ... CreateEx failed"));

		return m_hWnd;
	}

	inline void CTabbedMDI::RecalcLayout()
	{
		if (m_Tab.GetItemCount() >0)
		{
			CRect rc = GetClientRect();
			m_Tab.SetWindowPos(NULL, rc, SWP_SHOWWINDOW);
		}
		else
			m_Tab.ShowWindow(SW_HIDE);
	}

	inline LRESULT CTabbedMDI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case WM_ERASEBKGND:
			if (m_Tab.GetItemCount() >0)
				return 0;
			break;
		case WM_WINDOWPOSCHANGED:
			RecalcLayout();
			break;
		}

		return WndProcDefault(hWnd, uMsg, wParam, lParam);
	}

}

#endif // TABBEDMDI_H