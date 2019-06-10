PrintPreview Example
====================
This project demonstrates how to use CPrintPreview with a rich edit control. The Notepad sample extends the capabilities of PrintPreview by adding support for Rich Text Format, and various encodings.

CPrintDialog creates a modeless dialog for previewing. It is created in CMainFrame::OnCreate and is initially hidden. When the print preview dialog is displayed, the menu and toolbar are hidden, and the dialog is used as the frame's view window. When the print preview dialog is closed, the toolbar and menu are displayed and the frame's view is set back to the rich edit control. The print preview dialog isn't destroyed when it is closed, it is merely hidden.

The CRichView::PrintPage is used for both printing and print previewing. CPrintPreview's SetSource function is used to set CRichView as the source of the PrintPage function. CPrintPreview calls the PrintPage function to preview the specified page.  


Features demonstrated in this example
=====================================
* Loading the text document from a file.
* Saving the text document to a file.
* Printing the document.
* Use of CPrintPreview with a RichEdit control.
* Use of PrintPage in CRichView to support both printing and print previewing.
* Calculations of the Page Size and the Print Area used by the rich edit control.
* Use of CRichEdit's FormatRange to render the page for printing and print previewing.
* Dynamically hiding and displaying the frame's menu and toolbar.
* The use of CFrame's SetView function to dynamically swap the frame's view.
