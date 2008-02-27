// Win32++  Version 6.0
// Released: 4th March, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
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


//////////////////////////////////////////
// MDI.h
//  Declaration of the CMDIApp, CMDIChild,
//   CMDIClient,  CMDIFrame classes

// The classes defined here add MDI frames support to Win32++. MDI
// (Multiple Document Interface) frames host one or more child windows. The
// child windows hosted by a MDI frame can be different types. For example,
// some MDI child windows could be used to edit text, while others could be
// used to display a bitmap. Four classes are defined here to support MDI
// frames:

// 1) CMDIApp.  This class inherits from CWinApp. It modifies the message loop
//    to make it compatible with MDI applications. Inherit from this class to
//    start your Win32++ MDI application.
//
// 2) CMDIFrame. This class inherits from CFrame, and adds the functionality
//    required by MDI frames. It keeps track of the MDI children created and
//    destroyed, and adjusts the menu when a MDI child is activated. Use the
//    AddMDIChild function to add MDI child windows to the MDI frame. Inherit
//    from CMDIFrame to create your own MDI frame.
//
// 3) CMDIClient: This is an internal class used by CMDIFrame. You won't need
//    to use this class directly.
//
// 4) CMDIChild: All MDI child windows (ie. CWnd classes) should inherit from
//    this class. Each MDI child type can have a different frame menu.

// Use the MDIFrame generic application as the starting point for your own MDI
// frame applications.
// Refer to the MDIDemo sample for an example on how to use these classes to
// create a MDI frame application with different types of MDI child windows.


#ifndef MDI_H
#define MDI_H

#include <vector>
#include "Frame.h"


namespace Win32xx
{

	///////////////////////////////////
	// Declaration of the CMDIApp class
	//
	class CMDIApp : public CWinApp
	{
	public:
		CMDIApp();
	    virtual ~CMDIApp() {}

		// These are the functions you might wish to override
		virtual int MessageLoop();

	protected:
		HACCEL m_hAccel;
	};


	/////////////////////////////////////
	// Declaration of the CMDIChild class
	//
	class CMDIChild : public CWnd
	{
	public:
		CMDIChild();
		virtual ~CMDIChild();

		// These are the functions you might wish to override
		virtual HWND Create(HWND hWndParent = NULL);

		// Its unlikely you would need to override these functions
		virtual BOOL IsMDIChild() const {return TRUE;}
		virtual BOOL SetChildMenu(LPCTSTR MenuName);

	protected:
		// Its unlikely you would need to override these functions
		virtual LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		HMENU m_hChildMenu;
	};


	//////////////////////////////////////
	// Declaration of the CMDIClient class
	//
	class CMDIClient : public CWnd
	{
	public:
		// Its unlikely you would need to inherit from this class
		CMDIClient();
		virtual ~CMDIClient();
		virtual HWND Create(HWND hWndParent = NULL);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};


	/////////////////////////////////////
	// Declaration of the CMDIFrame class
	//
	class CMDIFrame : public CFrame
	{
		friend class CMDIClient;	// CMDIClient uses RemoveMDIChild
		friend class CMDIChild;     // CMDIChild uses m_hOrigMenu

	public:
		CMDIFrame();
		virtual ~CMDIFrame();

		// These are the functions you might wish to override
		virtual void AppendMDIMenu(HMENU hMenuWindow);
		virtual void UpdateFrameMenu(HMENU hMenu);

		// These functions aren't virtual, so don't override them
		std::vector <CMDIChild*>& GetMDIChildVect() {return m_MDIChildVect;}
		CMDIClient& GetMDIClient() {return m_MDIClient;}
		HWND GetActiveMDIChild() const {return m_hActiveMDIChild;}
		CMDIChild* GetActiveMDIChildCWnd() const;
		BOOL IsMDIChildMaxed() const;

	protected:
		// These are the functions you might wish to override
		virtual void OnClose();
		virtual BOOL OnCommandFrame(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanged();
		virtual void RecalcLayout();

		// Its unlikely you would need to override these functions
		virtual void AddMDIChild(CMDIChild* pMDIChild);
		virtual LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void RemoveMDIChild(HWND hWnd);
		virtual BOOL RemoveAllMDIChildren();
		virtual LRESULT WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CMDIClient m_MDIClient;
		std::vector <CMDIChild*> m_MDIChildVect;
		HWND m_hActiveMDIChild;
	};

} // namespace Win32xx

#endif // MDI_H

