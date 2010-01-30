//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes

#ifndef CLASSES_H
#define CLASSES_H


#include "ComboBoxEx.h"


// Declaration of the CViewClasses class
class CViewClasses : public CTreeView
{
public:
	CViewClasses();
	virtual ~CViewClasses();
	virtual void OnInitialUpdate();
	virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HIMAGELIST m_himlNormal;
	
};

// Declaration of the CContainClasses class
class CContainClasses : public CContainer
{
public:
	CContainClasses();
	virtual ~CContainClasses() {}
	void AddCombo();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();

private:
	CViewClasses m_ViewClasses;
	CComboBoxEx m_ComboBoxEx;
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
