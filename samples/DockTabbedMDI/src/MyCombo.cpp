//////////////////////////////////////////////
// MyCombo.cpp
//  Definitions for the CMyCombo class
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
    cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;

    // Set the hight of the combobox in order to see a dropdown list
    cs.cy = 100;
}

// Adds items to the ComboBoxEx control.
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
        if(InsertItem(cbei) == -1)
            return FALSE;
    }

    // Assign the existing image list to the ComboBoxEx control
    SetImageList(m_images);

    return TRUE;
}

// Loads the ComboBoxEx's images from a bitmap.
void CMyCombo::SetImages(int nImages, UINT imageID)
{
    m_images.DeleteImageList();

    CBitmap bm(imageID);
    assert(bm.GetHandle());
    BITMAP bmData = bm.GetBitmapData();
    int cy = bmData.bmWidth / nImages;
    int cx = bmData.bmHeight;

    m_images.Create(cx, cy, ILC_COLOR32 | ILC_MASK, nImages, 0);
    m_images.Add(bm, RGB(255, 0, 255));
}

