/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (disable : 4458) // disable warning: declaration hides class member
#endif

// Declare min and max for older versions of Visual Studio
#if defined (_MSC_VER) && (_MSC_VER < 1920) // < VS2019
using std::min;
using std::max;
#endif

#include <gdiplus.h>
#ifdef _MSC_VER


#pragma warning (default : 4458) // return warning to default
#endif

#include <memory>   // Required by VS2013 and VS2015 for unique_ptr
#include "View.h"

using BitmapPtr = std::unique_ptr<Gdiplus::Bitmap>;

///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    CDoc& GetDoc()      { return m_view.GetDoc(); }

    // Virtual functions that override public base class functions
    virtual ~CMainFrame() override;
    virtual HWND Create(HWND parent = nullptr) override;

    void AddMenuIconFromPNG(UINT pngID, UINT disabledPngID, UINT menuID);
    void AddIconFromPNG(CImageList& images, UINT pngID);
    BitmapPtr LoadPngResource(UINT id);

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command Handlers
    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnFilePreview();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    // Member variables
    CView m_view;
    CPrintPreview<CView> m_preview;
    bool m_isToolbarShown;
    ULONG_PTR   m_gdiplusToken;
    CImageList m_toolbarImages;
    CImageList m_disabledImages;
};

#endif //MAINFRM_H

