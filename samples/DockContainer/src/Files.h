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
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void    OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewFiles(const CViewFiles&);               // Disable copy construction
    CViewFiles& operator=(const CViewFiles&);    // Disable assignment operator

    // Command handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);

    void InsertItems();
    void SetColumns();
};


//////////////////////////////////////////////////
// CContainFiles manages a dock container. It uses
// CViewFiles as its view.
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    virtual ~CContainFiles() {}

protected:
    // Virtual functions that override base class functions.
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CContainFiles(const CContainFiles&);              // Disable copy construction
    CContainFiles& operator=(const CContainFiles&);   // Disable assignment operator

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
    CDockFiles(const CDockFiles&);              // Disable copy construction
    CDockFiles& operator=(const CDockFiles&);   // Disable assignment operator

    CContainFiles m_files;
};


#endif // FILES_H
