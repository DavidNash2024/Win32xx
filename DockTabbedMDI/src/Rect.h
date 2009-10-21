//////////////////////////////////////////////////////
// Rect.h
//  Declaration of the CViewRect class

#ifndef RECT_H
#define RECT_H


// Declaration of CViewRect
class CViewRect : public CWnd
{
public:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_cxClientMax;
	int m_cyClientMax;
};




#endif  //RECT_H
