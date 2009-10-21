DockContainer Sample
====================
This sample demonstrates "container" docking. Containers are a specialised
view window for dockers.  They are a tab control which provide support for
tab undocking, and "container within container docking" where docking the
container adds a tab to the container. They also provide an optional toolbar.
Containers in turn have their own "view window"

Classes titled CDockXXXX are inherited from CDocker. Classes titled 
CContainerXXXX are inherited from CContainer. Classes titled CViewXXXX
are the various view windows inherited from CWnd.

Refer to the documentation for further information on the use of the CDocker
and CContainer classes.


Features demonstrated in this sample
====================================
* Use of CFrame to provide a frame window.
* Use of a CDocker as the view window for the frame.
* Use of CContainer as a view window for CDocker.
* Use of RichEdit, ListView and TreeView windows as view windows for CContainer.
* Use of the container's optional toolbar.
* Use of a ComboBoxEx control within a toolbar.
* Displaying tabs at the top or bottom of the container.
* The use of several docking styles.
* Saving the dock layout in the registry.
