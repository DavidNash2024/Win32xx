/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"
#include "Mainfrm.h"
#include "resource.h"

using namespace Gdiplus;


///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor.
CMainFrame::CMainFrame() : m_preview(m_view), m_isToolbarShown(true)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
}

// Destructor.
CMainFrame::~CMainFrame()
{
    // Shutdown GDI+
    GdiplusShutdown(m_gdiplusToken);
}

// Adds normal and disabled icons to the dropdown menu.
void CMainFrame::AddMenuIconFromPNG(UINT pngID, UINT disabledPngID, UINT menuID)
{
    BitmapPtr bitmap = LoadPngResource(pngID);
    if (bitmap.get() != nullptr)
    {
        BitmapPtr disabledBitmap = LoadPngResource(disabledPngID);
        if (disabledBitmap.get() != nullptr)
        {
            HICON icon;
            HICON disabledIcon;
            bitmap->GetHICON(&icon);
            disabledBitmap->GetHICON(&disabledIcon);
            AddMenuIcon(menuID, icon, disabledIcon);
        }
    }
}

// Adds an icon from the PNG resource to the specified imagelist.
void CMainFrame::AddIconFromPNG(CImageList& images, UINT pngID)
{
    HICON icon;
    BitmapPtr bitmap = LoadPngResource(pngID);
    if (bitmap.get() != nullptr)
    {
        bitmap->GetHICON(&icon);
        images.Add(icon);
    }
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\FrameEx");

    return CFrame::Create(parent);
}

// Loads an image from a PNG resource and returns a unique_ptr to
// the Gdiplus::Bitmap containing the image.
// The returned unique_ptr contains nullptr if the resouce isn't found.
BitmapPtr CMainFrame::LoadPngResource(UINT id)
{
    BitmapPtr bitmap;
    HINSTANCE instance = GetApp()->GetResourceHandle();

    HRSRC resourceInfo = ::FindResource(instance, MAKEINTRESOURCE(id), L"PNG");
    if (resourceInfo != nullptr)
    {
        DWORD bufferSize = ::SizeofResource(instance, resourceInfo);
        if (bufferSize != 0)
        {
            HGLOBAL resource = ::LoadResource(instance, resourceInfo);
            if (resource != nullptr)
            {
                const void* resourceData = ::LockResource(resource);
                if (resourceData != nullptr)
                {
                    CHGlobal globalMemory(bufferSize);
                    if (globalMemory.Get() != nullptr)
                    {
                        CGlobalLock<CHGlobal> buffer(globalMemory);
                        if (buffer != nullptr)
                        {
                            memcpy(buffer, resourceData, bufferSize);

                            IStream* stream = nullptr;;
                            if (::CreateStreamOnHGlobal(buffer, FALSE, &stream) == S_OK)
                            {
                                bitmap.reset(Gdiplus::Bitmap::FromStream(stream));
                                stream->Release();
                            }
                        }
                    }
                }
            }
        }
    }

    return bitmap;
}

// Called when the frame window is asked to close.
void CMainFrame::OnClose()
{
    // Close the preview.
    if (GetView() == m_preview)
        OnPreviewClose();

    // Proceed with closing the frame.
    CFrame::OnClose();
}

// OnCommand responds to menu and and toolbar input.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSave();
    case IDM_FILE_PREVIEW:    return OnFilePreview();
    case IDM_FILE_PRINT:      return OnFilePrint();
    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();
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

    // Call the base class function.
    return CFrame::OnCreate(cs);
}

// Issue a close request to the frame to end the program.
BOOL CMainFrame::OnFileExit()
{
    Close();
    return TRUE;
}

// Limit the minimum size of the window.
LRESULT CMainFrame::OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam)
{
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lparam;
    const CSize minimumSize(400, 300);
    lpMMI->ptMinTrackSize.x = DpiScaleInt(minimumSize.cx);
    lpMMI->ptMinTrackSize.y = DpiScaleInt(minimumSize.cy);
    return FinalWindowProc(msg, wparam, lparam);
}

// Called after the window is created.
void CMainFrame::OnInitialUpdate()
{
    // The frame is now created.
    // Place any additional startup code here.

    TRACE("Frame created\n");
}

