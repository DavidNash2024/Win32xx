//////////////////////////////////////////////////////
// Rect.h
//  Declaration of the CViewRect class

#ifndef RECT_H
#define RECT_H


// Declaration of CViewRect
class CViewRect : public CWnd
{
public:
	virtual void OnCreate();
	virtual void OnDestroy();
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};




#endif  //RECT_H
