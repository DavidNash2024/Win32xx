/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(int resID);
    virtual ~CMyDialog();

protected:
    // Virtual functions that override base class functions
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMyDialog(const CMyDialog&);                // Disable copy construction
    CMyDialog& operator = (const CMyDialog&);   // Disable assignment operator

    CAnimation m_animation;
};

#endif //MYDIALOG_H
