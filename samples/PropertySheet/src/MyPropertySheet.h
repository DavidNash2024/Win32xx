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
    CMyPropertySheet(LPCWSTR caption = nullptr, HWND hParent = nullptr);
    virtual ~CMyPropertySheet() override {}

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnSetDefID(WPARAM wparam) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyPropertySheet(const CMyPropertySheet&) = delete;
    CMyPropertySheet& operator=(const CMyPropertySheet&) = delete;
};


/////////////////////////////////////////////////////////
// CButtonPage manages a property page that displays a
// number of buttons. A property page behaves much like a
// dialog.
class CButtonPage : public CPropertyPage
{
public:
    CButtonPage(UINT templateID, LPCWSTR title = nullptr);
    virtual ~CButtonPage() override {}

    BOOL Validate();

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnApply() override;
    virtual void OnCancel() override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;
    virtual BOOL OnQueryCancel() override;
    virtual BOOL OnSetActive() override;

private:
    CButtonPage(const CButtonPage&) = delete;
    CButtonPage& operator=(const CButtonPage&) = delete;
};


//////////////////////////////////////////////////////
// CComboPage manages a property page that displays a
// number of combo boxes. A property page behaves much
// like a dialog.
class CComboPage : public CPropertyPage
{
public:
    CComboPage(UINT templateID, LPCWSTR title  = nullptr);
    virtual ~CComboPage() override {}

protected:
    // Virtual functions that override base class functions.
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnApply() override;
    virtual BOOL OnInitDialog() override;
    virtual BOOL OnSetActive() override;

private:
    CComboPage(const CComboPage&) = delete;
    CComboPage& operator=(const CComboPage&) = delete;
};


#endif // MYPROPERTYSHEET_H
