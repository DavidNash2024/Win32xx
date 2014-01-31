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
	HIMAGELIST m_himlNormal;
};


class CMDIChildTreeView : public CMDIChild
{
public:
	CMDIChildTreeView();
	virtual ~CMDIChildTreeView();

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();

private:
	CViewTree m_TreeView;
};

#endif  //MDICHILDTREEVIEW_H
