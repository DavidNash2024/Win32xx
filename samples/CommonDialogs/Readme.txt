CommonDialogs Example
=====================

This sample program has a frame window which includes a menu bar, tool bar
and status bar, with a modeless dialog as the client window of the frame. 
It was adapted from the FormDocView and NotePad programs found in the Win32++ 
samples directory. It is meant to serve as the template for applications that 
require a full range of features, including context-sensitive help, full color 
selection of frame, controls, and client area, font selection for controls, 
standard file open and save-as dialogs, selection and use of printers, and 
persistence of the most-recently used list (MRU) data relegated to CArchive 
files located in the user's Application Data folder.

Features demonstrated in this example
=====================================

* App-Doc-Frame-View Single Document Interface (SDI) program architecture 
  much like MFC applications.

* Use of CFrame to provide a frame window (docking not supported).

* Extension of CMainFrame to add serialization of the MRU list and display of
  items that exceed the set maximum length using the  ellipsis "..." in the 
  middle of the truncated string, rather than at its beginning, as in Win32++.

* Implementation of a reusable AppGlobal class that provides a set of string 
  variables commonly needed by many applications, such as the application
  title, file name with extension, version number, full path name and the 
  directory part of that path, the data archive file path name and its
  directory, the help file path name and its directory, the Win32++ framework 
  version number, the compiler with its version number, the date of 
  compilation, the document extension and file dialog filter, and candidate
  information that will appear in the AboutBox when activated. It also 
  provides the size of the maximum Most Recently Used (MRU) file list.
  Application-specific data are relegated to a resource file AppGlobal.rc.

* Use of the Win32++ CArchive class to store and retrieve the frame settings 
  (including MRU list, status and toolbar display switches, and window
  placement), as well as perhaps some document status data. The frame's 
  persistent data is  stored in the path named 

	<app_data_path>\win32++\CommonDialogs.arc

  where <the app_data_path> is found using the Win32++ GetAppDataPath() 
  function.  

* Implementation of the AppHelp class that provides the AboutBox above and 
  also context sensitive help in the form of automated access to a compiled 
  HTML (.chm) file whose topics appear when the user selects the (?) help 
  button on the toolbar and then selects a feature of interest. This guide
  also contains a topic giving instructions on how to create or edit this 
  .chm file using freeware tools. The guide and supporting files are found
  in the relative path (from this readme.txt file)

       .\src\help\CommonDialogs.chm. 
 
  The CommonDialogs.hnd file appearing in this help subdirectory is the source 
  from which the .chm form was created using the HelpNDoc tool. The decompiled
  .htm, .hhp, .hhc, and .hhk are also located here.

  NOTE: In order for this sample program to locate the help file, a copy of
  the CommonDialogs.chm file must be placed in the help file directory 
  computed by the AppGlobal object, as above, which is nominally in the same 
  directory as the serialized data archive. To assist this process, the .\Setup
  directory contains an Install.bat file to copy the help and arcived 
  persistent data to the appropriate destination. If, on running the program, 
  access to topics in the guide to not appear, it may be necessary to unblock
  the file locks on CommonDialogs.chm in the file's property sheet.

* Use of the entire Win32++ common dialog classes (CFontDialog, CColorDialog,
  CFileDialog, CFindReplace, CPrintDialog, and CPageSetupDialog) for selection 
  of fonts, colors, files, text for find/replace purposes, and printer 
  parameters.

* Reusable classes CColorChoice and CListBoxDlg that, with CView, implement 
  the selection and display of the font foreground, font background, and 
  control colors for the RichEdit control, as well as the colors of the dialog
  and status bar backgrounds.

* Reusable derived classes MyFileDlg, MyFindReplaceDlg, MyFontDlg, MyPrinter,
  and MyPrintSetup that customize the appearance of their respective common
  dialog base classes and bring context help functions to these dialogs.

* Programmer-selectable menu, tool bar, and status bar themes, as well as
  user-selectable foreground and background colors of the dialog controls,
  client area, and status bar.

* Tool tips that appear when the mouse hovers over tool bar, dialog controls,
  and the dialog client area.

* Display of program title, executable name, program version number, Win32++ 
  version number, compiler name, and most recent compilation date in the 
  AboutBox dialog.

* Placing of certain string resources and their definitions, respectively,
  in separate .rc and .h files so as to more easily reuse these program 
  elements in other applications. These include the AppGlobal, AppHelp, 
  and ListBoxDlg classes.

* There are several features that appear in this demo program that would not
  perhaps appear in an actual program. For instance, message boxes may appear
  to inform the user of internal events. These would, of course, be eliminated
  in production progams.

* MRU strings that are recovered from serialization that are not valid file 
  paths are discarded at program statup by the CMainFrame::ValidateMRU() 
  method. In this way, files that may have been deleted between executions
  do not adversely affect operations.

* Use of a CRichEdit control as a file view window with extensive editing
  and access capabilies, including cut, copy, paste, undo, redo, save, 
  save as, drag-and-drop, and print capabilities. The control is limited 
  to textual content. Both ANSI and Unicode text operation are accommodated.

* Addition of the CPrintInfo and CPrintView classes for printing of documents.
  These emulate the manner of printing found in MFC applications. A special
  CPrintRichView class derived from CPrintView handles the printing of the
  rich edit window contents. Print preview and printing of multiple copies 
  are not supported.

