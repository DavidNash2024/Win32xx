/////////////////////////////
// Button.h


#ifndef BUTTON_H
#define BUTTON_H

#include "../Win32++/wincore.h"


class CButton : public CWnd
{
public:
	CButton() {}
	virtual ~CButton() {}

protected:
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};



#endif //BUTTON_H


