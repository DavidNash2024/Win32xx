//////////////////////////////////////////////////////////
// MDIChildTreeView.h
//  Declaration of the CViewTree and CMDIChildTreeView classes

#ifndef MDICHILDTREEVIEW_H
#define MDICHILDTREEVIEW_H


class CViewTree : public CTreeView
{
public:
	CViewTree();
	virtual ~CViewTree();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR sxText, int iImage);

private:
	CImageList m_imlNormal;
};


class CMDIChildTreeView : public CMDIChild
{
public:
	CMDIChildTreeView();
	virtual ~CMDIChildTreeView();

protected:
	virtual int  OnCreate(LPCREATESTRUCT pcs);

private:
	CViewTree m_TreeView;
	CMenu m_Menu;
};

#endif  //MDICHILDTREEVIEW_H
