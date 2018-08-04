
#ifndef DXAPP_H
#define DXAPP_H

#include "DXView.h"

class CDXApp : public CWinApp
{
public:
    virtual BOOL InitInstance();
    virtual int MessageLoop();

private:
    CDXView m_dxView;

};

#endif // DXAPP_H
