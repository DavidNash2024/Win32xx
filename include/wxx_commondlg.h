// Win32++   Version 8.4
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
// Acknowledgement:
//
// The original author of these classes is:
//
//      Robert C. Tausworthe
//      email: robert.c.tausworthe@ieee.org
//
////////////////////////////////////////////////////////

#ifndef _WIN32XX_COMMONDLG_H_
#define _WIN32XX_COMMONDLG_H_

#include <wxx_dialog.h>
#include "wxx_richedit.h"



///////////////////////////////////////////////////////////////////
// Definitions of the CCommonDialog, CColorDialog, CFileDialog, 
// CFindReplaceDialog, and CFontDialog classes.
//
// CCommonDialog is the base class for all common dialogs.
// CColorDialog supports the color common dialog.
// CFileDialog supports the FileOpen and FileSave common dialogs.
// CFindReplaceDialog supports the Find and Replace common dialogs.
// CFontDialog supports the Font common dialog.
//
// Note: A CWinException is thrown if the dialog can't be displayed.
// Use CWinException's GetMessageID to retrieve CommDlgExtendedError.



namespace Win32xx
{

	//============================================================================
	class CCommonDialog : public CDialog
	{
	public:
		CCommonDialog(UINT nID = 0) : CDialog((UINT)nID) {}
		virtual ~CCommonDialog(){}

	protected:
		virtual void 	OnCancel()	{}	// a required to override
		virtual	void 	OnHelpButton();
		virtual BOOL	OnInitDialog();
		virtual void 	OnOK()	{}		// a required to override

		// static callback
		static INT_PTR CALLBACK CDHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};



	//============================================================================
	class CColorDialog : public CCommonDialog
	// The color choice common dialog box class.
	{
	public:
		CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0);
		virtual ~CColorDialog(){}

