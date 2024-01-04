/////////////////////////////
// ClassicPrintDialog.h
//

#ifndef CLASSICPRINTDLG_H
#define CLASSICPRINTDLG_H

#include "resource.h"

class CClassicPrintDlg : public CDialog
{
public:
    CClassicPrintDlg();
    virtual ~CClassicPrintDlg();

    virtual INT_PTR DoModal(HWND owner = 0);
    virtual void DoDataExchange(CDataExchange& DX);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    INT_PTR DoSetupModal(HWND owner = 0);
    BOOL CreateGlobalHandles(LPCTSTR printerName, HGLOBAL* pHDevMode, HGLOBAL* pHDevNames);
    std::vector<CString> FindPrinters();
    int  GetCollate() const { return m_collate; }
    int  GetCopies() const;
    CDevMode GetDevMode();
    CDevNames GetDevNames();
    CString GetDriverName();
    CString GetDeviceName();
    CString GetPortName();
    CDC  GetPrinterDC();
    int GetFromPage() const;
    int GetRadio() const { return m_radio; }
    int GetToPage() const;
    bool IsPrintAll() const;
    bool IsPrintCollate();
    bool IsPrintRange() const;
    bool IsPrintSelection() const;
    BOOL OnPrintProperties();
    BOOL OnComboSelection();
    BOOL OnRadioSelection(UINT id);
    void SetCollate(int collate) { m_collate = collate; }
    void SetCopies(int copies) { m_copies = copies; }
    void SetFromPage(int fromPage) { m_fromPage = fromPage; }
    void SetRadio(int radio) { m_radio = radio; }
    void SetToPage(int toPage) { m_toPage = toPage; }
    BOOL SetPrinterFromDevMode(LPCTSTR deviceName, LPDEVMODE pDevMode);

private:
    CClassicPrintDlg(const CClassicPrintDlg&);
    CClassicPrintDlg& operator=(const CClassicPrintDlg&);

    CDataExchange m_dx;    // DataExchange object, called by UpdateData
    HGLOBAL m_hDevMode;    // Global memory for printer's DevMode
    HGLOBAL m_hDevNames;   // Global memory for printer's DevNames
    HWND m_owner;          // Owner window set in DoModal

    CComboBox m_comboBox;
    CEdit m_editFrom;
    CEdit m_editTo;
    CEdit m_editCopies;

    int m_copies;
    int m_fromPage;
    int m_radio;
    int m_toPage;
    int m_collate;
    int m_printToFile;

    CString m_status;
    CString m_type;
    CString m_where;
    CString m_comment;
};


#endif   // CLASSICPRINTDLG_H
