FilePrintPreview Example
====================
This project extends the Notepad and PrintPreview sample programs by redefining the
CPrintDialog class as an extensible CPrintPreview class that may be overridden by a
derived class that can be used by other applications in an architecture akin to 
that found in the Microsoft Foundation Classes (MFC) print preview methods.

As in its predecessors, the CPrintPreview class displays a detached dialog view of
a document, but rendered according to the needs of the program application. It 
consists of the usual title bar, the rendered file document in a client pane, and 
an array of buttons that allow navigation of the previewed document in several 
enlargement scales. The preview dialog screen is resizable by dragging an edge or 
the right-bottom corner.

Navigation includes first page, next page, previous page, last page, and GOTO a
user-input page number. Preview scales range from fitting an entire page on the
preview pane, to fitting the document to the pane width, or to selected fixed
scaling from 30% to 200%. All controls on the preview dialog display tool tips when
the mouse hovers over them.

When the print preview dialog is displayed, the frame and view remain visible and
may even appear on top of the preview window when selected. When the print preview
dialog is closed, frame and view remain in their normal forms.

The preview dialog also has a Print button, which initiates the printing of the
document, a Close button, which destroys the window (as does the [X] button in
on the title bar), and a Setup button, which accepts the sceen size and also
permits setup of the printer page parameters. It is necessary for the user to
enter the screen size on the first launch of the program, as computational means
of determining these parameters is unreliable when the screen resolution is not
made equal to the monitors maximum resolution. These parameters are recoded in the
system registry and retrieved on subsequent runs.

To use the class, the CPrintPreview::DoPreparePreview() method is overridden by the
application to set special device contexts, determine pagination, and set the
number of pages in the preview. The CPrintPreview::OnPrepareDC() method is 
overridden to make any adjustments of the device contexts needed on a page-by-page 
basis. The PrintPreview::DocPageToBmp() method is overridden to format a document
page for preview and to deposit the image into a memory device context. This ompatible bitmap is then copied to the PreviewPane's device context, rescaled to
fit the window in the chosen scale. 

So that the functionality of this architecture may be demonstrated, the base
CPrintPreview class DoPreparePreview(), OnPrepareDC(), and DocPageToBmp() methods
use the RichEdit features of the predecessor sample programs. Other usages of the RichEdit class as the RichView window and in the printer functions remain the same 
as in the ancestor sample programs.

Features demonstrated in this example
=====================================
* All the features demonstrated in the Notepad sample.
* Simultaneously displaying the preview window along with the frame and view.
* Adding custom controls and tool tips to a dialog.
* Supporting OnDraw for a custom control used in a dialog.
* The use of a memory device context and compatible bitmap to copy an image to.
* The use of StretchBlt to scale an image copied from a compatible bitmap
* A CPrintPreview class architecture that may be overridden to provide preview
  support for arbitrary applications.
* The use of CRichEdit features in the base class methods that will normally be
  overridden in order to demonstrate the features of this class.