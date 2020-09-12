/////////////////////////////
// RoundApp.h
//

#ifndef ROUNDAPP_H
#define ROUNDAPP_H

#include "View.h"

//////////////////////////////////////
// Declaration of the CRoundApp class.
//
class CRoundApp : public CWinApp
{
public:
    CRoundApp();
    virtual ~CRoundApp() {}
    virtual BOOL InitInstance();

private:
    CView m_view;
};


#endif
