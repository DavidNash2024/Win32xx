/////////////////////////////////////////////////
// Files.h - Declaration of the CViewFiles, CContainFiles
//           and CDockFiles classes
//

#ifndef FILES_H
#define FILES_H


//////////////////////////////////////
// Declaration of the CViewFiles class
//
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
    CImageList m_smallImages;
};

/////////////////////////////////////////
// Declaration of the CContainFiles class
//
class CContainFiles : public CDockContainer
{
public:
    CContainFiles();
    ~CContainFiles() {}

private:
    CViewFiles m_viewFiles;
};

//////////////////////////////////////
// Declaration of the CDockFiles class
//
class CDockFiles : public CDocker
{
public:
    CDockFiles();
    virtual ~CDockFiles() {}

private:
    CContainFiles m_files;
};


#endif // FILES_H
