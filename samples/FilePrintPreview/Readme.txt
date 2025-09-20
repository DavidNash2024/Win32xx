FilePrintPreview Example
====================
This project extends the Notepad sample programs by adding a CPrintPreviewEx
class. The CPrintPreviewEx has methods that may be overridden by a derived
class that can be used by other applications in an architecture akin to that
found in the Microsoft Foundation Classes (MFC) print preview methods. 

The CPrintPreviewEx class displays a detached dialog view of a document. The
user can specify the degree to which the print preview is scaled.

To use the class, the CPrintPreview::DoPreparePreview() method is overridden by
the application to set special device contexts, determine pagination, and set
the number of pages in the preview. The CPrintPreview::OnPrepareDC() method
is overridden to make any adjustments of the device contexts needed on a
page-by-page basis. The PrintPreview::DocPageToBmp() method is overridden to
format a document page for preview and to deposit the image into a memory
device context. This compatible bitmap is then copied to the PreviewPane's
device context, rescaled to fit the window in the chosen scale. 

Features demonstrated in this example
=====================================
* Simultaneously displaying the preview window along with the frame and view.
* Adding custom controls and tool tips to a dialog.
* Supporting OnDraw for a custom control used in a dialog.
* The use of a memory device context and compatible bitmap to copy an image to.
* Transfer of the bitmap image into a device-independent bitmap (DIB) for display.
* The use of StretchDIBits to scale an image copied from the DIB to the screen.
