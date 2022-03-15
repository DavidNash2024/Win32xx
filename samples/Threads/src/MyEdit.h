/////////////////////////////
// MyEdit.h
//

#ifndef MYEDIT_H
#define MYEDIT_H


/////////////////////////////////////////////////////
// CMyEdit manages an edit control. It is used as the
// child window of the main window.
class CMyEdit : public CEdit
{
public:
    CMyEdit();
    virtual ~CMyEdit() {}

protected:
    // Virtual functions that override base class functions 
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CFont m_font;
};


#endif // MYEDIT_H
