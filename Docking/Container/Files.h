/////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles 
//           and CDockFiles classes


#ifndef FILES_H
#define FILES_H


#include "../../Win32++/docking.h"
#include "../../Win32++/listview.h"
#include "resource.h"

// Declaration of the CViewFiles class
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

// Declaration of the CContainFiles class
class CContainFiles : public CContainer
{
public:
	CContainFiles(); 
	~CContainFiles() {}

private:
	CViewFiles m_ViewFiles;
};

// Declaration of the CDockFiles class
class CDockFiles : public CDockable
{
public:
	CDockFiles(); 
	virtual ~CDockFiles() {}
	virtual void OnInitialUpdate();

private:
	CContainFiles m_Files;
};


#endif // FILES_H
