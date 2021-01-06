DDXDemo Example
===============

This sample program has a frame window which includes a Menu bar, Tool Bar
and Status Bar, in which a modeless dialog is used as the client window of
the frame. It was adapted from the FormDocView sample program in the Win32++
release.

The project architecture is of the App-Frame-Doc-View form, an approach used
in of these sample programs.  Frame and view data that are to persist between
executions, as well as the data comprising the document, are stored in the
system registry,

Features demonstrated in this example
=====================================

* App-Frame-Doc-View program architecture.
* Use of a modeless dialog derived from CDialog as the view window
* Use of the CRegKey class to store and retrieve persistent data in the 
  system registry.
* Use of Dialog Data Exchange (DDX) and Validation (DDV) classes to create 
  an interactive application programming interface to dialog boxes and the 
  view class accessing or depositing data in those controls. These are 
  consistent with similar capabilities in the Microsoft Foundation Classes.
* A resizable form for the display, with a resizing gripper tool at the
  end of the status bar.
* Programmer-selectable menu, tool bar, and status bar themes, as well as
  selectable foreground and background colors of the dialog controls and
  client area.
* Tool tips that appear when the mouse hovers over tool bar, dialog controls,
  and the dialog client area
* Use of a rich edit control on the form derived from the CRichEdit class.



