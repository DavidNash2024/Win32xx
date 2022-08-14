/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H

#define ID_TIMER 101


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(int resID);
    virtual ~CMyDialog();

protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy();
    virtual BOOL    OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    OnOK();

private:
    CMyDialog(const CMyDialog&);                // Disable copy construction
    CMyDialog& operator = (const CMyDialog&);   // Disable assignment operator

    // Member variables
    CDateTime       m_dateTime;
    CMonthCalendar  m_monthCal;

};

#endif //MYDIALOG_H
