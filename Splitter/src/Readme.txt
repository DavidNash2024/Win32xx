Splitter Window
---------------

This program uses the CSplitter class to create several splitter windows
inside a window frame.  

The view for the frame window is CMainView, which is a CSplitter object.
This object has two child window panes, top and bottom.  The panes are set with the 
following line of code in CMainView::CMainView()

SetPanes(m_Top, m_Bottom);


Each of these panes are themselves CSplitter objects. The top pane
is a CTopView object and the bottom one is a CBottomView object.
The CTopView object has two panes (left and right), as does the 
CBottomView.

Each of the panes for the CTopView and CBottomView splitter objects
are CView objects.  The CView object places some text in the center of
its window.
