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
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions
    virtual void OnAttach();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CViewClasses(const CViewClasses&);                // Disable copy construction
    CViewClasses& operator = (const CViewClasses&);   // Disable assignment operator

    CImageList m_normalImages;

    // Command handlers
    LRESULT OnUserDPIChanged(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif  //CLASSES_H
