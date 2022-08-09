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
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewFiles(const CViewFiles&);                // Disable copy construction
    CViewFiles& operator = (const CViewFiles&);   // Disable assignment operator

    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void InsertItems();
    void SetColumns();

    // Member variables
    CImageList m_smallImages;
};


/////////////////////////////////////////////////////
// CContainClasses manages a dock container. It has a
// CViewClasses view.
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    virtual ~CContainFiles() {}

private:
    CContainFiles(const CContainFiles&);                // Disable copy construction
    CContainFiles& operator = (const CContainFiles&);   // Disable assignment operator

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

protected:
    virtual int OnCreate(CREATESTRUCT& cs);

private:
    CDockFiles(const CDockFiles&);                // Disable copy construction
    CDockFiles& operator = (const CDockFiles&);   // Disable assignment operator

    CContainFiles m_files;
};


#endif // FILES_H
