/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "Doc.h"
#include "AppHelp.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    void    ChooseHelpTopic();
    CString CreateAppDataFolder(const CString& subfolder);
    CDoc&   GetDoc()    { return m_doc; }
    UINT    GetIDFromCursorPos();
    void    OnF1();
    void    OnFileExit();
    void    OnShiftF1();
    void    OnUpdateCheckA(UINT id);
    void    OnUpdateCheckB(UINT id);
    void    OnUpdateCheckC(UINT id);
    void    OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);
    void    ShowHelpTopic(UINT id);

protected:
    virtual BOOL    LoadRegistrySettings(LPCTSTR keyName);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT id);
    virtual LRESULT OnLButtonDown(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSetCursor(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnSysCommand(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual BOOL    SaveRegistrySettings();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void    NotImplemented() const;

    CView       m_view;
    CDoc        m_doc;
    CAppHelp    m_appHelp;  // Help object for context help and help about dialog
    BOOL        m_isChoosing;
};


#endif //MAINFRM_H

