/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H

//////////////////////////////////////////////////////
// CMyCombo manages the ComboBoxEx control used by the
// main frame to hold the web address.
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo() {}
    virtual ~CMyCombo() {}
    const CEdit* GetCBEdit() const { return &m_edit; }

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CEdit m_edit;
};


#endif
