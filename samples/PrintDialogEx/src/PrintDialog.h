/////////////////////////////
// PrintDialog.h
//

#ifndef _PRINTDIALOG_H_
#define _PRINTDIALOG_H_

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

public:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
    {
        // This message is sent when the preference dialog is closed,
        // and this window is activated.
        if (msg == WM_ACTIVATE)
        {
            if (LOWORD(wparam) != WA_INACTIVE)
            {
                // Post a settings change message to the RichView.
                ::PostMessage(m_owner, UWM_SETTINGCHANGE, 0, 0);
            }
        }

        // Sent when a different property page is selected.
        // Required for Windows 2000.
        if (msg == WM_WINDOWPOSCHANGED)
        {
            // Post a settings change message to the RichView.
            ::PostMessage(m_owner, UWM_SETTINGCHANGE, 0, 0);
        }
        return WndProcDefault(msg, wparam, lparam);
    }

private:
    HWND m_owner;
};


//////////////////////////////////////////////
// CMyPrintDialog inherits from CPrintDialogEx
//
class CMyPrintDialogEx : public CPrintDialogEx
{
public:
    CMyPrintDialogEx()  {}
    virtual ~CMyPrintDialogEx() {}
    CPrintParent* GetPrintParent() { return &m_printParent; }

protected:
    // Called when the the dialog initializes.
    virtual BOOL OnInitDialog()
    {
        // Setup the CPrintParent object.
        m_printParent.Attach(GetParent());
        m_printParent.SetOwner(GetParameters().hwndOwner);

        return TRUE;
    }

private:
    CPrintParent m_printParent;
};

#endif // _PRINTDIALOG_H_