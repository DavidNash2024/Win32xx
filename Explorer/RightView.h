//////////////////////////////////////////////////////
// RightView.h
//  Declaration of the CMyListView class


#ifndef RIGHTVIEW_H
#define RIGHTVIEW_H


#include "../Win32++/ListView.h"
#include <Windowsx.h>
#include <vector>



class CMyListView : public CListView
{
public:
	CMyListView();
	virtual ~CMyListView();
	void DisplayFolder(CShellFolder& cParentFolder, Cpidl& cpidlParent, Cpidl& cpidlRel);
	void DoDisplay();
	void ViewLargeIcons();
	void ViewList();
	void ViewSmallIcons();
	void ViewReport();

protected:
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	class ListItemData  //A nested class inside CMyListView
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
	void DeleteItems();
	void DoBackgroundMenu(LPPOINT pptScreen);
	void DoContextMenu(LPPOINT pptScreen);
	void DoDefault(int iItem);
	void DoItemMenu(LPINT piItems, UINT cbItems, LPPOINT pptScreen);
	void EnumObjects(CShellFolder& cPFolder, Cpidl& cpidlFull);
	BOOL GetFileSizeText(HANDLE hFile, LPTSTR szSize);
	BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString);
	HIMAGELIST GetImageList(BOOL bLarge);
	void SetImageLists();

	Cpidl		  m_cpidlCurFull;  //Fully Qualified pidl
	CShellFolder  m_csfCurFolder;  //Current Folder
	CContextMenu2 m_ccm2;
	std::vector <ListItemData*> m_pItems;
	HIMAGELIST m_hLargeImageList;
	HIMAGELIST m_hSmallImageList;

};

#endif  // RIGHTVIEW_H
