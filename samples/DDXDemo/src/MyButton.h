/////////////////////////////////////////
// MyButton.h
// Authors: Robert Tausworthe, David Nash
//

#pragma once

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

