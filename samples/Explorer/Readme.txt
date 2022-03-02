Explorer Example
================
This example demonstrates how to work directly with the Shell Namespace to
produce an explorer-like utility. The Shell namespace organizes the file system
and other objects managed by the Shell into a single tree-structured hierarchy.
Conceptually, it is a larger and more inclusive version of the file system.

The Shell Namespace is managed through the IShellFolder interface.  The
CShellFolder class is used to wrap the various members of the IShellFolder
interface.

This sample also demonstrates the use of PIDLs (Pointer to Item Identifier List)
to walk through the file system. The various functions used to work with the
PIDLs are wrapped up in the Cpidl class.

This sample uses dockers to display the window panes separated by a splitter
bar. Even though dockers are used, CMainFrame inherits from CFrame rather than
CDockFrame. The main frame's view is CRightPane which is a docker. CRightPane
is the dock ancestor and CLeftPane is added as its dock child.

Typically, CMainFrame would inherit from CDockFrame and use CMyTreeView as
its view window. It inherits from CFrame on this occasion to demonstrate an
alternative approach.

This sample uses CFileFind to acquire a handle to interrogate the file's size
and modification time. This is more reliable than attempting to use CFile to
open the file which would be the typical approach. File ownership permissions
can prevent CFile from opening a file, unless the program is run with
administrator privileges.


Features demonstrated in this example
=====================================
* Displaying large toolbar buttons with text.
* Using a drop down menu with a toolbar button.
* Using CDocker to display window panes separated by a splitter bar.
* Using a TreeView control and ListView control as a view window for CDocker.
* Using PIDLs to walk through the file system.
* Use of the IShellFolder interface object to work directly with the Shell
   Namespace.
* Storing the column width of the list-view window in the registry.  
