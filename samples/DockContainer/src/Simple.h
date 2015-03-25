////////////////////////////////////////////
// Simple.h - Declaration of the CViewSimple 
//            and CDockSimple classes

#ifndef SIMPLE_H
#define SIMPLE_H


enum DockIDs
{
	ID_DOCK_CLASSES1 = 1,
	ID_DOCK_CLASSES2 = 2,
	ID_DOCK_FILES1 = 3,
	ID_DOCK_FILES2 = 4,
	ID_DOCK_TEXT1 = 5,
	ID_DOCK_OUTPUT1 = 6,
	ID_DOCK_TEXT2 = 7,
	ID_DOCK_OUTPUT2 = 8
};

// Declaration of the CViewSimple class
class CViewSimple : public CWnd
{
public:
	CViewSimple() {}
	virtual ~CViewSimple() {}

protected:
	virtual void OnDraw(CDC& dc);
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnWindowPosChanged(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // SIMPLE_H
