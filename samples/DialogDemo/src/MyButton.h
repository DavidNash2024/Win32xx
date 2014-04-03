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
	virtual void	OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void	OnNCHitTest(WPARAM wParam, LPARAM lParam);
	virtual void	OnSetCursor(WPARAM wParam, LPARAM lParam);
	virtual void	OnLButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void	OnLButtonUp(WPARAM wParam, LPARAM lParam);
	virtual void	OnRButtonDown(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


#endif //BUTTON_H


