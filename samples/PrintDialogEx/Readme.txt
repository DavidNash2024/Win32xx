PrintDialogEx Sample
====================
This project is an update to the Notepad sample. It demonstrates how to
use CPrintDialogEx instead of CPrintDialog to print the document. 


New Features demonstrated in this example
=========================================
* How to print an individual page.
* How to print several (up to 10) page ranges.
* How to print multiple copies, with or without collating.
* Using the Apply button (and then cancel) to save settings for later use.


Notes:
* The document needs to be over one page in length to enable the page ranges.
* CPrintDialogEx will display a messagebox if the pages selected are out of range. 
* This sample runs on Win2000 or later.
* This sample requires VS2008 or later, or a modern MinGW compiler.
