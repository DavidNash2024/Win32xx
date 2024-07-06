Scribble Example
================
This project demonstrates how to use messages from the mouse to draw on a 
device context. This provides a convenient way to demonstrate how process
window messages in WndProc.

When the user draws on the window with the mouse, the lines drawn are saved
in a vector. Information stored in the vector is used to repaint the screen as 
required. This information can also be saved to and loaded from a file.

The scribble example is also the final product of the set of tutorials provided
with Win32++.


Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame
* Toolbar configuration
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide toolbar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a rebar in the frame.
   - Use/don't use themes.
   - Use/don't use a toolbar in the frame.
* Setting the view window of the frame.
* Responding to toolbar and menu commands in OnCommand
* Responding to notifications in OnNotify.
* Capturing mouse input.
* Use of a vector to store data.
* Drawing to a window.
* Loading and saving data to a file.
* Printing the content of a window.
* Print previewing the content of a window.
* Responding to frame window messages in CFrame::WndProc
* Responding to view window messages in CView::WndProc
* Using the TRACE command to output debug text.
* Saving the frame settings in the registry.
* Implementing a Most Recently Used (MRU) list in the file menu.
* Use of the CDC class to work with device contexts.
* Use of double buffering to speed up drawing to a device context.
* Use of user defined messages to pass information between CView and CMainFrame.

