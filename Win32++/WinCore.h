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


///////////////////////////////////////////////////////
// WinCore.h
//  Declaration of the following classes:
//  CWinApp, CWnd, CWinClass, and CWinException



#ifndef WINCORE_H
#define WINCORE_H

#if defined (_MSC_VER) && _MSC_VER <= 1200
  // Correct for bug in Visual Studio 6
  #pragma warning (disable : 4786)
  #define DWORD_PTR DWORD
#endif // (_MSC_VER) && _MSC_VER <= 1200

#ifdef _MSC_VER
  #pragma warning (disable : 4511) // copy operator could not be generated
  #pragma warning (disable : 4512) // assignment operator could not be generated
#endif // _MSC_VER

#include <windows.h>
#include <commctrl.h>
#include <map>
#include <vector>


namespace Win32xx
{
	//
	// Forward declarations. These classes are defined later or elsewhere.
	//
	class CFrame;
	class CMenubar;
	class CMDIChild;
	class CWnd;
	
	
	//
	// Global functions (within the Win32xx namespace)
	//

	// Displays a warning message in a message box
	inline void DebugErrMsg(LPCTSTR ErrorMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, ErrorMsg, TEXT("Exception"), MB_ICONEXCLAMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(ErrorMsg);
	#endif  //_DEBUG
	}

	// Displays an error message in a message box
	inline void DebugWarnMsg(LPCTSTR WarnMsg)
	{
	#ifdef _DEBUG
		::MessageBox (0, WarnMsg, TEXT("Warning"), MB_ICONINFORMATION | MB_OK);
	#else
		UNREFERENCED_PARAMETER(WarnMsg);
	#endif  //_DEBUG
	}

	
	//
	// Global macros
	//
	#ifdef _DEBUG
			// Define global static TRACE macro for Debug mode only
	  #define TRACE(str) (GetApp()->Trace(str))
	#else  // _DEBUG not defined
			// Define a no-op static TRACE macro for Release mode
	  #define TRACE(str)
	#endif  // _DEBUG


	//
	// Global enumerations
	//
	enum Constants
	{
		MAX_STRING_SIZE = 255			// maximum string size
	};


	//
	// Global Structures
	//

	// The comparison function object used by CWinApp::m_HWNDmap
	struct CompareHWND
	{
		bool operator()(HWND const a, const HWND b) const
			{return ((DWORD_PTR)a < (DWORD_PTR)b);}
	};

	// Used for Thread Local Storage (TLS)
	struct TLSData
	{
		CWnd* pCWnd;		// pointer to CWnd object for Window creation
		HHOOK  hCBTHook;	// CBT hook for Window creation
		CMenubar* pMenubar;	// pointer to CMenubar object
		HHOOK  hMenuHook;	// MSG hook for CMenubar
	};


	/////////////////////////////////////////
	// Declarations for the CCriticalSection class
	//

	// This class is used for thread synchronisation
	class CCriticalSection
	{
		public:
		CCriticalSection()	{::InitializeCriticalSection(&m_cs);}
		~CCriticalSection()	{::DeleteCriticalSection(&m_cs);}

		void Lock()		{::EnterCriticalSection(&m_cs);}
		void Release()	{::LeaveCriticalSection(&m_cs);}

		private:
		CRITICAL_SECTION m_cs;
	};

	////////////////////////////////
	// Declaration of the CWnd class
	//
	class CWnd
	{
		friend class CMDIChild;
	public:
		CWnd();				// Constructor
		virtual ~CWnd();	// Destructor
		virtual BOOL Attach(HWND hWnd);
		virtual BOOL AttachDlgItem(UINT nID, CWnd* pParent);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, int x, int y, int nWidth, int nHeight, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, RECT rSize, HWND hParent, HMENU hMenu, LPVOID lpParam = NULL);
		virtual HWND Create(HWND hWndParent = NULL);
		virtual HWND Detach();
		virtual void DestroyWindow();
		virtual HWND GetAncestor(HWND hwnd);
		virtual CWnd* GetCWndObject(HWND hWnd);
		virtual HWND GetHwnd() {return m_hWnd;}
		virtual LPCTSTR LoadString(int nID);
		virtual LRESULT OnMessage(HWND hwndParent, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT& cs);
		virtual BOOL RegisterClassEx(WNDCLASSEX& wcx);
		virtual void SetParent(HWND hParent);
		static LRESULT CALLBACK StaticWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		virtual LRESULT CallPrevWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
		virtual void OnCreate();
		virtual void OnInitialUpdate();
		virtual void OnPaint(HDC hDC);
		virtual void RemoveHook();
		virtual void SetHook();
		virtual HICON SetIconLarge(int nIcon);
		virtual HICON SetIconSmall(int nIcon);
		virtual void Subclass();
		virtual void Superclass(LPCTSTR OldClass, LPCTSTR NewClass);
		virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		CREATESTRUCT m_cs;		// defines initialisation parameters for PreCreate and Create
		HWND m_hWnd;			// handle to this object's window
		HWND m_hWndParent;		// handle to this object's window parent
		TLSData* m_pTLSData;	// a structure for the Thread Local Storage data

