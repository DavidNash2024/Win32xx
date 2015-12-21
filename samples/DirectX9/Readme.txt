DirectX Example
==================
This project displays a moving DirectX picture in a simple window. The code
in this example is based on DirectX version 9. 

Additional software is required to run this example.
For MS compilers you will need the DirectX SDK v9
For Dev-C++ you will need the DirecX v9.0c DevPak

Microsoft makes the the DirectX Software Development Kit available for download
without charge.

This example displays a rotating triangle using DirectX within the DXView window.
- CDXView is an ordinary child window, and is the view window of CMainFrame.
- CDX is a child window of CDXView, and displays the DirectX scene. It runs
   in a separate thread to the CDXView window. 
- CDXThread inherits from CWinThread. It creates a separate thread for the CDX
   window, and customizes the message loop to support DirectX.  

In this sample, CDX and CDXThread classes are nested classes of CDXView. Taken
as a group, these class can be used as the view window for any class that uses
view windows. The could be used in a CTab, CMDIChild, CDocker, CDockContainer, 
CTabbedMDI or CFrame.  

Features demonstrated in this example
=====================================
* Displaying a moving DirectX picture in a window.  
* Putting the child window of a view window in a separate thread.
* Customizing a thread's message loop.
* Using PostQuitMessage to end a GUI thread.
* Posting messages to windows in a different thread.
* The use of WaitForSingleObject detect when a thread has ended.

