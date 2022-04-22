//////////////////////////////////////////////////////
// DialogsTree.cpp - Definitions for the CDialogsTree,
//                   and CDockDialogsTree classes
//


#include "stdafx.h"
#include "DialogsTree.h"
#include "FrameApp.h"
#include "UserMessages.h"
#include "resource.h"


////////////////////////////////////
// CDialogsTree function definitions
//

// Constructor.
CDialogsTree::CDialogsTree()
{
}

// Destructor.
CDialogsTree::~CDialogsTree()
{
    if (IsWindow()) DeleteAllItems();
}

void CDialogsTree::FillTree(const std::vector<ResourceInfo>& allInfo, LPCTSTR fileName)
{
    DeleteAllItems();
    CString rootName;
    rootName << fileName << " - Dialogs";
    HTREEITEM rootItem = InsertItem(rootName, 0, 0);
    HTREEITEM currentItem = rootItem;
    HTREEITEM prevItem = rootItem;

    std::vector<ResourceInfo>::const_iterator it;
    int index = 0;
    for (it = allInfo.begin(); it != allInfo.end(); ++it)
    {
        ResourceInfo info = *it;     
        CString prevType;
        ResourceInfo* prevInfo = (ResourceInfo*)GetItemData(prevItem);
        if (prevInfo != 0)
        {
            prevType = prevInfo->typeName;
        }
        if (info.typeName == ToCString(5))
        {
            CString itemName;
            itemName << info.resourceName << "   " << info.languageID;
            currentItem = InsertItem(itemName, 1, 1, rootItem);
            SetItemData(currentItem, (DWORD_PTR) &allInfo[index]);
        }       
        
        prevItem = currentItem;
        index++;
    }

    // Expand some tree-view items
    Expand(rootItem, TVE_EXPAND);

}

// Called when a window handle (HWND) is attached to CDialogsTree.
void CDialogsTree::OnAttach()
{
    //set the the tree-view's image list
    m_normalImages.Create(24, 24, ILC_COLOR32, 1, 0);
    m_normalImages.AddIcon(IDI_WINDOWS);
    m_normalImages.AddIcon(IDW_MAIN);
    SetImageList(m_normalImages, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button
    DWORD dwStyle = GetStyle();
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(dwStyle);
}

// Called when the window is destroyed.
void CDialogsTree::OnDestroy()
{
    SetImageList(0, LVSIL_SMALL);
}


// Called when the WM_NOTIFY message is reflected back to CViewTree
// by the framework.
LRESULT CDialogsTree::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMTREEVIEW pTreeView = (LPNMTREEVIEW)lparam;
    switch (pTreeView->hdr.code)
    {
    case TVN_SELCHANGED:       return OnSelChanged();
    }

    return 0;
}

// Called when a treview item is selected
BOOL CDialogsTree::OnSelChanged()
{
    // Send the message to CMainFrame.
    GetAncestor().SendMessage(UWM_ONSELECTTREEITEM, 0, 0);
    return TRUE;
}


// Set the CREATESTURCT parameters befoe the window is created.
void CDialogsTree::PreCreate(CREATESTRUCT& cs)
{
    cs.style = TVS_NOTOOLTIPS | WS_CHILD;
}


////////////////////////////////////
// CDockDialogsTree function definitions
//

// Constructor.
CDockDialogsTree::CDockDialogsTree()
{
    SetView(m_tree);

    // Set the width of the splitter bar
    SetBarWidth(8);
}


LRESULT CDockDialogsTree::OnNotify(WPARAM wparam, LPARAM lparam)
{
    // Pass the tree-view's notifications to CMainFrame
    return GetAncestor().SendMessage(WM_NOTIFY, wparam, lparam);
}