	private:
		CWnd(const CWnd&);				// Disable copy construction
		CWnd& operator = (const CWnd&); // Disable assignment operator
		static LRESULT CALLBACK StaticCBTProc(int msg, WPARAM wParam, LPARAM lParam);

		HICON m_hIconLarge;			// handle to the window's large icon
		HICON m_hIconSmall;			// handle to the window's small icon
		WNDPROC m_PrevWindowProc;	// Pre-Subclassed Window Procedure
		TCHAR m_szString[MAX_STRING_SIZE + 1];	// TCHAR array used in LoadString

	}; // class CWnd


	///////////////////////////////
	// Declaration of the CWinApp class
	//
	class CWinApp
	{
		friend class CWnd;		// CWnd uses m_MapLock and SetTlsIndex
		friend class CDialog;	// CDialog uses m_MapLock

	public:
		CWinApp(HINSTANCE hInstance);
		virtual ~CWinApp();
		virtual void CreateTrace();
		virtual CFrame* GetFrame() {return m_pFrame;}
		static CWinApp* GetApp() {return st_pTheApp;}
		virtual HINSTANCE GetInstanceHandle() {return m_hInstance;}
		virtual HINSTANCE GetResourceHandle() {return m_hResource;}
		virtual std::map <HWND, CWnd*, CompareHWND>& GetHWNDMap() {return m_HWNDmap;}
		virtual DWORD GetTlsIndex() {return st_dwTlsIndex;}
		virtual int MessageLoop();
		virtual void SetAcceleratorTable(INT ID_ACCEL);
		virtual void SetFrame(CFrame* pFrame){m_pFrame = pFrame;}
		virtual void SetResourceHandle(HINSTANCE hResource) {m_hResource = hResource;}
		virtual void Trace(LPCTSTR szString);

	private:
		virtual TLSData* SetTlsIndex();

		enum Constants
		{
			TRACE_HEIGHT = 200,
			TRACE_WIDTH  = 400
		};

		CCriticalSection m_MapLock;	// thread synchronisation for m_HWNDmap
		HACCEL m_hAccelTable;		// handle to the accelerator table
		HFONT m_hFont;				// handle to the font in the Trace window
		HINSTANCE m_hInstance;		// handle to the applications instance
		HINSTANCE m_hResource;		// handle to the applications resources
		HMODULE m_hRichEdit;		// handle to the module for the RichEdit dll
		HWND m_hTraceEdit;			// handle to the Trace edit window
		BOOL m_IsTlsAllocatedHere;	// a flag set for the Thread Local Storage
		CFrame* m_pFrame;			// pointer to the CFrame object
		CWnd* m_pTrace;				// pointer the Trace CWnd object
		std::map<HWND, CWnd*, CompareHWND> m_HWNDmap;	// maps window handles to CWnd objects
		std::vector<TLSData*> m_ThreadData;	// vector of TLSData pointers, one for each thread
		static DWORD    st_dwTlsIndex;	// Thread Local Storage index
		static CWinApp* st_pTheApp;		// a pointer to this CWinApp object
	};

	// Global function which returns a pointer to the CWinApp object
	inline CWinApp* GetApp(){ return CWinApp::GetApp(); }


	////////////////////////////////////////
	// Declaration of the CWinException class
	//
	class CWinException
	{
	public:
		CWinException (LPCTSTR msg) : m_err (::GetLastError()), m_msg(msg) {}
		CWinException (const CWinException&) {}
		void MessageBox() const;

	private:
		LPCTSTR GetMessage() const { return m_msg; }

		DWORD  m_err;
		LPCTSTR m_msg;
	};

} // namespace Win32xx

// Automatically include the Win32xx namespace
using namespace Win32xx;

#endif // WINCORE_H

