//////////////////////////////////////////////////////////
// Classes.h
//  Declaration of CViewClasses and CContainClasses


#ifndef CLASSES_H
#define CLASSES_H


#include "../../Win32++/treeview.h"
#include "../../Win32++/tab.h"
#include "resource.h"


class CViewClasses : public CTreeView
{
public:
	CViewClasses();
	virtual ~CViewClasses();
	virtual void OnInitialUpdate();
	virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);

private:
	HIMAGELIST m_himlNormal;
	
};



#endif  //CLASSES_H
