DockTabbedMDI Example
=====================
This example demonstrates tabbed MDIs in addition to "container" docking.

Dock containers are a specialised view window for dockers.  They are a tab
control which provide support for tab undocking, and "container within container
docking" where docking the container adds a tab to the container.
Dock containers have their own "view window". They also provide an optional
toolbar.

The dock containers are the view window for dockers, allowing them to be
undocked and docked as required.

A TabbedMDI is a specialised view window which provides support for a tabbed
MDI (Multiple Document Interface). It uses a tab control to display the MDI
children, and also provides optional close and list buttons. The tabs can be
display either at the top or bottom of the window.

Classes titled CDockXXXX are inherited from CDocker.
Classes titled CContainerXXXX are inherited from CDockContainer.
Classes titled CViewXXXX are the various view windows inherited from CWnd.
The CMyTabbedMDI class is inherited from CTabbedMDI.

Refer to the documentation for further information on the use of the CDocker,
CContainer and CTabbedMDI classes.


Features demonstrated in this example
=====================================
* Use of RichEdit, ListView and TreeView windows as view windows.
* Adding containers as view windows to CDocker.
* Adding view windows to CDockContainer.
* Adding view windows to CTabbedMDI.
* Use of the container's optional toolbar.
* Use of a ComboBoxEx control within a toolbar.
* Use of CWebBrowser as a view window for a TabbedMDI.
* Displaying tabs at the top or bottom of the container.
* Displaying tabs at the top or bottom of the TabbedMDI.
* Hiding tabs for dock containers with a single tab.
* The use of several docking styles.
* Saving the dock layout and TabbedMDI layout in the registry.
* Adding a "Window" menu item to the frame menu, updated in runtime.
