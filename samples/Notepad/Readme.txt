Notepad Example
===============
This project demonstrates how to use a Rich Edit control as a simple text 
editor. 

Features demonstrated in this example
=====================================
* Use of CFrame to display the window frame
* Using a Rich edit control in the view window (ver 2.0 for unicode support)
* File open and file save using stream callbacks
* Copy and paste
* File drag and drop
* Text file printing
* Choosing the font.
* Enabling and disabling word wrap
* Loading and saving of UTF-8 encoded files.
* Loading and saving of UTF-16 Little Ended encoded files.
* Loading and saving of ANSI files (plain text only).
* Loading and saving of both rich text format (RTF) and plain text files.


Notes:
* UTF-8 files need a Byte Order Mark (BOM), otherwise they are treated as ANSI 
  by the rich edit control.
* UTF-8 is the most popular encoding for text files, for good reason.
* UTF-8 is a super set of ASCII encoding. All ASCII text files are also UTF-8.
* UTF-8 uses one or more bytes to encode a character.
* ASCII encoding uses 7 bits, whereas ANSI encoding uses 8 bits.
* Microsoft typically uses the term "Unicode" to refer to UTF-16 Little Ended.
* UTF-16 Little Ended files can be loaded with or without a BOM.
* Files with a rtf extension are treated as rich text in this application.
* Files without a rtf extension are treated as plain text.
* Plain text files can only have a single font.
* Rich text files can have multiple fonts within the one document.
* Choosing an encoding determines how text is saved. It doesn't alter how text
  is displayed. 