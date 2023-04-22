/////////////////////////////
// Views.cpp
//

#include "stdafx.h"
#include "Views.h"
#include "resource.h"


////////////////////////////////////
// CViewSimple function definitions.
//
void CViewSimple::OnDraw(CDC& dc)
{
    // Use the message font for Windows 7 and higher.
    if (GetWinVersion() >= 2601)
    {
        NONCLIENTMETRICS info = GetNonClientMetrics();
        LOGFONT lf = DPIScaleLogfont(info.lfMessageFont, 10);
        dc.CreateFontIndirect(lf);
    }

    // Centre some text in the window.
    CRect rc = GetClientRect();
    dc.DrawText(_T("Simple View"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_SIZE:
            Invalidate();
            break;
        }

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

//////////////////////////////////
// CViewList function definitions.
//
CViewList::CViewList()
{
}

CViewList::~CViewList()
{
    if (IsWindow()) DeleteAllItems();
}
// Insert 4 list view items.
void CViewList::InsertItems()
{
    // Add 4th item.
    int item = InsertItem(0, _T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C Header file"));

    // Add 3rd item.
    item = InsertItem(item, _T("ListViewApp.cpp"), 1);
    SetItemText(item, 1, _T("3 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // Add 2nd item.
    item = InsertItem(item, _T("main.cpp"), 1);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // Add 1st item.
    item = InsertItem(item, _T("ListView"), 0);
    SetItemText(item, 2, _T("Folder"));
}


void CViewList::OnAttach()
{
    // Set the image lists
    SetDPIImages();

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
    SetDPIColumnWidths();
}

void CViewList::SetColumns()
{
    // Empty the list.
    DeleteAllItems();

    // Initialize the columns.
    LV_COLUMN column;
    ZeroMemory(&column, sizeof(column));
    column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.cx = 120;
    TCHAR string[3][20] = {TEXT("Name"), TEXT("Size"), TEXT("Type")};
    for(int i = 0; i < 3; ++i)
    {
        column.pszText = string[i];
        InsertColumn(i, column);
    }
}

// Adjusts the listview column widths in response to window DPI changes.
void CViewList::SetDPIColumnWidths()
{
    SetColumnWidth(0, DPIScaleInt(120));
    SetColumnWidth(1, DPIScaleInt(50));
    SetColumnWidth(2, DPIScaleInt(100));
}

// Adjusts the listview image sizes in response to window DPI changes.
void CViewList::SetDPIImages()
{
    // Set the image lists
    CBitmap bmImage(IDB_FILEVIEW);
    bmImage = DPIScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    m_smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    m_smallImages.Add(bmImage, RGB(255, 0, 255));
    SetImageList(m_smallImages, LVSIL_SMALL);
}


//////////////////////////////////
// CViewTree function definitions.
//
CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
    if (IsWindow()) DeleteAllItems();
}

void CViewTree::OnAttach()
{
    // Set the image lists.
    SetDPIImages();

    // Adjust style to show lines and [+] button.
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

    DeleteAllItems();

    // Add some tree-view items.
    HTREEITEM htiRoot = InsertItem(_T("TreeView"), 0, 0);
    HTREEITEM htiCTreeViewApp = InsertItem(_T("CTreeViewApp"), 1, 1, htiRoot);
    InsertItem(_T("CTreeViewApp()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("GetMainFrame()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("InitInstance()"), 3, 3, htiCTreeViewApp);
    HTREEITEM htiMainFrame = InsertItem(_T("CMainFrame"), 1, 1, htiRoot);
    InsertItem(_T("CMainFrame()"), 3, 3, htiMainFrame);
    InsertItem(_T("OnCommand()"), 4, 4, htiMainFrame);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiMainFrame);
    HTREEITEM htiView = InsertItem(_T("CView"), 1, 1, htiRoot);
    InsertItem(_T("CView()"), 3, 3, htiView);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiView);
    InsertItem(_T("WndProc()"), 4, 4, htiView);

    // Expand some tree-view items.
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}

// Adjusts the listview image sizes in response to window DPI changes.
void CViewTree::SetDPIImages()
{
    // Resize the image list.
    CBitmap bmImage(IDB_CLASSVIEW);
    bmImage = DPIScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    m_smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    m_smallImages.Add(bmImage, RGB(255, 0, 0));
    SetImageList(m_smallImages, LVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = m_smallImages.GetIconSize().cx;
    SetIndent(imageWidth);
}


//////////////////////////////////
// CViewText function definitions.
//
void CViewText::OnAttach()
{
    SetDPIFont();
    SetWindowText(_T("Text Edit Window\r\n\r\n\r\n\r\n You can type some text here ..."));
}

void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.lpszClass = _T("EDIT");
    cs.style = ES_MULTILINE|WS_CHILD;
}

// Adjusts the font size in response to window DPI changes.
void CViewText::SetDPIFont()
{
    m_font.CreatePointFont(100, _T("Courier New"));
    m_font = DPIScaleFont(m_font, 9);
    SetFont(m_font);
}
