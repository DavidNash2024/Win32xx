/////////////////////////////
// MyTaskDialog.h
//

#ifndef _MY_TASKDIALOG_H_
#define _MY_TASKDIALOG_H_

#include "IFileDialog.h"


/////////////////////////////////////////////
// CMyTaskDialog manages a task dialog. It is
// used as the application's main window.
class CMyTaskDialog : public CTaskDialog
{
public:
    CMyTaskDialog();
    virtual ~CMyTaskDialog() override = default;

protected:
    virtual BOOL OnTDButtonClicked(int buttonID) override;
    virtual LRESULT TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyTaskDialog(const CMyTaskDialog&) = delete;
    CMyTaskDialog& operator=(const CMyTaskDialog&) = delete;
};


#endif // _MY_TASKDIALOG_H_
