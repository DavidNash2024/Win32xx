/////////////////////////////
// scintillaview.cpp
//

#include "stdafx.h"
#include "scintilla.h"
#include "scintillaview.h"

// Constructor.
ScintillaView::ScintillaView() : m_directFunction(0), m_directPointer(0)
{
    m_fr = {};

    // Load the Scintalla dll.
    m_module = ::LoadLibrary(L"Scintilla.dll");
    if (m_module == nullptr)
        throw CResourceException(L"Failed to load scintilla.dll");
}

// Destructor.
ScintillaView::~ScintillaView()
{
    if (m_module != nullptr)
        ::FreeLibrary(m_module);
}

// Fills the Sci_RangeToFormatFull struct and calculates the character
// position of the page breaks. Returns the number of pages.
Sci_Position ScintillaView::CollatePages(Sci_Position startChar, Sci_Position endChar)
{
    if (endChar == -1)
        endChar = GetTextLength();

    assert(startChar <= endChar);
    m_pageBreaks.clear();
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    m_fr = {};
    m_fr.hdcTarget = printerDC;
    m_fr.hdc = printerDC;
    m_fr.rcPage = GetPageRect(printerDC);
    m_fr.rc = GetPrintRect(printerDC);
    m_fr.chrg.cpMin = startChar;
    m_fr.chrg.cpMax = endChar;

    // Calculate the page breaks
    Sci_Position lastChar = startChar;  // The index of the last char that fits on the page.
    do
    {
        lastChar = FormatRangeFull(false, &m_fr);

        if (lastChar < endChar)
        {
            m_fr.chrg.cpMin = lastChar;
            m_fr.chrg.cpMax = endChar;

            // Store the page break index in the vector.
            m_pageBreaks.push_back(lastChar);
        }
    } while (lastChar < endChar);

    // Add the final page break.
    m_pageBreaks.push_back(endChar);

    // return the number of pages.
    return m_pageBreaks.size();
}

// Returns the maximum printable area of rendering device.
Sci_Rectangle ScintillaView::GetPageRect(const CDC& dc) const
{
    // Retrieve the physical page size (in device units).
    CPoint ptPage;
    ptPage.x = dc.GetDeviceCaps(PHYSICALWIDTH);   // device units
    ptPage.y = dc.GetDeviceCaps(PHYSICALHEIGHT);  // device units

    // Retrieve the left and top unprintable margins in device units.
    Sci_Rectangle rectPhysMargins;
    rectPhysMargins.left = dc.GetDeviceCaps(PHYSICALOFFSETX);
    rectPhysMargins.top = dc.GetDeviceCaps(PHYSICALOFFSETY);

    // Calculate the right and bottom unprintable margins.
    rectPhysMargins.right = ptPage.x      // total paper width
        - dc.GetDeviceCaps(HORZRES)       // printable width
        - rectPhysMargins.left;              // left unprintable margin

    rectPhysMargins.bottom = ptPage.y     // total paper height
        - dc.GetDeviceCaps(VERTRES)          // printable height
        - rectPhysMargins.top;              // right unprintable margin

    // Calculate the entire printing area.
    Sci_Rectangle pageRect;
    pageRect.left = rectPhysMargins.left;
    pageRect.top = rectPhysMargins.top;
    pageRect.right = ptPage.x - rectPhysMargins.left - rectPhysMargins.right - 1;
    pageRect.bottom = ptPage.y - rectPhysMargins.top - rectPhysMargins.bottom - 1;

    return pageRect;
}

// Returns a rectangle specifying the area to print to.
Sci_Rectangle ScintillaView::GetPrintRect(const CDC& dc) const
{
    // Scale the print margins to the device dpi.
    int dpiX = dc.GetDeviceCaps(LOGPIXELSX);
    int dpiY = dc.GetDeviceCaps(LOGPIXELSY);
    Sci_Rectangle printMargins = { dpiX / 6, dpiY / 6, dpiX / 6, dpiY / 6 };

    // Adjust the page margins to accomodate the header and footer.
    printMargins.top += dpiY / 4;
    printMargins.bottom += dpiY / 4;

    Sci_Rectangle pageRect = GetPageRect(dc);
    Sci_Rectangle printRect;

    printRect.left = pageRect.left + printMargins.left;
    printRect.top = pageRect.top + printMargins.top;
    printRect.right = pageRect.right - printMargins.right;
    printRect.bottom = pageRect.bottom - printMargins.bottom;

    // Convert device coordinates into logical coordinates.
    DPtoLP(dc, reinterpret_cast<LPPOINT>(&printRect), 2);

    return printRect;
}

