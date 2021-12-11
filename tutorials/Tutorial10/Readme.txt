Tutorial 10
===========
This is part of a set of tutorials which demonstrate how to use Win32++.
The tutorials are explained in detail in the tutorials section of the
help documentation.

Features demonstrated in this tutorial
======================================
* How to use CPrintPreview to add a print preview to the application.
* How to declare CPrintPreview as CMainFrame's member, specifying the source's
   class as the template type. The source is where the PrintPage function
   resides.
* How to specify values for the string resources used by CPrintPreview in
   resource.rc.
* How to use SetSource to specify to where to call the PrintPage function.
* How to Declare a PrintPage function in the source for printing and previewing:
   void  PrintPage(CDC& dc, UINT page);
* How to call DoPrintPreview(HWND ownerWindow, UINT maxPage = 1) to initiate the
   print preview.
* How to create the preview window, and swap it into the frame's view.
* How to handle UWM_PREVIEWCLOSE to swap back to the default view when the preview
   closes.
* How to handle UWM_PREVIEWPRINT to send the print job to the default or selected
   printer.
* How to handle UWM_PREVIEWSETUP to initiate a PrintDlg to select the printer and
   page settings.

