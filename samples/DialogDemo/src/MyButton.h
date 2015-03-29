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
	virtual LRESULT	OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT	OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //BUTTON_H


