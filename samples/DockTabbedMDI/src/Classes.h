//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes


#ifndef CLASSES_H
#define CLASSES_H


#include "MyCombo.h"


// Declaration of the CViewClasses class
class CViewClasses : public CTreeView
{
public:
    CViewClasses();
    virtual ~CViewClasses();
    virtual HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR text, int image);

protected:
    virtual void OnAttach();
    virtual void OnDestroy();
    virtual void PreCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CImageList m_normalImages;

};

// Declaration of the CContainClasses class
class CContainClasses : public CDockContainer
{
public:
    CContainClasses();
    virtual ~CContainClasses() {}
    virtual void AddCombo();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnFileNew();
    virtual void OnHelpAbout();
    virtual void SetupToolBar();

private:
    CViewClasses m_viewClasses;
    CMyCombo m_comboBoxEx;
};

// Declaration of the CDockClasses class
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() {}

private:
    CContainClasses m_classes;

};



#endif  //CLASSES_H
