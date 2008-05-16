//////////////////////////////////////////////////////
// LeftView.h
//  Declaration of the CLeftView class


#ifndef LEFTVIEW_H
#define LEFTVIEW_H


#include <vector>
#include "ShellWrapper.h"

using namespace ShellWrapper;


class CLeftView : public CWnd
{
public:
	CLeftView();
	virtual ~CLeftView();
	BOOL GetRootItems();
	BOOL SelectFromListView(Cpidl& CpidlFull);

 protected:
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class TreeItemData //A nested class inside CLeftView
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
	void DoItemMenu(HTREEITEM hItem, LPPOINT pptScreen);
	void DoContextMenu(LPPOINT pptScreen);
	void EnumObjects(HTREEITEM hParentItem, CShellFolder& cParentFolder, Cpidl& cpidlParent);
	BOOL GetChildItems(HTREEITEM hParentItem);
	HIMAGELIST GetImageList(BOOL bLarge);
	void SetImageLists();

	HIMAGELIST m_hLargeImageList;
	HIMAGELIST m_hSmallImageList;
	CContextMenu2 m_ccm2;
	std::vector <TreeItemData*> m_pItems;
};

#endif  // LEFTVIEW_H
