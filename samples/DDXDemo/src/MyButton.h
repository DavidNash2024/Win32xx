/////////////////////////////////////////
// MyButton.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MYBUTTON_H
#define MYBUTTON_H


/////////////////////////////////////////////////////////////////////
// The CMyButton class manages the owner-drawn buttons in the dialog.
//
class CMyButton : public CButton
{
public:
    CMyButton() = default;
    virtual ~CMyButton() override = default;

    void DrawItem(LPDRAWITEMSTRUCT);

private:
    CMyButton(const CMyButton&) = delete;
    CMyButton& operator=(const CMyButton&) = delete;
};

#endif  // MYBUTTON_H
