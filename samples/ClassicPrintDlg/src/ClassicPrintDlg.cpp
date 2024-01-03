/////////////////////////////
// ClassicPrintDialog.cpp
//

#include "stdafx.h"
#include "resource.h"
#include "ClassicPrintDlg.h"
#include "UserMessages.h"


CClassicPrintDlg::CClassicPrintDlg()
    : CDialog(IDD_PRINTDLG), m_copies(1), m_fromPage(1), m_radio(0), m_toPage(9999), m_collate(0), m_printToFile(0)
{
    m_hDevMode = 0;
    m_hDevNames = 0;
    m_owner = 0;
}

CClassicPrintDlg::~CClassicPrintDlg()
{
}

// Used by UpdateData to exchange data with the dialog.
void  CClassicPrintDlg::DoDataExchange(CDataExchange& DX)
{
    // Connect the dialog controls to member variables.
    DX.DDX_Control(IDC_COMBOBOX, m_comboBox);
    DX.DDX_Control(IDS_FROM, m_editFrom);
    DX.DDX_Control(IDS_TO, m_editTo);
    DX.DDX_Control(IDS_COPIES, m_editCopies);

    // Connect radio boxes value to int member variable.
    DX.DDX_Radio(IDB_RADIOALL, m_radio);

    // Connect check box values to int member variables.
    DX.DDX_Check(IDB_COLLATE, m_collate);
    DX.DDX_Check(IDB_PRINTTOFILE, m_printToFile);

    // Connect numeric edit boxes values to int member variables.
    DX.DDX_Text(IDS_FROM, m_fromPage);
    DX.DDX_Text(IDS_TO, m_toPage);
    DX.DDX_Text(IDS_COPIES, m_copies);

    // Connect text edit boxes values to CString member variables.
    DX.DDX_Text(IDS_STATUS, m_status);
    DX.DDX_Text(IDS_TYPE, m_type);
    DX.DDX_Text(IDS_WHERE, m_where);
    DX.DDX_Text(IDS_COMMENT, m_comment);
}

// Display the print dialog, and allow the user to select various options.
// An exception is thrown if the dialog isn't created.
// An exception is thrown if there is no default printer.
inline INT_PTR CClassicPrintDlg::DoModal(HWND owner)
{
    assert(!IsWindow());    // Only one window per CWnd instance allowed

    if (FindPrinters().size() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());

    
    GetApp()->UpdateDefaultPrinter();

    // Assign values to member variables.
    m_hDevMode = GetApp()->GetHDevMode();
    m_hDevNames = GetApp()->GetHDevNames();
    m_owner = owner;

    // Display the modal dialog.
    INT_PTR result = CDialog::DoModal(owner);
    if (result == -1)
    {
        int error = static_cast<int>(CommDlgExtendedError());

        // Reset global memory
        m_hDevMode = 0;
        m_hDevNames = 0;
        GetApp()->ResetPrinterMemory();
        throw CWinException(GetApp()->MsgWndDialog(), error);
    }

    return result;
}

// Display the print setup dialog, and allow the user to select various options.
// An exception is thrown if there is no default printer.
INT_PTR CClassicPrintDlg::DoSetupModal(HWND owner)
{
    if (FindPrinters().size() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());

    // Update the default printer
    GetApp()->UpdateDefaultPrinter();

    // Display the print setup dialog.
    PRINTDLG pd;
    ZeroMemory(&pd, sizeof(pd));
    pd.lStructSize = sizeof(pd);
    pd.hDevMode = GetApp()->GetHDevMode();
    pd.hDevNames = GetApp()->GetHDevNames();
    pd.Flags = PD_PRINTSETUP;
    pd.hwndOwner = owner;

    INT_PTR result = ::PrintDlg(&pd);
    if (result != 0)
    {
        // User pressed OK
        GetApp()->UpdatePrinterMemory(pd.hDevMode, pd.hDevNames);
        SetPrinterFromDevMode(GetDeviceName().c_str(), GetDevMode());
        m_hDevMode = 0;
        m_hDevNames = 0;
    }
    else
    {
        m_hDevMode = 0;
        m_hDevNames = 0;
        int error = static_cast<int>(CommDlgExtendedError());
        if (error != 0)
            throw CWinException(GetApp()->MsgWndDialog(), error);
    }

    return result;
}

