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
    void DPISetFont();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyEdit(const CMyEdit&);               // Disable copy construction
    CMyEdit& operator=(const CMyEdit&);    // Disable assignment operator

    CFont m_font;
};


#endif // MYEDIT_H
