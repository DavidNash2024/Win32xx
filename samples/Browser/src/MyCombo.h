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
    virtual ~CMyCombo() override {}
    const CEdit* GetCBEdit() const { return &m_edit; }

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;

private:
    CMyCombo(const CMyCombo&) = delete;
    CMyCombo& operator=(const CMyCombo&) = delete;

    CEdit m_edit;
};


#endif
