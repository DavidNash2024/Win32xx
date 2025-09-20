CustomPrintDlg Sample
=====================
The print dialog has changed since the 22H2 update to Windows 10/11. The
new print dialog ignores the hDevMode and hDevNames supplied to it. The new
print dialog also displays a print preview we can't use, which is rather
unfortunate.

This project provides an alternative to the current print dialog. The 
CustomPrintDlg class emulates the print dialog from earlier versions of
Windows. It can be used as a replacement for CPrintDialog in the Notepad
sample as shown here. Unlike the PrintDlg and PrintDlgEx, the CustomPrintDlg
can be easily be customized to add or remove features appropriate to your
application.

This sample demonstrates how to manually create the hDevMode and hDevNames
global memory for a specified printer. This global memory is used by the
the CustomPrintDlg to retrieve and update the printer's DevNames and DevMode
information. 

This project also demonstrates how to use a Rich Edit control as a simple
text editor. It supports Rich Text Format (RTF), and plain text. It also 
supports ANSI, UTF-8 and UTF-16 encodings like the Notepad sample. 

Features demonstrated in this example
=====================================
* Using CCustomPrintDlg to display the print dialog.
* Enumerating the available printers.
* Choosing this application's printer.
* Creating a printer's hDevMode and hDevNames global memory.
* Retrieving and using the printer's DevMode and DevNames.
* Using CPrintPreview to preview the print job before printing.
* Printing the entire document document or a specified or page range.
* Printing multiple copies, with or without collating.

* Use of CFrame to display the window frame.
* Using a Rich edit control in the view window (ver 2.0 for unicode support).
* File open and file save using stream callbacks.
* Text operations, such as cut, copy, paste, delete, undo and redo.
* Selecting a file using drag and drop.
* Use of OnIdle to dynamically update the toolbar buttons.
* Use of OnMenuUpdate to dynamically update the menu items.
* Choosing the font to display the text.
* Enabling and disabling word wrap

* Loading and saving of UTF-8 encoded text files.
* Loading and saving of UTF-16 Little Ended encoded text files.
* Loading and saving of ANSI files (plain text only).
* Loading and saving of both rich text format (RTF) and plain text files.


Notes:
* UTF-8 files need a Byte Order Mark (BOM), otherwise they are treated as ANSI
  by the rich edit control.
* UTF-8 is the most popular encoding for text files, for good reason.
* UTF-8 is a super set of ASCII encoding. All ASCII text files are also UTF-8.
* UTF-8 uses one or more bytes to encode a character.
* ASCII encoding uses 7 bits, whereas ANSI encoding uses 8 bits.
* The way the ANSI text is displayed will depend on the code page.
* Microsoft typically uses the term "Unicode" to refer to UTF-16 Little Ended.
* UTF-16 Little Ended files can be loaded with a BOM.
* Files with a rtf extension are treated as rich text in this application.
* Files without a rtf extension are treated as plain text.
* Plain text files can only have a single font.
* Rich text files can have multiple fonts within the one document.
* Choosing an encoding determines how text is saved. It doesn't alter how text
  is displayed. 