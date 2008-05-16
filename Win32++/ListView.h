#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "../Win32++/WinCore.h"

namespace Win32xx
{

	class CListView : public CWnd
	{
	public:
		CListView() {}
		virtual ~CListView() {}
		virtual void PreCreate(CREATESTRUCT &cs) { cs.lpszClass = WC_LISTVIEW; }

		void ApproximateViewRect() {}
		void CancelEditLabel() {} 
		void GetInsertMark() {}
		void InsertMarkHitTest() {}
		void GetInsertMarkColor() {}
		void GetInsertMarkRect() {}
		void GetOutlineColor() {}
		void SetOutlineColor() {} 
		void GetTileInfo() {} 
		void GetTileViewInfo() {} 
		void GetSelectedColumn() {}  
		void IsGroupViewEnabled() {} 
		void SetInfoTip() {} 
		void SetInsertMark() {} 
		void SetInsertMarkColor() {}  
		void SetTileInfo() {} 
		void SetTileViewInfo() {}
		void EnableGroupView() {}
		void GetGroupInfo() {} 
		void GetGroupMetrics() {}
		void GetView() {} 
		void HasGroup() {} 
		void InsertGroup() {} 
		void InsertGroupSorted() {} 
		void MoveGroup() {} 
		void MoveItemToGroup() {}  
		void RemoveAllGroups() {} 
		void RemoveGroup() {}
		void SetGroupInfo() {} 
		void SetGroupMetrics() {} 
		void SetSelectedColumn() {} 
		void SetView() {} 
		void SortGroups() {} 
		void GetBkColor() {} 
		void GetBkImage() {} 
		void GetCallbackMask() {} 
		void GetCheck() {} 
		void GetColumn() {} 
		void GetColumnOrderArray() {} 
		void GetColumnWidth() {} 
		void GetCountPerPage() {} 
		void GetEditControl() {}
		void GetExtendedStyle() {} 
		void GetFirstSelectedItemPosition() {} 
		void GetHeaderCtrl() {} 
		void GetHotCursor() {} 
		void GetHotItem() {} 
		void GetHoverTime() {}
		void GetImageList() {} 
		void GetItem() {} 
		void GetItemCount() {}
		void GetItemData() {} 
		void GetItemPosition() {} 
		void GetItemRect() {} 
		void GetItemState() {} 
		void GetItemText() {} 
		void GetNextItem() {} 
		void GetNextSelectedItem() {} 
		void GetNumberOfWorkAreas() {} 
		void GetOrigin() {}
		void GetSelectedCount() {} 
		void GetSelectionMark() {} 
		void GetStringWidth() {} 
		void GetSubItemRect() {} 
		void GetTextBkColor() {} 
		void GetTextColor() {} 
		void GetToolTips() {}  
		void GetTopIndex() {} 
		void GetViewRect() {}
		void GetWorkAreas() {}
		void SetBkColor() {}
		void SetBkImage() {}
		void SetCallbackMask() {} 
		void SetCheck() {} 
		void SetColumn() {} 
		void SetColumnOrderArray() {} 
		void SetColumnWidth() {} 
		void SetExtendedStyle() {} 
		void SetHotCursor() {} 
		void SetHotItem() {} 
		void SetHoverTime() {} 
		void SetIconSpacing() {} 
		void SetImageList() {} 
		void SetItemSets() {} 
		void SetItemCount() {} 
		void SetItemCountEx() {} 
		void SetItemData() {} 
		void SetItemPosition() {} 
		void SetItemState() {} 
		void SetItemText() {} 
		void SetSelectionMark() {} 
		void SetTextBkColor() {} 
		void SetTextColor() {}
		void SetToolTips() {} 
		void SetWorkAreas() {} 
		void SubItemHitTest() {}

		void Arrange() {} 
		void CreateDragImage() {}
		void DeleteAllItems() {} 
		void DeleteColumn() {} 
		void DeleteItem() {} 
		void EditLabel() {} 
		void EnsureVisible() {}
		void FindItem() {} 
		void HitTest() {}
		void InsertColumn() {} 
		void InsertItem() {} 
		void RedrawItems() {} 
		void Scroll() {}
		void SortItems() {} 
		void Update() {} 

		void DrawItem() {} 

	};

} // namespace Win32xx

#endif // #ifndef LISTVIEW_H

