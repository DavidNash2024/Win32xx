INIFrame Example
================
This project demonstrates the use of the CFrame class to display a frame
window. A frame uses a separate view window which is displayed over its 
client area.

This program doesn't store frame settings in the Registry, it stores
the frame settings in a Frame.INI file instead.

This demo is the same as the Frame demo in all other respects.

Note that writing to an ini file is no longer the recommended way of storing
and retrieving an application's settings. The registry should be used for this.
 

Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame
* ToolBar configuration
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide ToolBar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a ReBar in the frame.
   - Use/don't use themes.
   - Use/don't use a ToolBar in the frame.
* Setting the view window of the frame.
* Responding to ToolBar and menu commands in OnCommand
* Responding to notifications in OnNotify.
* Use of OpenFile and print dialogs.
* Responding to frame window messages in CFrame::WndProc
* Responding to view window messages in CView::WndProc
* Using the TRACE command to output debug text
* Using the Window API functions write and read to an INI file, including:
   - WritePrivateProfileString
   - GetPrivateProfileInt
* Using a string stream to convert between an int and a TCHAR array or CString
   - CMainFrame::ItoT
   - CMainFrame::TtoI
* Support for print preview.
   

