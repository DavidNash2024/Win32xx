DialogTemplate Sample
=====================
This program identifies the dialogs within the selected exe or dll file
and displays them in a tree view. When a dialog is selected in the 
tree view, the dialog template is written to rich edit view in a form
that can be copied to another program.

Features demonstrated in this example
=====================================
* The use of CDockFrame and CDocker to build a frame which contains a 
  rich edit view, and a tree view.
* How to enumerate all the resources contained in an exe or dll file,
  using EnumResourceTypes, EnumResourceNames and EnumResourceLanguages.
* How to set CDialog's dialog from a dialog template and display it.
* How to print the contents of the rich edit view.
* How to print preview the contents of the rich edit view.
* How to save the contents of the rich edit view to a file.
* How to use OnIdle to dynamically update the toolbar buttons.
* How to use OnMenuUpdate to dynamically update enable state of the menu items.
 

The program will reconstruct the dialog template array from the text
output in the rich edit view. It will then use this array to display the
dialog.

The contents of the rich edit view can be saved to a header file.

Note that the application will fail to display a dialog if it contains
a custom control.




