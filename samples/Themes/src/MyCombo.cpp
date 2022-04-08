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
    SetImage(3, IDB_STATUS);
}

CMyCombo::~CMyCombo()
{
}

void CMyCombo::PreCreate(CREATESTRUCT& cs)
{
    cs.lpszClass = WC_COMBOBOXEX;
    cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;

    // Set the height of the combobox in order to see a dropdown list
    cs.cy = 100;
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

    ITEMINFO IInf[ ] =
    {
        { 0, 0,  0, _T("Item 1")},
        { 1, 1,  0, _T("Item 2")},
        { 2, 2,  0, _T("Item 3")}
    };

    int MaxItems = 3;
    for(int i = 0; i < MaxItems; ++i)
    {
        COMBOBOXEXITEM cbei;
        ZeroMemory(&cbei, sizeof(cbei));
        cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
        cbei.iItem          = i;
        cbei.pszText        = const_cast<LPTSTR>(IInf[i].text);
        cbei.cchTextMax     = sizeof(IInf[i].text);
        cbei.iImage         = IInf[i].image;
        cbei.iSelectedImage = IInf[i].selectedImage;
        cbei.iIndent        = IInf[i].indent;

        // Add the items to the ComboBox's dropdown list
        if(-1 == InsertItem(cbei))
            return FALSE;
    }

    // Assign the existing image list to the ComboBoxEx control
    SetImageList(m_images);

    return TRUE;
}

void CMyCombo::SetImage(int image, UINT ImageID)
{
    m_images.DeleteImageList();

    CBitmap bm(ImageID);
    BITMAP bmData = bm.GetBitmapData();
    int iImageWidth  = bmData.bmWidth / image;
    int iImageHeight = bmData.bmHeight;

    COLORREF crMask = RGB(255,0,255);
    m_images.Create(iImageWidth, iImageHeight, ILC_COLOR32 | ILC_MASK, image, 0);
    m_images.Add(bm, crMask);
}

