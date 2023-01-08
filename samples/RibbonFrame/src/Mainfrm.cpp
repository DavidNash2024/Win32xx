/////////////////////////////
// Mainfrm.cpp
//

#include "stdafx.h"

#include <UIRibbonPropertyHelpers.h>
#include "RibbonUI.h"
#include "Mainfrm.h"
#include "Resource.h"

///////////////////////////////////
// CMainFrame function definitions.
//

// Constructor for CMainFrame.
CMainFrame::CMainFrame() : m_pIUIRibbon(0)
{
}

// Destructor for CMainFrame.
CMainFrame::~CMainFrame()
{
}

// Create the frame window.
HWND CMainFrame::Create(HWND parent)
{
    // Set m_view as the view window of the frame.
    SetView(m_view);

    // Set the registry key name, and load the initial window position.
    // Use a registry key name like "CompanyName\\Application".
    LoadRegistrySettings(L"Win32++\\Ribbon Frame");

    // Load the settings from the registry with 4 MRU entries.
    LoadRegistryMRUSettings(4);

    return CRibbonFrame::Create(parent);
}

// This function is called when a ribbon button is pressed.
// Refer to IUICommandHandler::Execute in the Windows 7 SDK documentation.
STDMETHODIMP CMainFrame::Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp)
{
    if (UI_EXECUTIONVERB_EXECUTE == verb)
    {
        switch(cmdID)
        {
        case IDC_CMD_NEW:       OnFileNew();        break;
        case IDC_CMD_OPEN:      OnFileOpen();       break;
        case IDC_CMD_SAVE:      OnFileSave();       break;
        case IDC_CMD_SAVE_AS:   OnFileSaveAs();     break;
        case IDC_CMD_PRINT:     OnFilePrint();      break;
        case IDC_CMD_COPY:      TRACE(L"Copy\n");   break;
        case IDC_CMD_CUT:       TRACE(L"Cut\n");    break;
        case IDC_CMD_PASTE:     TRACE(L"Paste\n");  break;
        case IDC_CMD_ABOUT:     OnHelp();           break;
        case IDC_CMD_EXIT:      OnFileExit();       break;
        case IDC_RICHFONT:      TRACE(L"Font dialog\n");    break;
        case IDC_RIBBONHELP:    OnHelp();                   break;
        case IDC_MRULIST:       OnMRUList(key, ppropvarValue);      break;
        case IDC_PEN_COLOR:     OnPenColor(ppropvarValue, pCmdExProp);  break;
        case IDC_CUSTOMIZE_QAT: TRACE(L"Customize Quick Access ToolBar\n");  break;
        default:
            {
                CString str;
                str.Format(L"Unknown Button %d\n", cmdID);
                TRACE(str);
            }
            break;
        }
    }

    return S_OK;
}

// The IUIRibbon interface provides the ability to specify settings and properties for the ribbon.
IUIRibbon* CMainFrame::GetIUIRibbon() const
{
    return m_pIUIRibbon;
}

// Loads data from the specified file. It is called by OnFileOpen,
// and when a file is dragged and dropped on the window.
void CMainFrame::LoadFile(LPCTSTR fileName)
{
    try
    {
        // Retrieve the PlotPoint data
        GetDoc().FileOpen(fileName);
        m_pathName = fileName;
        AddMRUEntry(fileName);

        GetView().Invalidate();
    }

    catch (const CFileException& e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        m_pathName = L"";
        m_view.GetAllPoints().clear();
    }
}

// Opens a file from the Most Recently Used list.
void CMainFrame::MRUFileOpen(UINT mruIndex)
{
    CString mruText = GetMRUEntry(mruIndex);

    try
    {
        GetDoc().FileOpen(mruText);
        m_pathName = mruText;
        GetView().Invalidate();
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        RemoveMRUEntry(mruText);
        m_view.GetAllPoints().clear();
    }
}

