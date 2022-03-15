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
    CTaskDialogApp();
    virtual ~CTaskDialogApp();

protected:
    // Virtual functions that override base class functions 
    virtual BOOL InitInstance();
};


#endif // define TASKDIALOGAPP_H

