/////////////////////////////
// DX12App.h
//

#pragma once

#include "wxx_wincore.h"
#include "DXView.h"

////////////////////////////////////////////////////////////////////
// CDirectX12App manages the application. It initializes the Win32++
// framework when it is constructed, and creates the DirectX12 window
// and starts the message loop when it runs.
class CDirectX12App : public CWinApp
{
public:
    CDirectX12App() = default;
    virtual ~CDirectX12App() override = default;

protected:
    virtual BOOL InitInstance() override;
    virtual BOOL OnIdle(LONG count) override;

private:
    CDXView m_view;
};
