/////////////////////////////////////////////
// MyDockable.h

#ifndef MYDOCKABLE_H
#define MYDOCKABLE_H

#include "../Win32++/dockable.h"
#include "View.h"

class CMyDockable : public CDockable
{
protected:
	// Override nested class
//	class CMyDockBar : public CDockBar
//	{
//	};
public:
	CMyDockable();
	virtual ~CMyDockable() {}
//	virtual CMyDockBar& GetDockBar() const 
//	{
		// Override GetDockbar to use overridden nested class
	//	TRACE("GetMyDockBar\n");
//		return (CMyDockBar&)m_MyDockBar;
//	} 
private:

	CView m_View;
//	CMyDockBar m_MyDockBar;
};


#endif // MYDOCKABLE_H

