/////////////////////////////
// DarkDialog.h
//

#ifndef _DARK_ABOUT_H_
#define _DARK_ABOUT_H_


///////////////////////////////////////////////////////////
// CDarkAbout provides a dialog compatible with dark mode,
// for the Help About dialog.
// When a dark mode theme is active:
//  - the dialog's background color is set to black.
//  - the colors for the static static control are adjusted.
//  - the OK button is converted to owner drawn.
//  - the colors for the OK button are adjusted.
class CDarkAbout : public CDialog
{
public:
    CDarkAbout();
    virtual ~CDarkAbout();
    void SetDarkMode(bool darkMode);

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDarkAbout(const CDarkAbout&);                // Disable copy construction
    CDarkAbout& operator = (const CDarkAbout&);   // Disable assignment operator

    LRESULT OnDrawItem(WPARAM, LPARAM);
    LRESULT OnCtlColors(UINT, WPARAM, LPARAM);
    void    SetButtonOwnerDraw(bool isOwnerDraw);

    bool m_darkMode;
};


#endif // _DARK_ABOUT_H_
