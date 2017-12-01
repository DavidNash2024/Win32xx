//////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo() {}
    virtual ~CMyCombo() {}
    CEdit* GetEdit() const { return &m_Edit; } 

protected:
    virtual void OnAttach()
    {
        m_Edit.Attach(GetEditCtrl());
    }
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    mutable CEdit m_Edit;
};


#endif
