//////////////////////////////////////////////////////////
// Classes.h
//  Declaration of CViewClasses 

#ifndef CLASSES_H
#define CLASSES_H


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



#endif  //CLASSES_H
