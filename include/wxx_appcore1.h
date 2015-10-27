// Win32++   Version 8.1 beta
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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



#ifndef _WIN32XX_APPCORE1_H_
#define _WIN32XX_APPCORE1_H_


///////////////////////////////////////////////////////
// wxx_appcore1.h
// This file contains the declarations of the following set of classes.
//
// 1) CCriticalSection: This class is used internally to manage thread access
//            to shared resources. You can also use this class to lock and
//            release your own critical sections.
//
// 2) CWinException: This class is used internally by Win32++ to handle
//            exceptions. You can also use it to throw and catch exceptions.
//
// 3) CObject: A base class for CWnd and any other class that uses serialization.
//             It provides a virtual Serialize function for use by CArchive.
//
// 4) CWinThread: This class is the parent class for CWinApp. It is also the
//            class used to create additional GUI and worker threads.
//
// 5) CWinApp: This class is used start Win32++ and run the message loop. You
//            should inherit from this class to start Win32++ in your own
//            application.


namespace Win32xx
{
	// Registered messages defined by Win32++
	const UINT UWM_WINDOWCREATED = RegisterWindowMessage(_T("UWM_WINDOWCREATED"));	// Message - posted when a window is created or attached.

	////////////////////////////////////////////////
	// Forward declarations.
	//  These classes are defined later or elsewhere
	class CArchive;
	class CDC;
	class CGDIObject;
	class CMenu;
	class CMenuBar;
	class CWinApp;
	class CWnd;
	class CBitmap;
	class CBrush;
	class CFont;
	class CImageList;
	class CPalette;
	class CPen;
	class CRgn;
	class CDataExchange;

	// tString is a TCHAR std::string
	typedef std::basic_string<TCHAR> tString;
#ifndef _WIN32_WCE
	typedef std::basic_stringstream<TCHAR> tStringStream;
#endif

	// Some useful smart pointers
	typedef Shared_Ptr<CDC> DCPtr;
	typedef Shared_Ptr<CGDIObject> GDIPtr;
	typedef Shared_Ptr<CMenu> MenuPtr;
	typedef Shared_Ptr<CWnd> WndPtr;
	typedef Shared_Ptr<CBitmap> BitmapPtr;
	typedef Shared_Ptr<CBrush> BrushPtr;
	typedef Shared_Ptr<CFont> FontPtr;
	typedef Shared_Ptr<CImageList> ImageListPtr;
	typedef Shared_Ptr<CPalette> PalettePtr;
	typedef Shared_Ptr<CPen> PenPtr;
	typedef Shared_Ptr<CRgn> RgnPtr;

	enum Constants			// Defines the maximum size for TCHAR strings
	{
		MAX_MENU_STRING = 80,
		MAX_STRING_SIZE = 255
	};

	struct CDC_Data	// A structure that contains the data members for CDC
	{
		// Constructor
		CDC_Data() : hDC(0), Count(1L), IsManagedHDC(FALSE), hWnd(0), nSavedDCState(0) {}

		std::vector<GDIPtr> m_vGDIObjects;	// Smart pointers to internally created Bitmaps, Brushes, Fonts, Bitmaps and Regions
		HDC		hDC;			// The HDC belonging to this CDC
		long	Count;			// Reference count
		bool	IsManagedHDC;	// Delete/Release the HDC on destruction
		HWND	hWnd;			// The HWND of a Window or Client window DC
		int		nSavedDCState;	// The save state of the HDC.
	};

	struct CGDI_Data	// A structure that contains the data members for CGDIObject
	{
		// Constructor
		CGDI_Data() : hGDIObject(0), Count(1L), IsManagedObject(FALSE) {}

		HGDIOBJ hGDIObject;
		long	Count;
		bool	IsManagedObject;
	};

	struct CIml_Data	// A structure that contains the data members for CImageList
	{
		// Constructor
		CIml_Data() : hImageList(0), IsManagedHiml(FALSE), Count(1L) {}

		HIMAGELIST	hImageList;
		bool		IsManagedHiml;
		long		Count;
	};

#ifndef _WIN32_WCE
	struct CMenu_Data	// A structure that contains the data members for CMenu
	{
		// Constructor
		CMenu_Data() : hMenu(0), IsManagedMenu(FALSE), Count(1L) {}

		std::vector<MenuPtr> vSubMenus;	// A vector of smart pointers to CMenu
		HMENU hMenu;
		bool IsManagedMenu;
		long Count;
	};
#endif

