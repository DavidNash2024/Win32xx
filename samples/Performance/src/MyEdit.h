/////////////////////////////
// MyEdit.h
//

#ifndef MYEDIT_H
#define MYEDIT_H


///////////////////////////////////////////////////////
// CMyEdit manages an edit control. The edit control is
// a child window of the main window.
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