// Create the File Open dialog to choose the file to load.
BOOL CMainFrame::OnFileOpen()
{
    CString filter = "Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*|";
    CFileDialog fileDlg(TRUE);    // TRUE for file open
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".cpp");

    // Bring up the file open dialog retrieve the selected filename.
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileLoad(fileDlg.GetPathName());
    }

    return TRUE;
}

// Create the File Save dialog to choose the file to save.
BOOL CMainFrame::OnFileSave()
{
    CString filter = "Program Files (*.cpp; *.h)|*.cpp; *.h|All Files (*.*)|*.*|";
    CFileDialog fileDlg(FALSE);    // FALSE for file save
    fileDlg.SetFilter(filter);
    fileDlg.SetDefExt(L".cpp");

    // Bring up the file save dialog retrieve the selected filename.
    if (fileDlg.DoModal(*this) == IDOK)
    {
        GetDoc().FileStore(fileDlg.GetPathName());
    }

    return TRUE;
}

// Previews a print job before sending it to the printer.
BOOL CMainFrame::OnFilePreview()
{
    try
    {
        m_isToolbarShown = GetToolBar().IsWindow() && GetToolBar().IsWindowVisible();

        // Get the device context of the default or currently chosen printer.
        CPrintDialog printDlg;
        CDC printerDC = printDlg.GetPrinterDC();

        // Create the preview window if required.
        if (!m_preview.IsWindow())
            m_preview.Create(*this);

        // Set the preview's owner for notification messages.
        m_preview.DoPrintPreview(*this);

        // Swap views
        SetView(m_preview);

        // Hide the menu and toolbar.
        ShowMenu(FALSE);
        ShowToolBar(FALSE);

        // Update status.
        CString status = L"Printer: " + printDlg.GetDeviceName();
        SetStatusText(status);
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Preview Failed", MB_ICONWARNING);
        SetView(m_view);
        ShowMenu(GetFrameMenu() != nullptr);
        ShowToolBar(m_isToolbarShown);
    }

    return TRUE;
}

// Bring up a dialog to choose the printer.
BOOL CMainFrame::OnFilePrint()
{
    CPrintDialog printdlg;

    try
    {
        if (IDOK == printdlg.DoModal(*this))
        {
            m_view.QuickPrint(L"Frame Sample");
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetText(), L"Print Failed", MB_ICONWARNING);
    }

    return TRUE;
}

// Process notification messages (WM_NOTIFY) sent by child windows
LRESULT CMainFrame::OnNotify(WPARAM wparam, LPARAM lparam)
{
//  LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
//  switch (pHeader->code)
//  {
//      Add case statements for each notification message here
//  }

    // Some notifications should return a value when handled
    return CFrame::OnNotify(wparam, lparam);
}

// Called when the Print Preview's "Close" button is pressed.
LRESULT CMainFrame::OnPreviewClose()
{
    // Swap the view.
    SetView(m_view);

    // Show the menu and toolbar.
    ShowMenu(GetFrameMenu() != nullptr);
    ShowToolBar(m_isToolbarShown);
    UpdateSettings();

    SetStatusText(LoadString(IDW_READY));

    return 0;
}

// Called when the Print Preview's "Print Now" button is pressed.
LRESULT CMainFrame::OnPreviewPrint()
{
    m_view.QuickPrint(L"Frame Sample");
    return 0;
}

// Called when the Print Preview's "Print Setup" button is pressed.
LRESULT CMainFrame::OnPreviewSetup()
{
    // Call the print setup dialog.
    CPrintDialog printDlg(PD_PRINTSETUP);
    try
    {
        // Display the print dialog.
        if (printDlg.DoModal(*this) == IDOK)
        {
            CString status = L"Printer: " + printDlg.GetDeviceName();
            SetStatusText(status);
        }
    }

    catch (const CException& e)
    {
        // An exception occurred. Display the relevant information.
        MessageBox(e.GetErrorString(), e.GetText(), MB_ICONWARNING);
    }

    // Initiate the print preview.
    m_preview.DoPrintPreview(*this);

    return 0;
}

