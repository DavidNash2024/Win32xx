Toolbar Builder Sample
======================
This sample provides a utility for creating bitmaps for use with toolbars.

This utility can load, modify and save 4bit, 8bit 16bit, 24bit and 32bit
bitmaps. When an image loaded and then saved, it is saved with the same bits
per pixel as the loaded image. Images with a bits per pixel 24 or less can be
displayed with a transparency mask color.

Images can also be added to the current toolbar bitmap. When an image is added
to the toolbar it will be resized and have its bits per pixel adjusted to fit
the current bitmap.

This program works as follows.
The image is loaded from a chosen file. The file determines the image's
size and bits per pixel. A wide image is assumed to contain multiple images.
The images are loaded into an image list as a Device Independent Bitmap(DIB).
This image list is assigned to the toolbar in the view window.
A DIB is constructed from the image list with the original size and bits 
per pixel when saving to a file. CreateDIBSection is used to create a
handle to the DIB that is selected into a memory device context(dc).

Program menu and toolbar options
File New                     Choose the size of a new toolbar bitmap.
File Open                    Load a toolbar bitmap from a chosen file.
File Save                    Save a toolbar bitmap to a chosen file.
Image Add                    Add an image to the current toolbar bitmap.
Image Delete                 Delete the selected image(s).
Image Swap                   Swap the two selected images.
View Set Toolbar Background  Set the background color of the toolbar.
View Set Mask Color          Set the color of the transparency mask.
Display with Mask            Display the images using the transparency mask.

The Bitmaps folder contains a collection of toolbar bitmaps.
The Images folder contains a collection of png images.

Features demonstrated in this example
=====================================
* Loading a device independent bitmap(DIB) from a chosen file.
* Loading an image from file using GDIplus.
* Creating an image list and adding images to it.
* Creating the image list with or without a transparency color mask.
* Assigning the image list to a toolbar.
* Extracting a DIB from the image list.
* Saving a DIB to a chosen file.
* Using SetClassLongPtr to modify a window's background color.
* Displaying a toolbar within the view window.
* Displaying a toolbar within a pager control.
* Use of double buffering to display drawn contents.

