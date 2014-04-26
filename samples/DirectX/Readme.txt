DirectX Example
==================
This project displays a moving DirectX picture in a simple window. The code
in this example is based on DirectX version 9. 

Additional software is required to run this example.
For MS compilers you will need the DirectX SDK v9
For Dev-C++ you will need the DirecX v9.0c DevPak

Microsoft makes the the DirectX Software Development Kit available for download
without charge.

In this example we do things a little differently:
- CMainFrame has a CViewThread member variable. CViewThread inherits from
   CWinThread.
- CViewThread has a CView member variable for our view window. This CView is 
   set as the view window for CMainFrame in CMainFrame's constructor.
- The CViewThread thread is started in CMainFrame::OnCreate. Once the view's
   thread is started we then call CFrame::OnCreate.
- The view window is created automatically when the view thread starts.
- Once the view window's thread is started, the main thread waits for the view
   window to be created (or the view window creation to fail) before continuing.
- The view window runs a customized message loop. The message loop calls
   CView::Render to perform the DirectX drawing.

Features demonstrated in this example
=====================================
* Displaying a moving DirectX picture in a view window.  
* Putting a view window in a separate thread.
* Customizing a thread's message loop.
* The use of CreateEvent and SetEvent to signal events between threads.
* The use of WaitForSingleObject to synchronise threads.

