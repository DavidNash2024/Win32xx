//////////////////////////////////////////////////////
// ListView.h
//  Declaration of the CListView class


#ifndef LISTVIEW_H
#define LISTVIEW_H

//Include WinCore.h first
#include "..\Win32++\WinCore.h"
#include <Windowsx.h>
#include <vector>


class CListView : public CWnd
{
public:
	CListView();
	virtual ~CListView();
	virtual void DisplayFolder(CShellFolder& cParentFolder, Cpidl& cpidlParent, Cpidl& cpidlRel);
	virtual void DoDisplay();
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
	virtual void ViewLargeIcons();
	virtual void ViewList();
	virtual void ViewSmallIcons();
	virtual void ViewReport();

protected:
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class ListItemData  //A nested class inside CListView
	{
	public:
		ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder);
		~ListItemData();

		CShellFolder& GetParentFolder() {return m_cParentFolder;}
		Cpidl& GetFullPidl() {return m_cpidlFull;}
		Cpidl& GetRelPidl()  {return m_cpidlRel;}

	private:
		Cpidl        m_cpidlFull;	  //Fully Qualified PIDL
		Cpidl        m_cpidlRel;	  //Relative PIDL
		CShellFolder m_cParentFolder; //Parent IShellFolder

	};  //class ListItemData (nested class)

	static int CALLBACK CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	virtual void DeleteItems();
	virtual void DoBackgroundMenu(LPPOINT pptScreen);
	virtual void DoContextMenu(LPPOINT pptScreen);
	virtual void DoDefault(int iItem);
	virtual void DoItemMenu(LPINT piItems, UINT cbItems, LPPOINT pptScreen);
	virtual void EnumObjects(CShellFolder& cPFolder, Cpidl& cpidlFull);
	virtual BOOL GetFileSizeText(HANDLE hFile, LPTSTR szSize);
	virtual BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString);
	virtual HIMAGELIST GetImageList(BOOL bLarge);
	virtual void SetImageLists();
	virtual void SetMenuCheck(UINT ID);

	Cpidl		  m_cpidlCurFull;  //Fully Qualified pidl
	CShellFolder  m_csfCurFolder;  //Current Folder
	CContextMenu2 m_ccm2;
	WNDPROC       m_OldWindowProc; // Old Window Procedure
	std::vector <ListItemData*> m_pItems;
	HIMAGELIST m_hLargeImageList;
	HIMAGELIST m_hSmallImageList;
};

#endif  //LISTVIEW_H
