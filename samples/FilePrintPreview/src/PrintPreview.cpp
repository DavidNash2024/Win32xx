/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]         (PrintPreview.cpp) *
********************************************************************************

    Implementation of the CPrintPreview, CPreviewPane, and PreviewSetup 
    classes.

********************************************************************************

    Acknowledgment. These classes were adapted from the PrintPreview sample 
    program appearing in the Win32++ framework sample folder, created by  
    David Nash and published under the permissions granted in that work.
    The adaptation here reimplements the PrintView window as a separate 
    popup window that appears on screen along with the regular program 
    window. This work has been developed under the co-authorship of Robert
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2018. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"
#include "resource.h"
#include "PrintUtil.h"
#include "PrintPreview.h"

/*=============================================================================*

    Program constants													*/

  // zoom states
static const int	ZOOM_OUT    = 0;
static const int	ZOOM_WIDTH  = 1;
  // program options
static const int	BORDER      =   20;		// pixels around rendered preview
static const double	PREVIEW_MIN	=  2.0;		// minimum preview screen dimension
static const double	SCREEN_MIN	=  8.0;		// minimum monitor screen dimension
static const double SCREEN_MAX	= 35.0;		// maximum monitor screen dimension
  // Registry key for saving screen and initial print preview sizes. Note:
  // this should match the main frame's registry key, but with "Software"
  // added in order to make them appear together with the frame's entries.
static const LPCTSTR REGISTRY_KEY_NAME = _T("Software\\Win32++\\FilePrintPreview\\Sizes");

/*******************************************************************************

    Implementation of the CPrintPreview class.

*=============================================================================*/
    CPrintPreview::
CPrintPreview(UINT nResID, DWORD dwFlags /* = HIDE_HELP */ )			/*

    Construct the preview dialog object.
*-----------------------------------------------------------------------------*/
    : CDialog(nResID), m_dcMem(0), m_SetupDlg(IDD_PREVIEW_SETUP)
{
    m_nCurrentPage   = 0;
    m_dwFlags        = dwFlags;
    m_ScreenInches   = DSize(20.25, 11.5);
    m_PreviewInches  = DSize(8.0, 10.0);
    m_PreviewPane.SetPaneZoomState(ZOOM_OUT);
 }

/*============================================================================*/
    CPrintPreview::
~CPrintPreview()														/*

    Destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    BOOL CPrintPreview::
ClosePreview()															/*

    Close the preview dialog window and save the screen and initial preview
    window sizes.
*-----------------------------------------------------------------------------*/
{
    SaveSizesRegistry();
    m_PreviewPane.SetPaneZoomState(ZOOM_OUT);
    PostMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
    return TRUE;
}

/*============================================================================*/
    INT_PTR CPrintPreview::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)						/*

    Process special messages for the preview dialog.
*-----------------------------------------------------------------------------*/
{
      // Pass resizing messages on to the resizer
    m_Resizer.HandleMessage(uMsg, wParam, lParam);
    switch (uMsg)
    {
        case WM_SYSCOMMAND:
        {
			switch (LOWORD(wParam))
			{
				case SC_CLOSE: // close the window
					SaveSizesRegistry();
					Destroy();
					return TRUE;
			}
			break;
        }
    }
      // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(uMsg, wParam, lParam);
}

/*============================================================================*/
	void CPrintPreview::
DoDataExchange(CDataExchange& DX)										/*

	Attach data items to dialog controls and coordinate the exchange between
	the dialog and data values.
*-----------------------------------------------------------------------------*/
{
	DX.DDX_Control(IDC_PREVIEW_PRINT,	  m_ButtonPrint);
	DX.DDX_Control(IDC_PREVIEW_SETUP,     m_ButtonSetup);
	DX.DDX_Control(IDC_PREVIEW_PAGE,      m_EditPage);
	DX.DDX_Control(IDC_PREVIEW_FIRST,     m_ButtonFirst);
	DX.DDX_Control(IDC_PREVIEW_LAST,      m_ButtonLast);
	DX.DDX_Control(IDC_PREVIEW_PREV,      m_ButtonPrev);
	DX.DDX_Control(IDC_PREVIEW_NEXT,      m_ButtonNext);
	DX.DDX_Control(IDC_PREVIEW_CLOSE,     m_ButtonClose);
	DX.DDX_Control(IDC_PREVIEW_PANE,      m_PreviewPane);
	DX.DDX_Control(IDC_PREVIEW_HELP,      m_ButtonPvwHelp);
	DX.DDX_Control(IDC_PREVIEW_ZOOMCOMBO, m_ComboZoom);
}

/*============================================================================*/
    void CPrintPreview::
DocPageToBmp(UINT nPage)												/*

    Format the document nPage page for preview and deposit the image in the
    m_dcMem context. This base class method does this for the nPage page
    of the RichView document. Override this method for the particular
    document being previewed.
*-----------------------------------------------------------------------------*/
{
    GetRichView().PrintDC(nPage, m_dcPrinter, m_dcMem);
}

/*============================================================================*/
    void CPrintPreview::
DoPreparePreview()														/*

    Initialize the preview with values other than the defaults, if needed, 
    and determine the page divisions and number of pages to be previewed.
    This base class method prepares the preview of a RichEdit docuent. 
    Override it to create a printer  device context and a set of preview 
    particulars consistent with application needs.
*-----------------------------------------------------------------------------*/
{
    m_nNumPreviewPages = GetRichView().GetPageBreaks(m_dcPrinter);
}

