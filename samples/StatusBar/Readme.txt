StatusBar Example
=================
This project demonstrates the how do the following with a StatusBar
* Customize the color of the status bar
* Host a Hyperlink child window
* Host a ProgressBar child window
* Display an icon
* Define an owner drawn part and draw to its device context directly.


CMyStatusBar is derived from CStatusBar. We override CFrame::GetStatusBar
in CMainFrame to use CMyStatusBar in the frame. 

When XP themes are enabled, the StatusBar color is set in 
CMyStatusBar::OnEraseBkgnd. A color gradient is used as the StatusBar's
background. XP themes are enabled by default for Windows XP and above. 
For older systems without XP themes (windows 2000 and below), we set 
a solid background color using the SB_SETBKCOLOR message.

The status bar has 4 parts:
The Hyperlink control is placed over part 0. The Hyperlink control  is 
a modified static control which launches a web browser to display the 
web page when clicked.
 
The ProgressBar is placed over part 1. The progress bar is resized to
fit entirely over Part 1. GetPartRect is used to get the part's dimensions.  

An icon is displaced over part 2.

Part 3 is Owner Drawn. This part is drawn by CMainFrame::OnDrawItem. There
we set a gradient for the part's background and draw some italic text.


Features demonstrated in this example
=====================================
* Overriding CFrame::GetStatusBar to use a customized status bar
* Positioning child windows such as a hyperlink control and 
   progress bar on the StatusBar.
* Displaying an icon in a StatusBar part
* Performing owner drawing on a StatusBar part.
