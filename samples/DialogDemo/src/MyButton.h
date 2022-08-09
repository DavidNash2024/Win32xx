/////////////////////////////
// MyButton.h
//

#ifndef MYBUTTON_H
#define MYBUTTON_H


///////////////////////////////////////
// CMyButton manages the button control.
class CMyButton : public CButton
{
public:
    CMyButton() {}
    virtual ~CMyButton() {}

protected:
    // Virtual functions that override base class functions
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyButton(const CMyButton&);                // Disable copy construction
    CMyButton& operator = (const CMyButton&);   // Disable assignment operator

    // Message handlers
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif //BUTTON_H

