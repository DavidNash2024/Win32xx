BasicForm Example
===============

This sample program has a frame window which includes a menu bar, tool bar
and status bar, with a modeless dialog as the client window of the frame. 
It was adapted from the FormDocView sample program in the Win32++ release. 
It is meant to serve as the template for applications that require context-
sensitive help, full color selection of frame, controls, and client area, 
font selection for controls, standard file open and save-as dialogs, 
selection and use of printers, and a most-recently used list (MRU) in MFC-
like syntax and function, with persistent data relegated to CArchive files.

Features demonstrated in this example
=====================================

* App-Doc-Frame-View program architecture much like MFC applications.
* Use of CFrame to provide a frame window (docking not supported).
* Context-sensitive help via a compiled HTML (.chm) file whose topics
  appear when the user selects the (?) help button on the toolbar and then
  selects a feature of interest. The primary copy of the .chm file is found
  in the relative path ..\..\BasicForm\src\help\BasicForm.chm. 

  NOTE: In order for this sample program to locate the help file, a copy of
  this file must be placed in the same directory as the compiled executable
  BasicForm.exe. Instructions on how to create this .chm file using freeware
  tools appears as a topic in that file. The file BasicForm.hnd appearing
  in the help subdirectory is the source from which the chm form was created
  using the HelpNDoc tool. The decompiled .htm, .hhp, .hhc, and .hhk are
  also located here.

* Use of CFontDialog, CColorDialog, CFileDialog, CFindReplace, CPrintDialog,
  and CPageSetupDialog classes much like those in MFC for selection of fonts, 
  colors, files, text for find/replace purposes, and printer parameters.
* Ancillary classes CColorChoice, CListBoxDlg, and CtlColorChoice that
  implement display and selection of the font foreground, font background,
  and control colors for all colored elements of the client area.
* A derived CustomButton class that illustrates how to give buttons custom
  colors.
* Use of derived classes MyFileDlg, MyFindReplaceDlg, MyFontDlg, MyPrinter,
  and MyPrintSetup to customize the appearance of their respective dialogs
  and to extend context help functions to those dialogs.
* Use of the CArchive class to store and retrieve the frame settings 
  (including MRU list, status and toolbar display switches, and window
  placement) and document data. The frame's persistent data is stored in
  the file named BasicForm.arc, which, for this sample program, lies in the
  same directory containing the execuable, BasicForm.exe.
* A fixed-size form for the display, with no resizing gripper tool at the
  end of the status bar.
* Programmer-selectable menu, tool bar, and status bar themes, as well as
  user-selectable foreground and background colors of the dialog controls and
  client area.
* Tool tips that appear when the mouse hovers over tool bar, dialog controls,
  and the dialog client area.
* Display of Win32++ version and most recent compilation date in the About
  dialog box.