// This function is called before the frame is created.
// It provides an opportunity to modify the various CREATESTRUCT
// parameters used before the frame window is created.
void CMainFrame::PreCreate(CREATESTRUCT& cs)
{
    // Call base clase to set defaults.
    CFrame::PreCreate(cs);

    // cs.dwExStyle = WS_EX_LAYOUTRTL;  // Set Right-To-Left Window Layout
    // cs.style &= ~WS_VISIBLE;         // Remove the WS_VISIBLE style. The frame will be initially hidden.
}

// Specifies the images for some of the menu items.
void CMainFrame::SetupMenuIcons()
{
    AddMenuIconFromPNG(IDP_FILE_NEW,     IDP_FILE_NEWDISABLED,     IDM_FILE_NEW);
    AddMenuIconFromPNG(IDP_FILE_OPEN,    IDP_FILE_OPENDISABLED,    IDM_FILE_OPEN);
    AddMenuIconFromPNG(IDP_FILE_SAVE,    IDP_FILE_SAVEDISABLED,    IDM_FILE_SAVE);
    AddMenuIconFromPNG(IDP_EDIT_CUT,     IDP_EDIT_CUTDISABLED,     IDM_EDIT_CUT);
    AddMenuIconFromPNG(IDP_EDIT_COPY,    IDP_EDIT_COPYDISABLED,    IDM_EDIT_COPY);
    AddMenuIconFromPNG(IDP_EDIT_PASTE,   IDP_EDIT_PASTEDISABLED,   IDM_EDIT_PASTE);
    AddMenuIconFromPNG(IDP_FILE_PREVIEW, IDP_FILE_PREVIEWDISABLED, IDM_FILE_PREVIEW);
    AddMenuIconFromPNG(IDP_FILE_PRINT,   IDP_FILE_PRINTDISABLED,   IDM_FILE_PRINT);
    AddMenuIconFromPNG(IDP_HELP_ABOUT,   IDP_HELP_ABOUTDISABLED,   IDM_HELP_ABOUT);
}

// Set the resource IDs and images for the toolbar buttons.
void CMainFrame::SetupToolBar()
{
    // Assign resource IDs to the toolbar.
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );  // Disabled button
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );  // Disabled button
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );  // Disabled button

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_FILE_PRINT );

    AddToolBarButton( 0 );                      // Separator
    AddToolBarButton( IDM_HELP_ABOUT );

    // Assign normal images to the toolbar.
    int iconSize = DpiScaleInt(28);
    m_toolbarImages.Create(iconSize, iconSize, ILC_COLOR32, 8, 1);
    AddIconFromPNG(m_toolbarImages, IDP_FILE_NEW);
    AddIconFromPNG(m_toolbarImages, IDP_FILE_OPEN);
    AddIconFromPNG(m_toolbarImages, IDP_FILE_SAVE);
    AddIconFromPNG(m_toolbarImages, IDP_EDIT_CUT);
    AddIconFromPNG(m_toolbarImages, IDP_EDIT_COPY);
    AddIconFromPNG(m_toolbarImages, IDP_EDIT_PASTE);
    AddIconFromPNG(m_toolbarImages, IDP_FILE_PRINT);
    AddIconFromPNG(m_toolbarImages, IDP_HELP_ABOUT);
    GetToolBar().SetImageList(m_toolbarImages);

    // Assign disabled images to the toolbar.
    m_disabledImages.Create(iconSize, iconSize, ILC_COLOR32, 8, 1);
    AddIconFromPNG(m_disabledImages, IDP_FILE_NEWDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_FILE_OPENDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_FILE_SAVEDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_EDIT_CUTDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_EDIT_COPYDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_EDIT_PASTEDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_FILE_PRINTDISABLED);
    AddIconFromPNG(m_disabledImages, IDP_HELP_ABOUTDISABLED);
    GetToolBar().SetDisableImageList(m_disabledImages);
}

// Process the frame's window messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_GETMINMAXINFO:    return OnGetMinMaxInfo(msg, wparam, lparam);
        case UWM_PREVIEWCLOSE:    return OnPreviewClose();
        case UWM_PREVIEWPRINT:    return OnPreviewPrint();
        case UWM_PREVIEWSETUP:    return OnPreviewSetup();
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