	struct CompareHDC		// The comparison function object used by CWinApp::m_mapHDC
	{
		bool operator()(HDC const a, const HDC b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareGDI		// The comparison function object used by CWinApp::m_mapGDI
	{
		bool operator()(HGDIOBJ const a, const HGDIOBJ b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHIMAGELIST // The comparison function object used by CWinApp::m_mapHIMAGELIST
	{
		bool operator()(HIMAGELIST const a, const HIMAGELIST b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHMENU		// The comparison function object used by CWinApp::m_mapHMENU
	{
		bool operator()(HMENU const a, const HMENU b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct CompareHWND		// The comparison function object used by CWinApp::m_mapHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	struct TLSData			// Used for Thread Local Storage (TLS)
	{
		CWnd* pWnd;			// pointer to CWnd object for Window creation
		CWnd* pMainWnd;		// pointer to the main window for the thread (usually CFrame)
		CMenuBar* pMenuBar;	// pointer to CMenuBar object used for the WH_MSGFILTER hook
		HHOOK hMsgHook;		// WH_MSGFILTER hook for CMenuBar and Modal Dialogs
		long nDlgHooks;		// Number of Dialog MSG hooks

		TLSData() : pWnd(0), pMainWnd(0), pMenuBar(0), hMsgHook(0), nDlgHooks(0) {}	// Constructor
	};


	/////////////////////////////////////////
	// Declarations for the CCriticalSection class
	// This class is used for thread synchronisation
	class CCriticalSection
	{
	public:
		CCriticalSection()	{ ::InitializeCriticalSection(&m_cs); }
		~CCriticalSection()	{ ::DeleteCriticalSection(&m_cs); }

		void Lock() 	{ ::EnterCriticalSection(&m_cs); }
		void Release()	{ ::LeaveCriticalSection(&m_cs); }

	private:
		CCriticalSection ( const CCriticalSection& );
		CCriticalSection& operator = ( const CCriticalSection& );

		CRITICAL_SECTION m_cs;
	};


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	// Note: Each function guarantees not to throw an exception

	class CWinException : public std::exception
	{
	public:
		CWinException(LPCTSTR pszText) throw ();
		~CWinException() throw() {}
		DWORD GetError() const throw ();
		LPCTSTR GetErrorString() const throw ();
		LPCTSTR GetText() const throw ();
		const char * what () const throw ();

	private:
		DWORD  m_Error;
		LPCTSTR m_pszText;
		TCHAR m_szErrorString[MAX_STRING_SIZE];
	};

	// Typedef for _beginthreadex's callback function
	typedef UINT (WINAPI *PFNTHREADPROC)(LPVOID);


	///////////////////////////////////
	// Declaration of the CObject class
	//
	class CObject
	{
	public:
		CObject() {}
		virtual ~CObject() {}

		virtual void Serialize(CArchive& ar);

	private:
	};


	//////////////////////////////////////
	// Declaration of the CWinThread class
	//
	class CWinThread : public CObject
	{
	public:
		CWinThread();
		CWinThread(PFNTHREADPROC pfnThreadProc, LPVOID pParam);
		virtual ~CWinThread();

		// Overridables
		virtual BOOL InitInstance();
		virtual int MessageLoop();
		virtual BOOL OnIdle(LONG lCount);
		virtual BOOL PreTranslateMessage(MSG Msg);

		// Operations
		void	CreateThread(unsigned initflag = 0, unsigned stack_size = 0, LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL);
		HACCEL	GetAccelerators() const { return m_hAccel; }
		CWnd*	GetAcceleratorsWindow() const { return m_pWndAccel; }
		CWnd*	GetMainWnd() const;
		HANDLE	GetThread()	const;
		int		GetThreadID() const;
		int		GetThreadPriority() const;
		BOOL	PostThreadMessage(UINT message, WPARAM wParam, LPARAM lParam) const;
		DWORD	ResumeThread() const;
		void	SetAccelerators(HACCEL hAccel, CWnd* pWndAccel);
		void	SetMainWnd(CWnd* pWnd);
		BOOL	SetThreadPriority(int nPriority) const;
		DWORD	SuspendThread() const;
		operator HANDLE () const { return GetThread(); }

	private:
		CWinThread(const CWinThread&);				// Disable copy construction
		CWinThread& operator = (const CWinThread&);	// Disable assignment operator

		static	UINT WINAPI StaticThreadProc(LPVOID pCThread);

		PFNTHREADPROC m_pfnThreadProc;	// Callback function for worker threads
		LPVOID m_pThreadParams;			// Thread parameter for worker threads
		HANDLE m_hThread;				// Handle of this thread
		UINT m_nThreadID;				// ID of this thread
		DWORD m_dwThreadID;				// ID of this thread
		HACCEL m_hAccel;				// handle to the accelerator table
		CWnd* m_pWndAccel;				// pointer to the window for accelerator keys

	};

	///////////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp : public CWinThread
	{
		// Provide these access to CWinApp's private members:
		friend class CDC;
		friend class CDialog;
		friend class CFrame;
		friend class CGDIObject;
		friend class CImageList;
		friend class CMenu;
		friend class CMenuBar;
		friend class CPropertyPage;
		friend class CPropertySheet;
		friend class CTaskDialog;
		friend class CWinThread;
		friend class CWnd;
		friend CWinApp& GetApp();

		typedef Shared_Ptr<TLSData> TLSDataPtr;

	public:
		CWinApp();
		virtual ~CWinApp();

		// Overridables
		virtual BOOL InitInstance();
		virtual int Run();

		// Operations
		HINSTANCE GetInstanceHandle() const { return m_hInstance; }
		HINSTANCE GetResourceHandle() const { return (m_hResource ? m_hResource : m_hInstance); }
		HCURSOR LoadCursor(LPCTSTR lpszResourceName) const;
		HCURSOR LoadCursor(int nIDCursor) const;
		HCURSOR LoadStandardCursor(LPCTSTR lpszCursorName) const;
		HICON	LoadIcon(LPCTSTR lpszResourceName) const;
		HICON	LoadIcon(int nIDIcon) const;
		HICON   LoadStandardIcon(LPCTSTR lpszIconName) const;
		HANDLE  LoadImage(LPCTSTR lpszResourceName, UINT uType, int cx, int  cy, UINT fuLoad = LR_DEFAULTCOLOR) const;
		HANDLE  LoadImage(int nIDImage, UINT uType, int cx, int cy, UINT fuLoad = LR_DEFAULTCOLOR) const;
		HCURSOR SetCursor(HCURSOR hCursor) const;
		void	SetResourceHandle(HINSTANCE hResource);

	private:
		CWinApp(const CWinApp&);				// Disable copy construction
		CWinApp& operator = (const CWinApp&);	// Disable assignment operator

		CDC_Data* GetCDCDataFromMap(HDC hDC);
		CGDI_Data* GetCGDIDataFromMap(HGDIOBJ hObject);
		CIml_Data* GetCImlDataFromMap(HIMAGELIST himl);
#ifndef _WIN32_WCE
		CMenu_Data* GetCMenuDataFromMap(HMENU hMenu);
#endif

		CWnd* GetCWndFromMap(HWND hWnd);		TLSData* GetTlsData() const;
		void	SetCallback();
		TLSData* SetTlsData();
		static CWinApp* SetnGetThis(CWinApp* pThis = 0);

		std::map<HDC, CDC_Data*, CompareHDC> m_mapCDCData;
		std::map<HGDIOBJ, CGDI_Data*, CompareGDI> m_mapCGDIData;
		std::map<HIMAGELIST, CIml_Data*, CompareHIMAGELIST> m_mapCImlData;
#ifndef _WIN32_WCE
		std::map<HMENU, CMenu_Data*, CompareHMENU> m_mapCMenuData;
#endif

		std::map<HWND, CWnd*, CompareHWND> m_mapHWND;		// maps window handles to CWnd objects
		std::vector<TLSDataPtr> m_vTLSData;		// vector of TLSData smart pointers, one for each thread
		CCriticalSection m_csMapLock;	// thread synchronisation for m_mapHWND
		CCriticalSection m_csTLSLock;	// thread synchronisation for m_vTLSData
		CCriticalSection m_csAppStart;	// thread synchronisation for application startup
		HINSTANCE m_hInstance;			// handle to the applications instance
		HINSTANCE m_hResource;			// handle to the applications resources
		DWORD m_dwTlsData;				// Thread Local Storage data
		WNDPROC m_Callback;				// callback address of CWnd::StaticWndowProc

	};
	
	////////////////////////////////////////
	// Global Functions
	//

	inline CWinApp& GetApp()
	// Returns a reference to the CWinApp derived class
	{
		return *CWinApp::SetnGetThis();
	}	

}

#endif // _WIN32XX_APPCORE1_H_