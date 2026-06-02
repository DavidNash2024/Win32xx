/////////////////////////////
// ComboBoxEx.cpp
//

#include "stdafx.h"
#include "MyCombo.h"
#include "resource.h"

////////////////////////////////////
// Definitions of the CMyCombo class
//

// Return by reference to the edit control.
const CEdit& CMyCombo::GetCBEdit() const
{
    return m_edit;
}

// Called when the window handle (HWND) is attached to CMyCombo.
void CMyCombo::OnAttach()
{
    // Attach the edit control to m_edit so we can use it in our code.
    m_edit.Attach(GetEditCtrl());
}

// Set the CREATESTRUCT parameters before the window is created.
void CMyCombo::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_CLIPCHILDREN;
    cs.cy = DpiScaleInt(400);    // required to display several list box items
    cs.hMenu = (HMENU)IDC_COMBOBOXEX;
}

// Restore the combo box items from the m_items vector.
void CMyCombo::RestoreItems() const
{
    for (const auto& item : m_items)
    {
        COMBOBOXEXITEM comboItem = {};
        comboItem.mask = CBEIF_TEXT;
        comboItem.pszText = const_cast<LPWSTR>(item.c_str());
        InsertItem(comboItem);
    }
}

// Save the combo box items to the m_items vector.
void CMyCombo::SaveItems()
{
    m_items.clear();
    int count = GetCount();
    for (int i = 0; i < count; i++)
    {
        CString str;
        GetLBText(i, str.GetBuffer(GetLBTextLen(i)));
        str.ReleaseBuffer();
        m_items.push_back(str);
    }
}
