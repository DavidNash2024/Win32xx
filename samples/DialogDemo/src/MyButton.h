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
    virtual LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif //BUTTON_H


