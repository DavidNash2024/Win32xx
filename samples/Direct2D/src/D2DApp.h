/////////////////////////////
// D2DApp.h
//

#ifndef _D2DAPP_H_
#define _D2DAPP_H_

#include <d2d1.h>
#include <d2d1helper.h>
#include "D2DView.h"


///////////////////////////////////////////////////////////////
// CD2DApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main
// window when it runs.
class CD2DApp : public CWinApp
{
public:
    CD2DApp();
    virtual ~CD2DApp();
    ID2D1Factory* GetD2DFactory() { return m_pDirect2dFactory; }

protected:
    // Virtual functions that override base class functions
    virtual BOOL InitInstance();

private:
    HRESULT CreateDeviceIndependentResources();

    // Member variables
    CD2DView m_view;
    ID2D1Factory* m_pDirect2dFactory;
};

inline ID2D1Factory* GetD2DFactory()
{
    return ((CD2DApp*)GetApp())->GetD2DFactory();
}


#endif //  _D2DAPP_H_

