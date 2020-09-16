//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes
//

#ifndef CLASSES_H
#define CLASSES_H

#include "MyCombo.h"


///////////////////////////////////////////////
// CViewClasses manages a tree view control.
// This is the view window for CContainClasses,
// and is used to display classes.
class CViewClasses : public CTreeView
{
public:
    CViewClasses();
    virtual ~CViewClasses();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_normalImages;

};


////////////////////////////////////////////////////
// CContainClasses manages a dock container. It uses
// CViewClasses as its view.
class CContainClasses : public CDockContainer
{
public:
    CContainClasses();
    virtual ~CContainClasses() {}
    virtual void AddCombo();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnFileNew();
    virtual BOOL OnHelpAbout();
    virtual void SetupToolBar();

private:
    CViewClasses m_viewClasses;
    CMyCombo m_comboBoxEx;
};


/////////////////////////////////////////
// CDockClasses manages a docker. It uses
// CContainClasses as its view.
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() {}

private:
    CContainClasses m_classes;

};


#endif  //CLASSES_H
