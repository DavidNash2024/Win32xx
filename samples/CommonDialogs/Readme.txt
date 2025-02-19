CommonDialogs Example
=====================

This sample program has a frame window which includes a menu bar, tool bar,
and status bar, with a modeless dialog as the client window of the frame. 

The program demonstrates the use of the following common dialogs classes:
* CColorDialog
* CFileDialog
* CFindReplaceDialog
* CFontDialog
* CPageSetupDialog
* CPrintDialog


Features demonstrated in this example
=====================================

* App-Doc-Frame-View Single Document Interface (SDI) program architecture 
  much like MFC applications.

* Use of CFrame to provide a frame window (docking not supported).

* Generation of the basic set of string variables in CApp::InitInstance(), 
  such as the application title, file name with extension, version number, 
  full path name and the directory part of that path, the data archive file 
  path name and its directory, the Win32++ framework  version number, the 
  compiler with its version number, the date of compilation, the document
  extension, file dialog filter, and candidate information that will appear 
  in the AboutBox when activated. It also provides the size of the maximum 
  Most Recently Used (MRU) file list.

* Use of the entire Win32++ common dialog classes (CFontDialog, CColorDialog,
  CFileDialog, CFindReplaceDialog, CPrintDialog, and CPageSetupDialog) for the
  selection of fonts, colors, files, text for find/replace purposes, and
  printer parameters.

* Reusable classes CColorChoice and CListBoxDlg that, with CView, implement 
  the selection and display of the font foreground, font background, and 
  control colors for the RichEdit control, the backfrond color of the dialog.

* Reusable derived classes MyFileDlg, MyFindReplaceDlg, MyFontDlg, MyPrintDialog,
  and MyPrintSetup that customize the appearance of their respective common
  dialog base classes and bring context help functions to these dialogs.

* Tool tips that appear when the mouse hovers over tool bar, dialog controls,
  and the dialog client area.

* Display of program title, executable name, program version number, Win32++ 
  version number, compiler name, and most recent compilation date in the 
  AboutBox dialog.

* Use of a CRichEdit control as a file view window with extensive editing
  and access capabilities, including cut, copy, paste, undo, redo, save, 
  save as, drag-and-drop, and print capabilities. The control is limited 
  to textual content. Only ABSI/UTF8 text operation is accommodated. For a
  full Unicode capability consult the NotePad sample program of this release. 

* Addition of the CPrintInfo and CPrintView classes for printing of documents.
  These emulate the manner of printing found in MFC applications. A special
  CPrintRichView class derived from CPrintView handles the printing of the
  rich edit window contents. Print preview and printing of multiple copies 
  are not supported.

