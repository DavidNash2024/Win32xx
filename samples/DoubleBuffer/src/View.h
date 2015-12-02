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

	CBitmap CreateMaskBitmap(CBitmap& m_Blue, COLORREF clrTransparent);

protected:
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnDraw(CDC& dc);
	virtual void OnInitialUpdate();
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual void PreRegisterClass(WNDCLASS& wc);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CBitmap m_Blue;
	CBitmap m_Orange;
	CBitmap m_Mask;
	CSize m_BallSize;
};


#endif // VIEW_H
