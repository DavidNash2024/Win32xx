/////////////////////////////
// InputDlg.h
//

#ifndef __INPUTDLG_H__
#define __INPUTDLG_H__

#include "resource.h"


/////////////////////////////////////////////
// CInputDlg manages the dialog used to input
// the function for plotting.
class CInputDlg : public CDialog
{
public:
    CInputDlg(UINT resID) : CDialog(resID), m_xMin(-10), m_xMax(10) {}
    virtual ~CInputDlg() override = default;
    double GetMax() const { return m_xMax; }
    double GetMin() const { return m_xMin; }
    const CString& GetFunction() const { return m_function; }
    void SetFunction(const CString& function) { m_function = function; }

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void DoDataExchange(CDataExchange& DX) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override;

private:
    CInputDlg(const CInputDlg&) = delete;
    CInputDlg& operator=(const CInputDlg&) = delete;

    double m_xMin;
    double m_xMax;
    CString m_function;
};


#endif  // __INPUTDLG_H__
