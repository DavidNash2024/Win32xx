//////////////////////////////////////////////////////////
// Classes.h - Declaration of CViewClasses, CContainClasses,
//              and CDockClasses classes
//

#ifndef CLASSES_H
#define CLASSES_H


///////////////////////////////////////////////
// CViewClasses manages a tree view control.
// This is the view window for CContainClasses,
// and is used to display classes.
class CViewClasses : public CTreeView
{
public:
    CViewClasses() = default;
    virtual ~CViewClasses() override;
    void SetDPIImages();

protected:
    // Virtual functions that override base class functions.
    virtual void OnAttach() override;
    virtual void PreCreate(CREATESTRUCT& cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CViewClasses(const CViewClasses&) = delete;
    CViewClasses& operator=(const CViewClasses&) = delete;

    // Command handlers
    LRESULT OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);
};


////////////////////////////////////////////////////
// CContainClasses manages a dock container. It uses
// CViewClasses as its view.
class CContainClasses : public CDockContainer
{
public:
    CContainClasses();
    virtual ~CContainClasses() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual BOOL OnCommand(WPARAM, LPARAM) override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainClasses(const CContainClasses&) = delete;
    CContainClasses& operator=(const CContainClasses&) = delete;

    // Command handlers
    BOOL OnFileNew();
    BOOL OnHelpAbout();

    // Member variables
    CViewClasses m_viewClasses;
};


/////////////////////////////////////////
// CDockClasses manages a docker. It uses
// CContainClasses as its view.
class CDockClasses : public CDocker
{
public:
    CDockClasses();
    virtual ~CDockClasses() override = default;

protected:
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockClasses(const CDockClasses&) = delete;
    CDockClasses& operator=(const CDockClasses&) = delete;

    CContainClasses m_classes;
};


#endif  //CLASSES_H
