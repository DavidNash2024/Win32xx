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
}

CMyCombo::~CMyCombo()
{
}

void CMyCombo::OnAttach()
{
    SetImages(IDB_STATUS, 3);
}

BOOL CMyCombo::AddItems()
{
    std::vector<CString> itemsText;
    itemsText.push_back("Item 1");
    itemsText.push_back("Item 2");
    itemsText.push_back("Item 3");

    std::vector<CString>::iterator it;
    for (it = itemsText.begin(); it != itemsText.end(); ++it)
    {
        int i = static_cast<int>(it - itemsText.begin());
        COMBOBOXEXITEM cbei;
        ZeroMemory(&cbei, sizeof(cbei));
        cbei.mask = CBEIF_TEXT | CBEIF_IMAGE | CBEIF_SELECTEDIMAGE;
        cbei.iItem          = i;
        cbei.pszText        = const_cast<LPTSTR>(itemsText[i].c_str());
        cbei.iImage         = i;
        cbei.iSelectedImage = i;

        // Add the items to the ComboBox's dropdown list.
        if(-1 == InsertItem(cbei))
            return FALSE;
    }

    // Assign the existing image list to the ComboBoxEx control.
    SetImageList(m_images);

    // Adjust the ComboBoxEx height for the image height.
    SetItemHeight(-1, m_images.GetIconSize().cy +2);

    return TRUE;
}

void CMyCombo::SetImages(UINT bitmapID, int imageCount)
{
    m_images.DeleteImageList();

    CBitmap bitmap(bitmapID);
    bitmap = DpiScaleUpBitmap(bitmap);
    CSize bitmapSize = bitmap.GetSize();
    int imageWidth  = bitmapSize.cx / imageCount;
    int imageHeight = bitmapSize.cy;

    COLORREF mask = RGB(255,0,255);
    m_images.Create(imageWidth, imageHeight, ILC_COLOR32 | ILC_MASK, imageCount, 0);
    m_images.Add(bitmap, mask);
}

