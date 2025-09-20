/////////////////////////////
// Edit.h
//

#ifndef _EDIT_H_
#define _EDIT_H_


////////////////////////////////////////////
// CMyEdit manages and edit control used as
// CMainFrame's view window.
class CMyEdit : public CEdit
{
public:
    CMyEdit() = default;
    virtual ~CMyEdit() = default;

protected:
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyEdit(const CMyEdit&) = delete;
    CMyEdit& operator=(const CMyEdit&) = delete;

    // Message handlers.
    LRESULT OnKillFocus();
    LRESULT OnChar(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // _EDIT_H_
