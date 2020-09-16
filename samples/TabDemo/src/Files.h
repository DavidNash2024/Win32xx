/////////////////////////////
// Files.h
//

#ifndef FILES_H
#define FILES_H


//////////////////////////////////////////////////
// CViewDialog manages a list view control.
// It is used as a view window by the tab control.
class CViewFiles : public CListView
{
public:
    CViewFiles();
    virtual ~CViewFiles();
    virtual int  AddItem(LPCTSTR text, int image);
    virtual void InsertItems();
    virtual void OnAttach();
    virtual void SetColumns();

private:
    CImageList m_smallImages;
};

#endif // FILES_H
