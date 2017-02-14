ContextHelpDemo Example
=======================
This program has a frame window which includes a Tool Bar and 
Status Bar. A modeless dialog is used as the view window of our frame.

Context help topics are initiated using the F1 key, the Shift-F1 key, the
"?" in the title bar, or a context help button on the toolbar.

When the context help is initiated, the user chooses the help topic to
be displayed with a mouse click on the frame's client or non-client
area. Once the help topic is chosen, the ContextHelp.chm help file is
opened in a window and displays the chosen topic.


Instructions
============
When the program is first run, it copies the ContextHelp.chm file from the 
help folder to the %AppData%\Win32++\ContextHelp folder. Alternatively, run
the Install.bat file located in the Setup folder to copy the ContextHelp.chm
file there.

The program will also find and use the ContextHelp.chm file if it is located
in the same folder as the program's executable. 


Features demonstrated in this example
=====================================
* Use of CFrame to provide a frame window
* Using a modeless dialog as a view window for the frame.
* Implementing a Doc/View architecture much like MFC.
* Saving the dialog's settings in the registry.
* Use of OnMenuUpdate to update check state of menu items.
* A CAppHelp class which provides access to the context sensitive help,
  and the Help About dialog.
* A toolbar with an expanded number of images sized 24x24 pixels to illustrate
  a more comprehensive set of topics within the manual.
* The use of a separate bitmap containing a set of images sized 16x16 pixels
  for the various menu items.


About Dialogs
=============
Dialogs are easy to create, provided we have access to a resource 
editor to build the resource script file for us (often called
resource.rc). Commercial compilers usually include a resource editor
for this purpose, but free compilers generally don't. For a free 
resource editor, try resedit from here: http://www.resedit.net/.


About Compiled Help User Manuals
================================
Compiled help user manuals have a chm extension. They are compiled from a set
of HTML files using the Microsoft Help Workshop. The HTML files can be created
using an ordinary text editor or HTML editor. Consult the ContestHelpDemo.chm
user manual topic "Creating Help Files" for complete instructions.



