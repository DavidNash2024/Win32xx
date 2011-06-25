//////////////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H



// Declaration of the CView class
class CView : public CWnd
{
public:
	CView();
	virtual ~CView(){}

protected:
	virtual void OnInitialUpdate();
	virtual void OnPaint(CDC* pDC);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif // VIEW_H
