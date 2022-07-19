/////////////////////////////
// Edit.h
//

#ifndef _EDIT_H_
#define _EDIT_H_


//////////////////////////////////////////
// CView manages CMainFrame's view window.
class CMyEdit : public CEdit
{
public:
    CMyEdit();
    virtual ~CMyEdit();

protected:
    // Virtual functions that override base class functions.
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers.
    LRESULT OnKillFocus();
    LRESULT OnChar(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif // _EDIT_H_