void ScintillaView::InitialiseEditor()
{
    m_directPointer = SendMessage(SCI_GETDIRECTPOINTER, 0, 0);
    m_directFunction = reinterpret_cast<SciFnDirect>
        (SendMessage(SCI_GETDIRECTFUNCTION, 0, 0));

    assert(m_directPointer);
    assert(m_directFunction);

    // Set up the attributes of the global default style.
    StyleSetFont(STYLE_DEFAULT, "Courier New");
    StyleSetSize(STYLE_DEFAULT, 10);
    StyleSetBack(STYLE_DEFAULT, white);
    StyleSetFore(STYLE_DEFAULT, black);
    StyleClearAll();

    SetScrollWidth(1);
    SetScrollWidthTracking(true);
    SetFocus();
}

// Called after the scintilla control is created.
void ScintillaView::OnInitialUpdate()
{
    InitialiseEditor();
}

// Load the scintilla control from the specified file.
// Throws a CFileException on failure.
void ScintillaView::OpenFile(LPCWSTR fullPath)
{
    ClearAll();
    EmptyUndoBuffer();
    SetSavePoint();
    Cancel();
    SetUndoCollection(false);
    SetScrollWidth(1);

    // Open the file for reading.
    CFile file;
    file.Open(fullPath, OPEN_EXISTING | CFile::modeRead);

    // Read the file into the Scintilla control.
    size_t totalBytesRead = 0;
    size_t fileLength = static_cast<size_t>(file.GetLength());
    std::vector<char> buffer(blockSize);
    while (totalBytesRead < fileLength)
    {
        size_t bytesRead = file.Read(buffer.data(), blockSize);
        AddText(bytesRead, buffer.data());
        totalBytesRead += bytesRead;
    }

    SetHeaderText(fullPath);
    SetUndoCollection(true);
    EmptyUndoBuffer();
    SetSavePoint();
    GotoPos(0);
}

// Set the window creation parameters before the window is created.
void ScintillaView::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE;
    cs.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN;
}

// Set the window class parameters before the window is created.
void ScintillaView::PreRegisterClass(WNDCLASS& wc)
{
    wc.lpszClassName = L"Scintilla";
}

// Prints the header part of the page.
void ScintillaView::PrintFooter(CDC& dc, const CString& footerText) const
{
    // Draw the text.
    int DpiY = dc.GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
    int pointSize = ::MulDiv(60, DpiY, 72);
    dc.CreatePointFont(pointSize, L"Arial");
    dc.SetTextColor(gray);
    dc.SetBkColor(white);
    const UINT ta = dc.SetTextAlign(TA_TOP);
    CRect textRect(m_fr.rc.left, m_fr.rc.bottom, m_fr.rc.right, m_fr.rcPage.bottom);
    dc.DrawText(footerText, -1, textRect, DT_VCENTER | DT_CENTER |  DT_SINGLELINE | DT_END_ELLIPSIS);
    dc.SetTextAlign(ta);

    // Draw the line.
    TEXTMETRIC tm{};
    dc.GetTextMetrics(tm);
    int headerLineHeight = tm.tmHeight + tm.tmExternalLeading;
    dc.CreatePen(0, 1, gray);
    dc.MoveTo(m_fr.rc.left, m_fr.rc.bottom + (headerLineHeight / 2));
    dc.LineTo(m_fr.rc.right, m_fr.rc.bottom + (headerLineHeight / 2));
}

// Prints the header part of the page.
void ScintillaView::PrintHeader(CDC& dc, const CString& headerText) const
{
    // Draw the text.
    int DpiY = dc.GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction
    int pointSize = ::MulDiv(60, DpiY, 72);
    dc.CreatePointFont(pointSize, L"Arial");
    dc.SetTextColor(gray);
    dc.SetBkColor(white);
    const UINT ta = dc.SetTextAlign(TA_TOP);
    CRect textRect(m_fr.rc.left, m_fr.rcPage.top, m_fr.rc.right, m_fr.rc.top);
    dc.DrawText(headerText, -1, textRect, DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_PATH_ELLIPSIS);
    dc.SetTextAlign(ta);

    // Draw the line.
    TEXTMETRIC tm{};
    dc.GetTextMetrics(tm);
    int headerLineHeight = tm.tmHeight + tm.tmExternalLeading;
    dc.CreatePen(0, 1, gray);
    dc.MoveTo(m_fr.rc.left, m_fr.rc.top - headerLineHeight / 2);
    dc.LineTo(m_fr.rc.right, m_fr.rc.top - headerLineHeight / 2);
}

