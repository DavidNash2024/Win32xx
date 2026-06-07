/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "aboutdialog.h"
#include "richview.h"

//////////////////////////////////////
// Declaration of the CMainFrame class
//
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    void CheckForEndSpace(const CString& folder);
    void CheckForNonAscii(const CString& folder);
    void CheckForTabs(const CString& folder);

    CString GetStartFolder();
    bool HasEndSpace(const CString& input);
    bool HasNonAscii(const CString& fileName);
    bool HasTabs(const CString& input);
    bool IsSourceFile(const CString& fileName) const;
    bool PickFolder(const CString& startFolder);
    void ReadFile(const CString& fileName);
    void WriteFile(const CString& fileName);

    BOOL    OnCheckFiles();
    BOOL    OnChooseFolder();
    BOOL    OnFileExit();
    BOOL    OnFileNew();
    BOOL    OnFilePrint();
    BOOL    OnFilePreview();
    BOOL    OnFixFiles();
    BOOL    OnPreviewClose();
    BOOL    OnPreviewPrint();
    BOOL    OnPreviewSetup();

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CAboutDialog m_aboutDialog;
    CRichView m_view;
    CPrintPreview<CRichView> m_preview;
    std::vector<CStringA> m_fileContents;
    std::vector<CString> m_filesWithTabs;
    std::vector<CString> m_filesWithEndSpace;
    std::vector<CString> m_filesWithNonAscii;
    CString m_folder;
    bool m_isToolbarShown;
};

#endif //MAINFRM_H

