/////////////////////////////
// Files.h
//

#ifndef FILES_H
#define FILES_H

///////////////////////////////////////////////////////
// CViewFiles manages a list view control It is used as
// the view window by CContainFiles.
class CViewFiles : public CListView
{
public:
    CViewFiles();
    virtual ~CViewFiles();
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void SetColumns();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_imlSmall;
};


//////////////////////////////////////////////////
// CContainFiles manages a dock container. It uses
// CViewFiles as its view.
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    ~CContainFiles() {}

private:
    CViewFiles m_viewFiles;
};


///////////////////////////////////////
// CDockFiles manages a docker. It uses
// CContainFiles as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CContainFiles m_files;
};


#endif // FILES_H
