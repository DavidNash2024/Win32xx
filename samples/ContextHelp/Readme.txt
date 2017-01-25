ContextHelpDemo Example
=======================
This program has a frame window which includes a Tool Bar and 
Status Bar.  A modeless dialog is used as the view window of our frame.

This project builds on the FormDocView sample program found elsewhere within
this folder to add the AppHelp class, which enables access into a Compiled HTML
(.chm) user's manual on a selected topic basis.


Instructions
============
Run the Install.bat file located in the Setup folder. This copies
ContextHelp.chm to the %AppData%\Win32++\ContextHelp folder for use by the
sample when it runs.


Features demonstrated in this example
=====================================
* Use of CFrame to provide a frame window
* Using a modeless dialog as a view window for the frame.
* Implementing a Doc/View architecture much like MFC.
* Saving the dialog's settings in the registry.
* Use of OnMenuUpdate to update check state of menu items
* Use of the AppHelp class to enable access to context-selected help topics
  in the ContextHelpDemo.chm manual, whose source files and other materials
  required to generate the manual are included in the sample program package.
* Context help topics are initiated using the F1 key, the Shift-F1 key, and
  a context help button on the toolbar. These operate the same as the usage
  described in MFC documentation.
* The context help and AboutBox dialog are combined into one class, AppHelp.
* A complete description of the usage of the AppHelp class appears in the 
  ContextHelpDemo.chm topic "Using the AppHelp Class."
* A toolbar with an expanded number of 24x24-bit buttons to illustrate a
  more comprehensive set of topics within the manual. A 16x16-bit button toolbar
  is also used as the buttons appearing in the main menu.


About Dialogs
=============
Dialogs are easy to create, provided we have access to a resource 
editor to build the resource script file for us (often called
resource.rc). Commercial compilers usually include a resource editor
for this purpose, but free compilers generally don't. For a free 
resource editor, try resedit from here: http://www.resedit.net/.


About Compiled Help User Manuals
===== ======== ==== ==== =======
Compiled HTML user manuals are also reasonably straightforward to create
using an ordinary text editor or HTML editor and the Microsoft Help Workshop.
Consult the ContestHelpDemo.chm user manual topic "Creating Help Files" for 
complete instructtions.



