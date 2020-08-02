////////////////////////////////////////
// MyEdit.h
//  Declaration of the CMyEdit class

#ifndef MYEDIT_H
#define MYEDIT_H


class CMyEdit : public CEdit
{
public:
    CMyEdit();

protected:
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CFont m_font;
};

#endif // MYEDIT_H
