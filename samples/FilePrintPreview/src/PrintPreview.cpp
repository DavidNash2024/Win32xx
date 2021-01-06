/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]         (PrintPreview.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CPrintPreview, CPreviewPane,
    and PreviewSetupclasses using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "App.h"
#include "resource.h"
#include "PrintUtil.h"
#include "PrintPreview.h"

/*=============================================================================*

    Program constants                                                       */
  // zoom states
static const int    ZOOM_OUT    = 0;
static const int    ZOOM_WIDTH  = 1;
  // program options
static const int    BORDER      =   20;     // pixels around rendered preview
static const double PREVIEW_MIN =  2.0;     // minimum preview screen dimension
static const double SCREEN_MIN  =  8.0;     // minimum monitor screen dimension
static const double SCREEN_MAX  = 35.0;     // maximum monitor screen dimension
  // Registry key for saving screen and initial print preview sizes. Note:
  // this should match the mainframe's registry key, but with "Software"
  // prefixed, in order to place these entries with the frame's entries.
static const LPCTSTR PREVIEW_REGISTRY_KEY = _T("Software\\")
    _T("Win32++\\FilePrintPreview") _T("\\Sizes");

/*******************************************************************************

    Implementation of the CPrintPreview class.

*=============================================================================*/
    CPrintPreviewEx::
CPrintPreviewEx(DWORD flags /* = HIDE_HELP */ )                             /*

    Construct the preview dialog object.
*-----------------------------------------------------------------------------*/
    : CDialog(IDD_PRINTPREVIEW), m_dcMem(0)
{
    m_currentPage     = 0;
    m_flags           = flags;
    m_screenInches    = DSize(20.25, 11.5);
    m_previewInches   = DSize(8.0, 10.0);
    m_numPreviewPages = 0;
    m_shrink          = 0;
    m_previewPane.SetPaneZoomState(ZOOM_OUT);
 }

/*============================================================================*/
    BOOL CPrintPreviewEx::
ClosePreview()                                                              /*

    Close the preview dialog window and save the screen and initial preview
    window sizes.
*-----------------------------------------------------------------------------*/
{
    SaveSizesRegistry();
    m_previewPane.SetPaneZoomState(ZOOM_OUT);
    Close();
    return TRUE;
}

/*============================================================================*/
    INT_PTR CPrintPreviewEx::
DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)                          /*

    Process special messages for the preview dialog.
*-----------------------------------------------------------------------------*/
{
      // Pass resizing messages on to the resizer
    m_resizer.HandleMessage(msg, wparam, lparam);
    switch (msg)
    {
        case WM_SYSCOMMAND:
        {
            switch (LOWORD(wparam))
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
    return DialogProcDefault(msg, wparam, lparam);
}

/*============================================================================*/
    void CPrintPreviewEx::
DoDataExchange(CDataExchange& dx)                                           /*

    Attach data items to dialog controls and coordinate the exchange between
    the dialog and data values.
*-----------------------------------------------------------------------------*/
{
    dx.DDX_Control(IDC_PREVIEW_PRINT,     m_buttonPrint);
    dx.DDX_Control(IDC_PREVIEW_SETUP,     m_buttonSetup);
    dx.DDX_Control(IDC_PREVIEW_PAGE,      m_editPage);
    dx.DDX_Control(IDC_PREVIEW_FIRST,     m_buttonFirst);
    dx.DDX_Control(IDC_PREVIEW_LAST,      m_buttonLast);
    dx.DDX_Control(IDC_PREVIEW_PREV,      m_buttonPrev);
    dx.DDX_Control(IDC_PREVIEW_NEXT,      m_buttonNext);
    dx.DDX_Control(IDC_PREVIEW_CLOSE,     m_buttonClose);
    dx.DDX_Control(IDC_PREVIEW_PANE,      m_previewPane);
    dx.DDX_Control(IDC_PREVIEW_HELP,      m_buttonPvwHelp);
    dx.DDX_Control(IDC_PREVIEW_ZOOMCOMBO, m_comboZoom);
}

/*============================================================================*/
    void CPrintPreviewEx::
DocPageToBmp(UINT nPage)                                                    /*

    Format the document nPage page for preview and deposit the image in the
    m_dcMem context. This base class method does this for the nPage page
    of the RichView document. Override this method for the particular
    document being previewed.
*-----------------------------------------------------------------------------*/
{
    GetRichView().PrintDC(nPage, m_dcPrinter, m_dcMem);
}

/*============================================================================*/
    void CPrintPreviewEx::
DoPreparePreview()                                                          /*

    Initialize the preview with values other than the defaults, if needed,
    and determine the page divisions and number of pages to be previewed.
    This base class method prepares the preview of a RichEdit docuent.
    Override it to create a printer  device context and a set of preview
    particulars consistent with application needs.
*-----------------------------------------------------------------------------*/
{
    m_numPreviewPages = GetRichView().GetPageBreaks(m_dcPrinter);
}

/*============================================================================*/
    void CPrintPreviewEx::
InitializeContexts()                                                        /*

    Get the current device contexts of the default or currently chosen
    printer and the preview pane and save these as data members. Likewise,
    compute and save the printer and screen resolutions. Compute the shrink
    factor that maps the printer resolution to that of the screen. Create
    the memory context to receive the preview image.
*-----------------------------------------------------------------------------*/
{
      // Get the device context of the default or currently chosen printer.
    CDC dcPreview = m_previewPane.GetDC();
      // compute the screen pixels/inch
    m_screenPixels = CSize(dcPreview.GetDeviceCaps(HORZRES),
        dcPreview.GetDeviceCaps(VERTRES));
      // We will need to create a compatible bitmap in memory for the
      // preview. However, that may be too big for practicality, so we
      // will reduce the size of the memory bitmap from the full printer
      // resolution to fit the resolution of the screen:
    m_shrink = MAX(
        double(m_printerDots.cx) / m_screenPixels.cx,
        double(m_printerDots.cy) / m_screenPixels.cy);
      // Create a memory DC for the printer
    m_dcMem = CMemDC(m_dcPrinter);
      // Create a compatible bitmap in memory for the preview that reduces
      // the size of the memory bitmap.
    CSize viewport((int)(m_printerDots.cx / m_shrink + 0.5),
        (int)(m_printerDots.cy / m_shrink + 0.5));
    m_dcMem.CreateCompatibleBitmap(dcPreview, viewport.cx, viewport.cy);
      // set the mapping mode to translate between printer and screen
      // coordinates to utilize the bitmap dimensions
    m_dcMem.SetMapMode(MM_ANISOTROPIC);
    m_dcMem.SetWindowExtEx(m_printerDots.cx, m_printerDots.cy, NULL);
    m_dcMem.SetViewportExtEx(viewport.cx, viewport.cy, NULL);
}

/*============================================================================*/
    void CPrintPreviewEx::
InitializeControls()                                                        /*

    Load directional button bitmaps, and initiate resizing of the client
    area.
*-----------------------------------------------------------------------------*/
{
      // load the directional button bitmaps
    m_firstPage.LoadBitmap(IDB_PREVIEW_FIRST);
    m_buttonFirst.SetBitmap((HBITMAP)m_firstPage);
    m_prevPage.LoadBitmap(IDB_PREVIEW_PREV);
    m_buttonPrev.SetBitmap((HBITMAP)m_prevPage);
    m_nextPage.LoadBitmap(IDB_PREVIEW_NEXT);
    m_buttonNext.SetBitmap((HBITMAP)m_nextPage);
    m_lastPage.LoadBitmap(IDB_PREVIEW_LAST);
    m_buttonLast.SetBitmap((HBITMAP)m_lastPage);
      // enable resizing the preview pane of the dialog
    m_resizer.Initialize(*this, CRect(0, 0, 0, 0));
    m_resizer.AddChild(m_previewPane,   topleft,
        RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);
}

/*============================================================================*/
    void CPrintPreviewEx::
InitializeToolTips()                                                        /*

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
    void CPrintPreviewEx::
LoadSizesRegistry()                                                         /*

    Load the saved screen and initial preview window size parameters from
    the registry key labelled PREVIEW_REGISTRY_KEY.
*-----------------------------------------------------------------------------*/
{
    CRegKey key;
    CString strKey = PREVIEW_REGISTRY_KEY;
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
    {
        LPTSTR p;
        CString s = RegQueryStringValue(key, _T("Screen Width"));
        m_screenInches.cx = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Screen Height"));
        m_screenInches.cy = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Init Preview Width"));
        m_previewInches.cx = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Init Preview Height"));
        m_previewInches.cy = _tcstod(s, &p);
    }
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnCommand(WPARAM wparam, LPARAM lparam)                                     /*

    Direct the command messages to their processing functions.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(lparam);

    if (HIWORD(wparam) == CBN_SELCHANGE)
        return OnZoomChange();

    switch (LOWORD(wparam))
    {
        case IDC_PREVIEW_PRINT:     return OnPrintButton();
        case IDC_PREVIEW_SETUP:     PreviewAndPageSetup(); return TRUE;
        case IDC_PREVIEW_FIRST:     return OnFirstButton();
        case IDC_PREVIEW_PREV:      return OnPrevButton();
        case IDC_PREVIEW_NEXT:      return OnNextButton();
        case IDC_PREVIEW_LAST:      return OnLastButton();
        case IDC_PREVIEW_CLOSE:     return ClosePreview();
        case IDC_PREVIEW_HELP:      return OnPreviewHelp();
    }
    return FALSE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnInitDialog()                                                              /*

    Attach control IDs to the objects they identify, set up the  automatic
    resizing mechanism, engage tooltips, set the screen and initial preview
    window sizes, initialize the scale combo box values, and set up other
    entities before the dialog becomes visible.
*-----------------------------------------------------------------------------*/
{
      // load saved screen and initial preview window sizes
    LoadSizesRegistry();
      // register controls,  attach controls to numeric identifiers
    UpdateData(m_dx, SENDTOCONTROL);
      // enter initial values into controls
    InitializeControls();
      // add tooltips to the preview buttons
    InitializeToolTips();
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    m_dcPrinter = printDlg.GetPrinterDC();
      // Get the printer resolution,
    m_printerDots = CSize(m_dcPrinter.GetDeviceCaps(HORZRES),
        m_dcPrinter.GetDeviceCaps(VERTRES));
      // fill combo box with scale values
    PopulateScaleBox();
      // If screen and initial preview window sizes are initially out of
      // bounds, prompt the user for entry of sizes that are acceptable.
      // Do not take zero for an answer.
    while (m_screenInches.cx < SCREEN_MIN || m_screenInches.cx > SCREEN_MAX  ||
        m_screenInches.cy < SCREEN_MIN    || m_screenInches.cy > SCREEN_MAX  ||
        m_previewInches.cx < PREVIEW_MIN  || m_previewInches.cx > SCREEN_MAX ||
        m_previewInches.cy < PREVIEW_MIN  || m_previewInches.cy > SCREEN_MAX)
    {
        if (!PreviewAndPageSetup())
        {
            ClosePreview();
            return TRUE;
        }
    }
    SetWindowSizes();
      // hide the help button if so indicated
    if (m_flags & HIDE_HELP)
        m_buttonPvwHelp.ShowWindow(SW_HIDE);
      // start at page 1
    m_currentPage = 0;
      // update controls
    UpdateData(m_dx, SENDTOCONTROL);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnNextButton()                                                              /*

    Display the next page of the document. This method can only be called
    when there is a valid next page to view.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(++m_currentPage);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreviewEx::
OnOK()                                                                      /*

    This method is invoked when the ENTER key is pressed, and the base class
    response is to close the dialog. However, the desired behavior for this
    dialog is either to go to the page appearing in the page box when this box
    has the focus, or otherwise to ignore the ENTER key altogether..
*-----------------------------------------------------------------------------*/
{
    HWND hwnd = HWND(::GetFocus());
    UINT id = ::GetDlgCtrlID(hwnd);
      // if the control being activated is the page box, go to that page
    if (id == IDC_PREVIEW_PAGE)
    {
        CString sPage = m_editPage.GetWindowText();
        TCHAR *stop;
        UINT nPage = _tcstol(sPage, &stop, 10);
        nPage = MIN(MAX(1, nPage), m_numPreviewPages);
        OnPreviewPage(m_currentPage = nPage - 1);
    }
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnFirstButton()                                                             /*

    Display the first page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(m_currentPage = 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnLastButton()                                                              /*

    Display the last page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(m_currentPage = m_numPreviewPages - 1);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreviewEx::
OnPrepareDC()                                                               /*

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
    void CPrintPreviewEx::
OnPreparePrintPreview()                                                     /*

    Prepare to preview the document printout. Called by OnPreview() before
    a document is previewed. Create the print preview dialog window using
    the desktop window as owner, so the preview window is not hidden by
    the document view window. Override this function to specialize the
    preview for the application particulars.
*-----------------------------------------------------------------------------*/
{
    if (!IsWindow())
        Create(::GetDesktopWindow());
      // show the document path in the caption title
    SetWindowText(m_docPath);
      // set special device contexts, determine pagination, and max pages
    DoPreparePreview();
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnPrevButton()                                                              /*

    Display the previous page of the document. This method can only be
    called when there is a valid previous page to view.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(--m_currentPage);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnPreview(const CString &docPath)                                           /*

    Display the preview pages of the document.
*-----------------------------------------------------------------------------*/
{
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    HDC hPrinter = printDlg.GetPrinterDC();
    if (hPrinter == 0)
    {
        MessageBox(_T("Print preview requires a printer to copy settings from"),
            _T("No Printer found"), MB_ICONWARNING);
        return FALSE;
    }
      // save the doument path
    m_docPath = docPath;
      // set up device contexts, determine pagination, and number of pages
    OnPreparePrintPreview();
      // Preview the first page;
    OnPreviewPage(m_currentPage = 0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnPreviewHelp()                                                             /*

    Respond to requests for help on the print preview function.
*-----------------------------------------------------------------------------*/
{
    MessageBox(_T("Preview help not yet available."),
        _T("Information..."), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreviewEx::
OnPreviewPage(UINT nPage)                                                   /*

*-----------------------------------------------------------------------------*/
{
    OnPrepareDC();
    PreviewPage(m_currentPage = nPage);
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnPrintButton()                                                             /*

    Respond to requests for printing the document.
*-----------------------------------------------------------------------------*/
{
    GetFrame().PostMessage(WM_COMMAND, IDM_FILE_PRINT, 0);
    ClosePreview();
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnZoomChange()                                                              /*

    Set the zoom value for the display of the document page.
*-----------------------------------------------------------------------------*/
{
      // query the scale combo box selection
    int selection = m_comboZoom.GetCurSel();
      // if it is one of the first two entries, use that for the zoom
    if (selection <= ZOOM_WIDTH)
        m_previewPane.SetPaneZoomState(selection);
    else // it is a numeric scaling, figure it out
    {
        TCHAR val[20];
        TCHAR *stop;
        m_comboZoom.GetLBText(selection, val);
        m_previewPane.SetPaneZoomState(_tcstol(val, &stop, 10));
    }
    m_previewPane.ShowScrollBars(selection == ZOOM_OUT ?
        FALSE : TRUE);
    m_previewPane.SetScrollSizes(CSize(0, 0));
    m_previewPane.Invalidate();
    return TRUE;
}

/*============================================================================*/
    void CPrintPreviewEx::
PopulateScaleBox()                                                          /*

    Fill the scale combo box with the zoom settings.
*-----------------------------------------------------------------------------*/
{
    std::vector<CString> scale;   // scale selection choices
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
    m_comboZoom.ResetContent();
    for (UINT i = 0; i < scale.size(); i++)
        m_comboZoom.AddString(scale[i]);
    m_comboZoom.SetCurSel(0);
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
PreviewAndPageSetup()                                                       /*

    Prompt for user input of screen and initial preview window sizes. Do
    printer page setup if requested. Return TRUE if the setup dialog was
    terminated in OK, or FALSE otherwise.
*-----------------------------------------------------------------------------*/
{
    if (m_setupDlg.DoModal(*this) == IDOK)
    {
        SetWindowSizes();
        return TRUE;
    }
    return FALSE;
}

/*============================================================================*/
    void CPrintPreviewEx::
PreviewPage(UINT nPage)                                                     /*

    Display page numbered nPage to the screen (view port).
*-----------------------------------------------------------------------------*/
{
      // check validity of request
    assert(m_numPreviewPages > 0);
    assert(nPage < m_numPreviewPages);
      // Fill the bitmap with a white background
    CRect rc(0, 0, m_printerDots.cx, m_printerDots.cy);
    m_dcMem.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
      // render the nPage of the document into dcMem
    DocPageToBmp(nPage);
      // transfer the bitmap from the memory DC to the preview pane
    CBitmap Bitmap = m_dcMem.DetachBitmap();
    m_previewPane.SetBitmap(Bitmap);
      // reset the current status of the preview dialog's buttons
    UpdateButtons();
      // display the print preview
    m_previewPane.Invalidate();
}

/*============================================================================*/
    CString CPrintPreviewEx::
RegQueryStringValue(CRegKey &key, LPCTSTR name)                             /*

    Return the CString value of a specified value name found in the
    currently open registry key.
*-----------------------------------------------------------------------------*/
{
    ULONG len = 256;
    CString sValue;
    if (ERROR_SUCCESS == key.QueryStringValue(name, sValue.GetBuffer(255), &len))
    {
        sValue.ReleaseBuffer();
        return sValue;
    }
    else
        return _T("");
}

/*============================================================================*/
    void CPrintPreviewEx::
SaveSizesRegistry()                                                         /*

    Write the screen and initial preview size values into the registry key
    labeled PREVIEW_REGISTRY_KEY.
*-----------------------------------------------------------------------------*/
{
    CString strKey = PREVIEW_REGISTRY_KEY;
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, NULL);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        CString s;
        s.Format(_T("% .2f"),  m_screenInches.cx);
        key.SetStringValue(_T("Screen Width"), s.c_str());
        s.Format(_T("%.2f"),  m_screenInches.cy);
        key.SetStringValue(_T("Screen Height"), s.c_str());
        s.Format(_T("%.2f"),  m_previewInches.cx);
        key.SetStringValue(_T("Init Preview Width"), s.c_str());
        s.Format(_T("%.2f"),  m_previewInches.cy);
        key.SetStringValue(_T("Init Preview Height"), s.c_str());
    }
}

/*============================================================================*/
    void CPrintPreviewEx::
SetWindowSizes()                                                            /*

    Set the screen and initial preview window sizes using the current
    values of the m_screenInches and m_previewInches size members.
*-----------------------------------------------------------------------------*/
{
      // make sure the contexts are current
    InitializeContexts();
      // determine the printer dots/inch (this works for the printer!)
    DSize PrinterPPI(m_dcPrinter.GetDeviceCaps(LOGPIXELSX),
        m_dcPrinter.GetDeviceCaps(LOGPIXELSY));
      // compute the screen m_screenPixels/inch
    DSize ScreenPPI((m_screenPixels.cx / m_screenInches.cx),
        (m_screenPixels.cy / m_screenInches.cy));
    m_printerScreenRatio = DSize((PrinterPPI.cx / ScreenPPI.cx) / m_shrink,
        (PrinterPPI.cy / ScreenPPI.cy) / m_shrink);
      // compute the initial preview size, in pixels
    CSize frame(int(m_previewInches.cx * ScreenPPI.cx),
        int(m_previewInches.cy * ScreenPPI.cy));
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
    void CPrintPreviewEx::
UpdateButtons()                                                             /*

    Enable or disable buttons, according to current page visible.
*-----------------------------------------------------------------------------*/
{
    UINT end_page = m_numPreviewPages;
    m_buttonFirst.EnableWindow(m_currentPage > 0);
    m_buttonPrev.EnableWindow(m_currentPage  > 0);
    m_buttonNext.EnableWindow(m_currentPage  < end_page - 1);
    m_buttonLast.EnableWindow(m_currentPage  < end_page - 1);
    CString page;
    page.Format(_T("%d"), m_currentPage + 1);
    m_editPage.SetWindowText(page);
    page.Format(_T(" of %d"), end_page);
    SetDlgItemText(IDC_PREVIEW_OFPAGES, page.c_str());
}

/*******************************************************************************

    Implementation of the CPreviewPane class.

*=============================================================================*/
    CPreviewPaneEx::
CPreviewPaneEx()                                                            /*

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
    if (!::GetClassInfo(TheApp()->GetInstanceHandle(), ClassName, &wc))
    {
        wc.lpszClassName = ClassName;
        wc.lpfnWndProc   = ::DefWindowProc;
        wc.hInstance     = TheApp()->GetInstanceHandle();
        wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
        wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
        ::RegisterClass(&wc);
    }
    assert(::GetClassInfo(TheApp()->GetInstanceHandle(), ClassName, &wc));
    SetScrollSizes(m_scrollSize = m_prevWindowSize = CSize(0, 0));
    m_zoomState      = ZOOM_OUT;
    m_prevZoomState  = -1;
    m_showScrollBars = FALSE;
}

/*============================================================================*/
    DSize CPreviewPaneEx::
GetZoom()                                                                   /*

    Return the zoom ratio of the view that will contain the rendered
    document bitmap, under the current zoom state. Zoom is defined as the
    scaled image size divided by the bitmap size. The image size is the
    client area with a border around it. Set the scroll size to fit this
    zoom value.
*-----------------------------------------------------------------------------*/
{
    if (!m_bitmap.GetHandle())
        return DSize(0.0, 0.0);

      // The unit_zoom is meant to provide a screen image equal in size to
      // the printer page size. However, some difference may be seen on
      // some monitors due to roundoff.
    DSize printerScreenRatio = GetPreviewWnd().GetPrinterScreenRatio();
    DSize unit_zoom = DSize(1.0 / printerScreenRatio.cx,
        1.0 / printerScreenRatio.cx);
      // get client, bitmap, and preview window sizes
    BITMAP bm = m_bitmap.GetBitmapData();
    CSize Client = GetClientRect().Size(), Bitmap(bm.bmWidth, bm.bmHeight),
          Preview(Client.cx - (2 * BORDER), Client.cy - (2 * BORDER));
      // compute the zoom value
    DSize zoom = DSize(double(Preview.cx) / double(Bitmap.cx),
            double(Preview.cy) / double(Bitmap.cy));
    if (m_zoomState == ZOOM_OUT)
    {
        double min = MIN(zoom.cx, zoom.cy);
        zoom = DSize(min, min);
    }
      // see the notes below for the rationale of this value
    else if (m_zoomState == ZOOM_WIDTH)
    {
        zoom.cy = zoom.cx;
    }
    else
        zoom = DSize(unit_zoom.cx * m_zoomState / 100.0,
            unit_zoom.cy * m_zoomState / 100.0);

      // compute the scroll sizes
    CSize ScrollSize(0, 0); // default to ZOOM_OUT and no scroll bars case
    if (m_zoomState != ZOOM_OUT && m_showScrollBars)
    {
          // Compute the scroll sizes for this zoom (see notes below).
          // The zoom factor zf is
        DSize zf(1.0 / zoom.cx - 1.0, 1.0 / zoom.cy - 1.0);
        ScrollSize = CSize(m_zoomState == ZOOM_WIDTH ? 0 :
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
        if (m_prevZoomState != m_zoomState ||
            (ScrollSize != m_scrollSize && (m_zoomState != ZOOM_WIDTH ||
            (m_prevWindowSize != WindowSize))))
                SetScrollSizes(m_scrollSize = ScrollSize);
        m_prevWindowSize = WindowSize;
    }
    m_prevZoomState = m_zoomState;
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
    void CPreviewPaneEx::
OnDraw(CDC& dc)                                                             /*

    Copy the bitmap (m_bitmap) into the PreviewPane, scaling the image
    to fit the window. The dc is the CPaintDC of the screen. The zoom factor
    applied is the scaling of target size to source size. The three factors
    available range from fitting the entire bitmap page onto the rendered
    page (the zoom-out case), to rendering a fuller scale view of perhaps
    only a portion of the bitmap into the available screen area.
*-----------------------------------------------------------------------------*/
{
    if (m_bitmap.GetHandle())
    {
        CMemDC dcMem(dc);
        dcMem.SelectObject(m_bitmap);
        dcMem.SetMapMode(MM_TEXT);
        BITMAP bm = m_bitmap.GetBitmapData();
          // determine the size of the PreviewPane window with a border
          // around the area used to show the bitmap
        DSize zoom  = GetZoom();
        CSize
            Client = GetClientRect().Size(),
            Preview(Client.cx - (2 * BORDER), Client.cy - (2 * BORDER)),
            Bitmap(bm.bmWidth, bm.bmHeight);
        CPoint p = (m_zoomState == ZOOM_OUT ? CPoint (0, 0) :
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
        dc.SetBrushOrgEx(0, 0);
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
    BOOL CPreviewPaneEx::
OnEraseBkgnd(CDC& )                                                         /*

    Suppress the background redrawing of the preview pane to avoid flicker.
*-----------------------------------------------------------------------------*/
{
    return TRUE;
}

/*============================================================================*/
    LRESULT CPreviewPaneEx::
OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam)                           /*

    Respond to a horizontal scroll bar event and set the current scroll
    position accordingly. This override is necessary to prevent the
    CScrollView's automatic scroll of the window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    int xNewPos;
    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
      // generate information not available from CScrollView
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);

    switch (LOWORD(wparam))
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
        xNewPos = HIWORD(wparam);
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
    LRESULT CPreviewPaneEx::
OnMouseWheel(UINT msg, WPARAM wparam, LPARAM lparam)                        /*

    Position the vertical scroll bar using the mouse wheel. This override
    is necessary to prevent the CScrollView's automatic scroll of the
    window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);

    int WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);
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
    LRESULT CPreviewPaneEx::
OnPaint(UINT, WPARAM, LPARAM)                                               /*

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
    LRESULT CPreviewPaneEx::
OnVScroll(UINT msg, WPARAM wparam, LPARAM lparam)                           /*

    Respond to a vertical scroll bar event and set the current scroll
    position accordingly. This override is necessary to prevent the
    CScrollView's automatic scroll of the window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);

    int yNewPos;
    CPoint CurrentPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
      // generate information not available from CScrollView
    CSize sizePage(szTotal.cx / 10, szTotal.cy / 10);
    CSize sizeLine(szTotal.cx / 10, szTotal.cy / 10);
    switch (LOWORD(wparam))
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
            yNewPos = HIWORD(wparam);
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
    void PreviewSetup::
DoDataExchange(CDataExchange& dx)                                           /*

    Attach member data items to dialog controls and coordinate the exchange
    and validation of values entered into, and read from, these controls.
*-----------------------------------------------------------------------------*/
{
    CPrintPreviewEx& pvw = GetPreviewWnd();
    dx.DDX_Control(IDC_PAGE_SETUP,  m_pageSetup);
    dx.DDX_Text(IDC_SCREEN_WIDTH,   pvw.m_screenInches.cx);
    dx.DDV_MinMaxDouble(pvw.GetScreenSize().cx, SCREEN_MIN, SCREEN_MAX);
    dx.DDX_Text(IDC_SCREEN_HEIGHT,  pvw.m_screenInches.cy);
    dx.DDV_MinMaxDouble(pvw.GetScreenSize().cy, SCREEN_MIN, SCREEN_MAX);
    dx.DDX_Text(IDC_PREVIEW_WIDTH,  pvw.m_previewInches.cx);
    dx.DDV_MinMaxDouble(pvw.GetPreviewSize().cx, PREVIEW_MIN, SCREEN_MAX);
    dx.DDX_Text(IDC_PREVIEW_HEIGHT, pvw.m_previewInches.cy);
    dx.DDV_MinMaxDouble(pvw.GetPreviewSize().cy, PREVIEW_MIN, SCREEN_MAX);
}

/*============================================================================*/
    void PreviewSetup::
InitializeToolTips()                                                        /*

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
OnInitDialog()                                                              /*

    Perform initializations necessary for the setup dialog to operate
    correctly. Attach tooltips to controls, controls to objects, and
    deposit initial values in edit controls.
*-----------------------------------------------------------------------------*/
{
      // register controls
    UpdateData(m_dx, SENDTOCONTROL);
     // Add tooltips to the preview buttons
    InitializeToolTips();
      // update controls
    UpdateData(m_dx, SENDTOCONTROL);
      // save incoming sizes in case of cancel
    m_inScreenInches  = GetPreviewWnd().GetScreenSize();
    m_inPreviewInches = GetPreviewWnd().GetPreviewSize();
    return TRUE;
}

/*============================================================================*/
    INT_PTR PreviewSetup::
DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)                          /*

    The preview setup message processor.
*-----------------------------------------------------------------------------*/
{
    switch (msg)
    {
        case WM_SYSCOMMAND:
        {
            switch (LOWORD(wparam))
            {
                case SC_CLOSE: // close the window
                    OnCancel();
                    return TRUE;
            }
            break;
        }

    }
      // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

/*============================================================================*/
    BOOL    PreviewSetup::
OnCommand(WPARAM wparam, LPARAM)                                            /*

    Direct command messages to the appropriate processors.
*-----------------------------------------------------------------------------*/
{
    switch (LOWORD(wparam))
    {
        case IDC_PAGE_SETUP:
            return OnPageSetupButton();
    }
    return FALSE;
}

/*============================================================================*/
    BOOL    PreviewSetup::
OnPageSetupButton()                                                         /*

    If the current dialog values are valid, call the mainframe page setup
    procedure and reset the preview window appropriately for any changes that
    might have been made.
*-----------------------------------------------------------------------------*/
{
    if (!UpdateData(m_dx, READFROMCONTROL))
        return TRUE;

     GetFrame().SendMessage(WM_COMMAND, IDM_FILE_PRINTSETUP, 0);
      // in case the page setup has changed the preview layout, reset the
      // context and resize as necessary
    GetPreviewWnd().SetWindowSizes();

    return TRUE;
}

/*============================================================================*/
    void    PreviewSetup::
OnCancel()                                                                  /*

    Handle the cancel message from the preview setup dialog.
*-----------------------------------------------------------------------------*/
{
    GetPreviewWnd().SetScreenSize(m_inScreenInches);
    GetPreviewWnd().SetPreviewSize(m_inPreviewInches);

    CDialog::OnCancel();
}

/*============================================================================*/
    void    PreviewSetup::
OnOK()                                                                      /*

    Handle the response to the ENTER key. The default closes the dialog. If
    values are invalid, return for correction; otherwise close per the base
    class procedure.
*-----------------------------------------------------------------------------*/
{
      // check the edit control values, which must be positive
    if (!UpdateData(m_dx, READFROMCONTROL))
        return;

    CDialog::OnOK();
}
/*----------------------------------------------------------------------------*/