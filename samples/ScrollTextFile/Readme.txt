ScrollWnd Demo Example
==============
This project demonstrates how to use Win32++ to create a scrollable window to
display the contents of a text file using the familiar CApp-CMainFrame-CDoc-CView
architecture.  The CView class that controls the displayed material is derived from
the CScrollWnd class, introduced here, which performs the actual scrolling 
functions. The CApp class starts Win32++ and runs the message loop. The CDoc class
reads a text document and the CView class displays the document's text on the screen.  


Features demonstrated in this example
=====================================
* Inheriting CApp from CWinApp to start Win32++.
* Deriving a CScrollWnd class from CWnd to perform scrolling of the client area.
* Inheriting CView from the CScrollWnd class to create the CView class to display 
  a text document.
* Using a resource script (resource.rc) to add an icon, window caption, main menu,
  toolbar, about box, and tool tips.
* Deriving extended classes CColorExDialog and CFontExDialog from the common 
  dialog classes CColorDialog and CFontDialog to permit setting the titles of the
  color and font choice dialog boxes.
* Using the CColorExDialog class to input the window background color.
* Deriving a new CFontEx class from CFont that uses the CFontExDialog class to 
  input the view font face, style, color, size, and effects.
* Saving the current state of the application in an archive file that is compatable
  with both ANSI and UNICODE character modes.
* Processing window messages in CScrollWnd::WndProc and CView::WndProc.
* Overriding PreCreate to set the initial window parameters.
* Overriding the OnDraw method to draw the document text on the window's device
  context.
* The detection of the selected file's encoding and transforming file input into
  Windows Unicode format. Files encoded in ANSI, UTF-8, and UTF-16 are supported,
  the latter in both big endian and little endian formats.