/////////////////////////////
// HelpDialog.h
//

#ifndef _DARK_DIALOG_H_
#define _DARK_DIALOG_H_


///////////////////////////////////////////////////////////
// CDarkDialog provides a dialog compatible with dark mode.
// When a dark mode theme is active:
//  - the dialog's background color is set to black.
//  - the colors for the static static control are adjusted.
//  - the OK button is converted to owner drawn.
//  - the colors for the OK button are adjusted.
class CDarkDialog : public CDialog
{
public:
    CDarkDialog();
    virtual ~CDarkDialog();
    void SetDarkMode(bool darkMode);

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDarkDialog(const CDarkDialog&);                // Disable copy construction
    CDarkDialog& operator = (const CDarkDialog&);   // Disable assignment operator

    LRESULT OnDrawItem(WPARAM, LPARAM);
    LRESULT OnCtlColors(UINT, WPARAM, LPARAM);
    void    SetButtonOwnerDraw(bool isOwnerDraw);

    CBrush m_blackBrush;
    bool m_darkMode;
};


#endif // _DARK_DIALOG_H_
