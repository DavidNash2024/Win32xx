Notepad Example
===============
This project demonstrates how to use a Rich Edit control as a simple text 
editor. It supports the Rich Text Format(RTF), as well as ANSI, UTF-8 and
UTF-16 encodings.

This application loads files with a 'txt' extension as plain text, and files
with a 'rtf' extension as rich text. It can load and save plain text files
using ANSI, UTF-8 and Unicode (UTF-16LE) encodings. It can load and save
rich text files using the ANSI encoding. Choosing an encoding determines
how text is saved. It doesn't alter how text is displayed.

Files using the UTF-8 or UTF-16LE encoding are saved with a Byte Order Mark
(BOM). This allows the encoding to be determined when they are loaded.
Files without a UTF-8 or UTF-16LE BOM are presumed the be ANSI.

Plain text files can only have a single font. Rich text files can have multiple
fonts within a single document.


Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame.
* Use of a rich edit control as the frame's view window.
* Loading and saving text files with rich edit control stream callbacks.
* Supporting Copy, Paste, Undo and Redo.
* Using file drag and drop to load text files.
* Use of OnIdle to dynamically update the toolbar buttons.
* Use of OnMenuUpdate to dynamically update the menu items.
* Selecting a font to use with plain text and rich text in a rich edit control.
* Enabling and disabling word wrap.
* Printing the document.
* Use of CPrintPreview to preview the print job before printing.


Notes:
======
ASCII is a single byte encoding only using the bottom 7 bits
(Unicode code points 0-127). 

UTF-8 is a variable length encoding, capable of encoding all 1,112,064 valid
Unicode code points using one to four one-byte (8-bit) code units. The first
128 characters of Unicode correspond one-to-one with ASCII. They are encoded
using a single byte with the same binary value as ASCII, so that valid ASCII
text is valid UTF-8-encoded Unicode as well. UTF-8 text strings are stored
as an array of single byte char (or CHAR).

On Windows the term "Unicode" usually refers to UTF-16 little ended (UTF-16LE). 
Technically, the term Unicode refers to all unicode encodings, including
UTF-8, UTF-16 and UTF32.

On Windows, the term ANSI usually refers to the system's default code page 
(often Windows-1252). The code pages extend the ASCII standard. Most
code pages have a fixed length of one byte (8 bits).

The Rich Text Format(RTF) does not support the UTF-8 encoding.
