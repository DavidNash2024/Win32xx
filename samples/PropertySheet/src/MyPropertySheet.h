/////////////////////////////
// MyPropertySheet.h
//

#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H

////////////////////////////////////////////
// Declaration of the CMyPropertySheet class
// A property sheet has several property pages
//
class CMyPropertySheet : public CPropertySheet
{
public:
    CMyPropertySheet(LPCTSTR caption = NULL, HWND hParent = NULL);
    virtual void OnClose() { TRACE("OnClose\n"); CPropertySheet::OnClose(); }
    virtual void OnDestroy() { TRACE("OnDestroy\n"); }
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};

///////////////////////////////////////
// Declaration of the CButtonPage class
// The property sheet for the button page
//
class CButtonPage : public CPropertyPage
{
public:
    CButtonPage(UINT templateID, LPCTSTR title = NULL);
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnApply();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual BOOL OnQueryCancel();
    virtual BOOL OnSetActive();
    virtual BOOL Validate();
};

///////////////////////////////////////
// Declaration of the CComboPage class
// The property sheet for the combo page
//
class CComboPage : public CPropertyPage
{
public:
    CComboPage(UINT templateID, LPCTSTR title  = NULL);
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
    virtual BOOL OnApply()
    {
        SetModified(TRUE);
        return TRUE;
    }
};


#endif // MYPROPERTYSHEET_H

