//////////////////////////////////////////////////////////
// Files.h
//  Declaration of  CViewFiles

#ifndef FILES_H
#define FILES_H


class CViewFiles : public CListView
{
public:
	CViewFiles();
	virtual ~CViewFiles();
	virtual int  AddItem(LPCTSTR szText, int nImage);
	virtual void InsertItems();
	virtual void OnAttach();
	virtual void SetColumns();
	virtual BOOL SetSubItem(int nItem, int nSubItem, LPCTSTR szText);

private:
	CImageList m_imlSmall;

};


#endif // FILES_H

