// Win32++  Version 5.2
// Released: 20th May, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2007  David Nash
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
		CMDIApp(HINSTANCE hInstance);
	    virtual ~CMDIApp() {}
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
		virtual HWND Create(HWND hWndParent = NULL);
		virtual BOOL IsMDIChild() {return TRUE;}
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual BOOL SetChildMenu(LPCTSTR MenuName);
		virtual void UpdateFrameMenu(HMENU hMenu);

	protected:
		enum Constants
		{
			MAX_MENU_STRING = 32,
		};

		virtual LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		HMENU m_hChildMenu;
	};


	//////////////////////////////////////
	// Declaration of the CMDIClient class
	//
	class CMDIClient : public CWnd
	{
	public:
		CMDIClient();
		virtual ~CMDIClient();
		virtual HWND Create(HWND hWndParent /* = NULL*/);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};


	/////////////////////////////////////
	// Declaration of the CMDIFrame class
	//
	class CMDIFrame : public CFrame
	{
		friend class CMDIClient;	// CMDIClient uses RemoveMDIChild

	public:
		CMDIFrame();
		virtual ~CMDIFrame();
		virtual CMDIClient& GetMDIClient() {return m_MDIClient;}
		virtual HWND GetActiveMDIChild(BOOL* pIsMaxed = NULL );
		virtual BOOL IsMDIChildMaxed();
		std::vector <CMDIChild*>& GetMDIChildVect() {return m_MDIChildVect;}

	protected:
		virtual LRESULT DefWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnClose();
		virtual void OnWindowPosChanged();
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void AddMDIChild(CMDIChild* pMDIChild);
		virtual void RemoveMDIChild(HWND hWnd);
		virtual BOOL RemoveAllMDIChildren();
		virtual void RecalcLayout();

	private:
		CMDIClient m_MDIClient;
		std::vector <CMDIChild*> m_MDIChildVect;
	};

} // namespace Win32xx

#endif // MDI_H

