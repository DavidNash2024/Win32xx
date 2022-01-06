PrintDialogEx Sample
====================
This project is an update to the Notepad sample. It demonstrates how to
use CPrintDialogEx instead of CPrintDialog to print the document.

The use of CPrintDialogEx allows users to specify a group of page ranges to
be printed. 


New Features demonstrated in this example
=========================================
* How to print individual pages.
* How to print several page ranges (up to 10).
* How to print multiple copies, with or without collating.
* How to use the Apply button to save settings for later use.
* How to use a footer to display the page number on the printed page.
* How to acquire the printer's device context from the dialog's current
   settings while the print dialog is active.
* How to dynamically update the maximum pages to print while the print dialog
   is active.

Notes:
* The document needs to be over one page in length to enable the Pages option.
* The print dialog will display a message if the pages selected are out of range.
* This sample requires VS2008 or later, or a modern MinGW compiler.
* The settings saved by Apply button are kept if we then use the Cancel button.
* An example of a valid page range entry is: 1-4,2,6-2,8,4 
