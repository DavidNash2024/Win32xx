/////////////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles
//           and CDockFiles classes.
//

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

    void InsertItems();
    void SetColumns();

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_smallImages;
};


//////////////////////////////////////////////////
// CContainFiles manages a dock container. It uses
// CViewFiles as its view.
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    virtual ~CContainFiles() {}

private:
    CViewFiles m_viewFiles;
};


/////////////////////////////////////////////////////
// CDockFiles manages a docker. It uses CContainFiles
// as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CContainFiles m_files;
};


#endif // FILES_H
