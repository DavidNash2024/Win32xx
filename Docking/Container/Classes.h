//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes


#ifndef CLASSES_H
#define CLASSES_H


#include "../../Win32++/treeview.h"
#include "../../Win32++/dockable.h"
#include "resource.h"

// Declaration of the CViewClasses class
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

// Declaration of the CContainClasses class
class CContainClasses : public CContainer
{
public:
	CContainClasses();
	~CContainClasses() {}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM /*lParam*/);

private:
	CViewClasses m_ViewClasses;

};

// Declaration of the CDockClasses class
class CDockClasses : public CDockable
{
public:
	CDockClasses(); 
	virtual ~CDockClasses() {}
	virtual void OnInitialUpdate();

private:
	CContainClasses m_Classes;

};



#endif  //CLASSES_H