// Returns a vector of CString printer names.
std::vector<CString> CClassicPrintDlg::FindPrinters()
{
    std::vector<CString> printerNames;

    DWORD dwSize = 0;
    DWORD dwPrinters = 0;
    DWORD level = 2;        // for PRINTER_INFO_2
    ::SetLastError(0);

    // Identify size of PRINTER_INFO_2 buffer required.
    ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
        NULL, level, NULL, 0, &dwSize, &dwPrinters);

    // Check for ERROR_INSUFFICIENT_BUFFER
    if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        // Allocate the size of our vector.
        std::vector<BYTE> buffer(dwSize);

        // Fill the buffer. The buffer is actually an array of PRINTER_INFO_2.
        VERIFY(::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
            NULL, level, &buffer.front(), dwSize, &dwSize, &dwPrinters));

        // Do we have any printers?
        if (dwPrinters != 0)
        {
            // Get our array of PRINTER_INFO_2
            PRINTER_INFO_2* pInfo = reinterpret_cast<PRINTER_INFO_2*>(&buffer.front());

            for (DWORD i = 0; i < dwPrinters; i++, pInfo++)
            {
                // Store the printer name in our vector of CString.
                printerNames.push_back(pInfo->pPrinterName);
            }
        }
    }

    return printerNames;
}

// Retrieves the number of copies requested.
int CClassicPrintDlg::GetCopies() const
{
    return m_copies;
}

// Sets the printer and the page settings to default, without displaying a dialog.
// The hDevMode and hDevNames memory is freed and reallocated.
// Returns TRUE if a default printer exists.
BOOL CClassicPrintDlg::GetDefaults()
{
    GetApp()->ResetPrinterMemory();

    m_hDevMode = 0;
    m_hDevNames = 0;

    // Return TRUE if default printer exists
    return (GetApp()->GetHDevNames().Get()) ? TRUE : FALSE;
}

CDevMode CClassicPrintDlg::GetDevMode()
{
    if (GetApp()->GetHDevMode().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    if (GetApp()->GetHDevMode().Get() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());
    return CDevMode(GetApp()->GetHDevMode());
}

CDevNames CClassicPrintDlg::GetDevNames()
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    if (GetApp()->GetHDevNames().Get() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());
    return CDevNames(GetApp()->GetHDevNames());
}

CString CClassicPrintDlg::GetDriverName()
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    CString str;
    if (GetApp()->GetHDevNames().Get() != 0)
        str = GetDevNames().GetDriverName();
    return str;
}

CString CClassicPrintDlg::GetDeviceName()
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    CString str;
    if (GetApp()->GetHDevNames().Get() != 0)
        str = GetDevNames().GetDeviceName();
    return str;
}

CString CClassicPrintDlg::GetPortName()
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    CString str;
    if (GetApp()->GetHDevNames().Get() != 0)
        str = GetDevNames().GetPortName();
    return str;
}

// Returns the device context of the default or currently chosen printer.
// Throws on failure.
CDC CClassicPrintDlg::GetPrinterDC()
{
    CDC dc;
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    if ((GetApp()->GetHDevNames().Get() != 0) && (GetApp()->GetHDevMode().Get() != 0))
    {
        dc.CreateDC(GetDriverName(), GetDeviceName(),
            GetPortName(), GetDevMode());
    }
    else
        throw CResourceException(GetApp()->MsgPrintFound());


    return dc;
}

