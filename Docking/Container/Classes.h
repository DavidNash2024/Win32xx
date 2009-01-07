//////////////////////////////////////////////////////////
// Classes.h
//  Declaration of CViewClasses and CContainClasses


#ifndef CLASSES_H
#define CLASSES_H


#include "../../Win32++/treeview.h"
#include "../../Win32++/dockable.h"
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

class CContainClasses : public CContainer
{
public:
	CContainClasses();
	~CContainClasses() {}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM /*lParam*/);

private:
	CViewClasses m_ViewClasses;

};


#endif  //CLASSES_H
