//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes

#ifndef CLASSES_H
#define CLASSES_H


#include "MyCombo.h"


// Declaration of the CViewClasses class
class CViewClasses : public CTreeView
{
public:
	CViewClasses();
	virtual ~CViewClasses();
	virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);

protected:
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);

private:
	HIMAGELIST m_himlNormal;
	
};

// Declaration of the CContainClasses class
class CContainClasses : public CDockContainer
{
public:
	CContainClasses();
	virtual ~CContainClasses() {}
	void AddCombo();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolBar();

private:
	CViewClasses m_ViewClasses;
	CMyCombo m_ComboBoxEx;
};

// Declaration of the CDockClasses class
class CDockClasses : public CDocker
{
public:
	CDockClasses(); 
	virtual ~CDockClasses() {}
	virtual void OnInitialUpdate();

private:
	CContainClasses m_Classes;

};



#endif  //CLASSES_H
