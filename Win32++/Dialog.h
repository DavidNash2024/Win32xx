// Win32++  Version 5.11 beta
// Released: 27th February, 2007 by:
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
// Dialog.h
//  Declaration of the CDialog class
//

#ifndef DIALOG_H
#define DIALOG_H

#include "WinCore.h"


namespace Win32xx
{
	class CDialog : public CWnd
	{
	public:
		CDialog(UINT nResID, HWND hWndParent = NULL);
		CDialog(LPCTSTR lpszResName, HWND hWndParent = NULL);
		CDialog(LPCDLGTEMPLATE lpTemplate, HWND hWndParent = NULL);
		virtual ~CDialog();
		virtual INT_PTR DoModal();
		virtual HWND DoModeless();

	protected:
		virtual POINT Center(HWND hWnd);
		virtual BOOL DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void EndDialog(INT_PTR nResult);
		virtual void OnCancel();
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		static BOOL CALLBACK StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		BOOL IsIndirect;				// a flag for Indirect dialogs
		BOOL IsModal;					// a flag for modal dialogs
		LPCTSTR m_lpszResName;			// the resource name for the dialog
		LPCDLGTEMPLATE m_lpTemplate;	// the dialog template for indirect dialogs

	};

} // namespace Win32xx

#endif // DIALOG_H

