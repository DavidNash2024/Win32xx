// Win32++  Version 6.7
// Released: 6th November, 2009 by:
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
// dialog.h
//  Declaration of the CDialog class

// CDialog adds support for dialogs to Win32++. Dialogs are specialised
// windows which are a parent window for common controls. Common controls
// are special window types such as buttons, edit controls, tree views,
// list views, static text etc.

// The layout of a dialog is typically defined in a resource script file
// (often Resource.rc). While this script file can be constructed manually,
// it is often created using a resource editor. If your compiler doesn't
// include a resource editor, you might find ResEdit useful. It is a free
// resource editor available for download at:
// http://www.resedit.net/

// CDialog supports modal and modeless dialogs. It also supports the creation
// of dialogs defined in a resource script file, as well as those defined in
// a dialog template.

// Use the Dialog generic program as the starting point for your own dialog
// applications.
// The DlgSubclass sample demonstrates how to use subclassing to customise
// the behaviour of common controls in a dialog.


#ifndef _DIALOG_H_
#define _DIALOG_H_

#include "wincore.h"


namespace Win32xx
{

	class CDialog : public CWnd
	{
	public:
		CDialog(UINT nResID, HWND hParent = NULL);
		CDialog(LPCTSTR lpszResName, HWND hParent = NULL);
		CDialog(LPCDLGTEMPLATE lpTemplate, HWND hParent = NULL);
		virtual ~CDialog();

		// You probably won't need to override these functions
		virtual HWND Create(HWND hParent = 0);
		virtual INT_PTR DoModal();
		virtual HWND DoModeless();
		virtual void SetDlgParent(HWND hParent);

	protected:
		// These are the functions you might wish to override
		virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void EndDialog(INT_PTR nResult);
		virtual void OnCancel();
		virtual BOOL OnInitDialog();
		virtual void OnOK();

		// Can't override this function
		static BOOL CALLBACK StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CDialog(const CDialog&);				// Disable copy construction
		CDialog& operator = (const CDialog&); // Disable assignment operator

		BOOL IsIndirect;				// a flag for Indirect dialogs
		BOOL IsModal;					// a flag for modal dialogs
		LPCTSTR m_lpszResName;			// the resource name for the dialog
		LPCDLGTEMPLATE m_lpTemplate;	// the dialog template for indirect dialogs
		HWND m_hDlgParent;				// handle to the dialogs's parent window
	};
}


#endif // _DIALOG_H_

