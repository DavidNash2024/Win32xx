DLL Example
===========
This project demonstrates how to run Win32++ from inside a DLL.
The MakeDLL project creates the DLL which exports one function called ShowDialog.
The TestDLL project creates a program which uses the DLL to display the dialog.

Features demonstrated in this example
=====================================
* Running Win32++ from inside a DLL.
* Exporting functions from a DLL.
* Creating pointers to functions exported by a DLL.
* Using a DLL to display a dialog created by Win32++.
* Using TRACE within a DLL to send text to the Output window of the IDE.