// Process the commands from the (non-ribbon) Menu and Tool Bar.
BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDM_FILE_NEW:        return OnFileNew();
    case IDM_FILE_OPEN:       return OnFileOpen();
    case IDM_FILE_SAVE:       return OnFileSave();
    case IDM_FILE_SAVEAS:     return OnFileSaveAs();
    case IDM_FILE_PRINT:      return OnFilePrint();

    case IDM_FILE_EXIT:       return OnFileExit();
    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:
    {
        UINT uMRUEntry = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
        MRUFileOpen(uMRUEntry);
        return TRUE;
    }

    case IDM_PEN_RED:         return SetPenColor(RGB(255, 0, 0));
    case IDM_PEN_BLUE:        return SetPenColor(RGB(0, 0, 255));
    case IDM_PEN_GREEN:       return SetPenColor(RGB(0, 196, 0));
    case IDM_PEN_BLACK:       return SetPenColor(RGB(0, 0, 0));

    case IDW_VIEW_STATUSBAR:  return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:    return OnViewToolBar();
    case IDM_HELP_ABOUT:      return OnHelp();

    }

    return FALSE;
}

// OnDropFile called when a file is dragged and dropped on the window.
LRESULT CMainFrame::OnDropFile(WPARAM wparam)
{
    try
    {
        // wparam is a pointer (LPCTSTR) to the filename.
        LPCTSTR fileName = reinterpret_cast<LPCTSTR>(wparam);
        assert(fileName);

        // Load the file.
        LoadFile(fileName);
    }

    catch (const CFileException& e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        m_view.GetAllPoints().clear();
    }

    return 0;
}

// OnFileExit is called in response to the file exit button.
BOOL CMainFrame::OnFileExit()
{
    // Issue a close request to the frame.
    Close();
    return TRUE;
}

// OnFileNew is called in response to the File New button.
BOOL CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_pathName = L"";
    GetView().Invalidate();
    return TRUE;
}

// OnFileOpen is called in response to the File Open button.
BOOL CMainFrame::OnFileOpen()
{
    try
    {
        CFileDialog fileDlg(TRUE, L"dat", 0, OFN_FILEMUSTEXIST, L"Scribble Files (*.dat)\0*.dat\0\0");
        fileDlg.SetTitle(L"Open File");

        // Bring up the file open dialog retrieve the selected filename.
        if (fileDlg.DoModal(*this) == IDOK)
        {
            // Load the file
            LoadFile(fileDlg.GetPathName());
        }
    }

    catch (const CFileException& e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        m_view.GetAllPoints().clear();
    }

    return TRUE;
}

// Prints the view window to a printer of your choice.
BOOL CMainFrame::OnFilePrint()
{
    try
    {
        // Print the view window.
        m_view.Print();
    }

    catch (const CException& e)
    {
        // Display a message box indicating why printing failed.
        CString message = CString(e.GetText()) + CString("\n") + e.GetErrorString();
        CString type = CString(e.what());
        ::MessageBox(0, message, type, MB_ICONWARNING);
    }

    return TRUE;
}

// OnFileSave is called in response to the File Save button.
BOOL CMainFrame::OnFileSave()
{
    try
    {
        if (m_pathName == L"")
            OnFileSaveAs();
        else
            GetDoc().FileSave(m_pathName);
    }

    catch (const CFileException& e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Save File", MB_ICONWARNING);

        m_view.GetAllPoints().clear();
    }

    return TRUE;
}

// OnFileSave is called in response to the File SaveAs button.
BOOL CMainFrame::OnFileSaveAs()
{
    try
    {
        CFileDialog FileDlg(FALSE, L"dat", 0, OFN_OVERWRITEPROMPT, L"Scribble Files (*.dat)\0*.dat\0\0");
        FileDlg.SetTitle(L"Save File");

        // Bring up the file open dialog retrieve the selected filename.
        if (FileDlg.DoModal(*this) == IDOK)
        {
            CString str = FileDlg.GetPathName();

            // Save the file.
            GetDoc().FileSave(str);
            m_pathName = str;
            AddMRUEntry(m_pathName);
        }
    }

    catch (const CFileException& e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Save File", MB_ICONWARNING);

        m_view.GetAllPoints().clear();
    }

    return TRUE;
}

// OnMRUList is called in response to selection from the recent files list.
// The recent files list contains a list of the Most Recently Used files.
BOOL CMainFrame::OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue)
{
    try
    {
        if (ppropvarValue != NULL && key != NULL && UI_PKEY_SelectedItem == *key)
        {
            UINT mruItem = ppropvarValue->ulVal;
            MRUFileOpen(mruItem);
        }
    }

    catch (const CFileException &e)
    {
        // An exception occurred. Display the relevant information.
        ::MessageBox(0, e.GetText(), L"Failed to Load File", MB_ICONWARNING);

        m_view.GetAllPoints().clear();
    }

    return TRUE;
}

