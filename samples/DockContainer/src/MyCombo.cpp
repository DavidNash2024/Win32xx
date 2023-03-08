/////////////////////////////
// MyCombo.cpp
//

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"

////////////////////////////////
// CMyCombo function definitions
//

// Constructor.
CMyCombo::CMyCombo()
{
    SetImages(3, IDB_STATUS);
}

// Destructor.
CMyCombo::~CMyCombo()
{
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMyCombo::PreCreate(CREATESTRUCT& cs)
{
    cs.lpszClass = WC_COMBOBOXEX;
    cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;

    // Set the height of the combobox in order to see a dropdown list
    cs.cy = DPIScaleInt(100);
}

// Adds items to the ComboBoxEx.
BOOL CMyCombo::AddItems()
{
    struct ITEMINFO
    {
        int iImage;
        int iSelectedImage;
        int iIndent;
        LPCTSTR pszText;        // Note: LPTSTR would be incorrect here.
    };

    ITEMINFO IInf[ ] =
    {
        { 0, 0,  0, _T("Item 1")},
        { 1, 1,  0, _T("Item 2")},
        { 2, 2,  0, _T("Item 3")},
    };

    int MaxItems = 3;
    for(int i = 0; i < MaxItems; ++i)
    {
        COMBOBOXEXITEM cbei;
        ZeroMemory(&cbei, sizeof(COMBOBOXEXITEM));
        cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
        cbei.iItem          = i;
        cbei.pszText        = const_cast<LPTSTR>(IInf[i].pszText);
        cbei.cchTextMax     = sizeof(IInf[i].pszText);
        cbei.iImage         = IInf[i].iImage;
        cbei.iSelectedImage = IInf[i].iSelectedImage;
        cbei.iIndent        = IInf[i].iIndent;

        // Add the items to the ComboBox's dropdown list
        if(InsertItem(cbei) == -1)
            return FALSE;
    }

    // Assign the existing image list to the ComboBoxEx control
    SetImageList(m_imlImages);

    return TRUE;
}

// Assigns images to the ComboBoxEx control from a bitmap.
void CMyCombo::SetImages(int images, UINT imageID)
{
    m_imlImages.DeleteImageList();

    CBitmap bm(imageID);
    assert(bm.GetHandle());
    bm = DPIScaleUpBitmap(bm);

    BITMAP bmData = bm.GetBitmapData();
    int iImageWidth = bmData.bmWidth / images;
    int iImageHeight = bmData.bmHeight;

    m_imlImages.Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, images, 0);
    m_imlImages.Add(bm, RGB(255, 0, 255));
}

