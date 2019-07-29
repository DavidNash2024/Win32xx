DialogDemo Example
==================
This is a dialog application with fairly typical features. The dialog itself
is defined in the resource script (resource.rc).  The various controls
within the dialog are attached to CWnd objects using AttachDlgItem.

Also, refer to the FormDemo example for demonstration of additional features
such as saving the dialog's state in the registry.
  

Features demonstrated in this example
=====================================
* Using a dialog as an application.
* The use of AttachItem to attach a dialog control to a CWnd.
* The handling of messages in a button control.
* Using a static control as a hyperlink.
* Use of OnMessageReflect to handle notifications in the object that 
   generated them.