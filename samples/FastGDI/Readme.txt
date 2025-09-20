FastGDI Example
===============
This example demonstrates how to change the colour of a bitmap by manipulating
the bitmap data directly. This provides a rapid technique for changing the
colour in a device context associated with a window. Traditional GDI techniques
using GetPixel and SetPixel are much slower.

This program can load, modify and save bitmap files. It does not load other
image types.

The image is displayed with scrollbars if required. The image is centered in
in the view window if the view window is large than the image.

This program supports print and print preview.

Features demonstrated in this example
=====================================
* Use of large toolbar buttons.
* Implementation of a Most Recently Used (MRU) List in the File menu.
* Using of the CDC class to work with device contexts
* Use of the CBitmapInfoPtr class to access a bitmap image.
* Using the TintBitmap function to manipulate the bitmap date directly.
* Use of double buffering to render GDI quickly.
* Automatically resize the frame to match the image size when its loaded.
* Inheriting the image view from CScrollView to display scroll bars.
* Printing a bitmap.
* Using CPrintPreview to preview the print job before printing.
* Use of drag and drop.
* Using a dialog to input color tint values from slider controls.
* Use of OnIdle to dynamically update the toolbar buttons.
* Use of OnMenuUpdate to dynamically update the menu items.
* Use of a user defined message to pass information to a parent window.