		virtual INT_PTR	DoModal(HWND hWndOwner = 0);
		COLORREF  GetColor() const				{ return m_CC.rgbResult;}
		COLORREF* GetCustomColors()				{ return m_rgbCustomColors;}
		const CHOOSECOLOR& GetParameters() const { return m_CC; }		
		void 	SetColor(COLORREF clr)			{ m_CC.rgbResult = clr;}
		void    SetCustomColors(const COLORREF*	rgbCstmColors = NULL);
		void	SetParameters(CHOOSECOLOR cc);

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);

		// Not intended to be overridden
		INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		// private data
		CHOOSECOLOR 	m_CC;					// ChooseColor parameters
		COLORREF  		m_rgbCustomColors[16];	// Custom colors array
	};



	//============================================================================
	class CFileDialog : public CCommonDialog
	// The file open/save-as common dialog box class.
	{
	public:

		// Constructor/destructor
		CFileDialog (BOOL bOpenFileDialog = TRUE,
				LPCTSTR pszDefExt = NULL,
				LPCTSTR pszFileName = NULL,
				DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				LPCTSTR pszFilter   = NULL );

		virtual ~CFileDialog()	{}

		// Operations
		virtual INT_PTR	DoModal(HWND hWndOwner = 0);

		// methods valid after successful DoModal()
		CString GetFileName() const;
		CString GetFileExt() const;
		CString GetFileTitle() const;
		CString GetFolderPath() const;
		CString GetPathName() const;
		const OPENFILENAME& GetParameters() const { return m_OFN; }		

		// methods for setting parameters before DoModal()
		BOOL	IsOpenFileDialog()					{ return m_bOpenFileDialog; }
		void    SetDefExt(LPCTSTR szExt);
		void    SetFileName(LPCTSTR szFileName);
		void	SetFilter(LPCTSTR szFilter);
		void	SetParameters(OPENFILENAME ofn);
		void    SetTitle(LPCTSTR szTitle);

		// Enumerating multiple file selections
		CString	GetNextPathName(int& pos) const;

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
		virtual void 	OnFileNameChange();
		virtual LRESULT	OnFileNameOK();
		virtual void 	OnFolderChange();
		virtual void 	OnInitDone();
		virtual void 	OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);
		virtual LRESULT	OnNotify(WPARAM, LPARAM);
		virtual LRESULT	OnShareViolation(LPCTSTR lpszPathName);
		virtual void 	OnTypeChange();

		// Not intended to be overridden
		INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		BOOL 			m_bOpenFileDialog;  // TRUE = open, FALSE = save
		CString   		m_sFilter;          // File filter string
		CString			m_sFileName;		// File name string
		CString			m_sTitle;			// Dialog title
		CString         m_sDefExt;			// Default extension string
		OPENFILENAME	m_OFN;				// OpenFileName parameters
	};



	//============================================================================
	class CFindReplaceDialog : public CCommonDialog
	// Find/FindReplace modeless dialogs class
	{
	public:
		// constructor and destructor
		CFindReplaceDialog(BOOL bFindDialogOnly = TRUE);
		virtual ~CFindReplaceDialog() {}

        virtual HWND Create(HWND hWndParent = 0);
		virtual BOOL Create(BOOL bFindDialogOnly,
						LPCTSTR sFindWhat,
						LPCTSTR sReplaceWith = NULL,
						DWORD dwFlags = FR_DOWN,
						HWND hParentWnd = 0);

		virtual BOOL IsModal() const					{ return FALSE; }

		// Operations:
		BOOL 	FindNext() const;           // TRUE = find next
		CString GetFindString() const;      // get find string
		CString	GetReplaceString() const;   // get replacement string
		const	FINDREPLACE& GetParameters() const	{ return m_FR; }		
		BOOL	IsFindDialogOnly() const			{ return m_bFindDialogOnly; }
		BOOL 	IsTerminating();      	    // TRUE = terminate dialog
		BOOL 	MatchCase() const;          // TRUE = matching case
		BOOL 	MatchWholeWord() const;     // TRUE = whole words only
		BOOL 	ReplaceAll() const;         // TRUE = all occurrences
		BOOL 	ReplaceCurrent() const;     // TRUE = current string
		BOOL 	SearchDown() const;         // TRUE = down, FALSE = up
		void	SetParameters(FINDREPLACE fr);

		static CFindReplaceDialog* GetNotifier(LPARAM lParam);

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);

		// Not intended to be overridden
		INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		FINDREPLACE 	m_FR;				// FindReplace parameters
		BOOL			m_bFindDialogOnly;	// TRUE for a find only dialog
		CString 		m_strFindWhat;		// The Find string
		CString 		m_strReplaceWith;	// The Replace string
	};


	//============================================================================
	class CFontDialog : public CCommonDialog
	// The font choice common dialog box class.
	{
	public:
		CFontDialog(const LOGFONT& lfInitial, DWORD dwFlags = 0, HDC hdcPrinter = 0);
		CFontDialog(const CHARFORMAT& charformat, DWORD dwFlags = 0, HDC hdcPrinter = 0);
		CFontDialog(DWORD dwFlags = 0, HDC hdcPrinter = 0);

		virtual ~CFontDialog(void)	{}

		virtual INT_PTR	DoModal(HWND hWndOwner = 0);
		CHARFORMAT	GetCharFormat(void) const;
		COLORREF	GetColor(void) const 		{ return m_CF.rgbColors;}
		CString GetFaceName(void) const			{ return m_LogFont.lfFaceName;}
		LOGFONT	GetLogFont(void) const			{ return m_LogFont;}
		const CHOOSEFONT& GetParameters() const { return m_CF; }
		int 	GetSize(void) const;
		CString GetStyleName(void) const		{ return m_strStyleName;}
		long 	GetWeight(void) const  			{ return m_LogFont.lfWeight;}
		BOOL 	IsBold(void) const				{ return (m_LogFont.lfWeight >= FW_SEMIBOLD);}
		BOOL 	IsItalic(void) const			{ return m_LogFont.lfItalic;}
		BOOL 	IsStrikeOut(void) const			{ return m_LogFont.lfStrikeOut;}
		BOOL 	IsUnderline(void) const 		{ return m_LogFont.lfUnderline;}
		void    SetColor(const COLORREF rgb)	{ m_CF.rgbColors = rgb;}
		void	SetParameters(CHOOSEFONT cf);

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
		virtual void 	OnOK();

		// Not intended to be overridden
		INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		DWORD FillInLogFont(const CHARFORMAT& cf);

		// private data
		LOGFONT 	m_LogFont;			// Font characteristics
		CHOOSEFONT	m_CF;				// ChooseFont parameters
		CString		m_strStyleName;		// Style name on the dialog
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	//////////////////////////////////////////
	// Definitions for the CCommonDialog class
	//

	//============================================================================
	inline	void CCommonDialog::OnHelpButton()
	// Override this function to respond to the help button. The common dialog
	// requires the appropriate flag to be specified before a help button
	// is displayed.
	{
	}

	inline BOOL CCommonDialog::OnInitDialog()
	// Called when the dialog is initialized. Override it in your derived class
	// to automatically perform tasks. The return value is used by WM_INITDIALOG
	{
		return TRUE;
	}

	//============================================================================
	inline INT_PTR CALLBACK CCommonDialog::CDHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	// The callback function for the common dialog's hook procedure. Messages
	// intercepted by the hook procedure are processed here, and forwarded
	// on to the virtual DialogProc function.
	{
		// Find the CWnd pointer mapped to this HWND
		CCommonDialog* pCommonDlg = static_cast<CCommonDialog*>(GetCWndPtr(hWnd));
		if (pCommonDlg == 0)
		{
			// The HWND wasn't in the map, so add it now
			TLSData* pTLSData = GetApp().GetTlsData();
			assert(pTLSData);

			// Retrieve pointer to CWnd object from Thread Local Storage TLS
			pCommonDlg = static_cast<CCommonDialog*>(pTLSData->pWnd);
			assert(pCommonDlg);
			pTLSData->pWnd = NULL;

			// Attach the HWND to the CommonDialog object
			pCommonDlg->Attach(hWnd);
		}

		return pCommonDlg->DialogProc(uMsg, wParam, lParam);
	}


	/////////////////////////////////////////
	// Definitions for the CColorDialog class
	//

	//============================================================================
	inline CColorDialog::CColorDialog(COLORREF clrInit /* = 0 */, DWORD dwFlags /* = 0 */)
	// Construct a CColorDialog object. The initial color, and flags for the 
	// CHOOSECOLOR struct can be specified.  Refer to the description of the 
	// CHOOSECOLOR struct in the Windows API documentation.
	{
		// set the parameters in the CHOOSECOLOR struct
		ZeroMemory(&m_CC,  sizeof(CHOOSECOLOR));
		m_CC.rgbResult = clrInit;
		m_CC.Flags = dwFlags;

		// Set all custom colors to white
		for (int i = 0; i <= 15; ++i)
			m_rgbCustomColors[i] = RGB(255,255,255);

		// Enable the hook proc for the help button
		if (m_CC.Flags & CC_SHOWHELP)
			m_CC.Flags |= CC_ENABLEHOOK;

		// Set the CHOOSECOLOR struct parameters to safe values
		SetParameters(m_CC);
	}

	//============================================================================
	inline INT_PTR CColorDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Dialog procedure for the Color dialog. Override this function to
	// customise the message handling.
	{
		//	Message intercepted by the hook procedure are passed here.
		//	Note: OnCancel and OnOK are not called here. They are called by DoModal
		//  in response to value returned by called by ChooseColor.

		//  A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(uMsg, wParam, lParam);
	}

	//============================================================================
	inline INT_PTR CColorDialog::DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// The Default message handling for CColorDialog. Don't override this
	// function, override DialogProc instead.
	// Note: OnCancel and OnOK are called by DoModal.
	{
		UNREFERENCED_PARAMETER(lParam);

		switch (uMsg)
		{
		case WM_INITDIALOG:		return OnInitDialog();
		case WM_COMMAND:		if (LOWORD(wParam) == pshHelp)	OnHelpButton();
		}


		// Return 0 to allow default processing of the message.
		return 0;
	}

	//============================================================================
	inline INT_PTR CColorDialog::DoModal(HWND hWndOwner /* = 0 */)
	// Display the ChooseColor common dialog box and select the current color.
	// An exception is thrown if the dialog box isn't created.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;

		m_CC.hwndOwner = hWndOwner;

		// invoke the control and save the result on success
		BOOL ok = ::ChooseColor(&m_CC);

		m_hWnd = 0;

		if (!ok)
		{
			DWORD dwError = CommDlgExtendedError();
			if ((dwError != 0) && (dwError != CDERR_DIALOGFAILURE))
				// ignore the exception caused by closing the dialog
				throw CWinException(_T("CColorDialog::DoModal Failed"), dwError);

			OnCancel();
			return IDCANCEL;
		}

		OnOK();
		return IDOK;
	}

	//============================================================================
	inline void CColorDialog::SetCustomColors(const COLORREF* rgbCustomColors /* = NULL */)
	// The rgbCustomColors parameter is a pointer to an array of 16 COLORREF.
	// If the rgbCustomColors is NULL, all custom colors are set to white,
	// otherwise they are set to the colors specified in the rgbCustomColors array.
	{
		for (UINT i = 0; i < 16; i++)
		{
			COLORREF clr = rgbCustomColors? rgbCustomColors[i] : RGB(255,255,255);
			m_rgbCustomColors[i] = clr;
		}
	}

	//============================================================================
	inline void	CColorDialog::SetParameters(CHOOSECOLOR cc)
	// Sets the various parameters of the CHOOSECOLOR struct.
	// The parameters are set to sensible values.
	{
		m_CC.lStructSize	= sizeof(m_CC);
		m_CC.hwndOwner		= 0;			// Set this in DoModal
		m_CC.hInstance		= cc.hInstance;
		m_CC.rgbResult		= cc.rgbResult;
		m_CC.lpCustColors	= m_rgbCustomColors;
		m_CC.Flags			= cc.Flags;
		m_CC.lCustData		= cc.lCustData;
		m_CC.lpfnHook		= reinterpret_cast<LPCCHOOKPROC>(CDHookProc);
		m_CC.lpTemplateName = cc.lpTemplateName;
	}


	////////////////////////////////////////
	// Definitions for the CFileDialog class
	//

	//============================================================================
	inline CFileDialog::CFileDialog(BOOL bOpenFileDialog  /* = TRUE */,
		LPCTSTR pszDefExt /* = NULL */,
		LPCTSTR pszFileName /* = NULL */,
		DWORD dwFlags /* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT */,
		LPCTSTR pszFilter /* = NULL */)
	// Construct a CFileDialog object. bOpenFileDialog specifies the type of 
	// dialog box, OpenFile or SaveFile. The file's default extent and name can
	// be specified, along with the flags for the OPENFILENAME struct. 
	// The pszFilter contains a series of string pairs that specify file filters, 
	// separated by '\0' or '|' chars. Refer to the description of the OPENFILENAME
	// struct in the Windows API documentation.
	{
		// set open/saveas toggle
		m_bOpenFileDialog = bOpenFileDialog;

		// clear out the OPENFILENAME structure
		ZeroMemory(&m_OFN, sizeof(m_OFN));

		// fill in the OPENFILENAME struct
		m_OFN.lpstrFile		= const_cast<LPTSTR>(pszFileName);
		m_OFN.lpstrFilter	= pszFilter;
		m_OFN.lpstrDefExt	= pszDefExt;
		m_OFN.Flags			= dwFlags;

		// Enable the hook proc for the help button
		if (m_OFN.Flags & OFN_SHOWHELP)
			m_OFN.Flags |= OFN_ENABLEHOOK;

		// Safely set the remaining OPENFILENAME values
		SetParameters(m_OFN);
	}

	//============================================================================
	inline INT_PTR CFileDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Dialog procedure for the FileOpen and FileSave dialogs. Override
	// this function to customise the message handling.
	{
		//	Message intercepted by the hook procedure are passed here.
		//	Note: OnCancel and OnOK are not called here. They are called by DoModal
		//  in response to value returned by called by GetOpenFileName or GetSaveFileName.

		//  A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(uMsg, wParam, lParam);
	}

	//============================================================================
	inline	INT_PTR CFileDialog::DialogProcDefault(UINT message, WPARAM wParam, LPARAM lParam)
	// The Default message handling for CFileDialog. Don't override this function,
	// override DialogProc instead.
	// Note: OnCancel and OnOK are called by DoModal.
	{
		switch (message)
		{
			case WM_INITDIALOG:
			{
				// handle the initialization message
 				return OnInitDialog();
			}

			case WM_COMMAND:
			{
				// handle the HELP button for old-style file dialogs:
	 			if (LOWORD(wParam) == pshHelp)
					OnHelpButton();

				return 0;
			}

			case WM_NOTIFY:
			{
				// handle messages for Explorer-style hook procedures:
				if (m_OFN.Flags & OFN_EXPLORER)
				{
					LRESULT lr = OnNotify(wParam, lParam);
					SetWindowLongPtr(DWLP_MSGRESULT, lr);
					return lr;
				}

				return 0;
			}
		}

		// dispatch special open/save file dialog messages
 		if (message == UWM_HELPMSGSTRING)
 		{   // handle the HELP button for old-style file dialogs:
			// (this alternate handler may still be used in some systems)
			OnHelpButton();
			return 0;
		}

		if (message == UWM_LBSELCHSTRING)
		{	// handle the registered list box selection change
			// notifications:
			OnLBSelChangedNotify((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
			return 0;
		}

		if (message == UWM_SHAREVISTRING)
		{   // handle a sharing violation for the selected file that
			// occurred when the user clicked the OK button.
			return OnShareViolation(reinterpret_cast<LPCTSTR>(lParam));
		}

		if (message == UWM_FILEOKSTRING)
		{
			return OnFileNameOK();
		}

		// not processed here
		return 0;
	}


	//============================================================================
	inline INT_PTR CFileDialog::DoModal(HWND hWndOwner /* = 0 */)
	// Display either a FileOpen or FileSave dialog, and allow the user to
	// select various options. An exception is thrown if the dialog isn't created.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;

		m_OFN.hwndOwner = hWndOwner;
		m_OFN.lpstrFile = m_sFileName.GetBuffer(m_OFN.nMaxFile);
		int ok = (m_bOpenFileDialog ? ::GetOpenFileName(&m_OFN) : ::GetSaveFileName(&m_OFN));
		m_sFileName.ReleaseBuffer();
		m_OFN.lpstrFile = const_cast<LPTSTR>(m_sFileName.c_str());
		m_hWnd = 0;

		// the result of the file choice box is processed here:
		if (!ok)
		{
			DWORD dwError = CommDlgExtendedError();
			if (dwError != 0)
			{
				// ignore the exception caused by closing the dialog
				if (dwError != CDERR_DIALOGFAILURE || (m_OFN.Flags & OFN_EXPLORER))
					throw CWinException(_T("CFileDialog::DoModal Failed"), dwError);
			}

			OnCancel();
			return  IDCANCEL;
		}

		OnOK();
		return IDOK;
	}

	//============================================================================
	inline CString CFileDialog::GetFileName() const
	//	Return the name of the file that was entered in the DoModal() operation.
	//	This name consists of only the file title and extension. If the 
	//  OFN_ALLOWMULTISELECT flag is specified, only the first file path selected
	//  will be returned. If so, GetNextPathName can be used to retrieve subsequent
	// 	file names.
	{
		CString strResult = GetPathName();
		int pos = MAX(strResult.ReverseFind(_T("\\")),
			strResult.ReverseFind(_T("/")));
		if (pos >= 0)
			return strResult.Mid(pos + 1);

		strResult.Empty();
		return strResult;
	}

	//============================================================================
	inline CString CFileDialog::GetFileExt() const
	//	Return the file name's extension entered during the DoModal() operation.
	//	If the OFN_ALLOWMULTISELECT option set, only the extension on the first
	//	file path selected will be returned.
	{
		CString strResult = GetFileName();
		int pos = strResult.ReverseFind(_T("."));
		if (pos >= 0)
			return strResult.Mid(pos);

		strResult.Empty();
		return strResult;
	}

	//============================================================================
	inline CString CFileDialog::GetFileTitle() const
	//	Return the title of the file entered in the DoModal() operation. The
	//	title consists of the full path name with directory path and extension
	//	removed.
	{
		CString strResult = GetFileName();
		int pos = strResult.ReverseFind(_T("."));
		if (pos >= 0)
			return strResult.Left(pos);

		return strResult;
	}

	//============================================================================
	inline CString CFileDialog::GetNextPathName(int& pos) const
	//	Return the next file path name from a group of files selected. The 
	//	OFN_ALLOWMULTISELECT flag allows multiple files to be selected. Use pos = 0
	//	to retrieve the first file. The pos parameter is updated to point to the 
	// 	next file name. The pos parameter is set to -1 when the last file is retrieved.
	{
		assert(pos >= 0);

		BOOL bExplorer = m_OFN.Flags & OFN_EXPLORER;
		TCHAR chDelimiter = (bExplorer ? _T('\0') : _T(' '));
		
		CString strFile(m_OFN.lpstrFile + pos, m_OFN.nMaxFile);	// strFile can contain NULLs
		int Index = 0;
		if (pos == 0)
		{
			Index = strFile.Find(chDelimiter);
			
			if ( (Index < 0) || (strFile.GetAt(++Index) == _T('\0')))
			{
				// Only one file selected. m_OFN.lpstrFile contains a single string
				// consisting of the path and file name.
				pos = -1;
				return m_OFN.lpstrFile;
			}
		}

		// Multiple files selected. m_OFN.lpstrFile contains a set of substrings separated
		// by delimiters. The first substring is the path, the following ones are file names.
		
		// Fill strPath with the path
		CString strPath = m_OFN.lpstrFile; // strPath is terminated by first NULL
		if (!bExplorer)
		{
			int nDeliminator = strPath.Find(chDelimiter);
			strPath = strPath.Left(nDeliminator);
		}

		// Fill strFileName with the file name 
		CString strFileName = m_OFN.lpstrFile + pos + Index;
		if (!bExplorer)
		{
			int nDeliminator = strFileName.Find(chDelimiter);
			if (nDeliminator > 0)
				strFileName = strFileName.Left(nDeliminator);
		} 

		// Update pos to point to the next file
		int nFileLen = lstrlen(strFileName);
		if (strFile.GetAt(nFileLen + 1) == _T('\0'))
			pos = -1;
		else
			pos = pos + Index + nFileLen +1;

		
		if (!strPath.IsEmpty())
		{
			// Get the last character from the path
			int nPathLen = strPath.GetLength();
			TCHAR ch = strPath.GetAt(nPathLen -1);

			if ((ch == _T('\\')) || (ch == _T('/')))
			{
				// Path already ends with _T('\\') or _T('/')
				return strPath + strFileName;
			}
		}

		// Add _T('\\') to the end of the path 
		return strPath + _T("\\") + strFileName;
	}

	//============================================================================
	inline CString CFileDialog::GetFolderPath() const
	//	Return the path name of the folder or directory of files retrieved
	//  from the dialog. The final character of the name includes the
	//  directory separation character.
	{
		CString strResult = GetPathName();
		int pos = MAX(strResult.ReverseFind(_T("\\")),
			strResult.ReverseFind(_T("/")));
		if (pos >= 0)
			return strResult.Left(pos + 1);

		strResult.Empty();
		return strResult;
	}

	//============================================================================
	inline CString CFileDialog::GetPathName() const
	//	Returns the full path name of the file that was retrieved from the dialog.
	//  If the m_OFN.Flags member includes the OFN_ALLOWMULTISELECT option,
	//  this method returns only the first selected file. Use the GetNextPathName
	//  to retrieve the remaining file names.
	{
		if ((m_OFN.Flags & OFN_ALLOWMULTISELECT) == 0)
		{
			// just retrieve the path from the OPENFILENAME structure
			return m_OFN.lpstrFile;
		}
		else
		{
			int pos = 0;
			return GetNextPathName(pos);
		}
	}

	//============================================================================
	inline LRESULT CFileDialog::OnFileNameOK()
	//	Normally, overriding this function is not warranted because the
	//	framework provides default validation of file names.
	//  Override this function and return TRUE to reject a file name for
	//  any application specific reasons.
	{
		return 0L;
	}

	//============================================================================
	inline void CFileDialog::OnInitDone()
	//	Override this method to handle the WM_NOTIFY CDN_INITDONE message.
	//	The notification message is sent when the system has finished arranging
	//	controls in the Open or SaveAs dialog to make room for the controls of
	//	the child dialog box, but before the dialog is visible. Notification is
	//	sent only if the dialog box was created with the OFN_EXPLORER style.
	{
	}

	//============================================================================
	inline void CFileDialog::OnFileNameChange()
	//	Override this method to handle the WM_NOTIFY CDN_SELCHANGE message.
	//	The notification message is sent when the user selects a new file or
	//	folder in the file list of the Open or SaveAs dialog box. Notification
	//	is sent only if the dialog box was created with the OFN_EXPLORER style.
	{
	}

	//============================================================================
	inline void CFileDialog::OnFolderChange()
	//	Override this method to handle the WM_NOTIFY CDN_SELCHANGE message.
	//	The notification message is sent when the user selects a new file or
	//	folder in the file list of the Open or SaveAs dialog box. Notification
	//	is sent only if the dialog box was created with the OFN_EXPLORER style.
	{
	}

	//============================================================================
	inline void CFileDialog::OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode)
	//	This method is called whenever the current selection in a list box is
	//	about to change. Override this method to provide custom handling of
	//	selection changes in the list box. The ID of the list or combo box in
	//	which the selection occurred is nIDBox. The index of the current
	//	selection is iCurSel. The control notification code is nCode.
	{
		UNREFERENCED_PARAMETER(nIDBox);
		UNREFERENCED_PARAMETER(iCurSel);
		UNREFERENCED_PARAMETER(nCode);

	}

	//============================================================================
	inline LRESULT CFileDialog::OnNotify(WPARAM wParam, LPARAM lParam)
	//	This method handles the WM_NOTIFY message loop functions of the hook
	//	procedure.
	{
		UNREFERENCED_PARAMETER(wParam);

		OFNOTIFY* pNotify = reinterpret_cast<OFNOTIFY*>(lParam);
		assert(pNotify);
		switch(pNotify->hdr.code)
		{
			case CDN_INITDONE:
				OnInitDone();
				return TRUE;

			case CDN_SELCHANGE:
				OnFileNameChange();
				return TRUE;

			case CDN_FOLDERCHANGE:
				OnFolderChange();
				return TRUE;

			case CDN_SHAREVIOLATION:
				return OnShareViolation(pNotify->pszFile);
	
			case CDN_HELP:
				OnHelpButton();
				return TRUE;

			case CDN_FILEOK:
				return OnFileNameOK();

			case CDN_TYPECHANGE:
				OnTypeChange();
				return TRUE;
		}

		// The framework will call SetWindowLongPtr(DWLP_MSGRESULT, lr) for non-zero returns
		return FALSE;   // not handled
	}

	//============================================================================
	inline LRESULT CFileDialog::OnShareViolation(LPCTSTR lpszPathName )
	//	Override this function to provide custom handling of share violations.
	//	Normally, this function is not needed because the framework provides
	//	default checking of share violations and displays a message box if a
	//	share violation occurs. The path of the file on which the share
	//	violation occurred is lpszPathName. To disable share violation checking,
	//	use the bitwise OR operator to combine the flag OFN_SHAREAWARE with
	//	m_OFN.Flags.

	//  Return one of the following values to indicate how the dialog box
	//  should handle the sharing violation.
	//	OFN_SHAREFALLTHROUGH  - Accept the file name
	//  OFN_SHARENOWARN  - Reject the file name but do not warn the user.
	//                     The application is responsible for displaying a warning message.
	//	OFN_SHAREWARN    - Reject the file name and displays a warning message
	//                     (the same result as if there were no hook procedure).

	{
		UNREFERENCED_PARAMETER(lpszPathName);

		return OFN_SHAREWARN; // default:
	}

	//============================================================================
	inline void CFileDialog::OnTypeChange()
	//	Override this method to handle the WM_NOTIFY CDN_TYPECHANGE message.
	//	The notification message is sent when the user selects a new file type
	//	from the list of file types in the Open or SaveAs dialog box.
	//	Notification is sent only if the dialog box was created with the
	//	OFN_EXPLORER style.
	{
	}

	//============================================================================
	inline void CFileDialog::SetDefExt(LPCTSTR pszExt)
	//	Set the default extension of the dialog box to pszExt.
	//  Only the first three characters are sent to the dialog.
	{
		if (pszExt)
		{
			m_sDefExt = pszExt;
			m_OFN.lpstrDefExt = m_sDefExt.c_str();
		}
		else
		{
			m_sDefExt.Empty();
			m_OFN.lpstrDefExt = NULL;
		}
	}

	//============================================================================
	inline void CFileDialog::SetFileName(LPCTSTR pszFileName)
	//	Set the initial file name in the dialog box to pszFileName.
	{
		// setup initial file name
		if (pszFileName)
		{
			m_sFileName = pszFileName;
			m_OFN.lpstrFile = const_cast<LPTSTR>(m_sFileName.c_str());
		}
		else
		{
			m_sFileName.Empty();
			m_OFN.lpstrFile = NULL;
		}
	}

	inline void CFileDialog::SetFilter(LPCTSTR szFilter)
	//	Set the file choice dialog file name filter string to pszFilter.
	//  The string is a pair of strings delimited by NULL or '|'
	//  The string must be either double terminated, or use '|' instead of '\0' 
	//  For Example: _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0")
	//           or: _T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*|")
	{
		// Clear any existing filter
		m_OFN.lpstrFilter = NULL;
		m_sFilter.Empty();

		// convert any '|' characters in pszFilter to NULL characters
		if (szFilter)
		{
			CString str = szFilter;
			if (str.Find(_T('|')) >= 0)
			{
				str.Replace(_T('|'), _T('\0'));
				m_sFilter = str;
				m_OFN.lpstrFilter = m_sFilter.c_str();
			}
			else
			{
				// szFilter doesn't contain '|', so it should be double terminated
				int i = 0;
				while (i < _MAX_PATH)
				{
					// Search for double termination
					if (szFilter[i] == _T('\0') && szFilter[i + 1] == _T('\0'))
					{
						m_sFilter.Assign(szFilter, i+1);
						m_OFN.lpstrFilter = m_sFilter.c_str();
						break;
					}
					++i;
				}
			}
		}
	}

	//============================================================================
	inline void CFileDialog::SetParameters(OPENFILENAME ofn)
	//	Sets the various parameters of the OPENFILENAME struct.
	//  The parameters are set to sensible values.
	{
		// Set the correct struct size for all Windows versions and compilers
		DWORD StructSize = sizeof(m_OFN);

  #if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			StructSize = OPENFILENAME_SIZE_VERSION_400;
		if (GetWinVersion() >= 2500)
			m_OFN.FlagsEx =		ofn.FlagsEx;
  #endif

		SetFileName(ofn.lpstrFile);
		SetFilter(ofn.lpstrFilter);
		SetTitle(ofn.lpstrFile);

		m_OFN.lStructSize		= StructSize;
		m_OFN.hwndOwner			= 0;			// Set this in DoModal
		m_OFN.hInstance			= GetApp().GetInstanceHandle();
		m_OFN.lpstrCustomFilter = ofn.lpstrCustomFilter;
		m_OFN.nMaxCustFilter	= MAX(_MAX_PATH, ofn.nMaxCustFilter);
		m_OFN.nFilterIndex		= ofn.nFilterIndex;
		m_OFN.nMaxFile			= MAX(_MAX_PATH, ofn.nMaxFile);
		m_OFN.lpstrFileTitle	= ofn.lpstrFileTitle;
		m_OFN.nMaxFileTitle		= MAX(_MAX_PATH, ofn.nMaxFileTitle);
		m_OFN.lpstrInitialDir	= ofn.lpstrInitialDir;
		m_OFN.Flags				= ofn.Flags;
		m_OFN.nFileOffset		= ofn.nFileOffset;
		m_OFN.nFileExtension	= ofn.nFileExtension;
		m_OFN.lpstrDefExt		= ofn.lpstrDefExt;
		m_OFN.lCustData			= ofn.lCustData;
		m_OFN.lpfnHook			= reinterpret_cast<LPCCHOOKPROC>(CDHookProc);
	}

	//============================================================================
	inline void CFileDialog::SetTitle(LPCTSTR szTitle)
	// Sets the title of the fileopen or filesave dialog.
	{
		if (szTitle)
		{
			m_sTitle = szTitle;
			m_OFN.lpstrTitle = m_sTitle.c_str();
		}
		else
		{
			m_sTitle.Empty();
			m_OFN.lpstrTitle = NULL;
		}
	}


	///////////////////////////////////////////////
	// Definitions for the CFindReplaceDialog class
	//

	//============================================================================
	inline CFindReplaceDialog::CFindReplaceDialog(BOOL bFindDialogOnly /* = TRUE */)
	// Constructor for CCFindReplaceDialog. Refer to the Windows API documentation
	// for information of the FINDREPLACE structure.
	{
		ZeroMemory(&m_FR, sizeof(m_FR));
		m_bFindDialogOnly = bFindDialogOnly;
		SetParameters(m_FR);
	}

	//============================================================================	
	//	Create and display either a Find or indReplace dialog box. 	
	inline HWND CFindReplaceDialog::Create(HWND hParentWnd /* = 0*/)
	{
		Create(m_bFindDialogOnly, m_FR.lpstrFindWhat, m_FR.lpstrReplaceWith, m_FR.Flags, hParentWnd);
	    return m_hWnd;
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::Create(BOOL bFindDialogOnly, LPCTSTR pszFindWhat,
			LPCTSTR pszReplaceWith, DWORD dwFlags, HWND hParentWnd /* = 0*/)
	//	Create and display either a Find or indReplace dialog box. lpszFindWhat
	//  is the search string, and lpszReplaceWith is the replace string.
	//  Set dwFlags to a combination of one or more flags the dialog box.
	//  Set hParentWnd to the handle of the dialog box�s parent or owner window.
	//	An exception is thrown if the window isn't created.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		m_bFindDialogOnly = bFindDialogOnly;

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();
		pTLSData->pWnd = this;

		// Initialize the FINDREPLACE struct values
		m_FR.Flags = dwFlags;
		SetParameters(m_FR);
		m_FR.hwndOwner = hParentWnd;

		m_FR.lpstrFindWhat = m_strFindWhat.GetBuffer(m_FR.wFindWhatLen);
		if (pszFindWhat)
			lstrcpyn(m_FR.lpstrFindWhat, pszFindWhat, m_FR.wFindWhatLen);

		m_FR.lpstrReplaceWith = m_strReplaceWith.GetBuffer(m_FR.wReplaceWithLen);
		if (pszReplaceWith)
			lstrcpyn(m_FR.lpstrReplaceWith, pszReplaceWith, m_FR.wReplaceWithLen);

		// Display the dialog box
		HWND hWnd;
		if (bFindDialogOnly)
			hWnd = ::FindText(&m_FR);
		else
			hWnd = ::ReplaceText(&m_FR);

		if (hWnd == 0)
		{
			// Throw an exception when window creation fails
			throw CWinException(_T("CFindReplaceDialog::Create"));
		}

		m_strFindWhat.ReleaseBuffer();
		m_FR.lpstrFindWhat = const_cast<LPTSTR>(m_strFindWhat.c_str());
		m_strReplaceWith.ReleaseBuffer();
		m_FR.lpstrReplaceWith = const_cast<LPTSTR>(m_strReplaceWith.c_str());

		return TRUE;
	}

	inline INT_PTR CFindReplaceDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Dialog procedure for the Find and Replace dialogs. Override this function
	// to customise the message handling.
	{
		//	Message intercepted by the hook procedure are passed here.
		//  A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(uMsg, wParam, lParam);
	}

	//============================================================================
	inline INT_PTR CFindReplaceDialog::DialogProcDefault(UINT message, WPARAM wParam, LPARAM lParam)
	// The Default message handling for CFindReplaceDialog. Don't override this function,
	// override DialogProc instead.
	// Note: OnCancel and OnOK are called by DoModal.
	{
		UNREFERENCED_PARAMETER(lParam);

		switch (message)
		{
		case WM_INITDIALOG:
			{     // handle the initialization message
 				return OnInitDialog();
			}

		case WM_COMMAND:
			{
	 			if (LOWORD(wParam) == pshHelp)
					OnHelpButton();

				return 0;
			}
		}

		return 0;
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::FindNext() const
	//	Call this function to determine whether the user wants to find the next
	//  occurrence of the search string. 
	{
		return ((m_FR.Flags & FR_FINDNEXT )!= 0);
	}

	//============================================================================
	inline	CString CFindReplaceDialog::GetFindString() const
	//	Call this function to return the default string to find.
	{
		return m_FR.lpstrFindWhat;
	}

	//============================================================================
	inline CFindReplaceDialog* CFindReplaceDialog::GetNotifier(LPARAM lParam)
	//	Return a pointer to the current Find/Replace dialog box. This may be
	//	used when owner window responds to the UWM_FINDMSGSTRING message.
	//	The lParam value is that passed in the UWM_FINDMSGSTRING message.
	{
		assert(lParam != 0);
		LPFINDREPLACE pFR = reinterpret_cast<LPFINDREPLACE>(lParam);
		CFindReplaceDialog* pDlg = reinterpret_cast<CFindReplaceDialog*>(pFR->lCustData);
		return pDlg;
	}

	//============================================================================
	inline CString CFindReplaceDialog::GetReplaceString() const
	//	Call this function to return the current replace string.
	{
		return m_FR.lpstrReplaceWith == NULL ? _T("") : m_FR.lpstrReplaceWith;
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::IsTerminating()
	//	Returns TRUE if the user has decided to terminate the dialog box;
	{
		return ((m_FR.Flags & FR_DIALOGTERM) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::MatchCase() const
	//	Return TRUE if the user wants to find occurrences of the search string
	//	that exactly match the case of the search string; otherwise FALSE.
	{
		return ((m_FR.Flags & FR_MATCHCASE) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::MatchWholeWord() const
	//	Return TRUE if the user wants to match only the entire words of the
	//	search string.
	{
		return ((m_FR.Flags & FR_WHOLEWORD) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::ReplaceAll() const
	//	Return TRUE if the user has requested that all strings matching the
	//	replace string be replaced.
	{
		return ((m_FR.Flags & FR_REPLACEALL) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::ReplaceCurrent() const
	//	Return TRUE if the user has requested that the currently selected string
	//	be replaced with the replace string.
	{
		return ((m_FR.Flags & FR_REPLACE) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::SearchDown() const
	//	Return TRUE if the user wants the search to proceed in a downward
	//	direction; FALSE if the user wants the search to proceed in an upward
	//	direction.
	{
		return ((m_FR.Flags & FR_DOWN) != 0);
	}

	//============================================================================
	inline void	CFindReplaceDialog::SetParameters(FINDREPLACE fr)
	//	Sets the various parameters of the FINDREPLACE struct.
	//  The parameters are set to sensible values.
	{
		int MaxChars = 128;

		if (fr.lpstrFindWhat)
		{
			m_strFindWhat = fr.lpstrFindWhat;
			MaxChars = MAX(MaxChars, lstrlen(fr.lpstrFindWhat));
		}
		else
			m_strFindWhat.Empty();

		if (fr.lpstrReplaceWith)
		{
			m_strReplaceWith = fr.lpstrReplaceWith;
			MaxChars = MAX(MaxChars, lstrlen(fr.lpstrReplaceWith));
		}
		else
			m_strReplaceWith.Empty();

		m_FR.lStructSize		= sizeof(FINDREPLACE);
		m_FR.hwndOwner			= 0;		// Set this in Create
		m_FR.hInstance			= GetApp().GetInstanceHandle();
		m_FR.Flags				= fr.Flags;
		m_FR.lpstrFindWhat		= const_cast<LPTSTR>(m_strFindWhat.c_str());
		m_FR.lpstrReplaceWith	= const_cast<LPTSTR>(m_strReplaceWith.c_str());
		m_FR.wFindWhatLen		= static_cast<WORD>(MAX(fr.wFindWhatLen, MaxChars));
		m_FR.wReplaceWithLen	= static_cast<WORD>(MAX(fr.wReplaceWithLen, MaxChars));
		m_FR.lCustData			= reinterpret_cast<LPARAM>(this);
		m_FR.lpfnHook			= reinterpret_cast<LPCCHOOKPROC>(CDHookProc);
		m_FR.lpTemplateName		= fr.lpTemplateName;

		// Enable the hook proc for the help button
		if (m_FR.Flags & FR_SHOWHELP)
			m_FR.Flags |= FR_ENABLEHOOK;
	}


	////////////////////////////////////////
	// Definitions for the CFontDialog class
	//

	//============================================================================
	inline CFontDialog::CFontDialog(const LOGFONT& lfInitial, DWORD dwFlags /* = 0 */,
		HDC hdcPrinter /* = 0 */)
	//	Construct a CFontDialog object from values given. Note that these are
	//	stored into the members of the CHOOSEFONT structure.
	//  Refer to the description of the CHOOSEFONT structure in the Windows API
	//  documentation for more information on these parameters.
	{
		  // clear out logfont, style name, and choose font structure
		ZeroMemory(&m_LogFont, sizeof(m_LogFont));
		ZeroMemory(&m_CF, sizeof(m_CF));

		// set dialog parameters
		m_CF.rgbColors   = 0; // black
		m_CF.lStructSize = sizeof(m_CF);
		m_CF.Flags  = dwFlags;
		m_CF.Flags |= CF_INITTOLOGFONTSTRUCT;
		m_CF.lpLogFont = const_cast<LOGFONT*>(&lfInitial);

		if (hdcPrinter)
		{
			m_CF.hDC = hdcPrinter;
			m_CF.Flags |= CF_PRINTERFONTS;
		}

		// Enable the hook proc for the help button
		if (m_CF.Flags & CF_SHOWHELP)
			m_CF.Flags |= CF_ENABLEHOOK;

		SetParameters(m_CF);
	}

	//============================================================================
	inline CFontDialog::CFontDialog(const CHARFORMAT& charformat, DWORD dwFlags /* = 0 */,
		HDC hdcPrinter /* =  0 */)
	//	Construct a CFontDialog dialog object that can be used to create a font
	//	for use in a rich edit control. The charformat points to a CHARFORMAT
	//	data structure that allows setting some of the font's characteristics
	//	for a rich edit control.
	//  Refer to the description of the CHOOSEFONT structure in the Windows API
	//  documentation for more information on these parameters.
	{
		// clear out logfont, style name, and choose font structure
		ZeroMemory(&m_LogFont, sizeof(m_LogFont));
		ZeroMemory(&m_CF, sizeof(m_CF));

		// set dialog parameters
		m_CF.lStructSize = sizeof(m_CF);
		m_CF.Flags	= dwFlags;
		m_CF.Flags  |= FillInLogFont(charformat);

		if (charformat.dwMask & CFM_COLOR)
			m_CF.rgbColors = charformat.crTextColor;

		if (hdcPrinter)
		{
			m_CF.hDC = hdcPrinter;
			m_CF.Flags |= CF_PRINTERFONTS;
		}

		// Enable the hook proc for the help button
		if (m_CF.Flags & CF_SHOWHELP)
			m_CF.Flags |= CF_ENABLEHOOK;

		SetParameters(m_CF);
	}

	inline CFontDialog::CFontDialog(DWORD dwFlags /* = 0 */, HDC hdcPrinter /* =  0 */)
	//	Construct a default CFontDialog object. 
	{
		// clear out logfont, style name, and choose font structure
		ZeroMemory(&m_LogFont, sizeof(m_LogFont));
		ZeroMemory(&m_CF, sizeof(m_CF));

		// set dialog parameters
		m_CF.rgbColors = 0; // black
		m_CF.lStructSize = sizeof(m_CF);
		m_CF.Flags	= dwFlags;
		
		if (hdcPrinter)
		{
			m_CF.hDC = hdcPrinter;
			m_CF.Flags |= CF_PRINTERFONTS;
		}

		// Enable the hook proc for the help button
		if (m_CF.Flags & CF_SHOWHELP)
			m_CF.Flags |= CF_ENABLEHOOK;		

		SetParameters(m_CF);
	}

	//============================================================================
	inline INT_PTR CFontDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Dialog procedure for the Font dialog. Override this function
	// to customise the message handling.
	{
		//	Message intercepted by the hook procedure are passed here.
		//	Note: OnCancel and OnOK are not called here. They are called by DoModal
		//  in response to value returned by called by ChooseFont.

		//  A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(uMsg, wParam, lParam);
	}

	//============================================================================
	inline INT_PTR CFontDialog::DialogProcDefault(UINT message, WPARAM wParam, LPARAM lParam)
	//	The Default message handling for CFontDialog.
	//  Don't override this function, override DialogProc instead.
	//	Note: OnCancel and OnOK are called by DoModal.
	{
		UNREFERENCED_PARAMETER(lParam);
		if (message == WM_INITDIALOG)
		{
			OnInitDialog();
		}

		// dispatch special commdlg messages
		if (message == WM_COMMAND && LOWORD(wParam) == pshHelp)
			OnHelpButton();

		// default processing
		return 0;
	}

	//============================================================================
	inline INT_PTR CFontDialog::DoModal(HWND hWndOwner /* = 0 */)
	//	Display the FontDialog. hWndOwner specifies dialog's owner window.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;

		m_CF.hwndOwner = hWndOwner;
		m_CF.lpszStyle = m_strStyleName.GetBuffer(80);

		// open the font choice dialog
		BOOL ok = ::ChooseFont(&m_CF);

		m_strStyleName.ReleaseBuffer();
		m_CF.lpszStyle = const_cast<LPTSTR>(m_strStyleName.c_str());
		m_hWnd = 0;

		// process the result of the font choice box:
		if (!ok)
		{
			DWORD dwError = CommDlgExtendedError();
			if ((dwError != 0) && (dwError != CDERR_DIALOGFAILURE))
				// ignore the exception caused by closing the dialog
				throw CWinException(_T("CFontDialog::DoModal Failed"), dwError);

			OnCancel();
 			return IDCANCEL;
		}

		OnOK();
		return IDOK;
	}

	//============================================================================
	inline CHARFORMAT CFontDialog::GetCharFormat() const
	//	Returns the CHARFORMAT of this font object, as translated from the
	//	information in the m_FR CHOOSEFONT struct.
	{
		CHARFORMAT chfmt;
		chfmt.dwEffects = 0;
		chfmt.dwMask = 0;

		if ((m_CF.Flags & CF_NOSTYLESEL) == 0)
		{
			chfmt.dwMask    |= CFM_BOLD | CFM_ITALIC;
			chfmt.dwEffects |= (IsBold()) ? CFE_BOLD : 0;
			chfmt.dwEffects |= (IsItalic()) ? CFE_ITALIC : 0;
		}

		if ((m_CF.Flags & CF_NOSIZESEL) == 0)
		{
			chfmt.dwMask |= CFM_SIZE;
			  // GetSize() returns 1/10th points, convert to  twips
			chfmt.yHeight = GetSize() * 2;
		}

		if ((m_CF.Flags & CF_NOFACESEL) == 0)
		{
			chfmt.dwMask |= CFM_FACE;
			chfmt.bPitchAndFamily = m_CF.lpLogFont->lfPitchAndFamily;
			lstrcpy(chfmt.szFaceName, GetFaceName().c_str());
		}

		if (m_CF.Flags & CF_EFFECTS)
		{
			chfmt.dwMask |= CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
			chfmt.dwEffects |= (IsUnderline()) ? CFE_UNDERLINE : 0;
			chfmt.dwEffects |= (IsStrikeOut()) ? CFE_STRIKEOUT : 0;
			chfmt.crTextColor = GetColor();
		}

		if ((m_CF.Flags & CF_NOSCRIPTSEL) == 0)
		{
			chfmt.bCharSet = m_CF.lpLogFont->lfCharSet;
			chfmt.dwMask |= CFM_CHARSET;
		}

		chfmt.yOffset = 0;
		return chfmt;
	}

	//============================================================================
	inline int CFontDialog::GetSize() const
	//	Return the current font size, in 1/10th points (1 pt = 1/72 inch).
	{
		HDC hdc = ::GetDC(NULL); // the device context for the entire screen

		// number of pixels per inch along the screen height.
		int pxpi = GetDeviceCaps(hdc, LOGPIXELSY);

		// point size is (pixel height) * 72 / pxpi, so in 1/10ths size is
		int charsize = -MulDiv(m_LogFont.lfHeight, 720, pxpi);
		::ReleaseDC(NULL, hdc);
		return charsize;
	}

	//============================================================================
	inline void CFontDialog::OnOK()
	//	Called when the OK button is pressed on the Font Dialog.
	{
	}

	//============================================================================
	inline DWORD CFontDialog::FillInLogFont(const CHARFORMAT& cf)
	//	Translate the font character format cf properties of a CRichEdit control
	//	into elements of the m_LogFont member and settings of the option flags.
	{
		DWORD dwFlags = 0;
		if (cf.dwMask & CFM_SIZE)
		{
			CDC dc;
			dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
			LONG yPerInch = dc.GetDeviceCaps(LOGPIXELSY);
			m_LogFont.lfHeight = -(int) ((cf.yHeight * yPerInch) / 1440);
		}
		else
			m_LogFont.lfHeight = 0;

		m_LogFont.lfWidth = 0;
		m_LogFont.lfEscapement = 0;
		m_LogFont.lfOrientation = 0;

		if ((cf.dwMask & (CFM_ITALIC|CFM_BOLD)) == (CFM_ITALIC|CFM_BOLD))
		{
			m_LogFont.lfWeight = (cf.dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
			m_LogFont.lfItalic = (cf.dwEffects & CFE_ITALIC) ? TRUE : FALSE;
		}
		else
		{
			dwFlags |= CF_NOSTYLESEL;
			m_LogFont.lfWeight = FW_DONTCARE;
			m_LogFont.lfItalic = FALSE;
		}

		if ((cf.dwMask & (CFM_UNDERLINE|CFM_STRIKEOUT|CFM_COLOR)) ==
			(CFM_UNDERLINE|CFM_STRIKEOUT|CFM_COLOR))
		{
			dwFlags |= CF_EFFECTS;
			m_LogFont.lfUnderline = (cf.dwEffects & CFE_UNDERLINE) ? TRUE : FALSE;
			m_LogFont.lfStrikeOut = (cf.dwEffects & CFE_STRIKEOUT) ? TRUE : FALSE;
		}
		else
		{
			m_LogFont.lfUnderline = (BYTE)FALSE;
			m_LogFont.lfStrikeOut = (BYTE)FALSE;
		}

		if (cf.dwMask & CFM_CHARSET)
			m_LogFont.lfCharSet = cf.bCharSet;
		else
			dwFlags |= CF_NOSCRIPTSEL;

		m_LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		m_LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		m_LogFont.lfQuality = DEFAULT_QUALITY;

		if (cf.dwMask & CFM_FACE)
		{
			m_LogFont.lfPitchAndFamily = cf.bPitchAndFamily;
			lstrcpyn(m_LogFont.lfFaceName, cf.szFaceName, LF_FACESIZE);
		}
		else
		{
			m_LogFont.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
			m_LogFont.lfFaceName[0] = (TCHAR)0;
		}

		return dwFlags;
	}

	//============================================================================
	inline void CFontDialog::SetParameters(CHOOSEFONT cf)
	// Sets the various parameters of the CHOOSEFONT struct.
	// The parameters are set to safe values.
	{
		if (cf.lpLogFont)
			m_LogFont = *cf.lpLogFont;
		else
			ZeroMemory(&m_LogFont, sizeof(LOGFONT));

		if (cf.lpszStyle)
			m_strStyleName = cf.lpszStyle;
		else
			m_strStyleName.Empty();


		m_CF.lStructSize	= sizeof(CHOOSEFONT);
		m_CF.hwndOwner		= 0;		// Set this in DoModal
		m_CF.hDC			= cf.hDC;
		m_CF.lpLogFont		= &m_LogFont;
		m_CF.iPointSize		= cf.iPointSize;
		m_CF.Flags			= cf.Flags;
		m_CF.rgbColors		= cf.rgbColors;
		m_CF.lCustData		= cf.lCustData;
		m_CF.lpfnHook		= reinterpret_cast<LPCCHOOKPROC>(CDHookProc);
		m_CF.lpTemplateName	= cf.lpTemplateName;
		m_CF.hInstance		= GetApp().GetInstanceHandle();
		m_CF.lpszStyle		= const_cast<LPTSTR>(m_strStyleName.c_str());
		m_CF.nFontType		= cf.nFontType;
		m_CF.nSizeMin		= cf.nSizeMin;
		m_CF.nSizeMax		= cf.nSizeMax;
	}

}



#endif // _WIN32XX_COMMONDLG_H_

