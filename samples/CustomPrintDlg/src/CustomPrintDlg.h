/////////////////////////////
// CustomPrintDialog.h
//

#ifndef CUSTOMPRINTDLG_H
#define CUSTOMPRINTDLG_H

#include "resource.h"

class CCustomPrintDlg : public CDialog
{
public:
    CCustomPrintDlg();
    virtual ~CCustomPrintDlg();

    virtual INT_PTR DoModal(HWND owner = NULL);

    INT_PTR DoSetupModal(HWND owner = NULL);
    int  GetCopies() const;
    CDevMode GetDevMode() const;
    CDevNames GetDevNames() const;
    CString GetDriverName() const;
    CString GetDeviceName() const;
    CString GetPortName() const;
    CDC  GetPrinterDC() const;
    DWORD GetPrinterStatus(LPCTSTR szPrnName) const;
    int GetFromPage() const;
    int GetRadio() const;
    int GetPrintToFile() const;
    int GetToPage() const;
    bool IsCollate() const;
    bool IsPrintAll() const;
    bool IsPrintRange() const;
    bool IsPrintSelection() const;
    bool IsPrintToFile() const;
    void SetCollate(bool isCollate);
    void SetCopies(int copies);
    void SetFromPage(int fromPage);
    void SetMaxPage(int maxPage);
    void SetPrintToFile(bool isPrintToFile);
    void SetRadio(int radio);
    void SetToPage(int toPage);
    void UpdateStatusText();

protected:
    virtual void DoDataExchange(CDataExchange& DX);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    CCustomPrintDlg(const CCustomPrintDlg&);
    CCustomPrintDlg& operator=(const CCustomPrintDlg&);

    bool CreateGlobalHandles(LPCTSTR printerName, HGLOBAL* pHDevMode, HGLOBAL* pHDevNames);
    std::vector<CString> FindPrinters() const;
    BOOL OnPrintProperties();
    BOOL OnComboSelection();
    BOOL OnRadioSelection(UINT id);
    bool SetPrinterFromDevMode(LPCTSTR deviceName, LPDEVMODE pDevMode);

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
    int m_maxPage;

    CString m_status;
    CString m_type;
    CString m_where;
    CString m_comment;

    bool m_isPropertiesDisplayed;
};


#endif   // CUSTOMPRINTDLG_H
