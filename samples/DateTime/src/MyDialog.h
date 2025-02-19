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
    CMyDialog(UINT resID);
    virtual ~CMyDialog() override = default;

protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy() override;
    virtual BOOL    OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void    OnOK() override;

private:
    CMyDialog(const CMyDialog&) = delete;
    CMyDialog& operator=(const CMyDialog&) = delete;

    // Member variables
    CDateTime       m_dateTime;
    CMonthCalendar  m_monthCal;

};

#endif //MYDIALOG_H
