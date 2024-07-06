Scintilla Demo
==============
This project demonstrates how to use the Scintilla text editor as a view window
within a Win32++ frame. The Scintilla control is used to develop a simple
text editor capable of loading, saving and printing text files. It also
provides a print preview and the option to select the font, text color and
background color.

On Windows, Scintilla is a Windows control that can be as a direct replacement
for a RichEdit control. Scintilla has a number of features missing from
RichEdit controls and has better support for printing.

Scintilla is a free source code editing component. It comes with complete
source code and a license that permits use in any free project or commercial
product. The source code and documentation can be found at
https://www.scintilla.org/.

This code requires the scintilla.dll file to be in the same folder as the
program's executable file (ScintillaDemo.exe). The dlls are located in the DLL
folder of this project. 32 bit programs requires the 32 bit scintilla.dll, and
64 bit programs require the 64 bit scintilla.dll.

Features demonstrated in this example
=====================================
* Using a Scintilla control as the view window for a frame.
* Loading and saving text files for the Scintilla control.
* Loading files with drag and drop.
* Support for a Most Recently Used (MRU) list.
* Printing and print preview the contents of the Scintilla control.
* Adding a header and footer to the printed output.
* Selecting a font for use in the Scintilla control.
* Changing the color of the text and background.
* Dynamically updating the enable state of toolbar buttons and menu items.



