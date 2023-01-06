StatusBar Example
=================
This project demonstrates the how do the following with a StatusBar
* Customize the color of the status bar
* Host a Hyperlink child window
* Host a ProgressBar child window
* Display an icon
* Define an owner drawn part and draw to its device context directly.
 

When XP themes are enabled, the StatusBar color is set in 
CMmainFrame::DrawStatusBarBkgnd. A color gradient is used as the StatusBar's
background. XP themes are enabled by default for Windows XP and above. 
For older systems without XP themes (windows 2000 and below), we set 
a solid background color using the SB_SETBKCOLOR message.

The status bar has 9 parts (0 through 8):
 * The frame's status is displayed in part 0
 * The Hyperlink control is placed over part 1. The Hyperlink control  is 
   a modified static control which launches a web browser to display the 
   web page when clicked.
 * The ProgressBar is placed over part 2. The progress bar is resized to
   fit entirely over Part 1. GetPartRect is used to get the part's dimensions.
 * An icon is displaced over part 3.
 * Part 4 modifies the part's background and text color and font.
 * Parts 5, 6, 7, and 8 display indicator status.


Features demonstrated in this example
=====================================
* Positioning child windows such as a hyperlink control and 
   progress bar on the StatusBar.
* Displaying an icon in a StatusBar part
* Performing owner drawing on a StatusBar part.
* ExcludeClipRect is used to exclude the progress bar from the background's
   region.
* Overriding CFrame::DrawStatusbar to perform the owner drawing of the statusbar.
* Overriding CFrame::DrawStatusBarBkgnd to draw the statubar's background.
* Overriding CFrame::SetStatusIndicators to configure the status indicators.
* Overriding CFrame::SetStatusParts to configure the status parts.  
