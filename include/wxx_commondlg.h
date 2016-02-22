// Win32++   Version 8.2
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2016  David Nash
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


namespace Win32xx	
{

	//============================================================================
	class CCommonDialog : public CDialog
	{
	public:
		CCommonDialog(UINT nID = 0) : CDialog((UINT)nID) {}
		virtual ~CCommonDialog(){}

	protected:
		virtual void 	OnCancel()	{}	// required to override CDialog::OnCancel default behaviour
		virtual	void 	OnHelpButton();
		virtual void 	OnOK()	{}		// required to override CDialog::OnOK default behaviour
	
		// static callback
		static INT_PTR CALLBACK CDHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};


	//============================================================================
	class CColorDialog : public CCommonDialog 
	//	The color choice common dialog box class.
	{
	public:
		CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0, HWND hParentWnd = 0);
		virtual ~CColorDialog(){}

		virtual INT_PTR	DoModal(HWND hWndOwner = 0);
		const CHOOSECOLOR& GetParameters() const { return m_CC; }
		COLORREF  GetColor() const				{ return m_CC.rgbResult;}
		COLORREF* GetCustomColors()				{ return m_rgbCustomColors;}
		void 	SetColor(COLORREF clr)			{ m_CC.rgbResult = clr;}
		void    SetCustomColors(const COLORREF*	rgbCstmColors = NULL);
		void	SetParameters(CHOOSECOLOR cc);	

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);

		// You won't need to override this function
		virtual INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		// private data
		CHOOSECOLOR 	m_CC;					// ChooseColor parameters
		COLORREF  		m_rgbCustomColors[16];	// Custom colors array
	};


	//******************************************************************************
	class CFileDialog : public CCommonDialog
	//	The open/save-as file choice common dialog box class.
	{
	public:
	
		// Constructor/destructor
		CFileDialog(BOOL bOpenFileDialog = TRUE, 
				LPCTSTR pszDefExt = NULL,
				LPCTSTR pszFileName = NULL, 
				DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				LPCTSTR pszFilter   = NULL, 
				HWND  hParentWnd = 0);
		
		virtual ~CFileDialog()	{}
		
		// Operations
		virtual INT_PTR	DoModal(HWND hWndOwner = 0);
		
		// methods valid after successful DoModal()
		const OPENFILENAME& GetParameters() const { return m_OFN; }
		CString GetFileName() const; 
		CString GetFileExt() const;  
		CString GetFileTitle() const;
		CString GetFolderPath() const; 
		CString GetPathName() const;
		
		// methods for setting parameters before DoModal()
		BOOL	IsOpenFileDialog()					{ return m_bOpenFileDialog; }
		void    SetDefExt(LPCTSTR szExt);
		void    SetFileName(LPCTSTR szFileName);
		void	SetFilter(LPCTSTR pszFilter);
		void    SetMode(BOOL open)					{ m_bOpenFileDialog = open; }
		void	SetParameters(OPENFILENAME ofn);
		void    SetTitle(LPCTSTR szTitle)			{ m_OFN.lpstrTitle = szTitle; }

		// Enumerating multiple file selections
		CString	GetNextPathName(int& pos) const;

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
		virtual void 	OnFileNameChange();
		virtual BOOL 	OnFileNameOK();
		virtual void 	OnFolderChange();
		virtual void 	OnInitDone();
		virtual void 	OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);
		virtual LRESULT	OnNotify(WPARAM, LPARAM);
		virtual BOOL 	OnShareViolation(LPCTSTR lpszPathName);
		virtual void 	OnTypeChange();

		// You won't need to override this function
		virtual INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		BOOL 			m_bOpenFileDialog;  // TRUE = open, FALSE = save
		CString   		m_sFilter;          // file filter string
		CString			m_sFileName;		// file name string
		OPENFILENAME	m_OFN;				// OpenFileName parameters
	};

	//******************************************************************************
	class CFindReplaceDialog : public CCommonDialog
	//	Find/FindReplace modeless dialogs class
	{
	public:
		// constructor and destructor
		CFindReplaceDialog();
		virtual ~CFindReplaceDialog(){}

		virtual BOOL Create(BOOL bFindDialogOnly, 
						LPCTSTR sFindWhat, 
						LPCTSTR sReplaceWith = NULL,
						DWORD dwFlags = FR_DOWN, 
						HWND hParentWnd = 0);

		virtual BOOL IsModal() const					{ return FALSE; }

		// Operations:
		// Helpers for parsing information after successful return
		BOOL 	FindNext() const;           // TRUE = find next
		const	FINDREPLACE& GetParameters() const		{ return m_FR; }
		CString GetFindString() const;      // get find string
		CString	GetReplaceString() const;   // get replacement string
		BOOL	IsFindDialogOnly() const				{ return m_bFindDialogOnly; }
		BOOL 	IsTerminating();      	    // TRUE = terminate dialog
		BOOL 	MatchCase() const;          // TRUE = matching case
		BOOL 	MatchWholeWord() const;     // TRUE = whole words only
		BOOL 	ReplaceAll() const;         // TRUE = all occurrences
		BOOL 	ReplaceCurrent() const;     // TRUE = current string
		BOOL 	SearchDown() const;         // TRUE = down, FALSE = up
		void	SetParameters(FINDREPLACE fr);
		
		// static public methods
		static CFindReplaceDialog* GetNotifier(LPARAM lParam);

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);

		// You won't need to override this function
		virtual INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		FINDREPLACE 	m_FR;				// FindReplace paramaters
		BOOL			m_bFindDialogOnly;	// TRUE for a find only dialog
		CString 		m_strFindWhat;
		CString 		m_strReplaceWith;	
	};


	//*****************************************************************************
	class CFontDialog : public CCommonDialog
	//	The font choice box common dialog class.
	{
	public:
		CFontDialog(LPLOGFONT lplfInitial = NULL, 
				DWORD dwFlags = 0,
				HDC hdcPrinter = 0, 
				HWND hParentWnd = 0);
		CFontDialog(const CHARFORMAT& charformat, 
				DWORD dwFlags = 0,
				HDC hdcPrinter = 0, 
				HWND hParentWnd = 0);
	
		virtual ~CFontDialog(void)	{}

		virtual INT_PTR	DoModal(HWND hWndOwner = 0);
		DWORD FillInLogFont(const CHARFORMAT& cf);
		CHARFORMAT	GetCharFormat(void) const;
		COLORREF	GetColor(void) const 		{ return m_FR.rgbColors;}
		CString GetFaceName(void) const			{ return m_LogFont.lfFaceName;}
		LOGFONT	GetLogFont(void) const			{ return m_LogFont;}
		const CHOOSEFONT& GetParameters() const { return m_FR; }
		int 	GetSize(void) const;   
		CString GetStyleName(void) const		{ return m_strStyleName;}
		long 	GetWeight(void) const  			{ return m_LogFont.lfWeight;}
		BOOL 	IsBold(void) const				{ return (m_LogFont.lfWeight >= FW_SEMIBOLD);}
		BOOL 	IsItalic(void) const			{ return m_LogFont.lfItalic;}
		BOOL 	IsStrikeOut(void) const			{ return m_LogFont.lfStrikeOut;}
		BOOL 	IsUnderline(void) const 		{ return m_LogFont.lfUnderline;}		
		void    SetColor(const COLORREF rgb)	{ m_FR.rgbColors = rgb;}
		void	SetParameters(CHOOSEFONT cf);
		void    SetStyleName(LPCTSTR pszStyle);

	protected:
		virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM);
		virtual void 	OnOK();

		// You won't need to override this function
		virtual INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		// private data
		LOGFONT 	m_LogFont;			// font charactreristics
		CHOOSEFONT	m_FR;				// ChooseFont parameters
		CString		m_strStyleName;		// style name on the dialog
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{


	//============================================================================
	inline	void CCommonDialog::OnHelpButton()
	//	Override this function to respond to the help button. The commondialog 
	//  requires the appropriate flag to be specified before a help button
	//  is displayed. 
	{
	}

	//============================================================================
	inline INT_PTR CALLBACK CCommonDialog::CDHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//	The callback function for the common dialog's hook procedure. Messages 
	//  intercepted by the hook procedure are processed here, and forwarded
	//  on to the virtual DialogProc function.
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



	//============================================================================
	inline CColorDialog::CColorDialog(COLORREF clrInit /* = 0 */, DWORD dwFlags /* = 0 */,
		HWND hParentWnd /* = 0 */)
	//	Construct a CColorDialog object and initialize the CHOOSECOLOR struct
	//	with the current color, the option dwFlags, and the owner window.
	//  Refer to the description of the CHOOSECOLOR struct in the MSDN documentation.
	{
		// set the parameters in the CHOOSECOLOR struct
		ZeroMemory(&m_CC,  sizeof(CHOOSECOLOR));
		m_CC.hwndOwner = hParentWnd;
		m_CC.rgbResult = clrInit;
		m_CC.Flags = dwFlags;

		// Set all custom colors to white
		for (int i = 0; i <= 15; ++i)
			m_rgbCustomColors[i] = RGB(255,255,255);

		// Set the CHOOSECOLOR struct parameters to safe values
		SetParameters(m_CC);
	}

	//============================================================================
	inline INT_PTR CColorDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	//	The Default message handling for CColorDialog. Don't override this function, override
	//  DialogProc instead.
	//	Note: OnCancel and OnOK are not called by DialogProc. They are called by DoModal
	//  in response to value returned by called by ChooseColor.
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
	//	Show the ChooseColor common dialog box and select the current color.
	//	If hWndOwner is non-NULL, override the ownership set by the constructor.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;

		if (hWndOwner != 0)
			m_CC.hwndOwner = hWndOwner;
		
		// invoke the control and save the result on success
		BOOL ok = ::ChooseColor(&m_CC);
		
		m_hWnd = 0;

		if (!ok)
		{
			OnCancel();
			return IDCANCEL;
		}
		
		OnOK();
		return IDOK;		
	}

	//============================================================================
	inline void CColorDialog::SetCustomColors(const COLORREF* rgbCustomColors /* = NULL */)
	//	The rgbCustomColors parameter is a pointer to an array of 16 COLORREF.
	//  If the rgbCustomColors is NULL, all custom colors are set to white, 
	//  otherwise they are set to the colors specified in the rgbCustomColors array.
	{
		for (UINT i = 0; i < 16; i++)
		{
			COLORREF clr = rgbCustomColors? rgbCustomColors[i] : RGB(255,255,255);
			m_rgbCustomColors[i] = clr;
		}
	}

	//============================================================================
	inline void	CColorDialog::SetParameters(CHOOSECOLOR cc)
	//	Sets the various parameters of the CHOOSECOLOR struct.
	//  The parameters are set to safe values.
	{ 
		m_CC.lStructSize	= sizeof(m_CC);
		m_CC.hwndOwner		= cc.hwndOwner;
		m_CC.hInstance		= cc.hInstance;
		m_CC.rgbResult		= cc.rgbResult;		
		m_CC.lpCustColors	= m_rgbCustomColors;
		m_CC.Flags			= cc.Flags | CC_ANYCOLOR | CC_RGBINIT | CC_ENABLEHOOK;
		m_CC.lCustData		= cc.lCustData;
		m_CC.lpfnHook		= (LPCCHOOKPROC)CDHookProc;
		m_CC.lpTemplateName = cc.lpTemplateName;
	}	




	//============================================================================
	inline CFileDialog::CFileDialog(BOOL bOpenFileDialog  /* = TRUE */, 
		LPCTSTR pszDefExt /* = NULL */,
		LPCTSTR pszFileName /* = NULL */,
		DWORD dwFlags /* = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT */,
		LPCTSTR pszFilter /* = NULL */,
		HWND hParentWnd /* = 0 */)
	//	Construct a standard Windows File-Open or File-Save-As dialog box,
	//	depending on the value of bOpenFileDialog, TRUE for a File-Open dialog
	//	or FALSE to for a File-Save-As dialog.  The default file name extension
	//	is sDefExt, which is automatically appended to the file name if the
	//	user does not include an extension in the Filename edit box.  If
	//	sDefExt is empty, no file extension is appended.  The initial file
	//	name appearing in the File Name edit box is sFileName; if empty, no
	//	file name initially appears.  The dwFlags specifies a combination of one
	//	or more options that customize the dialog box.  The sFilter defines a
	//	series of string pairs that specify filters that appear in the dialog
	//	box. If not empty, only filtered files appear in the Files list box. The
	//	filter string elements may be separated by either '\0' or '|' chars.
	//	The hParentWnd is the handle of the file dialog's parent or owner window
	//	if nonzero; if zero, the top-level window is presumed.
	//
	//	Consult MSDN documentation of this constructor for details on flags and
	//	filters. The values entered upon construction may be altered via access
	//	to the public m_OFN OPENFILENAME attribute.
	{
		// set open/saveas toggle
		SetMode(bOpenFileDialog);
		 
		// clear out the OPENFILENAME structure
		ZeroMemory(&m_OFN, sizeof(m_OFN));
		  
		// fill in the OPENFILENAME struct
		m_OFN.lpstrFile		= (LPTSTR)pszFileName;
		m_OFN.hwndOwner		= hParentWnd;
		m_OFN.lpstrFilter	= pszFilter;
		m_OFN.lpstrDefExt	= pszDefExt;
		m_OFN.Flags			= dwFlags;

		// Safely set the remaining OPENFILENAME values
		SetParameters(m_OFN);
	}

	//============================================================================
	inline INT_PTR CFileDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	//	This is the file open/saveas dialog message loop. It handles those
	//	messages specific to this derived class.  Here it completes the actions
	//	of the OFNHookProc() procedure and returns the results of this
	//	processing back to the hook procedure for completing the default message
	//	handling. Specifically, however, the IDOK and IDCANCEL command messages
	//	are NOT processed here, so as to allow the natural termination mode of
	//	the open and saveas boxs to take place. See DoModal() for handling of OK
	//	and canceled responses.	Responding to WM_COMMAND, WM_NOTIFY, and other
	//	messages provides opportunities to engage context help and perform file
	//	related functions. Here, responses are also made to the HELPMSGSTRING,
	//	FILEOKSTRING, LBSELCHSTRING, AND SHAREVISTRING registered messages, as
	//	described in MSDN documentation.
	{
		switch (message)
		{
			case WM_INITDIALOG:
			{     // handle the initialization message
 			return OnInitDialog();
			}

			case WM_COMMAND:
			{     // handle the HELP button for old-style file dialogs:
	 		if (LOWORD(wParam) == pshHelp)
				OnHelpButton();

			return 0;
			}

			case WM_NOTIFY:
			{     // handle messages for Explorer-style hook procedures:
			if (m_OFN.Flags & OFN_EXPLORER)
				return OnNotify(wParam, lParam);

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
			OnLBSelChangedNotify(wParam, LOWORD(lParam),
				HIWORD(lParam));
			return 0;
		}
		if (message == UWM_SHAREVISTRING)
		{   // handle a sharing violation for the selected file that
			// occurred when the user clicked the OK button.
			return OnShareViolation((LPCTSTR)lParam);
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
	//	Display the common file dialog box for browsing files and directories
	//	and selection or entry of a file name for Open or SaveAs purposes,
	//	depending on the mode selected in the constructor. The file dialog box
	//	options in the m_OFN structure must have been set prior to invocation.
	//	Selecting the OK, Cancel, or the close option [X} terminates the modal
	//	action. If the OK button was the means of terminatin, engages OnOK() and
	//	returns IDOK. On any other termination the OnCancel() procecure is
	//	called and IDCANCEL is returned. When IDOK is returned, the m_OFN
	//	structure contains the choices made; if IDCANCEL is returned, the
	//	::CommDlgExtendedError() function returns the condition that occurred.
	//	If hWndOwner is non-NULL, override the ownership set by the constructor.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;

		if (hWndOwner)
			m_OFN.hwndOwner = hWndOwner;

		m_OFN.lpstrFile = (LPTSTR)m_sFileName.GetBuffer(m_OFN.nMaxFile);
		int ok = (m_bOpenFileDialog ? ::GetOpenFileName(&m_OFN) : ::GetSaveFileName(&m_OFN));
		m_sFileName.ReleaseBuffer();
		m_hWnd = 0;

		// the result of the file choice box is processed here: 
		if (!ok)
		{
			OnCancel();
			return  IDCANCEL;
		}

		OnOK();
		return IDOK;
	}

	//============================================================================
	inline CString CFileDialog::GetFileName() const
	//	Return the name of the file that was entered in the DoModal() operation.
	//	This name consists of only the file title and extension, i.e., the full
	//	path with the directory name removed. In cases where m_OFN.Flags has
	//	the OFN_ALLOWMULTISELECT option set, only the file name on the first
	//	file path selected will be returned.  In such cases, in order to obtain
	//	any of the remaining file names of the multiple names selected, the
	//	GetNextPathName() method is required to retrieve each file path, from
	//	which the file name itself may be extracted.
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
	//	Return only the extension of the file name entered during the DoModal()
	//	operation. The extension consists of the substring of the file
	//	path name after the final period ('.'), if any. If no period appears,
	//	this method returns the empty string.  In cases where m_OFN.Flags has
	//	the OFN_ALLOWMULTISELECT option set, only the extension on the first 
	//	file path selected will be returned.  In such cases, in order to obtain
	//	any of the remaining file extensions of the multiple names selected, the
	//	GetNextPathName() method is required to retrieve each file path, from
	//	which the extension may be extracted.
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
	//	removed. In cases where m_OFN.Flags has the OFN_ALLOWMULTISELECT option
	//	set, only the title on the first file path selected will be returned.
	//	In such cases, in order to obtain any of the remaining file titles of
	//	the multiple names selected, the GetNextPathName() method is required
	//	to retrieve each file path, from which the title may be extracted.
	{
		CString strResult = GetFileName();
		int pos = strResult.ReverseFind(_T("."));
		if (pos >= 0)
			return strResult.Left(pos);

		return strResult;
	}

	//============================================================================
	inline CString CFileDialog::GetNextPathName(int& pos) const
	//	Return the next file path name from the selection made in the DoModal()
	//	operation. If the m_OFN.Flags member includes the OFN_ALLOWMULTISELECT
	//	option, more than one file name may appear in the selection. Set pos = 0
	//	prior to entry to establish the initial operation, and then use the
	//	returned pos in subsequent calls as a forward iteration to retrieve
	//	the remaining path names in succession. The pos value is set to a
	//	negative number when no more path names are present. Do not use this
	//	method with a negative pos.
	{
		assert(pos >= 0);
		
		BOOL bExplorer = m_OFN.Flags & OFN_EXPLORER;
		TCHAR chDelimiter = (bExplorer ? _T('\0') : _T(' '));

		// let lpsz be a pointer to the position in the returned file name
		// string to start the search
		LPTSTR lpsz = m_OFN.lpstrFile + pos;
		if (pos == 0) // first time
		{
			if ((m_OFN.Flags & OFN_ALLOWMULTISELECT) == 0)
			{         // no multi-selection, lpstrFile is the path name
				pos = -1;
				return m_OFN.lpstrFile;
			}

			// multi-selection case: find the pos after first chDelimiter
			while(*lpsz != chDelimiter && *lpsz != _T('\0'))
				lpsz = _tcsinc(lpsz);
			lpsz = _tcsinc(lpsz);

			// if we've reached the end of the input already, only one file
			// file name was selected in multi-selection mode
			if (*lpsz == _T('\0'))
			{
				pos = -1;
				return m_OFN.lpstrFile;
			}
		}
		
		// This is not the first time, or it is the first time but the input
		// scan did not terminate; in either case, no file name has yet been
		// returned.
		CString strPath = m_OFN.lpstrFile; // convert to string
		if (!bExplorer)
		{
			LPTSTR lpszPath = m_OFN.lpstrFile;
			while(*lpszPath != chDelimiter)
				lpszPath = _tcsinc(lpszPath);
			strPath = strPath.Left(int(lpszPath - m_OFN.lpstrFile));
		}

		LPTSTR lpszFileName = lpsz;
		CString strFileName = lpsz;

		// find char pos at the next chDelimiter
		while(*lpsz != chDelimiter && *lpsz != _T('\0'))
			lpsz = _tcsinc(lpsz);

		if (!bExplorer && *lpsz == _T('\0'))
			pos = -1;
		else
		{
			if (!bExplorer)
				strFileName = strFileName.Left(int(lpsz - lpszFileName));

			lpsz = _tcsinc(lpsz);
			if (*lpsz == _T('\0')) // if double terminated then done
				pos = -1;
			else
				pos = lpsz - m_OFN.lpstrFile;
		}

		// only add _T("\\") if it is needed
		if (!strPath.IsEmpty())
		{
			// check for last back-slash or forward slash (handles DBCS)
			LPCTSTR lpsz = _tcsrchr(strPath, _T('\\'));
			if (lpsz == NULL)
				lpsz = _tcsrchr(strPath, _T('/'));
			// if it is at the last character, then an extra one is unneeded
			if (lpsz != NULL &&
				(lpsz - (LPCTSTR)strPath) == strPath.GetLength() - 1)
			{
				assert(*lpsz == _T('\\') || *lpsz == _T('/'));
				return strPath + strFileName;
			}
		}
		return strPath + _T("\\") + strFileName;
	}

	//============================================================================
	inline CString CFileDialog::GetFolderPath() const
	//	Return the path name of the folder or directory of files displayed in
	//	an Open or SaveAs file choice dialog box during the previous DoModal()
	//	operation. The final character of the name includes the directory
	//	separation character.
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
	//	Returns the full path name of the file that was entered during a
	//	DoModal() operation. If m_OFN.Flags has the OFN_ALLOWMULTISELECT option
	//	set, this method returns only the first among the group of files that
	//	was selected. Use the GetNextPathName() member function  to retrieve
	//	the complete list of file names in the list.
	{
		if ((m_OFN.Flags & OFN_ALLOWMULTISELECT) == 0)
		{	  // just retrieve the path from the OPENFILENAME structure
			return m_OFN.lpstrFile;
		}
		else
		{
			int pos = 0;
			return GetNextPathName(pos);
		}
	}

	//============================================================================
	inline BOOL CFileDialog::OnFileNameOK()
	//	Normally, overriding this function is not warranted because the
	//	framework provides default validation of file names and displays a
	//	message box if an invalid file name is entered; this action cannot
	//	be altered here. Override this function only if custom validation of
	//	syntactically correct file names that are entered into the file dialog
	//	box is required, such as to reject a name for any application-specific
	//	reason. If this method returns FALSE, the dialog box accepts the
	//	specified file name and closes. Returning a TRUE value causes the
	//	dialog to reject the specified file name and force the dialog box to
	//	remain open. In this case, this method should call the SetWindowLongPtr
	//	function to set a nonzero DWL_MSGRESULT value.
	{
		return FALSE;
	}

	//============================================================================
	inline void CFileDialog::OnInitDone()
	//	Override this method to handle the WM_NOTIFY CDN_INITDONE message.
	//	The notification message is sent when the system has finished arranging
	//	controls in the Open or SaveAs dialog to make room for the controls of
	//	the child dialog box, but before the dialog is visible. Notification is
	//	sent only if the dialog box was created with the OFN_EXPLORER style. For
	//	more information about the notification, or about the OFN_EXPLORER
	//	style, see MSDN documentation.
	{
		// no default processing needed
	}

	//============================================================================
	inline void CFileDialog::OnFileNameChange()
	//	Override this method to handle the WM_NOTIFY CDN_SELCHANGE message.
	//	The notification message is sent when the user selects a new file or
	//	folder in the file list of the Open or SaveAs dialog box. Notification
	//	is sent only if the dialog box was created with the OFN_EXPLORER style.
	//	For more information about the notification, or about the OFN_EXPLORER
	//	style, consult the MSDN documentation on this method.
	{
		// no default processing needed
	}

	//============================================================================
	inline void CFileDialog::OnFolderChange()
	//	Override this method to handle the WM_NOTIFY CDN_SELCHANGE message.
	//	The notification message is sent when the user selects a new file or
	//	folder in the file list of the Open or SaveAs dialog box. Notification
	//	is sent only if the dialog box was created with the OFN_EXPLORER style.
	//	For more information about the notification, or about the OFN_EXPLORER
	//	style, consult the MSDN documentation on this method.
	{
		// no default processing needed
	}

	//============================================================================
	inline void CFileDialog::OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode)
	//	This method is called whenever the current selection in a list box is
	//	about to change. Override this method to provide custom handling of
	//	selection changes in the list box. The ID of the list or combo box in
	//	which the selection occurred is nIDBox. The index of the current
	//	selection is iCurSel. The control notification code is nCode. See the
	//	MSDN documentation for the values of this parameter.
	{
		UNREFERENCED_PARAMETER(nIDBox);
		UNREFERENCED_PARAMETER(iCurSel);
		UNREFERENCED_PARAMETER(nCode);

		// no default processing needed
	}

	//============================================================================
	inline LRESULT CFileDialog::OnNotify(WPARAM wParam, LPARAM lParam) 
	//	This method handles the WM_NOTIFY message loop functions of the hook
	//	procedure.
	{
		UNREFERENCED_PARAMETER(wParam);

		OFNOTIFY* pNotify = (OFNOTIFY*)lParam;
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

		return FALSE;   // not handled
	}

	//============================================================================
	inline BOOL CFileDialog::OnShareViolation(LPCTSTR lpszPathName )
	//	Override this function to provide custom handling of share violations.
	//	Normally, this function is not needed because the framework provides
	//	default checking of share violations and displays a message box if a
	//	share violation occurs. The path of the file on which the share
	//	violation occurred is lpszPathName. To disable share violation checking,
	//	use the bitwise OR operator to combine the flag OFN_SHAREAWARE with
	//	m_OFN.Flags.
	//
	//	The return value indicates how the dialog box should handle the sharing
	//	violation. If the return is FALSE, the dialog box will display the
	//	standard warning message for a sharing violation. To prevent this
	//	display, call the SetWindowLongPtr function to set one of the following
	//	DWL_MSGRESULT values, and then return FALSE.
	//
	//	OFN_SHAREFALLTHROUGH: Causes the dialog box to return the file name
	//	without warning the user about the sharing violation.
	//	OFN_SHARENOWARN: Causes the dialog box to reject the file name
	//	without warning the user about the sharing violation.
	{
		UNREFERENCED_PARAMETER(lpszPathName);

		return TRUE; // default: no further action is requied
	}

	//============================================================================
	inline void CFileDialog::OnTypeChange()
	//	Override this method to handle the WM_NOTIFY CDN_TYPECHANGE message.
	//	The notification message is sent when the user selects a new file type
	//	from the list of file types in the Open or SaveAs dialog box.
	//	Notification is sent only if the dialog box was created with the
	//	OFN_EXPLORER style. For more information about the notification, see
	//	the MSDN documentation of CDN_TYPECHANGE. For information about the
	//	OFN_EXPLORER style, see the OPENFILENAME structure information.
	{
		// no default processing needed
	}

	//============================================================================
	inline void CFileDialog::SetDefExt(LPCTSTR pszExt)
	//	Set the default extension of the dialog box to sExt. This method must
	//	be applied to the CFileDialog object prior to invoking the DoModal()
	//	operation in order to have an effect. The sExt string must not contain
	//	a dot ('.'), and only the first three characters are sent to the dialog.
	{	
		m_OFN.lpstrDefExt = pszExt;
	}

	//============================================================================
	inline void CFileDialog::SetFileName(LPCTSTR pszFileName)
	//	Reset the initial file choice dialog file name to szFileName, for use
	//	before DoModal() operations.
	{
		  // setup initial file name
		if (pszFileName)
		{
			m_sFileName = pszFileName;
			m_OFN.lpstrFile = (LPTSTR)m_sFileName.c_str();
		}
		else
			m_OFN.lpstrFile = NULL;

	}

	inline void CFileDialog::SetFilter(LPCTSTR pszFilter)
	//	Reset the file choice dialog file name filter string to sFilter, for use
	//	before DoModal() operations.	
	{
		m_OFN.lpstrFilter = pszFilter;	// might contain embedded NULL characters
	
		// convert any '|' characters in pszFilter to NULL characters	
		if (pszFilter)
		{
			m_sFilter = pszFilter;
			if (m_sFilter.Find(_T('|')) >= 0)
			{
				m_sFilter.Replace(_T('|'), _T('\0'));
				m_OFN.lpstrFilter = m_sFilter.c_str();
			}
		}
	}

	//============================================================================
	inline void CFileDialog::SetParameters(OPENFILENAME ofn)
	//	Sets the various parameters of the OPENFILENAME struct.
	//  The parameters are set to safe values.	
	{
		// Set the correct struct size for all Windows versions and compilers
		DWORD StructSize = sizeof(m_OFN);

  #if defined OPENFILENAME_SIZE_VERSION_400
		if (GetWinVersion() < 2500)
			StructSize = OPENFILENAME_SIZE_VERSION_400;
		if (GetWinVersion() >= 2500)
			m_OFN.FlagsEx =		ofn.FlagsEx;
  #endif

		if (ofn.lpstrFile)
		{
			m_sFileName = ofn.lpstrFile;
			m_OFN.lpstrFile = (LPTSTR)m_sFileName.c_str();
		}
		else
			m_OFN.lpstrFile = NULL;

		SetFilter(ofn.lpstrFilter);

		m_OFN.lStructSize		= StructSize;
		m_OFN.hwndOwner			= ofn.hwndOwner;
		m_OFN.hInstance			= GetApp().GetInstanceHandle();
		m_OFN.lpstrCustomFilter	= ofn.lpstrCustomFilter;
		m_OFN.nMaxCustFilter	= _MAX_PATH;
		m_OFN.nFilterIndex		= ofn.nFilterIndex; 
		m_OFN.nMaxFile			= _MAX_PATH; 
		m_OFN.lpstrFileTitle	= ofn.lpstrFileTitle; 
		m_OFN.nMaxFileTitle		= _MAX_PATH; 
		m_OFN.lpstrInitialDir	= ofn.lpstrInitialDir; 
		m_OFN.lpstrTitle		= ofn.lpstrTitle; 
		m_OFN.Flags				= ofn.Flags | OFN_ENABLEHOOK; 
		m_OFN.nFileOffset		= ofn.nFileOffset; 
		m_OFN.nFileExtension	= ofn.nFileExtension; 
		m_OFN.lpstrDefExt		= ofn.lpstrDefExt; 
		m_OFN.lCustData			= ofn.lCustData; 
		m_OFN.lpfnHook			= (LPCCHOOKPROC)CDHookProc;
	}




	//============================================================================
	inline CFindReplaceDialog::CFindReplaceDialog()
	{
		ZeroMemory(&m_FR, sizeof(m_FR));
		m_bFindDialogOnly = TRUE;
		SetParameters(m_FR);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::Create(BOOL bFindDialogOnly, LPCTSTR pszFindWhat,
			LPCTSTR pszReplaceWith, DWORD dwFlags, HWND hParentWnd /* = 0*/)
	//	Create and display either a Find dialog box (bFindDialogOnly is TRUE) or
	//	a FindReplace dialog box (bFindDialogOnly is FALSE). Set lpszFindWhat to
	//	the string to search for; set lpszReplaceWith to the default string to
	//	replace found strings; set dwFlags to a combination of one or more flags
	//	that customize the settings of the dialog box using the | operator (the
	//	default value is FR_DOWN, which specifies that the search is to proceed
	//	in a downward direction--see the FINDREPLACE structure in the Win32 SDK
	//	documentation for more information on these flags); and set hParentWnd
	//	to the handle of the dialog box’s parent or owner window (this is the
	//	window that will receive the special message indicating that a
	//	find/replace action is requested; if NULL, the application’s main window
	//	is used). Return TRUE if the dialog box object was successfully created;
	//	otherwise FALSE.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		if (IsWindow())		// Only one window per CWnd instance allowed
		{
			LPCTSTR msg =  _T("Only one Find/Replace box at a time allowed.");
			::MessageBox(NULL, msg, _T("Error"), MB_OK | MB_ICONEXCLAMATION
				| MB_TASKMODAL);
			return FALSE;
		}

		m_bFindDialogOnly = bFindDialogOnly;
	
		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();
		pTLSData->pWnd = this;

		if (hParentWnd)
			m_FR.hwndOwner = hParentWnd;
			
		m_FR.Flags = dwFlags;
		SetParameters(m_FR);	

		m_FR.lpstrFindWhat = (LPTSTR)m_strFindWhat.GetBuffer(m_FR.wFindWhatLen);
		if (pszFindWhat)
			lstrcpyn(m_FR.lpstrFindWhat, pszFindWhat, m_FR.wFindWhatLen);

		m_FR.lpstrReplaceWith = (LPTSTR)m_strReplaceWith.GetBuffer(m_FR.wReplaceWithLen);
		if (pszReplaceWith)
			lstrcpyn(m_FR.lpstrReplaceWith, pszReplaceWith, m_FR.wReplaceWithLen);


		HWND hWnd;  // the returned find/replace dialog box handle
		if (bFindDialogOnly)
			hWnd = ::FindText(&m_FR);
		else
			hWnd = ::ReplaceText(&m_FR);

		m_strFindWhat.ReleaseBuffer();
		m_strReplaceWith.ReleaseBuffer();

		return hWnd != NULL;
	}

	inline INT_PTR CFindReplaceDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	//	Call this function from your callback function to determine whether the
	//	user wants to find the next occurrence of the search string. Returns
	//	TRUE if the user wants to find the next occurrence of the search string;
	//	otherwise FALSE.
	{
		return ((m_FR.Flags & FR_FINDNEXT )!= 0);
	}

	//============================================================================
	inline	CString CFindReplaceDialog::GetFindString() const
	//	Call this function from your callback function to return the default
	//	string to find.
	{
		return m_FR.lpstrFindWhat;
	}

	//============================================================================
	inline CFindReplaceDialog* CFindReplaceDialog::GetNotifier(LPARAM lParam)
	//	Return a pointer to the current Find/Replace dialog box. This may be
	//	used within the OnFindReplace() method to access the current dialog box,
	//	to call its member functions, and to access the m_FR structure.
	//	The lParam value is that passed to the frame window's OnFindReplace()
	//	function.
	{
		assert(lParam != 0);
		LPFINDREPLACE pFR = (LPFINDREPLACE)lParam;
		CFindReplaceDialog* pDlg = (CFindReplaceDialog*)pFR->lCustData;
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
	//	otherwise FALSE. Call this function within your callback function to
	//	determine whether the user has decided to terminate the dialog box. If
	//	this function returns TRUE, you should call the DestroyWindow member
	//	function of the current dialog box and set any dialog box pointer
	//	variable to NULL. Optionally, you can also store the find/replace text
	//	last entered and use it to initialize the next find/replace dialog box.
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
	//	search string; otherwise FALSE.
	{
		return ((m_FR.Flags & FR_WHOLEWORD) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::ReplaceAll() const
	//	Return TRUE if the user has requested that all strings matching the
	//	replace string be replaced; otherwise FALSE.
	{
		return ((m_FR.Flags & FR_REPLACEALL) != 0);
	}

	//============================================================================
	inline BOOL CFindReplaceDialog::ReplaceCurrent() const
	//	Return TRUE if the user has requested that the currently selected string
	//	be replaced with the replace string; otherwise FALSE.
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
	//  The parameters are set to safe values.		
	{
		const int MaxChars = 128;

		m_FR.lStructSize		= sizeof(FINDREPLACE);
		m_FR.hwndOwner			= fr.hwndOwner;
		m_FR.hInstance			= GetApp().GetInstanceHandle();
		m_FR.Flags				= fr.Flags | FR_ENABLEHOOK;
		m_FR.lpstrFindWhat		= NULL;		// is set within Create
		m_FR.lpstrReplaceWith	= NULL;		// is set within Create
		m_FR.wFindWhatLen		= MAX(fr.wFindWhatLen, MaxChars);
		m_FR.wReplaceWithLen	= MAX(fr.wReplaceWithLen, MaxChars);
		m_FR.lCustData			= (LPARAM)this;
		m_FR.lpfnHook			= (LPCCHOOKPROC)CDHookProc;
		m_FR.lpTemplateName		= fr.lpTemplateName;
	}


	//============================================================================
	inline CFontDialog::CFontDialog(LPLOGFONT lplfInitial /* = NULL */, DWORD dwFlags /* = 0 */,
		HDC hdcPrinter /* = 0 */, HWND hParentWnd /* = 0 */)
	//	Construct a CFontDialog object from values given. Note that these are
	//	stored into the members of the CHOOSEFONT structure. These may be
	//	changed later if the choose font dialog is desired to be differently
	//	initialized than by this default.  This is especially the case for the
	//	initial color presented to the dialog.  The pParentWnd is a pointer to
	//	a LOGFONT data structure that allows setting some of the font's
	//	characteristics. The dwFlags entry specifies one or more choose-font
	//	flags. One or more preset values can be combined using the bitwise OR
	//	operator. To modify the m_FR.Flags structure member, be sure to use a
	//	bitwise OR operator to retain the default behavior. For details on each
	//	of these flags, see the description of the CHOOSEFONT structure in the
	//	Windows SDK. The pdcPrinter parameter pointsto a printer-device context.
	//	If supplied, this parameter points to a printer-device context for the
	//	printer on which the fonts are to be selected. The pParentWnd paameter
	//	is a handle to the font dialog box's parent or owner window.
	{
		  // clear out logfont, style name, and choose font structure
		ZeroMemory(&m_LogFont, sizeof(m_LogFont));
		ZeroMemory(&m_FR, sizeof(m_FR));

		// set dialog parameters
		m_FR.rgbColors   = 0; // black
		m_FR.lStructSize = sizeof(m_FR);
		m_FR.hwndOwner   =  hParentWnd;
		m_FR.Flags  = dwFlags | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
		m_FR.lpszStyle = (LPTSTR)m_strStyleName.c_str();

		if (hdcPrinter)
		{
			m_FR.hDC = hdcPrinter;
			m_FR.Flags |= CF_PRINTERFONTS;
		}
		
		if (lplfInitial)
		{
			m_FR.lpLogFont = lplfInitial;
			m_FR.Flags |= CF_INITTOLOGFONTSTRUCT;
			memcpy(&m_LogFont, m_FR.lpLogFont, sizeof(m_LogFont));
		}
		else
		{
			m_FR.lpLogFont = &m_LogFont;
		}

		if (hdcPrinter)
		{
			m_FR.hDC = hdcPrinter;
			m_FR.Flags |= CF_PRINTERFONTS;
		}

		SetParameters(m_FR);
	}

	//============================================================================
	inline CFontDialog::CFontDialog(const CHARFORMAT& charformat, DWORD dwFlags /* = 0 */,
		HDC hdcPrinter /* =  0 */, HWND hParentWnd /* = 0*/)
	//	Construct a font choice dialog object that can be used to create a font
	//	for use in a rich edit control. The charformat points to a CHARFORMAT
	//	data structure that allows setting some of the font's characteristics
	//	for a rich edit control.  The dwFlags entry specifies one or more
	//	choose-font flags. One or more preset values can be combined using the
	//	bitwise OR operator. To modify the m_FR.Flags structure member, be sure
	//	to use a bitwise OR operator to retain the default behavior. For details
	//	on each of these flags, see the description of the CHOOSEFONT structure
	//	in the Windows SDK. The pdcPrinter parameter pointsto a printer-device
	//	context. If supplied, this parameter points to a printer-device context
	//	for the printer on which the fonts are to be selected. The pParentWnd
	//	paameter is a handle to the font dialog box's parent or owner window.
	{
		// clear out logfont, style name, and choose font structure
		ZeroMemory(&m_LogFont, sizeof(m_LogFont));
		ZeroMemory(&m_FR, sizeof(m_FR));

		// set dialog parameters
		m_FR.lStructSize = sizeof(m_FR);
		m_FR.hwndOwner   = hParentWnd;
		m_FR.lpszStyle   = (LPTSTR)m_strStyleName.c_str();
		m_FR.lpLogFont   = &m_LogFont;
		m_FR.Flags       = dwFlags | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_ENABLEHOOK;
		m_FR.Flags      |= FillInLogFont(charformat);
	
		if (charformat.dwMask & CFM_COLOR)
			m_FR.rgbColors = charformat.crTextColor;

		if (hdcPrinter)
		{
			m_FR.hDC = hdcPrinter;
			m_FR.Flags |= CF_PRINTERFONTS;
		}

		SetParameters(m_FR);
	}

	//============================================================================
	inline INT_PTR CFontDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	//	This is the font dialog message loop. It handles those messages
	//	specific to this derived class.  Here it completes the actions of the
	//	CFHookProc() procedure and returns the results of this processing
	//	back to the hook procedure for completing the default message handling.
	//	Specifically, however, the IDOK and IDCANCEL command messages are NOT
	//	processed here, so as to allow the natural termination mode of the
	//	ChooseFont() box to take place. See DoModal() for handling of OK and
	//	canceled responses.
	{
		UNREFERENCED_PARAMETER(lParam);
		if (message == WM_INITDIALOG)
		{
			OnInitDialog();
		}

		  // dispatch special commdlg messages
		if (message == WM_COMMAND && LOWORD(wParam) == pshHelp) 
			OnHelpButton();

		  // unhandled messages
		return 0;
	}

	//============================================================================
	inline INT_PTR CFontDialog::DoModal(HWND hWndOwner /* = 0 */)
	//	Bring up the ChooseFont common control to select font charcteristics.
	//	Return TRUE if a font was chosen or FALSE if one was not. Flags,
	//	initial font color, and parent window settings are presumed to have
	//	already been made. If hWndOwner is non-NULL, override the ownership set
	//	by the constructor.
	{
		assert( &GetApp() );	// Test if Win32++ has been started
		assert(!IsWindow());	// Only one window per CWnd instance allowed

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp().SetTlsData();

		// Create the modal dialog
		pTLSData->pWnd = this;		

		if (hWndOwner)
			m_FR.hwndOwner = hWndOwner;
		  
		// open the font choice dialog
		BOOL ok = ::ChooseFont(&m_FR);

		m_hWnd = 0;
		
		// process the result of the font choice box:
		if (!ok)
		{
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
		
		if ((m_FR.Flags & CF_NOSTYLESEL) == 0)
		{
			chfmt.dwMask    |= CFM_BOLD | CFM_ITALIC;
			chfmt.dwEffects |= (IsBold()) ? CFE_BOLD : 0;
			chfmt.dwEffects |= (IsItalic()) ? CFE_ITALIC : 0;
		}
		
		if ((m_FR.Flags & CF_NOSIZESEL) == 0)
		{
			chfmt.dwMask |= CFM_SIZE;
			  // GetSize() returns 1/10th points, convert to  twips
			chfmt.yHeight = GetSize() * 2;
		}

		if ((m_FR.Flags & CF_NOFACESEL) == 0)
		{
			chfmt.dwMask |= CFM_FACE;
			chfmt.bPitchAndFamily = m_FR.lpLogFont->lfPitchAndFamily;
			lstrcpy(chfmt.szFaceName, GetFaceName().c_str());
		}

		if (m_FR.Flags & CF_EFFECTS)
		{
			chfmt.dwMask |= CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
			chfmt.dwEffects |= (IsUnderline()) ? CFE_UNDERLINE : 0;
			chfmt.dwEffects |= (IsStrikeOut()) ? CFE_STRIKEOUT : 0;
			chfmt.crTextColor = GetColor();
		}
		
		if ((m_FR.Flags & CF_NOSCRIPTSEL) == 0)
		{
			chfmt.bCharSet = m_FR.lpLogFont->lfCharSet;
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
	//	This member is caused to execute after the DoModal() member has
	//	terminated with the OK button activated. 
	{
	}

	//============================================================================
	inline DWORD CFontDialog::FillInLogFont(const CHARFORMAT& cf)							
	//	Translate the font character format cf properties of a CRichEdit control
	//	into elements of the m_LogFont member and settings of the option flags,
	//	which will then be applied by the caller to specifiy the constructed
	//	font.
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
			m_LogFont.lfWeight = (cf.dwEffects & CFE_BOLD) ?
				FW_BOLD : FW_NORMAL;
			m_LogFont.lfItalic = (BYTE)((cf.dwEffects & CFE_ITALIC) ?
				TRUE : FALSE);
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
			m_LogFont.lfUnderline = (BYTE)((cf.dwEffects & CFE_UNDERLINE) ?
				TRUE : FALSE);
			m_LogFont.lfStrikeOut = (BYTE)((cf.dwEffects & CFE_STRIKEOUT) ?
				TRUE : FALSE);
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
			lstrcpy(m_LogFont.lfFaceName, (LPTSTR)cf.szFaceName);
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
	//	Sets the various parameters of the CHOOSEFONT struct.
	//  The parameters are set to safe values.	
	{
		if (cf.lpLogFont)
			m_LogFont = *cf.lpLogFont;
		else
			ZeroMemory(&m_LogFont, sizeof(LOGFONT));

		if (cf.lpszStyle)
		{
			m_strStyleName = cf.lpszStyle;
		}
		else
		{
			m_strStyleName.Empty();
		}

		m_FR.lStructSize	= sizeof(CHOOSEFONT);
		m_FR. hwndOwner		= cf.hwndOwner;
		m_FR.hDC			= cf.hDC;		
		m_FR.lpLogFont		= &m_LogFont;
		m_FR.iPointSize		= cf.iPointSize;
		m_FR.Flags			= cf.Flags | CF_ENABLEHOOK;
		m_FR.rgbColors		= cf.rgbColors;
		m_FR.lCustData		= cf.lCustData;
		m_FR.lpfnHook		= (LPCCHOOKPROC)CDHookProc;
		m_FR.lpTemplateName	= cf.lpTemplateName;
		m_FR.hInstance		= GetApp().GetInstanceHandle();
		m_FR.lpszStyle		= (LPTSTR)m_strStyleName.c_str();
		m_FR.nFontType		= cf.nFontType;
		m_FR.nSizeMin		= cf.nSizeMin;
		m_FR.nSizeMax		= cf.nSizeMax;
	}

	inline void CFontDialog::SetStyleName(LPCTSTR pszStyle)	
	{ 
		if (pszStyle)
			m_strStyleName = pszStyle;
		else
			m_strStyleName.Empty();
		
		m_FR.lpszStyle = (LPTSTR)m_strStyleName.c_str();
	}

}



#endif // _WIN32XX_COMMONDLG_H_
