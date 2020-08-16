/////////////////////////////
// InputDlg.h
//

#ifndef __INPUTDLG_H__
#define __INPUTDLG_H__

#include "resource.h"

//////////////////////////////////////
// Declaration of the CInputDlg class.
//
class CInputDlg : public CDialog
{
public:
    CInputDlg(UINT resID) : CDialog(resID), m_xMin(-10), m_xMax(10) {}
    virtual ~CInputDlg() {}

    virtual void DoDataExchange(CDataExchange& DX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    double& GetMax() { return m_xMax; }
    double& GetMin() { return m_xMin; }
    CString& GetFunction() { return m_function; }

private:
    double m_xMin;
    double m_xMax;
    CString m_function;
};

#endif  // __INPUTDLG_H__
