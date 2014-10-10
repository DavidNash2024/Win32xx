//////////////////////////////////////////////
// MyCombo.cpp
//  Definitions for the CMyCombo class

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"

CMyCombo::CMyCombo()
{
	SetImages(3, IDB_STATUS);
}

CMyCombo::~CMyCombo()
{
}

void CMyCombo::PreCreate(CREATESTRUCT &cs)
{
	cs.lpszClass = WC_COMBOBOXEX;
	cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;

	// Set the hight of the combobox in order to see a dropdown list
	cs.cy = 100;
}

BOOL CMyCombo::AddItems()
{
    typedef struct
	{
        int iImage;
        int iSelectedImage;
        int iIndent;
        LPTSTR pszText;
    } ITEMINFO;

    ITEMINFO IInf[ ] =
	{
        { 0, 0,  0, (LPTSTR)_T("Item 1")},
        { 1, 1,  0, (LPTSTR)_T("Item 2")},
        { 2, 2,  0, (LPTSTR)_T("Item 3")}
    };

    int MaxItems = 3;
	for(int i = 0; i < MaxItems; ++i)
	{
		COMBOBOXEXITEM cbei;
		ZeroMemory(&cbei, sizeof(COMBOBOXEXITEM));
		cbei.mask = CBEIF_TEXT | CBEIF_INDENT | CBEIF_IMAGE| CBEIF_SELECTEDIMAGE;
        cbei.iItem          = i;
        cbei.pszText        = IInf[i].pszText;
        cbei.cchTextMax     = sizeof(IInf[i].pszText);
        cbei.iImage         = IInf[i].iImage;
        cbei.iSelectedImage = IInf[i].iSelectedImage;
        cbei.iIndent        = IInf[i].iIndent;

        // Add the items to the ComboBox's dropdown list
        if(-1 == SendMessage(CBEM_INSERTITEM, 0L, (LPARAM)&cbei))
            return FALSE;
    }

	// Assign the existing image list to the ComboBoxEx control
	SetImageList(&m_imlImages);

    return TRUE;
}

void CMyCombo::SetImages(int nImages, UINT ImageID)
{
	m_imlImages.DeleteImageList();

	CBitmap bm(ImageID);
	assert(bm.GetHandle());
	BITMAP bmData = bm.GetBitmapData();
	int cy = bmData.bmWidth / nImages;
	int cx = bmData.bmHeight;

	m_imlImages.Create(cx, cy, ILC_COLOR32 | ILC_MASK, nImages, 0);
	m_imlImages.Add( &bm, RGB(255,0,255) );	
}

void CMyCombo::OnDestroy()
{
}

