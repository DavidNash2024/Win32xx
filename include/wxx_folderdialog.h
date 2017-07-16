// Win32++   Version 8.4.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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
//
//	Coding example.
//
//		CFolderDialog fd;
//
//		// Set the root folder to list the computer's drives (or C:).
//		ITEMIDLIST* pidlRoot = 0;
//		SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidlRoot);
//		// or
//		// ITEMIDLIST* pidl = ILCreateFromPath(_T("C:"));
//		fd.SetRoot(pidlRoot);
//
//		// Set the title for the dialog.
//		fd.SetTitle(_T("Choose a folder"));
//
//		// Display the dialog
//		if (fd.DoModal() == IDOK)
//		{
//			// Do something with the folder found
//			TRACE(fd.GetFolderPath()); TRACE("\n");
//		}
//
//		// Release the memory allocated for pidlRoot.
//		CoTaskMemFree(pidlRoot);
//

#ifndef _WIN32XX_FOLDERDIALOG_H_
#define _WIN32XX_FOLDERDIALOG_H_


#include "wxx_dialog.h"
#include <shlobj.h>


namespace Win32xx
{

	class CFolderDialog : public CDialog
	{
	public:
		CFolderDialog(UINT nID = 0);
		virtual ~CFolderDialog();

		virtual INT_PTR DoModal(HWND hWndParent = 0);

		CString GetDisplayName() const       { return m_strDisplayName; }
		CString GetFolderPath() const;
		LPITEMIDLIST GetFolderPidl() const   { return m_vPidl.back(); }
		int  GetImageIndex() const           { return m_imageIndex; }
		int  GetFlags() const                { return m_flags; }
		void EnableOK(BOOL bEnable = TRUE);
		void SetExpanded(LPCWSTR pszPath);
		void SetExpanded(LPITEMIDLIST pItemIDList);
		void SetFlags(UINT flags) { m_flags = flags; }
		void SetOKText(LPCWSTR pszText);
		void SetRoot(LPITEMIDLIST pidl);
		void SetSelection(LPITEMIDLIST pItemIDList);
		void SetSelection(LPCTSTR pszPath);
		void SetStatusText(LPCTSTR pszText);
		void SetTitle(LPCTSTR pszTitle);

	protected:
		virtual void OnCancel();
		virtual void OnInitialized();
		virtual void OnIUnknown(LPARAM lParam);
		virtual void OnOK();
		virtual void OnSelChanged();
		virtual int  OnValidateFailed(LPARAM lParam);

	private:
		static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

		CString m_strDisplayName;
		CString m_strTitle;
		BROWSEINFO m_bi;
		LPITEMIDLIST m_pidlRoot;
		int m_imageIndex;
		UINT m_flags;
		std::vector<LPITEMIDLIST> m_vPidl;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	inline CFolderDialog::CFolderDialog(UINT nID) : CDialog(nID), m_imageIndex(0), m_pidlRoot(0)
	{
		ZeroMemory(&m_bi, sizeof(m_bi));
		m_bi.lpfn = BrowseCallbackProc;
		m_bi.lParam = reinterpret_cast<LPARAM>(this);
		
		// Set the default flags.
		//  BIF_NEWDIALOGSTYLE    - Only return file system directories.
		//  BIF_NEWDIALOGSTYLE    - Provides a resizable dialog without an edit box.
		//  BIF_NONEWFOLDERBUTTON - Do not include the New Folder button in the browse dialog box.
		m_flags = BIF_RETURNONLYFSDIRS |BIF_NEWDIALOGSTYLE | BIF_NONEWFOLDERBUTTON;
		::CoInitialize(NULL);
	}
 

	inline CFolderDialog::~CFolderDialog()
	{
		// Free the memory allocated to our pidls.
		std::vector<LPITEMIDLIST>::iterator it;
		for (it = m_vPidl.begin(); it != m_vPidl.end(); ++it)
			CoTaskMemFree(*it);

		::CoUninitialize();
	}


	// The callback function used used to send messages to and process messages
	// from a Browse dialog box displayed in response to a call to SHBrowseForFolder.
	inline int CALLBACK CFolderDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		CFolderDialog* pThis = (CFolderDialog*)lpData;
		int result = 0;

		if (pThis->m_hWnd == 0)
			pThis->Attach(hwnd);

		switch (uMsg)
		{
		case BFFM_INITIALIZED:
			pThis->OnInitialized();
			break;
		case BFFM_SELCHANGED:
			pThis->OnSelChanged();
			break;
		case BFFM_VALIDATEFAILED:
			result = pThis->OnValidateFailed(lParam);
			break;
		case BFFM_IUNKNOWN:
			pThis->OnIUnknown(lParam);
			break;
		}

