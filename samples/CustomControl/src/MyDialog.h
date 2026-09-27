/////////////////////////////
// MyDialog.h
//

#pragma once

#include "MyWebControl.h"

///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog() override = default;

protected:
    virtual void    OnDestroy() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    OnCancel() override;
    virtual BOOL    OnInitDialog() override;
    virtual void    OnOK() override;

private:
    CMyDialog(const CMyDialog&) = delete;
    CMyDialog& operator=(const CMyDialog&) = delete;

    CResizer m_resizer;
    CWebControl m_webControl;
};

