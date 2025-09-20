/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MainView.h"
#include "ImageView.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    void DpiScaleToolBar();
    const CBitmap& GetImage() const { return m_mainView.GetImage(); }
    CImageView& GetImageView() { return m_mainView.m_imageView; }

    bool IsImageLoaded() const { return GetImage().GetHandle() != nullptr; }
    void SaveFile(CString& fileName);
    void UpdateToolbar() const;

protected:
    virtual void OnClose() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void OnInitialUpdate() override;
    virtual void OnMenuUpdate(UINT id) override;
    virtual LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnAdjustImage();
    BOOL OnFileExit();
    BOOL OnFileSave();
    BOOL OnFileSaveAs();
    LRESULT OnImageLoaded(LPCWSTR fileName);
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFileOpenMRU(WPARAM wparam, LPARAM lparam);
    BOOL OnFilePreview();
    BOOL OnFilePrint();

    // Message handlers
    LRESULT OnPreviewClose();
    LRESULT OnPreviewPrint();
    LRESULT OnPreviewSetup();

    void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL isGray);

    // Member variables
    CMainView m_mainView;
    CPrintPreview<CImageView> m_preview;   // CImageView is the source of the PrintPage function
    CString m_pathName;
    bool m_isToolbarShown;
};

#endif //MAINFRM_H

