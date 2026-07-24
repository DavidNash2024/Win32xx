/////////////////////////////
// DX12App.h
//


#pragma once

#include "wxx_wincore.h"
#include "DXView.h"


class CDirectX12App : public CWinApp
{
public:
    CDirectX12App() = default;
    virtual ~CDirectX12App() override = default;

protected:
    virtual BOOL InitInstance() override;
    virtual int MessageLoop() override;

private:
    CDXView m_view;
};
