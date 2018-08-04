////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"

#include <UIRibbonPropertyHelpers.h>
#include "RibbonUI.h"
#include "mainfrm.h"
#include "files.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
    // Constructor for CMainFrame. Its called after CFrame's constructor

    // Set m_View as the view window of the frame
    SetView(m_view);

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

STDMETHODIMP CMainFrame::Execute(UINT32 cmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCmdExProp)
{
    // This function is called when a ribbon button is pressed. 
    // Refer to IUICommandHandler::Execute in the Windows 7 SDK documentation 

    if (UI_EXECUTIONVERB_EXECUTE == verb)
    {
        switch(cmdID)
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
                str.Format(_T("Unknown Button %d\n"),cmdID);
                TRACE(str);
            }
            break;
        }
    }

    return S_OK;
}

void CMainFrame::MRUFileOpen(UINT mruIndex)
{
    CString mruText = GetMRUEntry(mruIndex);

    if (GetDoc().FileOpen(mruText))
        m_pathName = mruText;
    else
        RemoveMRUEntry(mruText);

    GetView().Invalidate();
}

void CMainFrame::OnMRUList(const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue)
{
    if (ppropvarValue != NULL && key != NULL && UI_PKEY_SelectedItem == *key)
    {
        UINT mruItem = ppropvarValue->ulVal;
        MRUFileOpen(mruItem);
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
                UINT color = var.uintVal;
                m_view.SetPenColor((COLORREF)color);
            }
        }
    }       
}

void CMainFrame::SetPenColor(COLORREF clr)
{
    // Used when there isn't a ribbon
    m_view.SetPenColor(clr);
}

BOOL CMainFrame::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Process the messages from the (non-ribbon) Menu and Tool Bar

    UNREFERENCED_PARAMETER(lparam);

    UINT nID = LOWORD(wparam);
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
            UINT uMRUEntry = LOWORD(wparam) - IDW_FILE_MRU_FILE1;
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

void CMainFrame::LoadFile(LPCTSTR fileName)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
{
    // Retrieve the PlotPoint data
    if (GetDoc().FileOpen(fileName))
    {
        // Save the filename
        m_pathName = fileName;
        AddMRUEntry(fileName);
    }
    else
        m_pathName=_T("");

    GetView().Invalidate();
}

