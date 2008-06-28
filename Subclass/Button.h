/////////////////////////////
// Button.h


#ifndef BUTTON_H
#define BUTTON_H

#include "../WinDev++/WinCore.h"


class CButton : public CWnd
{
public:
	CButton() {}
	virtual ~CButton();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);

protected:
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

};



#endif //BUTTON_H


