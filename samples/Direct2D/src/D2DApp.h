
#ifndef _D2DAPP_H_
#define _D2DAPP_H_

#include <d2d1.h>
#include <d2d1helper.h>
#include "D2DView.h"


class CD2DApp : public CWinApp
{
public:
    CD2DApp();
    ~CD2DApp();

    BOOL InitInstance();
    ID2D1Factory* GetD2DFactory() { return m_pDirect2dFactory; }

private:
    HRESULT CreateDeviceIndependentResources();

    CD2DView m_view;
    ID2D1Factory* m_pDirect2dFactory;
};

inline ID2D1Factory* GetD2DFactory()
{
    return ((CD2DApp*)GetApp())->GetD2DFactory();
}


#endif //  _D2DAPP_H_

