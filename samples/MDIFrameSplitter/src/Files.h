/////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles
//           and CDockFiles classes

#ifndef FILES_H
#define FILES_H


/////////////////////////////////////////////////////////////
// CViewFiles manages a list view control. It displays files.
// This is the view window for CContainClasses.
class CViewFiles : public CListView
{
public:
    CViewFiles();
    virtual ~CViewFiles();

protected:
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void SetColumns();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_smallImages;
};


/////////////////////////////////////////////////////
// CContainClasses manages a dock container. It has a
// CViewClasses view.
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    ~CContainFiles() {}

private:
    CViewFiles m_viewFiles;
};


/////////////////////////////////////////
// CDockFiles manages a docker that has a
// CContainFiles view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CContainFiles m_files;
};


#endif // FILES_H
