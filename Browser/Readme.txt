Browser Example
===============
This project demonstrates how to implement a simple web browser as a view
window for a frame. It uses the IWebBrowser2 interface to implement
the WebBrowser control (Microsoft ActiveX control).

This example also demonstrates how to use ATL to display an ActiveX control.

Note: This example requires ATL in order to run.  ATL is not available for 
Dev-C++.


Features demonstrated in this sample
====================================
* ComboBoxEx within a rebar
* Use of Active Template Library (ATL) to incorporate ActiveX components
* Use of the IWebBrowser2 interface to render a web page
* Sinking events from IWebBroser2   


Instructions for Visual Studio 2005 Express
===========================================
ATL is not included with VS2005 Express, but is included with
the other versions of Microsoft compilers, including VS2008 Express.

In order to use VS2005 Express to compile this program, you will 
need to use the copy of ATL which ships with the latest 
"Platform Software Development Kit". Note that the "Windows Software 
Development Kit" does NOT include ATL. 

The latest Platform SDK can be downloaded from:
http://www.microsoft.com/msdownload/platformsdk/sdkupdate/

Once the Platform SDK is installed, users of VS2005 Express and VS2008 
Express will need to make the following changes:

 1) Add the include\atl path of Platform SDK to the set of
     directories used for include files.

 2) Modify atlwin.h
     Change SetChainEntry function at line 1725 and define
     "int i" at the first line of the function body.

 3) Modify atlbase.h
     Change AllocStdCallThunk and FreeStdCallThunk at line 287

     Make these changes:

		/* Comment these lines
		PVOID __stdcall __AllocStdCallThunk(VOID);
		VOID __stdcall __FreeStdCallThunk(PVOID);

		#define AllocStdCallThunk() __AllocStdCallThunk()
		#define FreeStdCallThunk(p) __FreeStdCallThunk(p)

		#pragma comment(lib, "atlthunk.lib")
		*/

     and add these 2 lines:
		#define AllocStdCallThunk() HeapAlloc(GetProcessHeap(), 0, sizeof(_stdcallthunk))
		#define FreeStdCallThunk(p) HeapFree(GetProcessHeap(), 0, p)


This information was published by Baryon Lee, in the CodeProject article
titled "Using WTL with Visual C++ 2005 Express Edition"
URL:  http://www.codeproject.com/wtl/WTLExpress.asp

This information was tested on Platform SDK version Windows Server 2003 SP1