// Creates and assigns the hDevMode and hDevNames global memory for the specified printer.
BOOL CClassicPrintDlg::CreateGlobalHandles(LPCTSTR printerName, HGLOBAL* pHDevMode, HGLOBAL* pHDevNames)
{
    // HGLOBAL pHdevMode and pHDevNames are required.
    if (pHDevMode == NULL || pHDevNames == NULL)
        return FALSE;

    // Open printer
    HANDLE printer;
    if (OpenPrinter((LPTSTR)printerName, &printer, NULL) == FALSE)
        return FALSE;

    // Create PRINTER_INFO_2 structure.
    DWORD bytesNeeded = 0;
    GetPrinter(printer, 2, NULL, 0, &bytesNeeded);
    std::vector<BYTE> infoBuffer(bytesNeeded);
    PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)reinterpret_cast<PRINTER_INFO_2*>(&infoBuffer.front());

    // Fill the PRINTER_INFO_2 structure and close the printer handle.
    if (GetPrinter(printer, 2, &infoBuffer.front(), bytesNeeded, &bytesNeeded) == 0)
    {
        ::ClosePrinter(printer);
        return FALSE;
    }
    ::ClosePrinter(printer);

    // Allocate a global handle for DEVMODE.
    size_t bufferSize = sizeof(DEVMODE) + p2->pDevMode->dmDriverExtra;
    HGLOBAL newDevMode = GlobalAlloc(GHND, bufferSize);
    assert(newDevMode);
    if (newDevMode != 0)
    {
        // copy DEVMODE data from PRINTER_INFO_2::pDevMode.
        CDevMode pNewDevMode(newDevMode);
        memcpy(pNewDevMode, p2->pDevMode, sizeof(*p2->pDevMode) +
            p2->pDevMode->dmDriverExtra);

        // Compute size of DEVNAMES structure from PRINTER_INFO_2's data.
        size_t driverLength = lstrlen(p2->pDriverName) + 1;  // driver name
        size_t printerLength = lstrlen(p2->pPrinterName) + 1; // printer name
        size_t portLength = lstrlen(p2->pPortName) + 1;    // port name

        // Allocate a global handle big enough to hold DEVNAMES.
        bufferSize = sizeof(DEVNAMES) + (driverLength + printerLength + portLength) * sizeof(TCHAR);
        HGLOBAL newDevNames = GlobalAlloc(GHND, bufferSize);
        assert(newDevNames);
        CDevNames pNewDevNames(newDevNames);

        // Copy the DEVNAMES information from PRINTER_INFO_2.
        // tcOffset = TCHAR Offset into structure.
        size_t tcOffset = sizeof(DEVNAMES) / sizeof(TCHAR);

        pNewDevNames->wDriverOffset = static_cast<WORD>(tcOffset);
        memcpy(pNewDevNames.GetString() + tcOffset, p2->pDriverName,
            driverLength * sizeof(TCHAR));
        tcOffset = tcOffset + driverLength;

        pNewDevNames->wDeviceOffset = static_cast<WORD>(tcOffset);
        memcpy(pNewDevNames.GetString() + tcOffset, p2->pPrinterName,
            printerLength * sizeof(TCHAR));
        tcOffset = tcOffset + printerLength;

        pNewDevNames->wOutputOffset = static_cast<WORD>(tcOffset);
        memcpy(pNewDevNames.GetString() + tcOffset, p2->pPortName,
            portLength * sizeof(TCHAR));
        pNewDevNames->wDefault = 0;

        // Set the new hDevMode and hDevNames.
        *pHDevMode = newDevMode;
        *pHDevNames = newDevNames;
        return TRUE;
    }

    return FALSE;
}

// Retrieves the starting page of the print range.
int CClassicPrintDlg::GetFromPage() const
{
    return (IsPrintRange() ? m_fromPage : -1);
}

// Retrieves the ending page of the print range.
int CClassicPrintDlg::GetToPage() const
{
    return (IsPrintRange() ? m_toPage : -1);
}

