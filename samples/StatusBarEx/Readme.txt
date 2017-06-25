StatusBarEx Example
=================
This project demonstrates the how do the following with a status bar:
* Create a status bar with an arbitrary number of customized parts via a declared
  array describing the contents of each part.
* Set the color of the status bar.
* Define an owner drawn part and draw to its device context directly by using
  OnMessageReflect() to respond to the WM_DRAWITEM message initiated by the
  SBT_OWNERDRAW status bar part style.
* Host a Hyperlink in a status bar part that launches a web browser to display 
  the Win32++ SourceForge web page when clicked bu using OnMessageReflect() to 
  respond to the WM_CTLCOLORSTATIC message.
* Host a ProgressBar child in a status bar part.
* Display string and icon resources on the status bar.
* Display the status of the CAP, NUM, and SCRL keyboard states.
* Create the status bar in the WS_CLIPCHILDREN style to reduce flicker on resizing
  the frame.

The CustomStatusBar class is derived from CStatusBar and overrides the
CFrame::GetStatusBar in CMainFrame so as to use this class in the frame. 

When XP themes are enabled, the StatusBar color is set in CMyStatusBar::OnEraseBkgnd. 
A color gradient is used as the StatusBar's background. XP themes are enabled by 
default for Windows XP and above.  For older systems without XP themes (windows 2000 
and below), we set a solid background color using the SB_SETBKCOLOR message.

The status bar in this program has 8 parts: a menu and tooltip message area, the 
mainframe icon and string title resources part, a progress control, the hyperlink 
control, the owner drawn part, and the status indicators for CAP, NUM, and SCRL 
keyboard states. 

Features demonstrated in this example
=====================================
* Overriding CFrame::GetStatusBar to use a customized status bar.
* Using a PartInfo data struct to define the status bar parts.
* Displaying customized hyperlink and progress bar controls on the StatusBar.
* Handling of the drawing of status bar parts within the class owning the display
  of the part using message reflection.
* Displaying icon and string title resources in a StatusBar part.
* Performing owner drawing on a StatusBar part.
* Displaying the keyboard CAP, NUM, and SCRL states.
* Reduction of status bar flicker during resizing by clipping child windows during
  the redrawing process.
