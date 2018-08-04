/////////////////////////////
// Button.h


#ifndef BUTTON_H
#define BUTTON_H

#include "wxx_wincore.h"


class CButton : public CWnd
{
public:
    CButton() {}
    virtual ~CButton();
    virtual void PreCreate(CREATESTRUCT &cs);

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

};



#endif //BUTTON_H