// Call this function after calling DoModal to determine whether to print
// all pages in the document.
bool CClassicPrintDlg::IsPrintAll() const
{
    return (!IsPrintRange() && !IsPrintSelection());
}

// Call this function after calling DoModal to determine whether the printer
// should collate all printed copies of the document.
bool CClassicPrintDlg::IsPrintCollate()
{
    return !!m_collate;
}

// Call this function after calling DoModal to determine whether to print
// only a range of pages in the document.
bool CClassicPrintDlg::IsPrintRange() const
{
    return (m_radio == IDB_RADIORANGE - IDB_RADIOALL);
}

// Call this function after calling DoModal to determine whether to print
// only the currently selected items.
bool CClassicPrintDlg::IsPrintSelection() const
{
    return (m_radio == IDB_RADIOSELECTION - IDB_RADIOALL);
}

BOOL CClassicPrintDlg::OnPrintProperties()
{
    assert(m_owner != 0);
    // Get the printer name.
    CString dev = GetDeviceName();
    LPTSTR deviceName = (LPTSTR)dev.c_str();

    // Retrieve the printer handle with PRINTER_ALL_ACCESS if we can.
    HANDLE printer;
    PRINTER_DEFAULTS printerDefaults;
    ZeroMemory(&printerDefaults, sizeof(printerDefaults));
    printerDefaults.DesiredAccess = PRINTER_ALL_ACCESS;
    if (::OpenPrinter(deviceName, &printer, &printerDefaults) == FALSE)
        if (::OpenPrinter(deviceName, &printer, 0) == FALSE)
            return false;

    // Allocate the pDevMode buffer as an array of BYTE.
    size_t devModeSize = ::DocumentProperties(*this, printer, deviceName, 0, GetDevMode(), 0);
    std::vector<BYTE> buffer(devModeSize);
    LPDEVMODE pDevMode = reinterpret_cast<DEVMODE*>(&buffer.front());

    // Display the printer property sheet, and retrieve the updated devMode data.
    if (IDOK == ::DocumentProperties(0, printer, deviceName, pDevMode, 0, DM_IN_PROMPT | DM_OUT_BUFFER))
    {
        SetPrinterFromDevMode(deviceName, pDevMode);
        HGLOBAL newDevMode = 0;
        HGLOBAL newDevNames = 0;
        if (CreateGlobalHandles(deviceName, &newDevMode, &newDevNames))
        {
            // copy the updated devMode data to our global memory.
            CDevMode pNewDevMode(newDevMode);
            memcpy(pNewDevMode, pDevMode, devModeSize);

            // Save the new global memory handles.
            // CWinApp is assigned ownership of of the global memory.
            GetApp()->ResetPrinterMemory();
            m_hDevMode = newDevMode;
            m_hDevNames = newDevNames;
            GetApp()->UpdatePrinterMemory(newDevMode, newDevNames);
        }
        else
            return FALSE;
    }

    VERIFY(::ClosePrinter(printer));

    ::SendMessage(m_owner, UWM_PROPERTIESCLOSED, 0, 0);
    UpdateData(m_dx, SENDTOCONTROL);
    return TRUE;
}

BOOL CClassicPrintDlg::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDB_PRINTPROPERTIES:    return OnPrintProperties();

    case IDB_RADIOALL:
    case IDB_RADIORANGE:
    case IDB_RADIOSELECTION:
        CheckRadioButton(IDB_RADIOALL, IDB_RADIOSELECTION, id);
        UpdateData(m_dx, READFROMCONTROL);
        return TRUE;
    }

    UINT msg = HIWORD(wparam);
    switch (msg)
    {

    case CBN_SELCHANGE:
    {

        CString deviceName;
        int item = m_comboBox.GetCurSel();
        int length = m_comboBox.GetLBTextLen(item);
        m_comboBox.GetLBText(item, deviceName.GetBuffer(length));
        deviceName.ReleaseBuffer();
        if (deviceName != GetDeviceName())
        {
            HGLOBAL newHDevMode = 0;
            HGLOBAL newHDevNames = 0;
            CreateGlobalHandles(deviceName, &newHDevMode, &newHDevNames);
            GetApp()->ResetPrinterMemory();
            m_hDevNames = newHDevNames;
            m_hDevMode = newHDevMode;
            GetApp()->UpdatePrinterMemory(newHDevMode, newHDevNames);

            SetDlgItemText(IDS_WHERE, GetPortName());
            SetDlgItemText(IDS_TYPE, GetDriverName());
        }
    }
    break;
    }

    return FALSE;
}

