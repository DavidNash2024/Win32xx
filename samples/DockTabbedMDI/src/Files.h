/////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles
//           and CDockFiles classes
//

#ifndef FILES_H
#define FILES_H


/////////////////////////////////////////////////////////////
// CViewFiles manages a list view control. It displays files.
// This is the view window used by CContainFiles.
class CViewFiles : public CListView
{
public:
    CViewFiles();
    virtual ~CViewFiles();

protected:
    // Virtual functions that override base class functions
    virtual void    OnAttach();
    virtual void    OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    void InsertItems();
    void SetColumns();

    // Member variables
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


/////////////////////////////////////////
// CDockFiles manages a docker that has a
// CContainFiles view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CContainFiles m_files;
};


#endif // FILES_H
