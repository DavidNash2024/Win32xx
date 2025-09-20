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
    CMyButton() = default;
    virtual ~CMyButton() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyButton(const CMyButton&) = delete;
    CMyButton& operator=(const CMyButton&) = delete;

    // Message handlers
    LRESULT OnMouseMove(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNCHitTest(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnLButtonUp(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif //BUTTON_H