// Prints the specified page to specified dc.
// Called by CPrintPreview, and also used for printing.
void ScintillaView::PrintPage(CDC& dc, int page)
{
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Assign values to the Sci_RangeToFormatFull member struct.
    m_fr.hdcTarget = printerDC;
    m_fr.hdc = dc;
    m_fr.chrg.cpMin = (page > 0) ? m_pageBreaks[page - 1] : 0;
    m_fr.chrg.cpMax = m_pageBreaks[page];

    // Print the header to the specified dc.
    PrintHeader(dc, m_header);

    // Print the text from the scintilla control on the specified dc.
    FormatRangeFull(true, &m_fr);

    // Print the footer to the specified dc.
    CString footerText;
    footerText << L"Page " << page + 1;
    PrintFooter(dc, footerText);

    // Tell the control to release the cached information.
    FormatRangeFull(false, 0);
}

// Print the document without bringing up a print dialog.
// docName - specifies the document name for the print job.
void ScintillaView::QuickPrint(LPCWSTR docName)
{
    // Acquire the currently selected printer and page settings.
    CPrintDialog printDlg;
    CDC printerDC = printDlg.GetPrinterDC();

    // Start print job.
    DOCINFO di{};
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = docName;
    di.lpszOutput = nullptr;   // Do not print to file.
    printerDC.StartDoc(&di);

    int maxPages = static_cast<int>(CollatePages());
    for (int page = 0; page < maxPages; ++page)
    {
        // Start the page.
        printerDC.StartPage();

        // Print the page.
        PrintPage(printerDC, page);

        // End the page.
        printerDC.EndPage();
    }

    // End the print job.
    printerDC.EndDoc();
}

// Saves the contents of the Scintilla control to the specified file.
// Throws a CFileException on failure.
void ScintillaView::SaveFile(LPCWSTR fullPath)
{
    // Open the file for writing.
    CFile file;
    file.Open(fullPath, CREATE_ALWAYS);

    // Save the contents of the Scintilla control.
    Sci_Position fileLength = GetTextLength();
    std::vector<char> buffer(blockSize);
    for (Sci_Position i = 0; i < fileLength; i += blockSize)
    {
        Sci_Position grabSize = fileLength - i;
        if (grabSize > blockSize)
            grabSize = blockSize;
        GetRangeFull(i, i + grabSize, buffer.data());
        file.Write(buffer.data(), static_cast<UINT>(grabSize));
    }

    // Set the file as unmodified.
    EmptyUndoBuffer();
    SetSavePoint();
}

// Handle the window's messages.
LRESULT ScintillaView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
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

////////////////////////////////////////////////////
// Scintalla control functions.
// This is just a subset of the functions available.
// Refer to the Scintilla documentation for a
// description of these functions.

inline LRESULT ScintillaView::Call(UINT message, WPARAM wParam, LPARAM lParam)
{
    assert(IsWindow());
    assert(m_directFunction);
    assert(m_directPointer);

    return m_directFunction(m_directPointer, message, wParam, lParam);
}

void ScintillaView::AddText(Sci_Position length, const char* text)
{
    Call(SCI_ADDTEXT, length, reinterpret_cast<LPARAM>(text));
}

void ScintillaView::Cancel()
{
    Call(SCI_CANCEL, 0, 0);
}

bool ScintillaView::CanPaste()
{
    return Call(SCI_CANPASTE, 0, 0) != 0;
}

bool ScintillaView::CanRedo()
{
    return Call(SCI_CANREDO, 0, 0) != 0;
}

bool ScintillaView::CanUndo()
{
    return Call(SCI_CANUNDO, 0, 0) != 0;
}

void ScintillaView::Clear()
{
    Call(SCI_CLEAR, 0, 0);
}

void ScintillaView::ClearAll()
{
    Call(SCI_CLEARALL, 0, 0);
}

void ScintillaView::Copy()
{
    Call(SCI_COPY, 0, 0);
}

void ScintillaView::Cut()
{
    Call(SCI_CUT, 0, 0);
}

void ScintillaView::EmptyUndoBuffer()
{
    Call(SCI_EMPTYUNDOBUFFER, 0, 0);
}

Sci_Position ScintillaView::FormatRangeFull(bool draw, Sci_RangeToFormatFull* fr)
{
    return Call(SCI_FORMATRANGEFULL, draw, reinterpret_cast<LPARAM>(fr));
}

bool ScintillaView::GetModify()
{
    return Call(SCI_GETMODIFY, 0, 0) != 0;
}

