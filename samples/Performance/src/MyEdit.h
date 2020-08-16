/////////////////////////////
// MyEdit.h
//

#ifndef MYEDIT_H
#define MYEDIT_H

/////////////////////////////////
// Declaration of the CEdit class
//
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
