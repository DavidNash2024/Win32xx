//////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


class CMyCombo : public CComboBoxEx
{
public:
    CMyCombo() {}
    virtual ~CMyCombo() {}
    CEdit* GetEdit() const { return &m_edit; } 

protected:
    virtual void OnAttach()
    {
        m_edit.Attach(GetEditCtrl());
    }
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    mutable CEdit m_edit;
};


#endif