BOOL CClassicPrintDlg::OnInitDialog()
{
    UpdateData(m_dx, SENDTOCONTROL);

    std::vector<CString> names = FindPrinters();
    std::vector<CString>::iterator it;
    for (it = names.begin(); it != names.end(); ++it)
    {
        m_comboBox.AddString(*it);
    }

    int item = m_comboBox.FindString(0, GetDeviceName());
    m_comboBox.SetCurSel(item);

    // Set the dialog's PortName and DriverName text.
    HGLOBAL hDevMode = 0;
    HGLOBAL hDevNames = 0;
    if (CreateGlobalHandles(GetDeviceName(), &hDevMode, &hDevNames))
    {
        CDevNames pDevNames(hDevNames);

        SetDlgItemText(IDS_WHERE, pDevNames.GetPortName());
        SetDlgItemText(IDS_TYPE, pDevNames.GetDriverName());
    }
    if (hDevMode != 0)
        ::GlobalFree(hDevMode);
    if (hDevNames != 0)
        ::GlobalFree(hDevNames);

    return TRUE;
}

void CClassicPrintDlg::OnOK()
{
    UpdateData(m_dx, READFROMCONTROL);
    GetApp()->UpdatePrinterMemory(m_hDevMode, m_hDevNames);
    m_hDevMode = 0;
    m_hDevNames = 0;

    CDialog::OnOK();
}


BOOL CClassicPrintDlg::SetPrinterFromDevMode(LPCTSTR deviceName, LPDEVMODE pDevMode)
{
    assert(deviceName);
    assert(pDevMode);

    // Retrieve the printer handle with PRINTER_ALL_ACCESS if we can.
    HANDLE printer;
    PRINTER_DEFAULTS printerDefaults;
    ZeroMemory(&printerDefaults, sizeof(printerDefaults));
    printerDefaults.DesiredAccess = PRINTER_ALL_ACCESS;
    if (::OpenPrinter((LPTSTR)deviceName, &printer, &printerDefaults) == FALSE)
        if (::OpenPrinter((LPTSTR)deviceName, &printer, 0) == FALSE)
            throw CWinException(_T("Failed to get printer handle."));;

    // Determine the size of the printerInfo buffer.
    DWORD bufferSize = 0;
    SetLastError(0);
    VERIFY(::GetPrinter(printer, 2, 0, 0, &bufferSize) == FALSE);
    if ((GetLastError() != ERROR_INSUFFICIENT_BUFFER) || (bufferSize == 0))
        throw CWinException(_T("Failed to get printer info buffer size."));

    // Allocate the printerInfo global memory buffer.
    CHGlobal hDevMode(bufferSize);
    CDevMode devMode(hDevMode);
    PRINTER_INFO_2* printerInfo = reinterpret_cast<PRINTER_INFO_2*>(devMode.Get());

    // Update the printer.
    if (::GetPrinter(printer, 2, reinterpret_cast<LPBYTE>(printerInfo), bufferSize, &bufferSize))
    {
        printerInfo->pDevMode = pDevMode;
        ::SetPrinter(printer, 2, reinterpret_cast<LPBYTE>(printerInfo), 0);
        return TRUE;
    }

    return FALSE;
}