		return result;
	}


	// Displays the folder browser dialog.
	inline INT_PTR CFolderDialog::DoModal(HWND hWndParent)
	{
		m_bi.lpszTitle = m_strTitle.c_str();
		m_bi.pszDisplayName = m_strDisplayName.GetBuffer(MAX_PATH);
		m_bi.ulFlags = m_flags;
		m_bi.hwndOwner = hWndParent;
		m_bi.pidlRoot = m_pidlRoot;

		LPITEMIDLIST pidl = ::SHBrowseForFolder(&m_bi);
		m_strDisplayName.ReleaseBuffer();

		INT_PTR result = 0;
		if (pidl)
		{
			m_vPidl.push_back(pidl);
			result = IDOK;
			OnOK();
		}
		else
		{
			result = IDCANCEL;
			OnCancel();
		}

		// The dialog is closed so detach the HWND.
		Detach();

		return result;
	}


	// Enables or disables the OK button
	inline void CFolderDialog::EnableOK(BOOL bEnable /*TRUE*/)
	{
		SendMessage(BFFM_ENABLEOK, bEnable, 0L);
	}


	// Returns the path of the selected folder.
	CString CFolderDialog::GetFolderPath() const
	{
		CString str;
		SHGetPathFromIDList(m_vPidl.back(), str.GetBuffer(MAX_PATH));
		str.ReleaseBuffer(MAX_PATH);

		return str;
	}


	// Called when the cancel button is pressed
	inline void CFolderDialog::OnCancel()
	{
	}


	// Called when the Folder dialog is displayed
	// Override this function to perform tasks when the dialog starts.
	inline void CFolderDialog::OnInitialized()
	{
	//  An example of things that can be done here.

	//	EnableOK(FALSE);
	//	SetOKText(L"OK Text");
	//	SetStatusText(_T("Here is some status text"));
	//	SetExpanded(L"C:\\Program Files");
	//	SetSelection(_T("C:\\Temp"));
	}
	

	// Called when an IUnknown interface is available to the dialog box.
	inline void CFolderDialog::OnIUnknown(LPARAM)
	{		
	}
	

	// Called when the OK button is pressed.
	inline void CFolderDialog::OnOK()
	{
	}


	// Called when the selection has changed in the dialog box.
	inline void CFolderDialog::OnSelChanged()
	{
	}
	

	// Called when the user typed an invalid name into the dialog's edit box.
	inline int CFolderDialog::OnValidateFailed(LPARAM)
	{	
		// returns zero to dismiss the dialog or nonzero to keep the dialog displayed.
		return 1;
	}


	// Specifies the path of a folder to expand in the Browse dialog box.
	inline void CFolderDialog::SetExpanded(LPCWSTR pszPath)
	{
		SendMessage(BFFM_SETEXPANDED, TRUE, (LPARAM)pszPath);
	}


	// Specifies the path of a folder to expand in the Browse dialog box.
	inline void CFolderDialog::SetExpanded(LPITEMIDLIST pItemIDList)
	{
		SendMessage(BFFM_SETEXPANDED, FALSE, (LPARAM)pItemIDList);
	}


	// Sets the text of the OK button.
	inline void CFolderDialog::SetOKText(LPCWSTR pszText)
	{
		SendMessage(BFFM_SETOKTEXT, 0, (LPARAM)pszText);
	}

	
	// Sets the location of the root folder from which to start browsing.
	inline void CFolderDialog::SetRoot(LPITEMIDLIST pidl)
	{
		m_pidlRoot = pidl;
	}

	
	// Specifies the path of a folder to select.
	inline void CFolderDialog::SetSelection(LPITEMIDLIST pItemIDList)
	{
		SendMessage(BFFM_SETSELECTION, FALSE, (LPARAM)pItemIDList);
	}

	
	// Specifies the path of a folder to select.
	inline void CFolderDialog::SetSelection(LPCTSTR pszPath)
	{
		SendMessage(BFFM_SETSELECTION, TRUE, (LPARAM)pszPath);
	}

	
	// Sets the status text.
	// This is incompatible with the BIF_USENEWUI or BIF_NEWDIALOGSTYLE flags.
	inline void CFolderDialog::SetStatusText(LPCTSTR pszText)
	{
		SendMessage(BFFM_SETSTATUSTEXT, 0, (LPARAM)pszText);
	}


	// Sets the title of the folder browse dialog.
	inline void CFolderDialog::SetTitle(LPCTSTR pszTitle)
	{
		if (pszTitle)
			m_strTitle = pszTitle;
		else
			m_strTitle.Empty();

		m_bi.lpszTitle = m_strTitle.c_str();
	}

}
#endif // _WIN32XX_FOLDERDIALOG_H_

