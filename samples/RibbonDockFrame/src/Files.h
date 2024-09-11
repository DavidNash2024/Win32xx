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
    virtual ~CViewFiles() override;

    void InsertItems();
    void SetDPIColumnWidths();
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewFiles(const CViewFiles&) = delete;
    CViewFiles& operator=(const CViewFiles&) = delete;

    // Message handlers
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);

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
    virtual ~CContainFiles() override {}

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainFiles(const CContainFiles&) = delete;
    CContainFiles& operator=(const CContainFiles&) = delete;

    CViewFiles m_viewFiles;
};


///////////////////////////////////////
// CDockFiles manages a docker. It uses
// CContainFiles as its view.
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() override {}

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockFiles(const CDockFiles&) = delete;
    CDockFiles& operator=(const CDockFiles&) = delete;

    CContainFiles m_files;
};


#endif // FILES_H
