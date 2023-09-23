FrameEx Example
===============
This project extends the Frame sample by using PNG images instead of 
bitmaps for the for the toolbar and menu item icons. PNG images are
better suited to being resized than bitmaps. This makes PNG images
the better choice for images that need to be re-scaled to match the
window's DPI (Dots Per Inch).

This is a project which could be used as a starting point for your own
frame based applications. It is well suited applications that support
high resolution monitors or Per-Monitor DPI scaling. 


Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame
* ToolBar configuration
* Enabling/Disabling various frame features in OnCreate, namely:
   - show/hide ToolBar and menu status in the status bar.
   - show/hide indicators in the status bar.
   - Use/don't use a ReBar in the frame.
   - Use/don't use themes.
   - Use/don't use a ToolBar in the frame.
* Setting the view window of the frame.
* Responding to ToolBar and menu commands in OnCommand
* Responding to notifications in OnNotify.
* Use of OpenFile and print dialogs.
* Responding to frame window messages in CFrame::WndProc.
* Responding to view window messages in CView::WndProc.
* Using the TRACE command to output debug text.
* Saving the frame settings in the registry.
* Support for print preview.
* Use of PNG files as the resources included in resource.rc.
* Use of FindResource to locate the PNG resource.
* Use of IStream to copy the PNG data from the resource.
* Use of GDIPlus create a Gdiplus::Bitmap object.
* Retrieving an icon from a Gdiplus::Bitmap object using GetHICON. 
