///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#define ID_TIMER 101

// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT nResID);
    virtual ~CMyDialog();

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
    virtual void OnOK();

private:

    CDateTime       m_DateTime;
    CMonthCalendar  m_MonthCal;

};

#endif //MYDIALOG_H