void CMainFrame::OnFileOpen()
{
    CFileDialog fileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    fileDlg.SetTitle(_T("Open File"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        // Load the file
        LoadFile(fileDlg.GetPathName());
    }
}

void CMainFrame::OnFileNew()
{
    GetDoc().GetAllPoints().clear();
    m_pathName = _T("");
    GetView().Invalidate();
}

void CMainFrame::OnFileSave()
{
    if (m_pathName == _T(""))
        OnFileSaveAs();
    else
        GetDoc().FileSave(m_pathName);
}

void CMainFrame::OnFileSaveAs()
{
    CFileDialog fileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
    fileDlg.SetTitle(_T("Save File"));

    // Bring up the file open dialog retrieve the selected filename
    if (fileDlg.DoModal(*this) == IDOK)
    {
        CString str = fileDlg.GetPathName();

        // Save the file
        if (GetDoc().FileSave(str))
        {
            // Save the file name
            m_pathName = str;
            AddMRUEntry(m_pathName);
        }
    }

}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CMainFrame::OnFilePrint()
{
    // Get the dimensions of the View window
    CRect viewRect = GetView().GetClientRect();
    int width = viewRect.Width();
    int height = viewRect.Height();

    // Copy the bitmap from the View window
    CClientDC viewDC(GetView());
    CMemDC memDC(viewDC);
    CBitmap bmView;
    bmView.CreateCompatibleBitmap(viewDC, width, height);
    memDC.SelectObject(bmView);
    BitBlt(memDC, 0, 0, width, height, viewDC, 0, 0, SRCCOPY);

    CPrintDialog printDlg;

    try
    {
        // Bring up a dialog to choose the printer
        if (printDlg.DoModal(GetView()) == IDOK)    // throws exception if there is no default printer
        {
            // Zero and then initialize the members of a DOCINFO structure.
            DOCINFO di;
            memset(&di, 0, sizeof(DOCINFO));
            di.cbSize = sizeof(DOCINFO);
            di.lpszDocName = _T("Scribble Printout");
            di.lpszOutput = static_cast<LPTSTR>(NULL);
            di.lpszDatatype = static_cast<LPTSTR>(NULL);
            di.fwType = 0;

            // Begin a print job by calling the StartDoc function.
            CDC printDC = printDlg.GetPrinterDC();
            if (SP_ERROR == StartDoc(printDC, &di))
                throw CUserException(_T("Failed to start print job"));

            // Inform the driver that the application is about to begin sending data.
            if (0 > StartPage(printDC))
                throw CUserException(_T("StartPage failed"));

            BITMAPINFOHEADER bi;
            ZeroMemory(&bi, sizeof(bi));
            bi.biSize = sizeof(bi);
            bi.biHeight = height;
            bi.biWidth = width;
            bi.biPlanes = 1;
            bi.biBitCount = 24;
            bi.biCompression = BI_RGB;

            // Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
            // Get the size of the image data
            memDC.GetDIBits(bmView, 0, height, NULL, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

            // Retrieve the image data
            std::vector<byte> vBits(bi.biSizeImage, 0); // a vector to hold the byte array
            byte* pByteArray = &vBits.front();
            memDC.GetDIBits(bmView, 0, height, pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

            // Determine the scaling factors required to print the bitmap and retain its original proportions.
            float logPelsX1 = static_cast<float>(viewDC.GetDeviceCaps(LOGPIXELSX));
            float logPelsY1 = static_cast<float>(viewDC.GetDeviceCaps(LOGPIXELSY));
            float logPelsX2 = static_cast<float>(GetDeviceCaps(printDC, LOGPIXELSX));
            float logPelsY2 = static_cast<float>(GetDeviceCaps(printDC, LOGPIXELSY));
            float scaleX = logPelsX2 / logPelsX1;
            float scaleY = logPelsY2 / logPelsY1;

            int scaledWidth = static_cast<int>(static_cast<float>(width) * scaleX);
            int scaledHeight = static_cast<int>(static_cast<float>(height) * scaleY);

            // Use StretchDIBits to scale the bitmap and maintain its original proportions
            UINT result = StretchDIBits(printDC, 0, 0, scaledWidth, scaledHeight, 0, 0, width, height,
                                        pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, SRCCOPY);
            if (GDI_ERROR == result)
            {
                throw CUserException(_T("Failed to resize image for printing"));
            }

            // Inform the driver that the page is finished.
            if (0 > EndPage(printDC))
                throw CUserException(_T("EndPage failed"));

            // Inform the driver that document has ended.
            if (0 > EndDoc(printDC))
                throw CUserException(_T("EndDoc failed"));
        }
    }

    catch (const CException& e)
    {
        // Display a message box indicating why printing failed.
        CString message = CString(e.GetText()) + CString("\n") + e.GetErrorString();
        CString type = CString(e.what());
        ::MessageBox(NULL, message, type, MB_ICONWARNING);
    }

}

void CMainFrame::OnInitialUpdate()
{
    // Add some Dockers to the Ribbon Frame
    DWORD style = DS_CLIENTEDGE; // The style added to each docker
    int dockWidth = 150;
    CDocker* pDock1 = AddDockedChild(new CDockFiles, DS_DOCKED_LEFT | style, dockWidth);
    CDocker* pDock2 = AddDockedChild(new CDockFiles, DS_DOCKED_RIGHT | style, dockWidth);

    assert (pDock1->GetContainer());
    assert (pDock2->GetContainer());
    pDock1->GetContainer()->SetHideSingleTab(TRUE);
    pDock2->GetContainer()->SetHideSingleTab(TRUE);
}

void CMainFrame::SetupToolBar()
{
    // Define our toolbar. Used when there isn't a ribbon.
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

STDMETHODIMP CMainFrame::UpdateProperty(UINT32 cmdID, __in REFPROPERTYKEY key,  __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue) 
{   
    // This function is called when a ribbon button is updated. 
    // Refer to IUICommandHandler::UpdateProperty in the Windows 7 SDK documentation
    UNREFERENCED_PARAMETER(currentValue);

    HRESULT result = E_NOTIMPL;
    if(UI_PKEY_Enabled == key)
    {
        return UIInitPropertyFromBoolean(UI_PKEY_Enabled, TRUE, newValue);
    }

    switch(cmdID)
    {
    case IDC_MRULIST:
        // Set up the Most Recently Used (MRU) menu
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
        // Set the initial pen color
		result = UIInitPropertyFromUInt32(key, RGB(1, 1, 1), newValue);
        break;
    } 

    return result;
}

LRESULT CMainFrame::OnDropFile(WPARAM wparam)
// Called in reponse to a UWM_DROPFILE message
{
    // wParam is a pointer (LPCTSTR) to the filename
    LPCTSTR fileName = reinterpret_cast<LPCTSTR>(wparam);
    assert(fileName);
    
    // Load the file
    LoadFile(fileName);
    return 0;
}

LRESULT CMainFrame::OnGetAllPoints()
// Called in response to a UWN_GETALLPOINTS message
{
    // Get a pointer to the vector of PlotPoints
    std::vector<PlotPoint>* pAllPoints = &GetDoc().GetAllPoints();

    // Cast the pointer to a LRESULT and return it
    return reinterpret_cast<LRESULT>(pAllPoints);
}

LRESULT CMainFrame::OnSendPoint(WPARAM wparam)
// Called in response to a UWM_SENDPOINT message
{
    // wParam is a pointer to the vector of PlotPoints
    PlotPoint* pPP = reinterpret_cast<PlotPoint*>(wparam);

    // Dereference the pointer and store the vector of PlotPoints in CDoc
    GetDoc().StorePoint(*pPP);
    return 0;
}

LRESULT CMainFrame::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case UWM_DROPFILE:          return OnDropFile(wparam);
    case UWN_GETALLPOINTS:      return OnGetAllPoints();
    case UWM_SENDPOINT:         return OnSendPoint(wparam);
    }

    //Use the default message handling for remaining messages
    return WndProcDefault(msg, wparam, lparam);
}

