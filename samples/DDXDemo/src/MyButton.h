/////////////////////////////////////////
// MyButton.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef CMyButton_H
#define CMyButton_H


////////////////////////////////////////////////////////
// The CMyButton class manages the owner-drawn button controls
// on dialog forms.
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

#endif  // CMyButton_H
