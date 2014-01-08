MDIFrame Example
================
This project demonstrates the use of the CMDIFrame class to display a MDI frame
window. MDI (Multiple Document Interface) frames allow different types of view
windows to be displayed simultaneously. 

In this case the MDI Child's view window uses CDocker to create a splitter window.
The splitter window has dock containers docked at the left and the right.


Features demonstrated in this example
=====================================
* Use of CMDIFrame and CMDIChild classes to implement a MDI frame
* Setting the separate menu for the CMDIChild.
* Responding to the MDI specific commands in CMDIFrame::OnCommand
* Using a splitter(docker) window as a MDI child.
* Adding dock containers to the splitter window.
* Use of dock styles to prevent undocking.
