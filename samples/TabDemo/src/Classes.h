//////////////////////////////////////////////////////////
// Classes.h
//  Declaration of CViewClasses 

#ifndef CLASSES_H
#define CLASSES_H


class CViewClasses : public CTreeView
{
public:
    CViewClasses();
    virtual ~CViewClasses();
    virtual void OnAttach();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

private:
    CImageList m_normalImages;
    
};



#endif  //CLASSES_H
