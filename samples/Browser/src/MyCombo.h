//////////////////////////////////////////////
// MyCombo.h

#ifndef MYCOMBO_H
#define MYCOMBO_H


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
    CEdit m_edit;
};


#endif
