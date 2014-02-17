/////////////////////////////
// Button.h

#ifndef MYBUTTON_H
#define MYBUTTON_H


class CMyButton : public CButton
{
public:
	CMyButton() {}
	virtual ~CMyButton() {}

protected:
	virtual LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnNCHitTest(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //BUTTON_H


