/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"


//////////////////////////////////
// CMainFrame function definitions
//

// Constructor.
CMainFrame::CMainFrame() : m_newDialog(IDD_DIALOG2)
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\ToolbarBuilder");

    return CFrame::Create(parent);
}

// Choose the color of the toolbar background.
bool CMainFrame::OnChooseBackground()
{
    if (IDOK == m_colorDialog.DoModal(*this))
    {
        m_view.SetBackground(m_colorDialog.GetColor());
        return true;
    }

    return false;
}

// Choose the color of the transparancy mask.
bool CMainFrame::OnChooseMask()
{
    if (IDOK == m_colorDialog.DoModal(*this))
    {
        m_view.SetMaskColor(m_colorDialog.GetColor());
        return true;
    }

    return false;
}

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Proceed with closing the frame.
    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_NEW_TOOLBAR:   return OnFileNewToolbar();
    case IDM_FILE_LOAD_TOOLBAR:  return OnFileOpen();
    case IDM_FILE_SAVE_TOOLBAR:  return OnFileSave();
    case IDM_FILE_EXIT:          return OnFileExit();
    case IDM_IMAGE_ADD:          return OnImageAdd();
    case IDM_IMAGE_DELETE:       return OnImageDelete();
    case IDM_IMAGE_SWAP:         return OnImageSwap();
    case IDM_VIEW_BACKGROUND:    return OnChooseBackground();
    case IDM_VIEW_CHOOSEMASK:    return OnChooseMask();
    case IDM_VIEW_USEMASK:       return OnViewUseMask();
    case IDW_VIEW_STATUSBAR:     return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:       return OnViewToolBar();
    case IDM_HELP_ABOUT:         return OnHelp();
    }

    return FALSE;
}

// OnCreate controls the way the frame is created.
// Overriding CFrame::OnCreate is optional.
int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
    // A menu is added if the IDW_MAIN menu resource is defined.
    // Frames have all options enabled by default.
    // Use the following functions to disable options.

    // UseIndicatorStatus(FALSE);    // Don't show keyboard indicators in the StatusBar.
    // UseMenuStatus(FALSE);         // Don't show menu descriptions in the StatusBar.
    // UseOwnerDrawnMenu(FALSE);     // Don't use owner draw for popup menu items.
    // UseReBar(FALSE);              // Don't use a ReBar.
    // UseStatusBar(FALSE);          // Don't use a StatusBar.
    // UseThemes(FALSE);             // Don't use themes.
    // UseToolBar(FALSE);            // Don't use a ToolBar.

    // call the base class function
    return CFrame::OnCreate(cs);
}

// Issue a close request to the frame to end the program.
bool CMainFrame::OnFileExit()
{
    Close();
    return true;
}

bool CMainFrame::OnFileNewToolbar()
{
    if (m_newDialog.DoModal(*this) == IDOK)
    {
        int imageSize = m_newDialog.GetImageSize();
        int colorBits = m_newDialog.GetColorBits();
        return m_view.NewToolbar(imageSize, colorBits);
    }

    return false;
}

// Create the File Open dialog to choose the file to load.
bool CMainFrame::OnFileOpen()
{
    CString filter = "Windows Bitmap(*.bmp)|*.bmp|All Files (*.*)|*.*|";
    CFileDialog fileDlg(TRUE);    // TRUE for file open
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".bmp");

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        m_fileName = fileDlg.GetPathName();
        return m_view.LoadToolbar(fileDlg.GetPathName());
    }

    return true;
}

// Create the File Save dialog to choose the file to save.
bool CMainFrame::OnFileSave()
{
    CString filter = "Windows Bitmap(*.bmp) | *.bmp | All Files(*.*) | *.* | ";
    CFileDialog fileDlg(FALSE);    // FALSE for file save
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".bmp");
    fileDlg.SetFileName(m_fileName);

    // Bring up the file save dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        m_view.SaveToolbarImage(fileDlg.GetPathName());
        return true;
    }

    return FALSE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(300, 250);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Call when the Help button or F1 is pressed.
BOOL CMainFrame::OnHelp()
{
    // Ensure only one dialog displayed even for multiple hits of the F1 button.
    if (!m_aboutDialog.IsWindow())
    {
        m_aboutDialog.DoModal(*this);
    }

    return TRUE;
}

