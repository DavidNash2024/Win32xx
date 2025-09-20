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
* Use of the the CFontDialog, CColorDialog, CFileDialog, CFindReplaceDialog,
  CPrintDialog, and CPageSetupDialog classes.

* Use of CFrame to provide a frame window.

* App-Doc-Frame-View Single Document Interface (SDI) program architecture 
  much like MFC applications.

* Use of a CRichEdit control that supports cut, copy, paste, undo, redo, save,
  save as, drag-and-drop, and print.

* Use of the CColorChoice and CListBoxDlg classes that implement the selection
  and display of the font foreground, font background, and control colors for
  the RichEdit control, the background color of the dialog.

* Use of the MyFileDlg, MyFindReplaceDlg, MyFontDlg, MyPrintDialog, and
  MyPrintSetup classes that customize the appearance of their respective common
  dialog base classes and bring context help functions to these dialogs.

* Tool tips that appear when the mouse hovers over tool bar, dialog controls,
  and the dialog client area.

* Display of the program title, executable name, program version number,
  Win32++ version number, compiler name, and most recent compilation date in
  the AboutBox dialog.

* The CPrintInfo and CPrintView classes used for printing of documents.
  These emulate the manner of printing found in MFC applications.