/*============================================================================*/
    void CPrintPreview::
InitializeContexts()													/*

    Get the current device contexts of the default or currently chosen 
    printer and the preview pane and save these as data members. Likewise, 
    compute and save the printer and screen resolutions. Compute the shrink 
    factor that maps the printer resolution to that of the screen. Create 
    the memory context to receive the preview image.
*-----------------------------------------------------------------------------*/
{
      // Get the device context of the default or currently chosen printer.
    CDC dcPreview = m_PreviewPane.GetDC();
      // compute the screen pixels/inch
    m_ScreenPixels = CSize(dcPreview.GetDeviceCaps(HORZRES), 
        dcPreview.GetDeviceCaps(VERTRES));
      // We will need to create a compatible bitmap in memory for the 
      // preview. However, that may be too big for practicality, so we
      // will reduce the size of the memory bitmap from the full printer
      // resolution to fit the resolution of the screen:
    m_shrink = MAX(
        double(m_PrinterDots.cx) / m_ScreenPixels.cx, 
        double(m_PrinterDots.cy) / m_ScreenPixels.cy);
      // Create a memory DC for the printer
    m_dcMem = CMemDC(m_dcPrinter);
      // Create a compatible bitmap in memory for the preview that reduces
      // the size of the memory bitmap.
	CSize viewport((int)(m_PrinterDots.cx / m_shrink + 0.5),
        (int)(m_PrinterDots.cy / m_shrink + 0.5));
    m_dcMem.CreateCompatibleBitmap(dcPreview, viewport.cx, viewport.cy);
      // set the mapping mode to translate between printer and screen
      // coordinates to utilize the bitmap dimensions
    m_dcMem.SetMapMode(MM_ANISOTROPIC);
    m_dcMem.SetWindowExtEx(m_PrinterDots.cx, m_PrinterDots.cy, NULL);
    m_dcMem.SetViewportExtEx(viewport.cx, viewport.cy, NULL);
}

/*============================================================================*/
    void CPrintPreview::
InitializeControls()													/*

    Load directional button bitmaps, and initiate resizing of the client
    area.
*-----------------------------------------------------------------------------*/
{

      // load the directional button bitmaps
    m_FirstPage.LoadBitmap(IDB_PREVIEW_FIRST);
    m_ButtonFirst.SetBitmap((HBITMAP)m_FirstPage);
    m_PrevPage.LoadBitmap(IDB_PREVIEW_PREV);
    m_ButtonPrev.SetBitmap((HBITMAP)m_PrevPage);
    m_NextPage.LoadBitmap(IDB_PREVIEW_NEXT);
    m_ButtonNext.SetBitmap((HBITMAP)m_NextPage);
    m_LastPage.LoadBitmap(IDB_PREVIEW_LAST);
    m_ButtonLast.SetBitmap((HBITMAP)m_LastPage);
      // enable resizing the preview pane of the dialog
    m_Resizer.Initialize(*this, CRect(0, 0, 0, 0)); 
    m_Resizer.AddChild(m_PreviewPane,   topleft, 
        RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);
}

/*============================================================================*/
    void CPrintPreview::
InitializeToolTips()													/*

    Add tooltips to the preview buttons.
*-----------------------------------------------------------------------------*/
{
    CreateToolTip(*this);
    AddToolTip(IDC_PREVIEW_PRINT);
    AddToolTip(IDC_PREVIEW_PRINT);
    AddToolTip(IDC_PREVIEW_SETUP);
    AddToolTip(IDC_PREVIEW_FIRST);
    AddToolTip(IDC_PREVIEW_PREV);
    AddToolTip(IDC_PREVIEW_NEXT);
    AddToolTip(IDC_PREVIEW_LAST);
    AddToolTip(IDC_PREVIEW_PAGE);
    AddToolTip(IDC_PREVIEW_OFPAGES);
    AddToolTip(IDC_PREVIEW_ZOOMCOMBO);
    AddToolTip(IDC_PREVIEW_CLOSE);
    AddToolTip(IDC_PREVIEW_HELP);
    AddToolTip(IDC_PREVIEW_PANE);
}

/*============================================================================*/
    void CPrintPreview::
LoadSizesRegistry()														/*

    Load the saved screen and initial preview window size parameters from 
    the registry key labeled REGISTRY_KEY_NAME. 
*-----------------------------------------------------------------------------*/
{
    CRegKey key;
    CString strKey = REGISTRY_KEY_NAME;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
    {
        LPTSTR p;
        CString s = RegQueryStringValue(key, _T("Screen Width"));
        m_ScreenInches.cx = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Screen Height"));
        m_ScreenInches.cy = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Init Preview Width"));
        m_PreviewInches.cx = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Init Preview Height"));
        m_PreviewInches.cy = _tcstod(s, &p);
    }
}

/*============================================================================*/
    BOOL CPrintPreview::
