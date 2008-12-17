//////////////////////////////////////////////////////////
// Files.h
//  Declaration of CContainFiles and CViewFiles


#ifndef FILES_H
#define FILES_H


#include "../Win32++/listview.h"
#include "../Win32++/container.h"
#include "resource.h"


class CViewFiles : public CListView
{
public:
	CViewFiles();
	virtual ~CViewFiles();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void InsertItems();
	virtual void OnInitialUpdate();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);

private:
	HIMAGELIST m_himlSmall;

};

class CContainFiles : public CContainer
{
public:
	CContainFiles(); 
	~CContainFiles() {}

private:
	CViewFiles m_ViewFiles;
};





#endif // FILES_H

