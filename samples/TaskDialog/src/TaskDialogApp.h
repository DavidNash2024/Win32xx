/////////////////////////////
// TaskDialogApp.h
//

#ifndef TASKDIALOGAPP_H
#define TASKDIALOGAPP_H


////////////////////////////////////////////////////////////////
// CFrameApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the task dialog
// when it runs.
class CTaskDialogApp : public CWinApp
{
public:
    CTaskDialogApp() = default;
    virtual ~CTaskDialogApp() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance() override;

private:
    CTaskDialogApp(const CTaskDialogApp&) = delete;
    CTaskDialogApp& operator=(const CTaskDialogApp&) = delete;
};


#endif // define TASKDIALOGAPP_H

