//////////////////////////////////////////////////////
// View.h
//  Declaration of the CView class

#ifndef VIEW_H
#define VIEW_H

#include "wincore.h"
#include <vector>

using std::vector;

// A class that inherits from CWnd. It is used to create the window.
class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void OnDestroy();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void DrawLine(int x, int y);
	LRESULT OnLButtonDown(LPARAM lParam);
	LRESULT OnLButtonUp(LPARAM lParam);
	LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);

	POINT m_OldPt;
};


#endif // CVIEW_H
