/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]         (PrintPreview.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the CPrintPreview and CPreviewPane
    classes using the Win32++ Windows interface classes.

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
static const int    FIT_PAGE    = 0;
static const int    FIT_WIDTH   = 1;
  // program options
static const int    BORDER      =   20;     // pixels around rendered preview
static const double PREVIEW_MIN =  2.0;     // minimum preview screen inches
  // Registry key for saving print preview sizes. Note: this should match the
  // mainframe's registry key, but with "Software" prefixed, in order to place
  // these entries with the frame's entries.
static const LPCTSTR PREVIEW_REGISTRY_KEY = _T("Software\\")
    _T("Win32++\\FilePrintPreview") _T("\\Sizes");

/*******************************************************************************

    Implementation of the CPrintPreview class.

*=============================================================================*/
    CPrintPreviewEx::
CPrintPreviewEx(DWORD flags /* = HIDE_HELP */ )                             /*

    Construct the preview dialog object. Use a nominal screen size, to be
    adjusted later for the actual monitor being used.
*-----------------------------------------------------------------------------*/
    : CDialog(IDD_PRINTPREVIEW), m_dcMem(0)
{
    m_previewInches   = DSize(8.0, 10.0);
    m_currentPage     = 0;
    m_flags           = flags;
    m_numPreviewPages = 0;
    m_previewPane.SetPaneZoomState(FIT_PAGE);
 }

/*============================================================================*/
    BOOL CPrintPreviewEx::
ClosePreview()                                                              /*

    Close the preview dialog window and save the screen and initial preview
    window sizes.
*-----------------------------------------------------------------------------*/
{
    SaveSizesRegistry();
    m_previewPane.SetPaneZoomState(FIT_PAGE);
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
      // We will need to create a compatible bitmap in memory for the
      // preview. However, that may be too big for practicality, so we
      // will reduce the size of the memory bitmap from the full printer
      // resolution to fit the resolution of the screen:
    DSize shrink(DSize(m_screenPPI) / m_printerPPI);
      // Create a memory DC for the printer
    m_dcMem = CMemDC(m_dcPrinter);
      // Create a bitmap in memory for the preview compatible with the
      // printer DC that reduces the size of the memory bitmap.
    CSize viewport(ToCSize(DSize(m_printerDots) * shrink));
    m_dcMem.CreateCompatibleBitmap(m_dcPrinter, viewport.cx, viewport.cy);
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
    m_resizer.AddChild(m_previewPane, CResizer::topleft,
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
        CString s = RegQueryStringValue(key, _T("Init preview Width"));
        m_previewInches.cx = _tcstod(s, &p);
        s = RegQueryStringValue(key, _T("Init preview Height"));
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
      // get the printer context, no collation and a single copy only
    CPrintDialog printDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
    m_dcPrinter = printDlg.GetPrinterDC();
      // Get the printer resolution,
    m_printerDots = CSize(m_dcPrinter.GetDeviceCaps(PHYSICALWIDTH),
                          m_dcPrinter.GetDeviceCaps(PHYSICALHEIGHT));
      // determine the printer dots/inch
    m_printerPPI = GetPPI(m_dcPrinter);
      // Get the device context of the default or currently chosen printer.
    CClientDC dcPreview = m_previewPane.GetDC();
      // compute the screen pixels
    m_screenPixels = CSize(dcPreview.GetDeviceCaps(HORZRES),
                           dcPreview.GetDeviceCaps(VERTRES));
      // compute the screen size, in inches
    m_screenInches = DSize(dcPreview.GetDeviceCaps(HORZSIZE),
                           dcPreview.GetDeviceCaps(VERTSIZE)) * IN_PER_MM;
      // compute the screen pixels / inch
    m_screenPPI = GetPPI(dcPreview);
     // fill combo box with scale values
    PopulateScaleBox();
      // If the initial preview window size is initially out of bounds,
      // limit to acceptable values.
    m_previewInches.cx = MAX(PREVIEW_MIN,
        MIN(m_previewInches.cx, m_screenInches.cx));
    m_previewInches.cy = MAX(PREVIEW_MIN,
        MIN(m_previewInches.cy, m_screenInches.cy));
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
        OnPreviewPage(nPage - 1);
    }
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnFirstButton()                                                             /*

    Display the first page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnLastButton()                                                              /*

    Display the last page of the document.
*-----------------------------------------------------------------------------*/
{
    OnPreviewPage(m_numPreviewPages - 1);
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
        ::MessageBox(0, _T("Print preview requires a printer to copy settings from"),
            _T("No Printer found"), MB_ICONWARNING);
        return FALSE;
    }
      // save the doument path
    m_docPath = docPath;
      // set up device contexts, determine pagination, and number of pages
    OnPreparePrintPreview();
      // preview the first page;
    OnPreviewPage(0);
    return TRUE;
}

/*============================================================================*/
    BOOL CPrintPreviewEx::
OnPreviewHelp()                                                             /*

    Respond to requests for help on the print preview function.
*-----------------------------------------------------------------------------*/
{
    MessageBox(_T("preview help has not been provided."),
        _T("Information..."), MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
    return TRUE;
}

/*============================================================================*/
    void CPrintPreviewEx::
OnPreviewPage(UINT nPage)                                                   /*

    Display page numbered nPage to the screen(view port).
*-----------------------------------------------------------------------------*/
{
    OnPrepareDC();
    m_currentPage = nPage;
    // check validity of request
    assert(m_numPreviewPages > 0);
    assert(nPage < m_numPreviewPages);
    // Fill the bitmap with a white background
    CRect rc(0, 0, m_printerDots.cx, m_printerDots.cy);
    m_dcMem.FillRect(rc, (HBRUSH)::GetStockObject(WHITE_BRUSH));
    // render the nPage of the document into dcMem
    DocPageToBmp(nPage);
    // transfer the bitmap from the memory DC to the preview pane
    CBitmap bitmap = m_dcMem.DetachBitmap();
    m_previewPane.SetBitmap(bitmap);
    // reset the current status of the preview dialog's buttons
    UpdateButtons();
    // display the print preview
    m_previewPane.Invalidate();
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
    if (selection <= FIT_WIDTH)
        m_previewPane.SetPaneZoomState(selection);
    else // it is a numeric scaling, figure it out
    {
        TCHAR val[20];
        TCHAR *stop;
        m_comboZoom.GetLBText(selection, val);
        m_previewPane.SetPaneZoomState(_tcstol(val, &stop, 10));
    }
    m_previewPane.ShowScrollBars(selection == FIT_PAGE ?
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

    Write the current preview size value into the registry key
    labeled PREVIEW_REGISTRY_KEY.
*-----------------------------------------------------------------------------*/
{
      // get the current preview size
    m_previewInches = DSize(GetWindowRect().Size()) / m_screenPPI;
      // save the size in the registry
    CString strKey = PREVIEW_REGISTRY_KEY;
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, NULL);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        CString s;
        s.Format(_T("%.2f"),  m_previewInches.cx);
        key.SetStringValue(_T("Init preview Width"), s.c_str());
        s.Format(_T("%.2f"),  m_previewInches.cy);
        key.SetStringValue(_T("Init preview Height"), s.c_str());
    }
}

/*============================================================================*/
    void CPrintPreviewEx::
SetWindowSizes()                                                            /*

    Set the preview window size using the current m_previewInches size.
*-----------------------------------------------------------------------------*/
{
       // make sure the contexts are current
    InitializeContexts();
      // compute the initial preview size, in pixels
    CSize frame(int(m_previewInches.cx * m_screenPPI.cx),
                int(m_previewInches.cy * m_screenPPI.cy));
      // set the dialog size
    CSize preview(MIN(frame.cx, m_screenPixels.cx),
                  MIN(frame.cy, m_screenPixels.cy));
      // center the preview window on the screen
    CSize ctr((m_screenPixels.cx - preview.cx) / 2,
              (m_screenPixels.cy - preview.cy) / 2);
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
      // resource.rc must match this name.
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
    m_zoomState      = FIT_PAGE;
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

      // get client, bitmap, and preview window sizes
    BITMAP bm = m_bitmap.GetBitmapData();
    CSize client = GetClientRect().Size(), bitmap(bm.bmWidth, bm.bmHeight),
          preview(client.cx - (2 * BORDER), client.cy - (2 * BORDER));
      // compute the zoom value
    DSize zoom = DSize(double(preview.cx) / double(bitmap.cx),
            double(preview.cy) / double(bitmap.cy));
    if (m_zoomState == FIT_PAGE)
    {
        double min = MIN(zoom.cx, zoom.cy);
        zoom = DSize(min, min);
    }
      // see the notes below for the rationale of this value
    else if (m_zoomState == FIT_WIDTH)
    {
        zoom.cy = zoom.cx;
    }
    else
    {     // a unit zoom nominally provides a screen image equal in size to
          // the printer page size. However, some slight difference may
          // occur on some monitors.
        zoom = DSize(m_zoomState / 100.0, m_zoomState / 100.0);
    }
      // compute the scroll sizes
    CSize scrollSize(0, 0); // default to FIT_PAGE and no scroll bars case
    if (m_zoomState != FIT_PAGE && m_showScrollBars)
    {
          // Compute the scroll sizes for this zoom (see notes below).
          // The zoom factor zf is
        DSize zf(1.0 / zoom.cx - 1.0, 1.0 / zoom.cy - 1.0);
        scrollSize = CSize(m_zoomState == FIT_WIDTH ? 0 :
            MAX(0, bitmap.cx + 2 * BORDER - static_cast<int>(preview.cx * zf.cx)),
            MAX(0, bitmap.cy + 2 * BORDER - static_cast<int>(preview.cy * zf.cy)));
          // Reset the scrolling sizes only if the bars are visible
          // and either (1) the zoom state changed, or (2) the size
          // has chanted and the scaling is not FIT_WIDTH, or, (3)
          // when scaling FIT_WIDTH, when the window has also changed
          // size. During FIT_WIDTH, the preview width changes as
          // scrollbars are added, and this would cause change in
          // the zoom, which would cause resizing and screen flicker.
        CSize WindowSize = GetWindowRect().Size();
        if (m_prevZoomState != m_zoomState ||
            (scrollSize != m_scrollSize && (m_zoomState != FIT_WIDTH ||
            (m_prevWindowSize != WindowSize))))
                SetScrollSizes(m_scrollSize = scrollSize);
        m_prevWindowSize = WindowSize;
    }
    m_prevZoomState = m_zoomState;
    return zoom;

/*  Notes on scroll sizing: At a that scroll position p (.x or .y), at
*   which the pixel at the bottom-right of the preview (.cx or .cy), with
*   scaling, is the final one in the bitmap (.cx or .cy), the p value
*   (call it pos0) must satisfy
*
*       (bitmap - pos0) * zoom = preview
*
*   or  pos0 = bitmap - preview / zoom
*
*   The total scroll size required to achieve this will then be
*
*       scrollSize = pos0 + preview + 2 * Border
*              = BitMap + 2 * Border - preview * (1 / zoom - 1)
*
*   We may also note that the zoom value that gives a pos0 value of zero
*   will be
*
*       zoom_out = preview / bitmap
*
*   Since two dimensions are involved, the one that must be used is the
*   least, so that both dimensions fit on one preview screen.  No zoom
*   value less than this can achieve a full-preview view. Choosing the
*   .cx value of this gives the zoom-to-width scale
*
*       zoom_width = preview.cx / bitmap.cx
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
    only a portion of the bitmap into the available screen area. The given
    bitmap data is inserted into a device-independent bitmap (DIB) for display.
*-----------------------------------------------------------------------------*/
{
    if (m_bitmap.GetHandle())
    {
        BITMAP bm = m_bitmap.GetBitmapData();
          // determine the size of the PreviewPane window with a border
          // around the area used to show the bitmap
        DSize zoom = GetZoom();
        CSize
            client = GetClientRect().Size(),
            preview(client.cx - (2 * BORDER), client.cy - (2 * BORDER)),
            bitmap(bm.bmWidth, bm.bmHeight);
          // compute the scroll position p for the page
        CPoint p(0, 0);
        if (m_zoomState != FIT_PAGE)
        {     // DIB origin is the bottom of the bitmap, whereas the
              // scroll position is at the top. So make the adjustment.
            int maxPosY = GetTotalScrollSize().cy - client.cy;
            p = GetScrollPosition();
            p.y = MAX(maxPosY - p.y, 0);
        }
          // resize the document preview window according to bitmap
          // size, scroll position, and zoom level
        preview.cx = MIN(preview.cx, int((bitmap.cx - p.x) * zoom.cx));
        preview.cy = MIN(preview.cy, int((bitmap.cy - p.y) * zoom.cy));

          // set the size of the borders around the document view
        CSize Border(MAX((client.cx - preview.cx) / 2, BORDER),
                     MAX((client.cy - preview.cy) / 2, BORDER));
          // Process the bitmap as a device-independent structure: first,
          // locate and access the bitmap information
        CBitmapInfoPtr pbmi(m_bitmap);
        BITMAPINFOHEADER* pBIH = reinterpret_cast<BITMAPINFOHEADER*>
            (pbmi.get());
          // Extract the m_bitmap information into a device independent image
          // data format (DIB).
        CMemDC dcMem(dc);
        dcMem.GetDIBits(m_bitmap, 0, bm.bmHeight, NULL, pbmi, DIB_RGB_COLORS);
        std::vector<byte> byteArray(pBIH->biSizeImage, 0);
        byte* pByteArray = &byteArray.front();
        dcMem.GetDIBits(m_bitmap, 0, bm.bmHeight, pByteArray, pbmi,
            DIB_RGB_COLORS);
          // draw a line on the bitmap around the document page
        CRect rc(0, 0, bitmap.cx, bitmap.cy);
        CRgn rg; rg.CreateRectRgnIndirect(rc);
        dcMem.FrameRgn(rg, HBRUSH(::GetStockObject(BLACK_BRUSH)), 3, 3);
          // Copy from the memory DC to the PreviewPane's DC with
          // scaling, using HALFTONE anti-aliasing for better quality
        dc.SetStretchBltMode(HALFTONE);
        dc.SetBrushOrgEx(0, 0);
          // Stretch fit the device-independent bitmap into the preview area
        dc.StretchDIBits(Border.cx, Border.cy, preview.cx, preview.cy,
            p.x, p.y,  int(preview.cx / zoom.cx), int(preview.cy / zoom.cy),
            pByteArray, pbmi, DIB_RGB_COLORS, SRCCOPY);
          // draw a gray border around the preview:
        // 1. left stripe down
        CRect rcFill(0, 0, Border.cx, preview.cy + Border.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          // 2. top stripe across
        rcFill.SetRect(Border.cx, 0, preview.cx + Border.cx , Border.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          // 3. right stripe down
        rcFill.SetRect(preview.cx + Border.cx , 0, client.cx,
            client.cy);
        dc.FillRect(rcFill, HBRUSH(::GetStockObject(GRAY_BRUSH)));
          //4.  bottom stripe across
        rcFill.SetRect(0, preview.cy + Border.cy, preview.cx + Border.cx,
            client.cy);
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

    Override the base class method to respond to a horizontal scroll bar event
    and set the current bitmap scrollposition accordingly. This override is
    necessary to prevent the CScrollView's automatic scroll of window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);
      // retrieve scroll parameters
    CPoint newPos = GetScrollPosition();
    CSize szTotal = GetTotalScrollSize();
    CSize sizePage = GetPageScrollSize();
    CSize sizeLine = GetLineScrollSize();
      // respond to the event
    switch (LOWORD(wparam))
    {
    case SB_PAGEUP: // clicked the scroll bar left of the scroll box.
        newPos.x -= sizePage.cx;
        break;

    case SB_PAGEDOWN: // clicked the scroll bar right of the scroll box.
        newPos.x += sizePage.cx;
        break;

    case SB_LINEUP: // clicked the left arrow.
        newPos.x -= sizeLine.cx;
        break;

    case SB_LINEDOWN: // clicked the right arrow.
        newPos.y += sizeLine.cx;
        break;

    case SB_THUMBTRACK: // dragging the scroll box.
        newPos.x = HIWORD(wparam);
        break;

    default:
       break;
    }
      // set new position
    int maxPosX = szTotal.cx - GetClientRect().Width();
    newPos.x = MIN(newPos.x, maxPosX);
    newPos.x = MAX(0, newPos.x);
    SetScrollPosition(newPos);
      // display the offset pane contents
    Invalidate();
    return 0L;
}

/*============================================================================*/
    LRESULT CPreviewPaneEx::
OnMouseWheel(UINT msg, WPARAM wparam, LPARAM lparam)                        /*

    Override the base class method to respond to a vertial scroll mouse event
    and set the current bitmap scroll position accordingly. This override is
    necessary to prevent the CScrollView's automatic scroll of window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);
    // retrieve scroll parameters
    CPoint newPos  = GetScrollPosition();
    CSize szTotal  = GetTotalScrollSize();
    CSize sizeLine = GetLineScrollSize();
      // respond to the wheel event
    int WheelDelta = GET_WHEEL_DELTA_WPARAM(wparam);
    int cyPos = ::MulDiv(WheelDelta, sizeLine.cy, WHEEL_DELTA);
      // set the new position
    newPos.y -= cyPos;
    int maxPosY = szTotal.cy - GetClientRect().Height();
    newPos.y = MIN(newPos.y, maxPosY);
    newPos.y = MAX(newPos.y, 0);
      // display the offset pane contents
    SetScrollPosition(newPos);
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

    Override the base class method to respond to a vertical scroll bar event
    and set the current bitmap scroll position accordingly. This override is
    necessary to prevent the CScrollView's automatic scroll of window pixels.
*-----------------------------------------------------------------------------*/
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(lparam);
      // retrieve scroll parameters
    CPoint newPos  = GetScrollPosition();
    CSize szTotal  = GetTotalScrollSize();
    CSize sizePage = GetPageScrollSize();
    CSize sizeLine = GetLineScrollSize();
      // respond to the event
    switch (LOWORD(wparam))
    {
        case SB_PAGEUP: // clicked the scroll bar above the scroll box
            newPos.y -= sizePage.cy;
            break;

        case SB_PAGEDOWN: // clicked the scroll bar below the scroll box
            newPos.y += sizePage.cy;
            break;

        case SB_LINEUP: // clicked the top arrow
            newPos.y = sizeLine.cy;
            break;

        case SB_LINEDOWN: // clicked the bottom arrow
            newPos.y += sizeLine.cy;;
            break;

        case SB_THUMBTRACK: // dragging the scroll box
            newPos.y = HIWORD(wparam);
            break;

        default:
            break;
    }
      // set the new position
    int maxPosY = szTotal.cy - GetClientRect().Height();
    newPos.y = MIN(newPos.y, maxPosY);
    newPos.y = MAX(0, newPos.y);
    SetScrollPosition(newPos);
    // display the offset pane contents
    Invalidate();
    return 0L;
}
/*----------------------------------------------------------------------------*/