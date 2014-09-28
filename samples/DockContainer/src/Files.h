/////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles 
//           and CDockFiles classes

#ifndef FILES_H
#define FILES_H


// Declaration of the CViewFiles class
class CViewFiles : public CListView
{
public:
	CViewFiles();
	virtual ~CViewFiles();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void InsertItems();
	virtual void OnDestroy();
	virtual void OnInitialUpdate();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);

private:
	CImageList m_imlSmall;
};

// Declaration of the CContainFiles class
class CContainFiles : public CDockContainer
{
public:
	CContainFiles(); 
	~CContainFiles() {}

private:
	CViewFiles m_ViewFiles;
};

// Declaration of the CDockFiles class
class CDockFiles : public CDocker
{
public:
	CDockFiles(); 
	virtual ~CDockFiles() {}
	virtual int OnCreate(LPCREATESTRUCT pcs);

private:
	CContainFiles m_Files;
};


#endif // FILES_H
