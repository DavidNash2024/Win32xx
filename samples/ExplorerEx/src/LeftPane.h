/////////////////////
// LeftPane.h
//

#ifndef _LEFTPANE_H_
#define _LEFTPANE_H_

#include "MyTreeView.h"

// The docker identifiers (dock IDs)
const int ID_DOCK_LEFTPANE = 1;


////////////////////////////////////////////////////
// CLeftPane is a docker that manages the left view.
// CLeftPane's view window is CMyTreeView.
class CLeftPane : public CDocker
{
public:
    CLeftPane();
    virtual ~CLeftPane() override = default;

protected:
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CLeftPane(const CLeftPane&) = delete;
    CLeftPane& operator=(const CLeftPane&) = delete;

    CMyTreeView m_view;
};



#endif // _LEFTPANE_H_