void ScintillaView::GetRangeFull(Sci_Position start, Sci_Position end, char* text)
{
    Sci_TextRangeFull trf;
    trf.chrg.cpMin = start;
    trf.chrg.cpMax = end;
    trf.lpstrText = text;

    // Fill the specified text buffer from the specified positions.
    Call(SCI_GETTEXTRANGEFULL, 0, reinterpret_cast<LPARAM>(&trf));
}

Sci_Position ScintillaView::GetSelectionEnd()
{
    return Call(SCI_GETSELECTIONEND, 0, 0);
}

Sci_Position ScintillaView::GetSelectionStart()
{
    return Call(SCI_GETSELECTIONSTART, 0, 0);
}

int  ScintillaView::GetWrapMode()
{
    return static_cast<int>(Call(SCI_GETWRAPMODE, 0, 0));
}

Sci_Position ScintillaView::GetTextLength()
{
    return Call(SCI_GETTEXTLENGTH, 0, 0);
}

void ScintillaView::GotoPos(Sci_Position position)
{
    Call(SCI_GOTOPOS, position, 0);
}

void ScintillaView::Paste()
{
    Call(SCI_PASTE, 0, 0);
}

void ScintillaView::Redo()
{
    Call(SCI_REDO, 0, 0);
}

void ScintillaView::SelectAll()
{
    Call(SCI_SELECTALL, 0, 0);
}

void ScintillaView::SetSavePoint()
{
    Call(SCI_SETSAVEPOINT, 0, 0);
}

void ScintillaView::SetScrollWidth(int pixelWidth)
{
    Call(SCI_SETSCROLLWIDTH, pixelWidth, 0);
}

void ScintillaView::SetScrollWidthTracking(bool tracking)
{
    Call(SCI_SETSCROLLWIDTHTRACKING, tracking, 0);
}

void ScintillaView::SetUndoCollection(bool collectUndo)
{
    Call(SCI_SETUNDOCOLLECTION, collectUndo, 0);
}

void ScintillaView::SetWrapMode(int mode)
{
    Call(SCI_SETWRAPMODE, mode, 0);
}

void ScintillaView::StyleClearAll()
{
    Call(SCI_STYLECLEARALL, 0, 0);
}

COLORREF ScintillaView::StyleGetBack(int style)
{
    return static_cast<COLORREF>(Call(SCI_STYLEGETBACK, style, 0));
}

bool ScintillaView::StyleGetBold(int style)
{
    return Call(SCI_STYLEGETBOLD, style, 0) != 0;
}

int ScintillaView::StyleGetFont(int style, char* fontName)
{
    // Updates the specified fontName.
    LPARAM lparam = reinterpret_cast<LPARAM>(fontName);
    return static_cast<int>(Call(SCI_STYLEGETFONT, style, lparam));
}

COLORREF ScintillaView::StyleGetFore(int style)
{
    return static_cast<COLORREF>(Call(SCI_STYLEGETFORE, style, 0));
}

bool ScintillaView::StyleGetItalic(int style)
{
    return Call(SCI_STYLEGETITALIC, style, 0) != 0;
}

int ScintillaView::StyleGetSize(int style)
{
    return static_cast<int>(Call(SCI_STYLEGETSIZE, style, 0));
}

bool ScintillaView::StyleGetUnderLine(int style)
{
    return Call(SCI_STYLEGETUNDERLINE, style, 0) != 0;
}

bool ScintillaView::StyleGetWeight(int style)
{
    return Call(SCI_STYLEGETWEIGHT, style, 0) != 0;
}

void ScintillaView::StyleSetBack(int style, COLORREF back)
{
    Call(SCI_STYLESETBACK, style, back);
}

void ScintillaView::StyleSetBold(int style, bool bold)
{
    Call(SCI_STYLESETBOLD, style, bold);
}

void ScintillaView::StyleSetFont(int style, const char* fontName)
{
    Call(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(fontName));
}

void ScintillaView::StyleSetFore(int style, COLORREF fore)
{
    Call(SCI_STYLESETFORE, style, fore);
}

void ScintillaView::StyleSetItalic(int style, bool italic)
{
    Call(SCI_STYLESETITALIC, style, italic);
}

void ScintillaView::StyleSetSize(int style, int size)
{
    Call(SCI_STYLESETSIZE, style, size);
}

void ScintillaView::StyleSetUnderLine(int style, bool underLine)
{
    Call(SCI_STYLESETUNDERLINE, style, underLine);
}

void ScintillaView::StyleSetWeight(int style, int weight)
{
    Call(SCI_STYLESETWEIGHT, style, weight);
}

void ScintillaView::Undo()
{
    Call(SCI_UNDO, 0, 0);
}
