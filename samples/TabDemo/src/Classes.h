/////////////////////////////
// Classes.h
//

#ifndef CLASSES_H
#define CLASSES_H


//////////////////////////////////////////////////
// CViewClasses manages a tree view control.
// It is used as a view window by the tab control.
class CViewClasses : public CTreeView
{
public:
    CViewClasses();
    virtual ~CViewClasses();

protected:
    // Virtual functions that override base class functions 
    virtual void OnAttach();

private:
    CImageList m_normalImages;
};


#endif  //CLASSES_H
