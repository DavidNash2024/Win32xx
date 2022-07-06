DirectX9 Example
==================
This project displays a moving DirectX picture in a frame window. The code
in this example is based on DirectX version 9.

Additional software is required to run this example.
For MS compilers you will need the DirectX SDK v9
For Dev-C++ you will need the DirecX v9.0c DevPak

Microsoft makes the the DirectX Software Development Kit available for download
without charge. The latest version is of the DirectX SDK was published in
June 2010. This version supports Window OS versions 98 through to Windows 11.
Users will need install the DirectX End-User Runtimes (June 2010 for Win2000
and higher).

This example displays a rotating triangle using DirectX within the DXView window.
- CDXView is the view window of CMainFrame. It runs in a separate thread.
- CDXThread inherits from CWinThread. It creates a separate thread for the
   CDXView window, and customizes the message loop to support DirectX.


Features demonstrated in this example
=====================================
* Displaying a moving DirectX picture in a window.
* Putting the view window of a frame in a separate thread.
* Using a message loop in a separate thread to support DirectX.
* Customizing a thread's message loop.
* Using PostThreadMessage to post WM_QUIT and end a GUI thread.
* The use of CEvent to signal when a window has been created.
* The use of WaitForSingleObject detect when a thread has ended.
* Using a window that utilizes DirectX as the view window for a frame.
