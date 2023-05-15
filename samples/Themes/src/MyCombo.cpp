/////////////////////////////
// MyCombo.cpp
//

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"

/////////////////////////////////
// CMyCombo function definitions.
//
CMyCombo::CMyCombo()
{
    SetImages(3, IDB_STATUS);
}

CMyCombo::~CMyCombo()
{
}

BOOL CMyCombo::AddItems()
{
    typedef struct
    {
        int image;
        int selectedImage;
        int indent;
        LPCTSTR text;        // Note: LPTSTR would be incorrect here.
    } ITEMINFO;

    ITEMINFO itemInfo[ ] =
    {
        { 0, 0, 0, _T("Item 1")},
        { 1, 1, 0, _T("Item 2")},
        { 2, 2, 0, _T("Item 3")}
    };

    int MaxItems = 3;
    for(int i = 0; i < MaxItems; ++i)
    {
        COMBOBOXEXITEM cbei;
        ZeroMemory(&cbei, sizeof(cbei));
        cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
        cbei.iItem          = i;
        cbei.pszText        = const_cast<LPTSTR>(itemInfo[i].text);
        cbei.iImage         = itemInfo[i].image;
        cbei.iSelectedImage = itemInfo[i].selectedImage;
        cbei.iIndent        = itemInfo[i].indent;

        // Add the items to the ComboBox's dropdown list.
        if(-1 == InsertItem(cbei))
            return FALSE;
    }

    // Assign the existing image list to the ComboBoxEx control.
    SetImageList(m_images);

    return TRUE;
}

void CMyCombo::SetImages(int images, UINT imageID)
{
    m_images.DeleteImageList();

    CBitmap bm(imageID);
    bm = DpiScaleUpBitmap(bm);

    BITMAP bmData = bm.GetBitmapData();
    int iImageWidth  = bmData.bmWidth / images;
    int iImageHeight = bmData.bmHeight;

    COLORREF crMask = RGB(255,0,255);
    m_images.Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, images, 0);
    m_images.Add(bm, crMask);
}