OnCommand(WPARAM wParam, LPARAM lParam)									/*

    Direct the command messages to their processing functions.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lParam);

    if (HIWORD(wParam) == CBN_SELCHANGE)
        return OnZoomChange();

    switch (LOWORD(wParam))
    {
        case IDC_PREVIEW_PRINT: 
            return OnPrintButton();

        case IDC_PREVIEW_SETUP: 
            PreviewAndPageSetup();
            return TRUE;

        case IDC_PREVIEW_FIRST: 
            return OnFirstButton();

        case IDC_PREVIEW_PREV:  
            return OnPrevButton();

        case IDC_PREVIEW_NEXT:  
            return OnNextButton();

        case IDC_PREVIEW_LAST:  
            return OnLastButton();

        case IDC_PREVIEW_CLOSE: 
            return ClosePreview();

        case IDC_PREVIEW_HELP:  
            return OnPreviewHelp();
    }
    return FALSE;
}

/*============================================================================*/
    BOOL CPrintPreview::
OnInitDialog()															/*

    Attach control IDs to the objects they identify, set up the  automatic 
    resizing mechanism, engage tooltips, set the screen and initial preview
    window sizes, initialize the scale combo box values, and set up other 
    entities before the dialog becomes visible.
*-----------------------------------------------------------------------------*/
{
      // load saved screen and initial preview window sizes
    LoadSizesRegistry();
 	  // register controls,  attach controls to numeric identifiers
	UpdateData(m_DX, SENDTOCONTROL);
	  // enter initial values into controls
    InitializeControls();
      // add tooltips to the preview buttons
    InitializeToolTips();   
    CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    m_dcPrinter = PrintDlg.GetPrinterDC();
      // Get the printer resolution, 
    m_PrinterDots = CSize(m_dcPrinter.GetDeviceCaps(HORZRES),
        m_dcPrinter.GetDeviceCaps(VERTRES));
	  // fill combo box with scale values
	PopulateScaleBox();
      // If screen and initial preview window sizes are initially out of 
	  // bounds, prompt the user for entry of sizes that are acceptable. 
	  // Do not take zero for an answer.
    while (m_ScreenInches.cx < SCREEN_MIN || m_ScreenInches.cx > SCREEN_MAX  ||
		m_ScreenInches.cy < SCREEN_MIN    || m_ScreenInches.cy > SCREEN_MAX  ||
		m_PreviewInches.cx < PREVIEW_MIN  || m_PreviewInches.cx > SCREEN_MAX ||
		m_PreviewInches.cx < PREVIEW_MIN  || m_PreviewInches.cx > SCREEN_MAX)
    {
	    if (!PreviewAndPageSetup())
        {
			ClosePreview();	
			return TRUE;
        }
    }
    SetWindowSizes();
    // hide the help button if so indicated
    if (m_dwFlags & HIDE_HELP)
        m_ButtonPvwHelp.ShowWindow(SW_HIDE);
      // start at page 1
    m_nCurrentPage = 0;
	  // update controls
	UpdateData(m_DX, SENDTOCONTROL);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreview::
OnNextButton()															/*

    Display the next page of the document. This method can only be called
    when there is a valid next page to view.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(++m_nCurrentPage);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreview::
OnOK()																	/*

	This method is invoked when the ENTER key is pressed, and the base class
	response is to close the dialog. However, the desired behavior for this 
	dialog is either to go to the page appearing in the page box when this box
	has the focus, or otherwise to ignore the ENTER key altogether..
*-----------------------------------------------------------------------------*/
{
    HWND hwnd = HWND(::GetFocus());
    UINT nID = ::GetDlgCtrlID(hwnd);
      // if the control being activated is the page box, go to that page
    if (nID == IDC_PREVIEW_PAGE)
    {
        CString sPage = m_EditPage.GetWindowText();
        TCHAR *stop;
        UINT nPage = _tcstol(sPage, &stop, 10);
        nPage = MIN(MAX(1, nPage), m_nNumPreviewPages);
        OnPreviewPage(m_nCurrentPage = nPage - 1);
    }
}

/*============================================================================*/
    BOOL CPrintPreview::
OnFirstButton()															/*

    Display the first page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(m_nCurrentPage = 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreview::
OnLastButton()															/*

    Display the last page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(m_nCurrentPage = m_nNumPreviewPages - 1);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreview::
OnPrepareDC()															/*

    Called by the OnPrint() member function for each page during print 
    preview. Set all device contexts and associated objects to current
    values. Override this function in derived classes to adjust attributes 
    of the device contexts on a page-by-page basis, as needed. Be sure to
    call this base class at the beginning of the override.
*-----------------------------------------------------------------------------*/
{
    InitializeContexts();
}

/*============================================================================*/
    void CPrintPreview::
OnPreparePrintPreview()													/*

    Prepare to preview the document printout. Called by OnPreview() before 
    a document is previewed. Create the print preview dialog window using
    the desktop window as owner, so the previewe window is not hidden by
    the document view window. Override this function to specialize the
    preview for the application particulars. 
*-----------------------------------------------------------------------------*/
{
    if (!IsWindow())
        Create(::GetDesktopWindow());
      // show the document path in the caption title
    SetWindowText(m_sDocPath);
      // set special device contexts, determine pagination, and max pages
    DoPreparePreview();
}

/*============================================================================*/
    BOOL CPrintPreview::
OnPrevButton()															/*

    Display the previous page of the document. This method can only be 
    called when there is a valid previous page to view.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(--m_nCurrentPage);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreview::
OnPreview(const CString &docPath)										/*

    Display the preview pages of the document. 
*-----------------------------------------------------------------------------*/
{
      // save the doument path
    m_sDocPath = docPath;
      // set up device contexts, determine pagination, and number of pages
    OnPreparePrintPreview();
      // Preview the first page;
    OnPreviewPage(m_nCurrentPage = 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreview::
OnPreviewHelp()															/*

    Respond to requests for help on the print preview function.
*-----------------------------------------------------------------------------*/
{
    ::MessageBox(NULL, _T("Preview help not yet available."), 
        _T("Information..."), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreview::
OnPreviewPage(UINT nPage)												/*

*-----------------------------------------------------------------------------*/
{
    OnPrepareDC();
    PreviewPage(m_nCurrentPage = nPage);
}

/*============================================================================*/
    BOOL CPrintPreview::
OnPrintButton()															/*

    Respond to requests for printing the document.
*-----------------------------------------------------------------------------*/
{
    TheApp().TheFrame().PostMessage(WM_COMMAND, IDM_FILE_PRINT, 0);
    ClosePreview();
    return TRUE; 
}

/*============================================================================*/
    BOOL CPrintPreview::
OnZoomChange()															/*

    Set the zoom value for the display of the document page.
*-----------------------------------------------------------------------------*/
{
      // query the scale combo box selection
    int selection = m_ComboZoom.GetCurSel();
      // if it is one of the first two entries, use 0 and 1
      // for the zoom indicator 
    if (selection <= ZOOM_WIDTH)
        m_PreviewPane.SetPaneZoomState(selection);
    else
    {
        TCHAR val[20];
        TCHAR *stop;
        m_ComboZoom.GetLBText(selection, val);
        m_PreviewPane.SetPaneZoomState(_tcstol(val, &stop, 10));
    }
    m_PreviewPane.ShowScrollBars(selection == ZOOM_OUT ? 
        FALSE : TRUE);
    m_PreviewPane.SetScrollSizes(CSize(0, 0));
    m_PreviewPane.Invalidate();
    return TRUE;
}

/*============================================================================*/
	void CPrintPreview::
PopulateScaleBox()														/*

	Fill the scale combo box with the zoom settings.
*-----------------------------------------------------------------------------*/
{
	std::vector<CString> scale;	  // scale selection choices
	scale.push_back(_T("Fit page"));
	scale.push_back(_T("Fit width"));
	scale.push_back(_T("30%"));
	scale.push_back(_T("40%"));
	scale.push_back(_T("50%"));
	scale.push_back(_T("60%"));
	scale.push_back(_T("70%"));
	scale.push_back(_T("80%"));
	scale.push_back(_T("90%"));
	scale.push_back(_T("100%"));
	scale.push_back(_T("125%"));
	scale.push_back(_T("150%"));
	scale.push_back(_T("175%"));
	scale.push_back(_T("200%"));
	  // put the scales in the combo box, select top item
	m_ComboZoom.ResetContent();
	for (UINT i = 0; i < scale.size(); i++)
		m_ComboZoom.AddString(scale[i]);
	m_ComboZoom.SetCurSel(0);
}

/*============================================================================*/
    BOOL CPrintPreview::
PreviewAndPageSetup()													/*

    Prompt for user input of screen and initial preview window sizes. Do
    printer page setup if requested. Return TRUE if the setup dialog was 
    terminated in OK, or FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    if (m_SetupDlg.DoModal(*this) == IDOK)
	{
		SetWindowSizes();
		return TRUE;
	}
	return FALSE;
}

/*============================================================================*/
    void CPrintPreview::
PreviewPage(UINT nPage)													/*

    Display page numbered nPage to the screen (view port).
*-----------------------------------------------------------------------------*/
{
      // check validity of request
    assert(m_nNumPreviewPages > 0);
    assert(nPage < m_nNumPreviewPages);
      // Fill the bitmap with a white background
    CRect rc(0, 0, m_PrinterDots.cx, m_PrinterDots.cy);
    m_dcMem.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
      // render the nPage of the document into dcMem
    DocPageToBmp(nPage);
      // transfer the bitmap from the memory DC to the preview pane
    CBitmap Bitmap = m_dcMem.DetachBitmap();
    m_PreviewPane.SetBitmap(Bitmap);
      // reset the current status of the preview dialog's buttons
    UpdateButtons();
      // display the print preview
    m_PreviewPane.Invalidate();
}

/*============================================================================*/
    CString CPrintPreview::
RegQueryStringValue(CRegKey &key, LPCTSTR pName)						/*

    Return the CString value of a specified value pName found in the
    currently open registry key.
*-----------------------------------------------------------------------------*/
{
    ULONG len = 256;
    CString sValue;
    if (ERROR_SUCCESS == key.QueryStringValue(pName, sValue.GetBuffer(255), &len))
    {
        sValue.ReleaseBuffer();
        return sValue;
    }
    else
        return _T("");
}

/*============================================================================*/
    void CPrintPreview::
SaveSizesRegistry()														/*

    Write the screen and initial preview size values into the registry key 
    labeled REGISTRY_KEY_NAME.
*-----------------------------------------------------------------------------*/
{
    CString strKey = REGISTRY_KEY_NAME;
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, NULL);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        CString s;
        s.Format(_T("% .2f"),  m_ScreenInches.cx);
        key.SetStringValue(_T("Screen Width"), s.c_str());
        s.Format(_T("%.2f"),  m_ScreenInches.cy);
        key.SetStringValue(_T("Screen Height"), s.c_str());
        s.Format(_T("%.2f"),  m_PreviewInches.cx);
        key.SetStringValue(_T("Init Preview Width"), s.c_str());
        s.Format(_T("%.2f"),  m_PreviewInches.cy);
        key.SetStringValue(_T("Init Preview Height"), s.c_str());
    }
}

/*============================================================================*/
    void CPrintPreview::
SetWindowSizes()														/*

    Set the screen and initial preview window sizes using the current
    values of the m_ScreenInches and m_PreviewInches size members.
*-----------------------------------------------------------------------------*/
{
      // make sure the contexts are current
    InitializeContexts();
      // determine the printer dots/inch (this works for the printer!)
    DSize PrinterPPI(m_dcPrinter.GetDeviceCaps(LOGPIXELSX), 
        m_dcPrinter.GetDeviceCaps(LOGPIXELSY));
      // compute the screen m_ScreenPixels/inch
    DSize ScreenPPI((m_ScreenPixels.cx / m_ScreenInches.cx), 
        (m_ScreenPixels.cy / m_ScreenInches.cy));
    m_PrinterScreenResRatio = DSize((PrinterPPI.cx / ScreenPPI.cx) / m_shrink,
        (PrinterPPI.cy / ScreenPPI.cy) / m_shrink);
      // compute the initial preview size, in pixels
    CSize frame(int(m_PreviewInches.cx * ScreenPPI.cx), 
        int(m_PreviewInches.cy * ScreenPPI.cy));
      // set the dialog size
    CRect rcWorkArea;
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkArea, 0);
	CSize screen(rcWorkArea.Width(), rcWorkArea.Height());
	CSize preview(MIN(frame.cx, screen.cx), MIN(frame.cy, screen.cy));
      // center the preview window on the screen
	CSize ctr((screen.cx - preview.cx) / 2, (screen.cy - preview.cy) / 2);
    CRect rcPos(ctr.cx, ctr.cy, ctr.cx + preview.cx, ctr.cy + preview.cy);
    SetWindowPos(NULL, rcPos, SWP_SHOWWINDOW);
}

/*============================================================================*/
    void CPrintPreview::
UpdateButtons()															/*

    Enable or disable buttons, according to current page visible.
*-----------------------------------------------------------------------------*/
{
    UINT end_page = m_nNumPreviewPages;
    m_ButtonFirst.EnableWindow(m_nCurrentPage > 0);
    m_ButtonPrev.EnableWindow(m_nCurrentPage  > 0);
    m_ButtonNext.EnableWindow(m_nCurrentPage  < end_page - 1);
    m_ButtonLast.EnableWindow(m_nCurrentPage  < end_page - 1);
    CString page;
    page.Format(_T("%d"), m_nCurrentPage + 1);
    m_EditPage.SetWindowText(page);
    page.Format(_T(" of %d"), end_page);
    SetDlgItemText(IDC_PREVIEW_OFPAGES, page.c_str());
}

/*******************************************************************************

    Implementation of the CPreviewPane class.

*=============================================================================*/
    CPreviewPane::
CPreviewPane()                                                          /*

    Construct and register the custom preview window pane for displaying 
    a bitmap.
*-----------------------------------------------------------------------------*/
{
      // Note: The entry for the dialog's IDC_PREVIEW_PANE control in 
      // resource.rc  must match this name.
    CString ClassName = _T("PreviewPane");

      // Register the window class for use as a custom control in the dialog
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    if (!::GetClassInfo(TheApp().GetInstanceHandle(), ClassName, &wc))
    {
        wc.lpszClassName = ClassName;
        wc.lpfnWndProc   = ::DefWindowProc;
        wc.hInstance     = TheApp().GetInstanceHandle();
        wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
        wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        ::RegisterClass(&wc);
    }
    assert(::GetClassInfo(TheApp().GetInstanceHandle(), ClassName, &wc));
    SetScrollSizes(m_ScrollSize = m_LastWindowSize = CSize(0, 0));
    m_ZoomState      = ZOOM_OUT;
    m_PrevZoomState  = -1;
    m_ShowScrollBars = FALSE;
}

/*============================================================================*/
    DSize CPreviewPane::
GetZoom()																/*

    Return the zoom ratio of the view that will contain the rendered 
    document bitmap, under the current zoom state. Zoom is defined as the
    scaled image size divided by the bitmap size. The image size is the
    client area with a border around it. Set the scroll size to fit this
    zoom value.
*-----------------------------------------------------------------------------*/
{
    if (!m_Bitmap.GetHandle())
        return DSize(0.0, 0.0);

      // The unit_zoom is meant to provide a screen image equal in size to 
      // the printer page size. However, some difference may be seen on
      // some monitors due to roundoff.
    DSize printerScreenRatio = GetParent().GetPrinterScreenRatio();
    DSize unit_zoom = DSize(1.0 / printerScreenRatio.cx, 
        1.0 / printerScreenRatio.cx);
      // get client, bitmap, and preview window sizes
    BITMAP bm = m_Bitmap.GetBitmapData();
    CSize Client = GetClientRect().Size(), Bitmap(bm.bmWidth, bm.bmHeight),
          Preview(Client.cx - (2 * BORDER), Client.cy - (2 * BORDER));
      // compute the zoom value
    DSize zoom = DSize(double(Preview.cx) / double(Bitmap.cx), 
            double(Preview.cy) / double(Bitmap.cy));
    if (m_ZoomState == ZOOM_OUT)
    {
        double min = MIN(zoom.cx, zoom.cy);
        zoom = DSize(min, min);
    }
      // see the notes below for the rationale of this value
    else if (m_ZoomState == ZOOM_WIDTH)
    {
        zoom.cy = zoom.cx;
    }
    else 
        zoom = DSize(unit_zoom.cx * m_ZoomState / 100.0, 
            unit_zoom.cy * m_ZoomState / 100.0);

      // compute the scroll sizes
    CSize ScrollSize(0, 0); // default to ZOOM_OUT and no scroll bars case
    if (m_ZoomState != ZOOM_OUT && m_ShowScrollBars)
    {
          // Compute the scroll sizes for this zoom (see notes below). 
          // The zoom factor zf is
        DSize zf(1.0 / zoom.cx - 1.0, 1.0 / zoom.cy - 1.0);
        ScrollSize = CSize(m_ZoomState == ZOOM_WIDTH ? 0 :
            MAX(0, Bitmap.cx + 2 * BORDER - static_cast<int>(Preview.cx * zf.cx)),
            MAX(0, Bitmap.cy + 2 * BORDER - static_cast<int>(Preview.cy * zf.cy)));
          // Reset the scrolling sizes only if the bars are visible 
          // and either (1) the zoom state changed, or (2) the size 
          // has chanted and the scaling is not ZOOM_WIDTH, or, (3) 
          // when scaling ZOOM_WIDTH, when the window has also changed 
          // size. During ZOOM_WIDTH, the preview width changes as 
          // scrollbars are added, and this would cause change in 
          // the zoom, which would cause resizing and screen flicker.
        CSize WindowSize = GetWindowRect().Size();
        if (m_PrevZoomState != m_ZoomState ||
            (ScrollSize != m_ScrollSize && (m_ZoomState != ZOOM_WIDTH ||
            (m_LastWindowSize != WindowSize))))
				SetScrollSizes(m_ScrollSize = ScrollSize);
        m_LastWindowSize = WindowSize;
    }
    m_PrevZoomState = m_ZoomState;
    return zoom;

/*  Notes on scroll sizing: At a that scroll position p (.x or .y), at
*   which the pixel at the bottom-right of the Preview (.cx or .cy), with
*   scaling, is the final one in the Bitmap (.cx or .cy), the p value
*   (call it pos0) must satisfy
*
*       (Bitmap - pos0) * zoom = Preview
*
*   or  pos0 = Bitmap - Preview / zoom 
*
*   The total scroll size required to achieve this will then be
*
*       ScrollSize = pos0 + Preview + 2 * Border
*              = BitMap + 2 * Border - Preview * (1 / zoom - 1)
*
*   We may also note that the zoom value that gives a pos0 value of zero
*   will be 
*
*       zoom_out = Preview / Bitmap
*
*   Since two dimensions are involved, the one that must be used is the
*   least, so that both dimensions fit on one Preview screen.  No zoom 
*   value less than this can achieve a full-Preview view. Choosing the
*   .cx value of this gives the zoom-to-width scale
*
*       zoom_width = Preview.cx / Bitmap.cx
*/
}

/*============================================================================*/
    void CPreviewPane::
OnDraw(CDC& dc)															/*

    Copy the bitmap (m_Bitmap) into the PreviewPane, scaling the image
    to fit the window. The dc is the CPaintDC of the screen. The zoom factor
    applied is the scaling of target size to source size. The three factors
    available range from fitting the entire bitmap page onto the rendered 
    page (the zoom-out case), to rendering a fuller scale view of perhaps 
    only a portion of the bitmap into the available screen area.
*-----------------------------------------------------------------------------*/
{
    if (m_Bitmap.GetHandle())
    {
        CMemDC dcMem(dc);
        dcMem.SelectObject(m_Bitmap);
        dcMem.SetMapMode(MM_TEXT);
        BITMAP bm = m_Bitmap.GetBitmapData();
          // determine the size of the PreviewPane window with a border
          // around the area used to show the bitmap
        DSize zoom  = GetZoom();
        CSize
            Client = GetClientRect().Size(),
            Preview(Client.cx - (2 * BORDER), Client.cy - (2 * BORDER)),
            Bitmap(bm.bmWidth, bm.bmHeight);
        CPoint p = (m_ZoomState == ZOOM_OUT ? CPoint (0, 0) : 
            GetScrollPosition());
          // resize the document preview window according to bitmap
          // size and zoom level
        Preview.cx = MIN(Preview.cx, int((Bitmap.cx - p.x) * zoom.cx));
        Preview.cy = MIN(Preview.cy, int((Bitmap.cy - p.y) * zoom.cy));
          // set the size of the borders around the document view
         CSize Border(MAX((Client.cx - Preview.cx)  / 2, BORDER),
            MAX((Client.cy - Preview.cy) / 2, BORDER));
          // draw a line on the bitmap around the document page 
        CRect rc(0, 0, Bitmap.cx, Bitmap.cy);
        CRgn rg; rg.CreateRectRgnIndirect(rc);
        dcMem.FrameRgn(rg, HBRUSH(::GetStockObject(BLACK_BRUSH)), 3, 3);
          // Copy from the memory DC to the PreviewPane's DC with 
          // scaling, using HALFTONE anti-aliasing for better quality
        dc.SetStretchBltMode(HALFTONE);
        ::SetBrushOrgEx(dc, Border.cx, Border.cy, NULL);
        dc.StretchBlt(Border.cx, Border.cy, Preview.cx, Preview.cy, 
            dcMem, p.x, p.y,  int(Preview.cx / zoom.cx), 
            int(Preview.cy / zoom.cy), SRCCOPY);
          // draw a grey border around the preview:
        // 1. left stripe down
        CRect rcFill(0, 0, Border.cx, Preview.cy + Border.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          // 2. top stripe across
        rcFill.SetRect(Border.cx, 0, Preview.cx + Border.cx , Border.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          // 3. right stripe down
        rcFill.SetRect(Preview.cx + Border.cx , 0, Client.cx, 
            Client.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          //4.  bottom stripe across
        rcFill.SetRect(0, Preview.cy + Border.cy, Preview.cx + Border.cx, 
            Client.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
        SetFocus();
    }
}

/*============================================================================*/
    BOOL CPreviewPane::
OnEraseBkgnd(CDC& )														/*

    Suppress the background redrawing of the preview pane to avoid flicker.
*-----------------------------------------------------------------------------*/
{
    return TRUE; 
}

/*============================================================================*/
    LRESULT CPreviewPane::
OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam)						/*

    Respond to a horizontal scroll bar event and set the current scroll
    position accordingly. This override is necessary to prevent the
    CScrollView's automatic scroll of the window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(lParam);

    int xNewPos;
    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
      // generate information not available from CScrollView
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);

    switch (LOWORD(wParam))
    {
    case SB_PAGEUP: // clicked the scroll bar left of the scroll box.
        xNewPos = CurrentPos.x - sizePage.cx;
        break;

    case SB_PAGEDOWN: // clicked the scroll bar right of the scroll box.
        xNewPos = CurrentPos.x + sizePage.cx;
        break;

    case SB_LINEUP: // clicked the left arrow.
        xNewPos = CurrentPos.x - sizeLine.cx;
        break;

    case SB_LINEDOWN: // clicked the right arrow.
        xNewPos = CurrentPos.x + sizeLine.cx;
        break;

    case SB_THUMBTRACK: // dragging the scroll box.
        xNewPos = HIWORD(wParam);
        break;

    default:
        xNewPos = CurrentPos.x;
    }

      // set new position
    xNewPos = MAX(0, xNewPos);
    xNewPos = MIN(xNewPos, szTotal.cx - GetClientRect().Width());
    CurrentPos.x = xNewPos;
    SetScrollPosition(CurrentPos);
      // display the offset pane contents
    Invalidate();
    return 0L;
}

/*============================================================================*/
    LRESULT CPreviewPane::
OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam)					/*

    Position the vertical scroll bar using the mouse wheel. This override 
    is necessary to prevent the CScrollView's automatic scroll of the 
    window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(lParam);

    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);

    int WheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    int cyPos = ::MulDiv(WheelDelta, sizeLine.cy, WHEEL_DELTA);

      // set the new position
    int yNewPos = CurrentPos.y - cyPos;
    yNewPos = MIN(yNewPos, szTotal.cy - GetClientRect().Height());
    yNewPos = MAX(yNewPos, 0);
    CurrentPos.y = yNewPos;
    SetScrollPosition(CurrentPos);
      // display the offset pane contents
    Invalidate();
    return 0L;
}

/*============================================================================*/
    LRESULT CPreviewPane::
OnPaint(UINT, WPARAM, LPARAM)											/*

    OnDraw is usually suppressed for controls, but it is needed for this
    one, since it is actually the preview window.
*-----------------------------------------------------------------------------*/
{
    if (::GetUpdateRect(*this, NULL, FALSE))
    {
        CPaintDC dc(*this);
        OnDraw(dc);
    }
    else
    {     // If no region is specified, the RedrawWindow() will repaint
          // the entire window.
        CClientDC dc(*this);
        OnDraw(dc);
    }
      // No more drawing required
    return 0L;
}

/*============================================================================*/
    LRESULT CPreviewPane::
OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam)						/*

    Respond to a vertical scroll bar event and set the current scroll
    position accordingly. This override is necessary to prevent the
    CScrollView's automatic scroll of the window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(lParam);

    int yNewPos;
    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
      // generate information not available from CScrollView
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);
    switch (LOWORD(wParam))
    {
        case SB_PAGEUP: // clicked the scroll bar above the scroll box
            yNewPos = CurrentPos.y - sizePage.cy;
            break;

        case SB_PAGEDOWN: // clicked the scroll bar below the scroll box
            yNewPos = CurrentPos.y + sizePage.cy;
            break;

        case SB_LINEUP: // clicked the top arrow
            yNewPos = CurrentPos.y - sizeLine.cy;
            break;

        case SB_LINEDOWN: // clicked the bottom arrow
            yNewPos = CurrentPos.y + sizeLine.cy;;
            break;

        case SB_THUMBTRACK: // dragging the scroll box
            yNewPos = HIWORD(wParam);
            break;

        default:
            yNewPos = CurrentPos.y;
    }

      // set the new position
    yNewPos = MAX(0, yNewPos);
    yNewPos = MIN( yNewPos, szTotal.cy - GetClientRect().Height() );
    CurrentPos.y = yNewPos;
    SetScrollPosition(CurrentPos);
      // display the offset pane contents
    Invalidate();
    return 0L;
}


/*******************************************************************************

    Implementation of the PreviewSetup class.

*=============================================================================*/
    PreviewSetup::
PreviewSetup(UINT nResID)												/*

    Construct the preview dialog object.
*-----------------------------------------------------------------------------*/
    : CDialog(nResID)
{
}

/*============================================================================*/
	void PreviewSetup::
DoDataExchange(CDataExchange& DX)										/*

	Attach member data items to dialog controls and coordinate the exchange
	and validation of values entered into, and read from, these controls.
*-----------------------------------------------------------------------------*/
{
	CPrintPreview& pvw = GetParent();
	DX.DDX_Control(IDC_PAGE_SETUP,	m_PageSetup);
	DX.DDX_Text(IDC_SCREEN_WIDTH,pvw.m_ScreenInches.cx);
	DX.DDV_MinMaxDouble(pvw.m_ScreenInches.cx, SCREEN_MIN, SCREEN_MAX);
	DX.DDX_Text(IDC_SCREEN_HEIGHT,	pvw.m_ScreenInches.cy);
	DX.DDV_MinMaxDouble(pvw.m_ScreenInches.cy, SCREEN_MIN, SCREEN_MAX);
	DX.DDX_Text(IDC_PREVIEW_WIDTH,	pvw.m_PreviewInches.cx);
	DX.DDV_MinMaxDouble(pvw.m_PreviewInches.cx, PREVIEW_MIN, SCREEN_MAX);
	DX.DDX_Text(IDC_PREVIEW_HEIGHT,	pvw.m_PreviewInches.cy);
	DX.DDV_MinMaxDouble(pvw.m_PreviewInches.cy, PREVIEW_MIN, SCREEN_MAX);
}

/*============================================================================*/
	void PreviewSetup::
InitializeToolTips()														/*

	Add tooltips to the preview buttons.
*-----------------------------------------------------------------------------*/
{
    CreateToolTip(*this);
    AddToolTip(IDC_SCREEN_WIDTH);
    AddToolTip(IDC_SCREEN_HEIGHT);
    AddToolTip(IDC_PREVIEW_WIDTH);
    AddToolTip(IDC_PREVIEW_HEIGHT);
    AddToolTip(IDC_PAGE_SETUP);
	AddToolTip(IDOK);
	AddToolTip(IDCANCEL);
}

/*============================================================================*/
    BOOL    PreviewSetup::
OnInitDialog()															/*

    Perform initializations necessary for the setup dialog to operate
    correctly. Attach tooltips to controls, controls to objects, and 
    deposit initial values in edit controls.
*-----------------------------------------------------------------------------*/
{
 	  // register controls
	UpdateData(m_DX, SENDTOCONTROL);
     // Add tooltips to the preview buttons
	InitializeToolTips();
  	  // update controls
	UpdateData(m_DX, SENDTOCONTROL);
    return TRUE;
}

/*============================================================================*/
    INT_PTR PreviewSetup::
DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)						/*

    The preview setup message processor.
*-----------------------------------------------------------------------------*/
{
    switch (uMsg)
    {
        case WM_SYSCOMMAND:
        {
			switch (LOWORD(wParam))
			{
				case SC_CLOSE: // close the window
					OnCancel();
					return TRUE;
			}
			break;
		}
        
    }
      // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(uMsg, wParam, lParam);
}

/*============================================================================*/
    BOOL    PreviewSetup::
OnCommand(WPARAM wParam, LPARAM)										/*

    Direct command messages to the appropriate processors.
*-----------------------------------------------------------------------------*/
{
    switch (LOWORD(wParam))
    {
        case IDC_PAGE_SETUP:    
            return OnPageSetupButton();
    }
    return FALSE;
}

/*============================================================================*/
    BOOL    PreviewSetup::
OnPageSetupButton()														/*

    If the current dialog values are valid, call the mainframe page setup 
	procedure and reset the preview window appropriately for any changes that 
	might have been made.
*-----------------------------------------------------------------------------*/
{
	if (!UpdateData(m_DX, READFROMCONTROL))
		return TRUE;

     TheApp().TheFrame().SendMessage(WM_COMMAND, IDM_FILE_PRINTSETUP, 0);
      // in case the page setup has changed the preview layout, reset the
      // context and resize as necessary
    GetParent().SetWindowSizes();

    return TRUE;
}

/*============================================================================*/
    void    PreviewSetup::
OnCancel()																/*
                
    Handle the cancel message from the preview setup dialog.
*-----------------------------------------------------------------------------*/
{
    CDialog::OnCancel();
}

/*============================================================================*/
    void    PreviewSetup::
OnOK()																	/*

    Handle the response to the ENTER key. The default closes the dialog. If
	values are invalid, return for correction; otherwise close per the base
	class procedure.
*-----------------------------------------------------------------------------*/
{
      // check the edit control values, which must be positive
	if (!UpdateData(m_DX, READFROMCONTROL))
		return;

    CDialog::OnOK();
}
