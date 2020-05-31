//////////////////////////////////////////////////////////
// Files.h
//  Declaration of  CViewFiles

#ifndef FILES_H
#define FILES_H


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

