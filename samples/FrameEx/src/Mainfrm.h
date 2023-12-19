/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H


#if defined (_MSC_VER) && (_MSC_VER == 1900) // == VS2015
#pragma warning (disable : 4458) // disable warning: declaration hides class member
#endif
#include <gdiplus.h>
#ifdef _MSC_VER
#pragma warning (default : 4458) // return warning to default
#endif

#include <memory>   // Required by VS2013 and VS2015 for unique_ptr
#include "View.h"

typedef std::unique_ptr<Gdiplus::Bitmap> BitmapPtr;

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
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

    void AddMenuIconFromPNG(UINT pngID, UINT disabledPngID, UINT menuID);
    void AddIconFromPNG(CImageList& images, UINT pngID);
    BitmapPtr LoadPngResource(UINT id);

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void    PreCreate(CREATESTRUCT& cs);
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);               // Disable copy construction
    CMainFrame& operator=(const CMainFrame&);    // Disable assignment operator

    // Command Handlers
    BOOL    OnFileExit();
    BOOL    OnFileOpen();
    BOOL    OnFilePrint();
    BOOL    OnFileSave();
    BOOL    OnFilePreview();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT  OnPreviewClose();
    LRESULT  OnPreviewPrint();
    LRESULT  OnPreviewSetup();

    // Member variables
    CView m_view;
    CPrintPreview<CView> m_preview;
    bool m_isToolbarShown;
    ULONG_PTR   m_gdiplusToken;
    CImageList m_toolbarImages;
    CImageList m_disabledImages;
    CImageList m_menuImages;
};

#endif //MAINFRM_H

