//////////////////////////////////////////////////////
// TreeView.h
//  Declaration of the CTreeView class


#ifndef TREEVIEW_H
#define TREEVIEW_H


#include <vector>
#include "ShellWrapper.h"

using namespace ShellWrapper;


class CTreeView : public CWnd
{
public:
	CTreeView();
	virtual ~CTreeView();
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
	virtual BOOL GetRootItems();
	virtual BOOL SelectFromListView(Cpidl& CpidlFull);

protected:
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class TreeItemData //A nested class inside CTreeView
	{
	public:
		TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder);
		TreeItemData(Cpidl& cpidl);
		~TreeItemData();

		CShellFolder& GetParentFolder() {return m_cParentFolder;}
		Cpidl& GetFullCpidl() {return m_cpidlFull;}
		Cpidl& GetRelCpidl()  {return m_cpidlRel;}

	private:
		Cpidl        m_cpidlFull;		//Fully Qualified PIDL
		Cpidl        m_cpidlRel;		//Relative PIDL
		CShellFolder m_cParentFolder;	//Parent Folder
	
	};  //class TreeItemData

	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual void DoItemMenu(HTREEITEM hItem, LPPOINT pptScreen);
	virtual void DoContextMenu(LPPOINT pptScreen);
	virtual void EnumObjects(HTREEITEM hParentItem, CShellFolder& cParentFolder, Cpidl& cpidlParent);
	virtual BOOL GetChildItems(HTREEITEM hParentItem);
	virtual HIMAGELIST GetImageList(BOOL bLarge);
	virtual void SetImageLists();

	HIMAGELIST m_hLargeImageList;
	HIMAGELIST m_hSmallImageList;
	CContextMenu2 m_ccm2;
	std::vector <TreeItemData*> m_pItems;
	WNDPROC       m_OldWindowProc;	// Old Window Procedure
};

#endif  //TREEVIEW_H
