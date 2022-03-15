/////////////////////////////
// MyPropertySheet.h
//

#ifndef MYPROPERTYSHEET_H
#define MYPROPERTYSHEET_H


///////////////////////////////////////////////
// CMyPropertySheet manages the property sheet.
// A property sheet has several property pages.
class CMyPropertySheet : public CPropertySheet
{
public:
    CMyPropertySheet(LPCTSTR caption = NULL, HWND hParent = 0);
    virtual ~CMyPropertySheet() {}

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void OnInitialUpdate();
    virtual LRESULT OnSetDefID(WPARAM wparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


/////////////////////////////////////////////////////////
// CButtonPage manages a property page which displays a
// number of buttons. A property page behaves much like a
// dialog.
class CButtonPage : public CPropertyPage
{
public:
    CButtonPage(UINT templateID, LPCTSTR title = NULL);
    virtual ~CButtonPage() {}

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnApply();
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual BOOL OnQueryCancel();
    virtual BOOL OnSetActive();
    virtual BOOL Validate();
};


//////////////////////////////////////////////////////
// CComboPage manages a property page which displays a
// number of combo boxes. A property page behaves much
// like a dialog.
class CComboPage : public CPropertyPage
{
public:
    CComboPage(UINT templateID, LPCTSTR title  = NULL);
    virtual ~CComboPage() {}

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnApply();
    virtual BOOL OnInitDialog();
    virtual BOOL OnSetActive();
};


#endif // MYPROPERTYSHEET_H