// Adds an image or toolbar bitmap to the existing bitmap.
// The image is resized, and the color depth adjusted as required.
// Image types supported include bmp, gif, ico, jpg, png and tif.
bool CMainFrame::OnImageAdd()
{
    CString filter;
    filter += "Common Graphics Files|*.bmp;*.gif;*.ico;*.jpg;*.png;*.tif|";
    filter += "Windows Bitmap(*.bmp)|*.bmp|";
    filter += "Graphics Interchange Format(*.gif)|*.gif|";
    filter += "Windows Icon(*.ico)|*.ico|";
    filter += "Joint Photographic Experts Group(*.jpg)|*.jpg|";
    filter += "Portable Network Graphics(*.png)|*.png|";
    filter += "Tagged Image Format(*.tif)|*.tif|";
    filter += "All Files(*.*)|*.*|";
    CFileDialog fd;
    fd.SetFilter(filter);
    if (IDOK == fd.DoModal())
    {
        return m_view.ImageAdd(fd.GetFileName());
    }

    return false;
}

bool CMainFrame::OnImageDelete()
{
    return m_view.ImageDelete();
}

bool CMainFrame::OnImageSwap()
{
    return m_view.ImageSwap();
}

bool CMainFrame::OnViewUseMask()
{
    return m_view.SetMaskState(!m_view.IsMasked());
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

void CMainFrame::OnMenuUpdate(UINT id)
{
    int pressed = m_view.GetPressedButtons();
    UINT enabled;
    bool canMask = m_view.GetColorBits() <= 24;

    switch (id)
    {
    case IDM_IMAGE_DELETE:
        enabled = pressed > 0 ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;

    case IDM_IMAGE_SWAP:
        enabled = pressed == 2 ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;

    case IDM_VIEW_USEMASK:
        enabled = canMask ? MF_ENABLED : MF_GRAYED;
        GetFrameMenu().EnableMenuItem(id, enabled);
        break;
    }

    // Call base class function.
    CFrame::OnMenuUpdate(id);
}

// Specifies the images for menu item IDs matching the toolbar data.
void CMainFrame::SetupMenuIcons()
{
    std::vector<UINT> data = GetToolBarData();

    if (GetMenuIconHeight() >= 24)
        SetMenuIcons(data, RGB(192, 192, 192), IDB_TOOLBAR, IDB_TOOLBAR_DIS);
    else
        SetMenuIcons(data, RGB(192, 192, 192), IDB_MENUICONS);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    AddToolBarButton(IDM_FILE_NEW_TOOLBAR);
    AddToolBarButton(IDM_FILE_LOAD_TOOLBAR);
    AddToolBarButton(IDM_FILE_SAVE_TOOLBAR);

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_IMAGE_ADD);
    AddToolBarButton(IDM_IMAGE_DELETE, FALSE); // Initially disabled
    AddToolBarButton(IDM_IMAGE_SWAP, FALSE);   // Initially disabled

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_VIEW_USEMASK, FALSE); // Initially disabled

    AddToolBarButton(0);                       // Separator
    AddToolBarButton(IDM_HELP_ABOUT);

    // Set the IDM_VIEW_USEMASK to a toggle button.
    GetToolBar().SetButtonStyle(IDM_VIEW_USEMASK, BTNS_CHECK);

    SetTBImageList(GetToolBar(), IDB_TOOLBAR, RGB(0,0,0));
}

// Called by CFrameApp::OnIdle to update toolbar buttons.
void CMainFrame::UpdateToolbar()
{
    bool canDelete = m_view.GetPressedButtons() > 0;
    bool canMask = m_view.GetColorBits() <= 24;
    bool canSwap = m_view.GetPressedButtons() == 2;
    UINT maskState = m_view.IsMasked() ?
        TBSTATE_CHECKED | TBSTATE_ENABLED : TBSTATE_ENABLED;

    GetToolBar().EnableButton(IDM_IMAGE_DELETE, canDelete);
    GetToolBar().EnableButton(IDM_IMAGE_SWAP, canSwap);

    // Update the pushed state of the mask button
    if (canMask)
        GetToolBar().SetButtonState(IDM_VIEW_USEMASK, maskState);
    else
        GetToolBar().SetButtonState(IDM_VIEW_USEMASK, 0);

    // Redraw the view when the number of pressed buttons press change.
    static int oldButtonCount = 0;
    if (m_view.GetPressedButtons() != oldButtonCount)
    {
        m_view.Invalidate();
        oldButtonCount = m_view.GetPressedButtons();
    }
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, L"Error: std::exception", MB_ICONERROR);
    }

    return 0;
}
