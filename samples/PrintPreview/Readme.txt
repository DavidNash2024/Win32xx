PrintPreview Example
====================
This project extends the Notepad sample by adding a print preview.

The print preview is displayed within a dialog. The dialog has buttons such as "Prev Page" and "Next Page", as well as a custom control used to render the print preview image. The class for the print preview dialog is CPreviewDialog. The class for the custom control with the dialog where the print preview is rendered is CPreviewPane. 

The print dialog is a modeless window. It is created in CMainFrame::OnCreate and is initially hidden. When the print preview dialog is displayed, the menu and toolbar are hidden, and the dialog is used as the frame's view window. When the print preview dialog is closed, the toolbar and menu are displayed and the frame's view is set back to the rich edit control. The print dialog isn't destroyed when it is closed, it is merely hidden.

To render the print preview, the CRichEdit's FormatRange function is used to copy an image of the text to a compatible bitmap selected into a memory device context (dc). This compatible bitmap is then copied to the PreviewPane's device context, rescaled to fit the window as we do so. 


Features demonstrated in this example
=====================================
* All the features demonstrated in the Notepad sample.
* Dynamically hiding and displaying the frame's menu and toolbar.
* The use of CFrame's SetView function to dynamically swap the frame's view.
* Adding a custom control to a dialog.
* Supporting OnDraw for a custom control used in a dialog.
* The use of a memory dc and compatible bitmap to copy an image to.
* The use of BitBlt to scale an image copied from a compatible bitmap
* The use of CRichEdit's FormatRange function to extract a text image for the print preview.