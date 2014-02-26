Picture Example
===============
This example uses the IPicture interface to load and display a graphic 
image.


Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame
* Use of the IPicture interface to load and render the picture
* Automatic resizing of the frame to accommodate the picture size when loaded
* Use of scroll bars
* Use of file open dialogs
* Use of a Most Recently Used (MRU) List in the File menu.
* Use of drag and drop.
* Use of a user defined message to pass information to a parent window


About iPicture.
iPicture is a COM interface, so COM should be initialized before it is used.
iPicture manages a picture object and its properties. It can work with bitmaps,
icons, and metafiles. The program can also load gif and jpeg files, Gif, but
these will be converted to bitmaps.