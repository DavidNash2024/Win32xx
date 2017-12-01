////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"

#include <UIRibbonPropertyHelpers.h>
#include "RibbonUI.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    // Set m_View as the view window of the frame
    SetView(m_View);

    // Set the registry key name, and load the initial window position
    // Use a registry key name like "CompanyName\\Application"
    LoadRegistrySettings(_T("Win32++\\Ribbon Frame"));

    // Load the settings from the registry with 4 MRU entries
    LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
    // Destructor for CMainFrame.
    DestroyRibbon();
}

STDMETHODIMP CMainFrame::Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp)
{
    // This function is called when a ribbon button is pressed. 
    // Refer to IUICommandHandler::Execute in the Windows 7 SDK documentation 

    if (UI_EXECUTIONVERB_EXECUTE == verb)
    {
        switch(nCmdID)
        {
        case IDC_CMD_NEW:       OnFileNew();        break;
        case IDC_CMD_OPEN:      OnFileOpen();       break;
        case IDC_CMD_SAVE:      OnFileSave();       break;
        case IDC_CMD_SAVE_AS:   OnFileSaveAs();     break;
        case IDC_CMD_PRINT:     OnFilePrint();      break;
        case IDC_CMD_COPY:      TRACE("Copy\n");    break;
        case IDC_CMD_CUT:       TRACE("Cut\n");     break;
        case IDC_CMD_PASTE:     TRACE("Paste\n");   break;
        case IDC_CMD_ABOUT:     OnHelp();           break;
        case IDC_CMD_EXIT:      OnFileExit();       break;
        case IDC_RICHFONT:      TRACE("Font dialog\n");     break;
        case IDC_RIBBONHELP:    OnHelp();                   break;
        case IDC_MRULIST:       OnMRUList(key, ppropvarValue);      break;
        case IDC_PEN_COLOR:     OnPenColor(ppropvarValue, pCmdExProp);  break;
        case IDC_CUSTOMIZE_QAT: TRACE("Customize Quick Access ToolBar\n");  break;
        default:
            {
                CString str;
                str.Format(_T("Unknown Button %d\n"),nCmdID);
                TRACE(str);
            }
            break;
        }
    }

    return S_OK;
}

void CMainFrame::MRUFileOpen(UINT nMRUIndex)
{
    CString strMRUText = GetMRUEntry(nMRUIndex);

    if (GetDoc().FileOpen(strMRUText))
        m_PathName = strMRUText;
    else
        RemoveMRUEntry(strMRUText);

    GetView().Invalidate();
}

LRESULT CMainFrame::OnDropFile(WPARAM wParam)
// Called in response to the UWM_DROPFILE user defined message
{
    // wParam is a pointer (LPCTSTR) to the filename
    LPCTSTR szFileName = reinterpret_cast<LPCTSTR>(wParam);
    assert(szFileName);

    // Load the file
    LoadFile(szFileName);
    return 0L;
}

void CMainFrame::OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue)
{
    if (ppropvarValue != NULL && key != NULL && UI_PKEY_SelectedItem == *key)
    {
        UINT uSelectedMRUItem = ppropvarValue->ulVal;
        MRUFileOpen(uSelectedMRUItem);
    }
}

void CMainFrame::OnPenColor(const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp)
{
     // DropdownColorPicker button pressed
            
    if (ppropvarValue != NULL)
    {
        // Retrieve color type. 
        UINT type = ppropvarValue->uintVal; 
    
        // The Ribbon framework passes color as additional property if the color type is RGB.
        if (type == UI_SWATCHCOLORTYPE_RGB && pCmdExProp != NULL)
        {
            // Retrieve color.
            PROPVARIANT var;
            if (0 <= pCmdExProp->GetValue(UI_PKEY_Color, &var))
            {   
                COLORREF color = PropVariantToUInt32WithDefault(var, 0);
                m_View.SetPenColor(color);
            }
        }
    }       
}

