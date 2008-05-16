#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "../Win32++/WinCore.h"


	namespace Win32xx
	{

	class CTreeView : public CWnd
	{
	public:
		CTreeView() {}
		virtual ~CTreeView() {}

		virtual void PreCreate(CREATESTRUCT &cs);
	};

	inline void CTreeView::PreCreate(CREATESTRUCT &cs)
	{
		cs.lpszClass = WC_TREEVIEW;
	}

} // namespace Win32xx

#endif // #ifndef TREEVIEW_H

