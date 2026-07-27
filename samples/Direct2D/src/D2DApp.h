/////////////////////////////
// D2DApp.h
//

#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <wrl/client.h> // Required header for Microsoft::WRL::ComPtr
#include "D2DView.h"


///////////////////////////////////////////////////////////////
// CD2DApp manages the application. It initializes the Win32++
// framework when it is constructed, and creates the main
// window when it runs.
class CD2DApp : public CWinApp
{
public:
    CD2DApp() = default;
    virtual ~CD2DApp() override = default;

protected:
    virtual BOOL InitInstance() override;

private:
    CD2DApp(const CD2DApp&) = delete;
    CD2DApp& operator=(const CD2DApp&) = delete;

    CD2DView m_view;
};
