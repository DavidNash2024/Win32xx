/////////////////////
// RightPane.h
//

#ifndef _RIGHTPANE_H_
#define _RIGHTPANE_H_

#include "MyListView.h"



//////////////////////////////////////////////////////
// CRightPane is a docker that manages the right view.
// CRightPane's view window is CMyListView.
class CRightPane : public CDocker
{
public:
    CRightPane();
    virtual ~CRightPane() override = default;

protected:
    virtual DockPtr NewDockerFromID(int id) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CRightPane(const CRightPane&) = delete;
    CRightPane& operator=(const CRightPane&) = delete;

    CMyListView m_view;
};

#endif // _RIGHTPANE_H_

