/////////////////////////////
// PrintParent.h
//

#ifndef _PRINTPARENT_H_
#define _PRINTPARENT_H_

// A windows message indicating the print settings have changed.
#define UWM_SETTINGCHANGE   (WM_APP + 0x0001)

///////////////////////////////////////////////////////////////////////
// CPrintParent manages the parent window of the CPrintDialogEx dialog.
// CPrintParent is the "General" dialog sheet, which holds all
// the button and edit controls, excluding the "Print", "Cancel"
// and "Apply" buttons.
//
// This window receives a WM_ACTIVATE message after the settings
// dialog is completed, so we then send a UWN_SETTINGCHANGE message
// to the rich view window.
//
// CRichView updates the PrintDlgEx's maximum page parameter when it
// receives the UWN_SETTINGCHANGE message.
//
class CPrintParent : public CWnd
{
public:
    CPrintParent() : m_owner(0) {}
    virtual ~CPrintParent() {}
    void SetOwner(HWND owner) { m_owner = owner; }

    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam);

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CPrintParent(const CPrintParent&);                // Disable copy construction
    CPrintParent& operator = (const CPrintParent&);   // Disable assignment operator

    HWND m_owner;
};


#endif // _PRINTPARENT_H_
