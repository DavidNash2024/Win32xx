/* (22-Oct-2024) [Tab/Indent: 4/4][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CMainFrame class for this sample
    program using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H

/******************************************************************************/


/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    This class administers the user interaction with the file to be displayed
    and the view class that displays it.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame(void);
        virtual     ~CMainFrame() override = default;
        virtual HWND Create(HWND parent = nullptr) override;

        CDoc&       TheDoc()      {return m_view.TheDoc();}
        AboutBox&   GetAboutBox()   { return m_aboutDialog; }

    protected:
        virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
        virtual int  OnCreate(CREATESTRUCT& cs) override;
        virtual BOOL OnHelp() override;
        virtual void OnInitialUpdate() override;
        virtual void SetupMenuIcons() override;
        virtual void SetupToolBar() override;

    private:
        CMainFrame(const CMainFrame&) = delete;
        CMainFrame& operator=(const CMainFrame&) = delete;

        BOOL OnColorChoice();
        BOOL OnFileClose();
        BOOL OnFileExit();
        BOOL OnFileOpenMRU(WPARAM wparam);
        BOOL OnFileOpen();
        BOOL OnFontChoice() { m_view.ClientFontChoice(); return TRUE; }
        BOOL OnLineNumbering();

          // private data members
         CView      m_view;           // the view
         AboutBox   m_aboutDialog;    // about dialog handled here
};
/*------------------------------------------------------------------------------*/
#endif // SDI_MAINFRM_H

