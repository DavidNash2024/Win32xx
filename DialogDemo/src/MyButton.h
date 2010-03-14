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
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

};



#endif //BUTTON_H


