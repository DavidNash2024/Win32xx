// Win32++  Version 6.61
// Released: 30th August, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
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


////////////////////////////////////////////////////////
// mdi.h
//  Declaration of the CMDIChild and CMDIFrame classes

// The classes defined here add MDI frames support to Win32++. MDI
// (Multiple Document Interface) frames host one or more child windows. The
// child windows hosted by a MDI frame can be different types. For example,
// some MDI child windows could be used to edit text, while others could be
// used to display a bitmap. Four classes are defined here to support MDI
// frames:


// 1) CMDIFrame. This class inherits from CFrame, and adds the functionality
//    required by MDI frames. It keeps track of the MDI children created and
//    destroyed, and adjusts the menu when a MDI child is activated. Use the
//    AddMDIChild function to add MDI child windows to the MDI frame. Inherit
//    from CMDIFrame to create your own MDI frame.
//
// 2) CMDIChild: All MDI child windows (ie. CWnd classes) should inherit from
//    this class. Each MDI child type can have a different frame menu.

// Use the MDIFrame generic application as the starting point for your own MDI
// frame applications.
// Refer to the MDIDemo sample for an example on how to use these classes to
// create a MDI frame application with different types of MDI child windows.


#ifndef _MDI_H_
#define _MDI_H_

#include "frame.h"
#include <vector>



namespace Win32xx
{
	/////////////////////////////////////
	// Declaration of the CMDIChild class
	//
	class CMDIChild : public CWnd
	{
		friend class CMDIFrame;
	public:
		CMDIChild();
		virtual ~CMDIChild();

		// These are the functions you might wish to override
		virtual HWND Create(HWND hWndParent = NULL);

		// Its unlikely you would need to override these functions
		virtual BOOL IsMDIChild() const {return TRUE;}
		virtual BOOL SetChildMenu(LPCTSTR MenuName);
		virtual CWnd* GetView() const	{return m_pwndView;}
		virtual void SetView(CWnd& pView);

	protected:
		// Its unlikely you would need to override these functions
		virtual LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIChild(const CMDIChild&);				// Disable copy construction
		CMDIChild& operator = (const CMDIChild&); // Disable assignment operator
		
		CWnd* m_pwndView;				// pointer to the View CWnd object
		HMENU m_hChildMenu;
	};

	/////////////////////////////////////
	// Declaration of the CMDIFrame class
	//
	class CMDIFrame : public CFrame
	{
		friend class CMDIChild;     // CMDIChild uses m_hOrigMenu

	public:
		CMDIFrame();
		virtual ~CMDIFrame();
		virtual CMDIChild* AddMDIChild(CMDIChild* pMDIChild);
		virtual BOOL IsMDIFrame() const	 { return TRUE; }
		virtual void RemoveMDIChild(HWND hWnd);
		virtual BOOL RemoveAllMDIChildren();
		virtual void UpdateCheckMarks();

		// These functions aren't virtual, so don't override them
		std::vector <CMDIChild*>& GetAllMDIChildren() {return m_vMDIChild;}
		CMDIChild* GetActiveMDIChild() const;
		BOOL IsMDIChildMaxed() const;
		void SetActiveMDIChild(CMDIChild* pChild);

	protected:
		// These are the functions you might wish to override
		virtual void RecalcLayout();

		// Its unlikely you would need to override these functions
		virtual void OnClose();
		virtual void OnWindowPosChanged();
		virtual void OnViewStatusbar();
		virtual void OnViewToolbar();
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		class CMDIClient : public CWnd  // a nested class within CMDIFrame
		{
		public:
			CMDIClient() {}
			virtual ~CMDIClient() {}
			virtual HWND Create(HWND hWndParent = NULL);
			virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		private:
			CMDIClient(const CMDIClient&);				// Disable copy construction
			CMDIClient& operator = (const CMDIClient&); // Disable assignment operator
		};


		CMDIFrame(const CMDIFrame&);				// Disable copy construction
		CMDIFrame& operator = (const CMDIFrame&); // Disable assignment operator
		void AppendMDIMenu(HMENU hMenuWindow);
		LRESULT FinalWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void UpdateFrameMenu(HMENU hMenu);

		CMDIClient m_wndMDIClient;
		std::vector <CMDIChild*> m_vMDIChild;
		HWND m_hActiveMDIChild;
	};

}



#endif // _MDI_H_

