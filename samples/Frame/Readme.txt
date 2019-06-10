Frame Example
=============
This project demonstrates the use of the CFrame class to display a frame
window. A frame uses a separate view window which is displayed over its 
client area.

CFrame provides a set of features including:
- A ReBar which hold the frame's ToolBar and MenuBar
- A MenuBar which provides a menu within the ReBar 
- A ToolBar
- A StatusBar, displaying ToolBar/MenuBar status and indicators
- Support for a separate view window
- ToolTips for ToolBars
- Themes support to set colours for the ReBar, MenuBar and ToolBar
- Most Recently Used (MRU) list support
- The ability to save its settings in the registry 


The application starts as follows:
- In WinMain we create an instance of CFrameApp called theApp. CFrameApp 
   inherits from CWinApp. Creating an instance of CFrameApp starts the
   Win32++ framework.
- CFrameApp has a CMainFrame member variable. CMainFrame inherits from CFrame.
- Calling theApp.Run calls CFrameApp::InitInstance. This in turn creates the 
   frame window and runs the application's message loop.
- During the creating of the frame window, CMainFrame::OnCreate is called. Here
   we can set some optional parameters before calling CFrame::OnCreate. 
- CFrame::OnCreate creates the frame's child windows, namely the ReBar, 
   MenuBar, ToolBar, StatusBar, and View Window. The View window is specified
   in CMainFrame's constructor using SetView.
- CMainFrame::OnInitialUpdate is called once the creation of the frame window
   and all its child windows is complete.    


This is a very simple project which could be used as a starting point for your
own frame based applications.


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
* Responding to frame window messages in CFrame::WndProc.
* Responding to view window messages in CView::WndProc.
* Using the TRACE command to output debug text.
* Saving the frame settings in the registry.
* Support for print preview.


