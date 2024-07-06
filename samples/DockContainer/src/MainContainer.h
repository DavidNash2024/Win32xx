//////////////////////////////////////////////////////////////
// MainContainer.h - Declaration of the CMainContainer classes
//

#ifndef _MAIN_CONTAINER_H_
#define _MAIN_CONTAINER_H_

/////////////////////////////////////////////////////////////
// CMainContainer manages the dock frame's dock container.
// It ensures the dock frame's dock container has at least
// one tab that can't be undocked or closed.
class CMainContainer : public CDockContainer
{
public:
    CMainContainer() {}
    virtual ~CMainContainer() {}

protected:
    // Virtual functions that override base class functions.
    virtual void AddContainer(CDockContainer* pContainer, BOOL insert, BOOL selectPage);
    virtual void RemoveContainer(CDockContainer* pContainer, BOOL updateParent);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainContainer(const CMainContainer&);               // Disable copy construction
    CMainContainer& operator=(const CMainContainer&);    // Disable assignment operator
};

#endif  // _MAIN_CONTAINER_H_
