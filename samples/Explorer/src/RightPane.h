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
    virtual ~CRightPane() {}

protected:
    // Virtual functions that override base class functions
    virtual CDocker* NewDockerFromID(int id);

private:
    CRightPane(const CRightPane&);                // Disable copy construction
    CRightPane& operator = (const CRightPane&);   // Disable assignment operator

    CMyListView m_view;
};

#endif // _RIGHTPANE_H_

