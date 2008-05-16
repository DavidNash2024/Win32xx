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
		virtual void PreCreate(CREATESTRUCT &cs);

	};

	inline void CListView::PreCreate(CREATESTRUCT &cs)
	{
		cs.lpszClass = WC_LISTVIEW;
	}

} // namespace Win32xx

#endif // #ifndef LISTVIEW_H

