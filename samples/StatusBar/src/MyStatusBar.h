/////////////////////////////
// MyStatusBar.h
//

#ifndef MYSTATUSBAR_H
#define MYSTATUSBAR_H

#include "Hyperlink.h"


/////////////////////////////////////////
// Declaration of the CMyStatusBar class.
//
class CMyStatusBar : public CStatusBar
{
public:
    CMyStatusBar();
    virtual ~CMyStatusBar();
    void    SetStatusIndicators();

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual BOOL    OnEraseBkgnd(CDC& dc);
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnDrawItem(UINT, WPARAM, LPARAM lparam);
    void    OnSize();
    void    OnTimer();

    int     AdjustForDPI(int value);
    int     GetTextPartWidth(LPCTSTR text) const;
    void    SetupParts();

    // Member variables
    CHyperlink m_hyperlink;
    CProgressBar m_progressBar;
    CString m_ownerDraw;
};


#endif      // MYSTATUSBAR_H
