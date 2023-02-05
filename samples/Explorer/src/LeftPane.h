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
    virtual ~CLeftPane() {}

private:
    CLeftPane(const CLeftPane&);                // Disable copy construction
    CLeftPane& operator = (const CLeftPane&);   // Disable assignment operator

    CMyTreeView m_view;
};



#endif // _LEFTPANE_H_

