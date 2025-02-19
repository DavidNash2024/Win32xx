/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CMainFrame class declaration. This class
    controls the appearance and frame-related actions of the DDXDemo program.
    This class presents a fixed-size frame whose status bar has no resizing
    gripper in the lower-right-hand corner.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    Declaration of the CMainFrame class of the App-Frame-Doc-View architecture.
*----------------------------------------------------------------------------*/
{
    public:
        CMainFrame();
        virtual ~CMainFrame() override = default;
        virtual HWND Create(HWND parent = nullptr) override;

        CDoc&   TheDoc()   { return m_view.TheDoc(); }
        BOOL    UpdateDialog(BOOL bReadFromControl);

   protected:
       virtual BOOL    LoadRegistrySettings(LPCWSTR keyName) override;
       virtual void    OnClose() override;
       virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
       virtual int     OnCreate(CREATESTRUCT& cs) override;
       virtual void    OnInitialUpdate() override;
       virtual void    OnMenuUpdate(UINT id) override;
       virtual void    SetupMenuIcons() override;
       virtual void    SetupToolBar() override;
       virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

    private:
        CMainFrame(const CMainFrame&) = delete;
        CMainFrame& operator=(const CMainFrame&) = delete;

        BOOL    OnCheckButton(UINT id);
        BOOL    OnEditCopy();
        BOOL    OnEditCut();
        BOOL    OnEditDelete();
        BOOL    OnEditPaste();
        BOOL    OnEditRedo();
        BOOL    OnEditUndo();
        BOOL    OnFileExit();
        BOOL    OnRadioButton(UINT id);

        CView   m_view;
};
/*----------------------------------------------------------------------------*/
#endif //MAINFRM_H

