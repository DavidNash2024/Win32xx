/////////////////////////////
// MyCombo.h
//

#ifndef MYCOMBO_H
#define MYCOMBO_H


////////////////////////////////////////////////////////////////////
// CMyCombo manages the ComboBoxEx control used by the main frame to
// hold the web address.
class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo() {}
    virtual ~CMyCombo() {}
    const CEdit* GetCBEdit() const { return &m_edit; }

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CMyCombo(const CMyCombo&);                // Disable copy construction
    CMyCombo& operator = (const CMyCombo&);   // Disable assignment operator

    CEdit m_edit;
};


#endif