// OnPenColor is called when the DropdownColorPicker button is pressed.
BOOL CMainFrame::OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp)
{
    if (ppropvarValue != NULL)
    {
        // Retrieve color type.
        UINT type = ppropvarValue->uintVal;

        // The Ribbon framework passes color as additional property if the color type is RGB.
        if (type == UI_SWATCHCOLORTYPE_RGB && pCmdExProp != NULL)
        {
            // Retrieve color.
            PROPVARIANT var;
            PropVariantInit(&var);
            if (0 <= pCmdExProp->GetValue(UI_PKEY_Color, &var))
            {
                COLORREF color = PropVariantToUInt32WithDefault(var, 0);
                m_view.SetPenColor(color);
            }
        }
    }

    return TRUE;
}

// OnViewChanged is called when the ribbon is changed.
STDMETHODIMP CMainFrame::OnViewChanged(UINT32, UI_VIEWTYPE typeId, IUnknown* pView, UI_VIEWVERB verb, INT32)
{
    HRESULT result = E_NOTIMPL;

    // Checks to see if the view that was changed was a Ribbon view.
    if (UI_VIEWTYPE_RIBBON == typeId)
    {
        switch (verb)
        {
        case UI_VIEWVERB_CREATE:    // The ribbon has been created.
            m_pIUIRibbon = reinterpret_cast<IUIRibbon*>(pView);
            result = S_OK;
            break;
        case UI_VIEWVERB_SIZE:      // The ribbon's size has changed.
            RecalcLayout();
            result = S_OK;
            break;
        case UI_VIEWVERB_DESTROY:   // The ribbon has been destroyed.
            result = S_OK;
            break;
        case UI_VIEWVERB_ERROR:
            result = E_FAIL;
            break;
        }
    }

    return result;
}

// Used when there isn't a ribbon.
BOOL CMainFrame::SetPenColor(COLORREF clr)
{
    m_view.SetPenColor(clr);
    return TRUE;
}

// Configures the ToolBar. Used when there isn't a ribbon.
void CMainFrame::SetupToolBar()
{
    // Define our toolbar buttons. Used when there isn't a ribbon.
    AddToolBarButton( IDM_FILE_NEW   );
    AddToolBarButton( IDM_FILE_OPEN  );
    AddToolBarButton( IDM_FILE_SAVE  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_EDIT_CUT,   FALSE );
    AddToolBarButton( IDM_EDIT_COPY,  FALSE );
    AddToolBarButton( IDM_EDIT_PASTE, FALSE );
    AddToolBarButton( IDM_FILE_PRINT );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_PEN_RED    );
    AddToolBarButton( IDM_PEN_BLUE   );
    AddToolBarButton( IDM_PEN_GREEN  );
    AddToolBarButton( IDM_PEN_BLACK  );
    AddToolBarButton( 0 );              // Separator
    AddToolBarButton( IDM_HELP_ABOUT );
}

// This function is called when a ribbon button is updated.
// Refer to IUICommandHandler::UpdateProperty in the Windows 7 SDK documentation.
STDMETHODIMP CMainFrame::UpdateProperty(UINT32 cmdID, __in REFPROPERTYKEY key,  __in_opt  const PROPVARIANT*, __out PROPVARIANT* newValue)
{
    HRESULT result = E_NOTIMPL;
    if(UI_PKEY_Enabled == key)
    {
        return UIInitPropertyFromBoolean(UI_PKEY_Enabled, TRUE, newValue);
    }

    switch(cmdID)
    {
    case IDC_MRULIST:
        // Set up the Most Recently Used (MRU) menu.
        if (UI_PKEY_Label == key)
        {
            WCHAR label[MAX_PATH] = L"Recent Files";
            result = UIInitPropertyFromString(UI_PKEY_Label, label, newValue);
        }
        else if (UI_PKEY_RecentItems == key)
        {
            result = PopulateRibbonRecentItems(newValue);
        }
        break;

    case IDC_PEN_COLOR:
        // Set the initial pen color.
        result = UIInitPropertyFromUInt32(key, RGB(1, 1, 1), newValue);
        break;
    }

    return result;
}

// The window procedure handles the window's messages.
LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_DROPFILE:          return OnDropFile(wparam);
        }

        // Use the default message handling for remaining messages.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

