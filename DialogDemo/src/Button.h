/////////////////////////////
// Button.h


#ifndef BUTTON_H
#define BUTTON_H



class CButton : public CWnd
{
public:
	CButton() {}
	virtual ~CButton() {}

protected:
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

};



#endif //BUTTON_H


