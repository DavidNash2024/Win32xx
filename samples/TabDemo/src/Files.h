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
    void InsertItems();
    void SetColumns();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();

private:
    CImageList m_smallImages;
};


#endif // FILES_H
