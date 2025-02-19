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
    CMainContainer() = default;
    virtual ~CMainContainer() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual void AddContainer(CDockContainer* pContainer, BOOL insert, BOOL selectPage) override;
    virtual void RemoveContainer(CDockContainer* pContainer, BOOL updateParent) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainContainer(const CMainContainer&) = delete;
    CMainContainer& operator=(const CMainContainer&) = delete;
};

#endif  // _MAIN_CONTAINER_H_