void CMainFrame::SetPenColor(COLORREF clr)
{
    // Used when there isn't a ribbon
    m_View.SetPenColor(clr);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Process the messages from the (non-ribbon) Menu and Tool Bar

    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch (nID)
    {
    case IDM_FILE_NEW:          OnFileNew();            return TRUE;
    case IDM_FILE_OPEN:         OnFileOpen();           return TRUE;
    case IDM_FILE_SAVE:         OnFileSave();           return TRUE;
    case IDM_FILE_SAVEAS:       OnFileSaveAs();         return TRUE;
    case IDM_FILE_PRINT:        OnFilePrint();          return TRUE;

    case IDM_FILE_EXIT:         OnFileExit();           return TRUE;
    case IDW_FILE_MRU_FILE1:
    case IDW_FILE_MRU_FILE2:
    case IDW_FILE_MRU_FILE3:
    case IDW_FILE_MRU_FILE4:
    case IDW_FILE_MRU_FILE5:
        {
            UINT uMRUEntry = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
            MRUFileOpen(uMRUEntry);
            return TRUE;
        }

    case IDM_PEN_RED:   SetPenColor(RGB(255, 0, 0));    return TRUE;
    case IDM_PEN_BLUE:  SetPenColor(RGB(0, 0, 255));    return TRUE;
    case IDM_PEN_GREEN: SetPenColor(RGB(0, 196, 0));    return TRUE;
    case IDM_PEN_BLACK: SetPenColor(RGB(0, 0, 0));      return TRUE;

    case IDW_VIEW_STATUSBAR:    return OnViewStatusBar();
    case IDW_VIEW_TOOLBAR:      return OnViewToolBar();
    case IDM_HELP_ABOUT:        return OnHelp();

    }

    return FALSE;
}

void CMainFrame::OnFileExit()
{
    // Issue a close request to the frame
    PostMessage(WM_CLOSE);
}

void CMainFrame::LoadFile(LPCTSTR str)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
{
    // Retrieve the PlotPoint data
    if (GetDoc().FileOpen(str))
    {
        // Save the filename
        m_PathName = str;
        AddMRUEntry(str);
    }
    else
        m_PathName=_T("");

    GetView().Invalidate();
}

void CMainFrame::OnFileOpen()
{
    CFileDialog FileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    FileDlg.SetTitle(_T("Open File"));

    // Bring up the file open dialog retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        // Load the file
        LoadFile(FileDlg.GetPathName());
    }
}

void CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_PathName = _T("");
    GetView().Invalidate();
}

void CMainFrame::OnFileSave()
{
    if (m_PathName == _T(""))
        OnFileSaveAs();
    else
        GetDoc().FileSave(m_PathName);
}

void CMainFrame::OnFileSaveAs()
{
    CFileDialog FileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    FileDlg.SetTitle(_T("Save File"));

    // Bring up the file open dialog retrieve the selected filename
    if (FileDlg.DoModal(*this) == IDOK)
    {
        CString str = FileDlg.GetPathName();

        // Save the file
        if (GetDoc().FileSave(str))
        {
            // Save the file name
            m_PathName = str;
            AddMRUEntry(m_PathName);
        }
    }
}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CMainFrame::OnFilePrint()
{
    // Pass the print job to CDoc
    GetDoc().Print();
}


void CMainFrame::SetupToolBar()
// Configures the ToolBar. Used when there isn't a ribbon.
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

STDMETHODIMP CMainFrame::UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key,  __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue) 
{   
    // This function is called when a ribbon button is updated. 
    // Refer to IUICommandHandler::UpdateProperty in the Windows 7 SDK documentation
    UNREFERENCED_PARAMETER(currentValue);

    HRESULT hr = E_NOTIMPL;
    if(UI_PKEY_Enabled == key)
    {
        return UIInitPropertyFromBoolean(UI_PKEY_Enabled, TRUE, newValue);
    }

    switch(nCmdID)
    {
    case IDC_MRULIST:
        // Set up the Most Recently Used (MRU) menu
        if (UI_PKEY_Label == key)
        {
            WCHAR label[MAX_PATH] = L"Recent Files";
            hr = UIInitPropertyFromString(UI_PKEY_Label, label, newValue);
        }
        else if (UI_PKEY_RecentItems == key)
        {
            hr = PopulateRibbonRecentItems(newValue);
        }
        break;

    case IDC_PEN_COLOR:
        // Set the initial pen color
        hr = UIInitPropertyFromUInt32(key, RGB(1, 1, 1), newValue);
        break;
    } 

    return hr;
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called to handle the window's messages
{
    switch (uMsg)
    {
    case UWM_DROPFILE:          return OnDropFile(wParam);
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(uMsg, wParam, lParam);
}

