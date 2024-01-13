/////////////////////////////
// ClassicPrintDialog.cpp
//

#include "stdafx.h"
#include "resource.h"
#include "ClassicPrintDlg.h"
#include "UserMessages.h"


CClassicPrintDlg::CClassicPrintDlg()
    : CDialog(IDD_PRINTDLG), m_copies(1), m_fromPage(1), m_radio(0), m_toPage(9999),
                             m_collate(0), m_printToFile(0), m_isPropertiesDisplayed(false)
{
    m_hDevMode = 0;
    m_hDevNames = 0;
    m_owner = 0;
}

CClassicPrintDlg::~CClassicPrintDlg()
{
}

// Creates and assigns the hDevMode and hDevNames global memory for the specified printer.
bool CClassicPrintDlg::CreateGlobalHandles(LPCTSTR printerName, HGLOBAL* pHDevMode, HGLOBAL* pHDevNames)
{
    // HGLOBAL pHdevMode and pHDevNames are required.
    assert(pHDevMode);
    assert(pHDevNames);

    bool result = false;

    // Open printer
    HANDLE printer;
    if (::OpenPrinter(const_cast<LPTSTR>(printerName), &printer, NULL))
    {
        // Create PRINTER_INFO_2 structure.
        DWORD bytesNeeded = 0;
        ::GetPrinter(printer, 2, NULL, 0, &bytesNeeded);
        std::vector<BYTE> infoBuffer(bytesNeeded);
        PRINTER_INFO_2* p2 = reinterpret_cast<PRINTER_INFO_2*>(&infoBuffer.front());

        // Fill the PRINTER_INFO_2 structure and close the printer handle.
        if (::GetPrinter(printer, 2, &infoBuffer.front(), bytesNeeded, &bytesNeeded))
        {
            // Allocate a global handle for DEVMODE.
            size_t bufferSize = sizeof(DEVMODE) + p2->pDevMode->dmDriverExtra;
            HGLOBAL newDevMode = ::GlobalAlloc(GHND, bufferSize);
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
                HGLOBAL newDevNames = ::GlobalAlloc(GHND, bufferSize);
                assert(newDevNames);
                if (newDevNames != 0)
                {
                    CDevNames pNewDevNames(newDevNames);

                    // Copy the DEVNAMES information from PRINTER_INFO_2.
                    // offset = TCHAR Offset into structure.
                    size_t offset = sizeof(DEVNAMES) / sizeof(TCHAR);

                    pNewDevNames->wDriverOffset = static_cast<WORD>(offset);
                    memcpy(pNewDevNames.GetString() + offset, p2->pDriverName,
                        driverLength * sizeof(TCHAR));
                    offset = offset + driverLength;

                    pNewDevNames->wDeviceOffset = static_cast<WORD>(offset);
                    memcpy(pNewDevNames.GetString() + offset, p2->pPrinterName,
                        printerLength * sizeof(TCHAR));
                    offset = offset + printerLength;

                    pNewDevNames->wOutputOffset = static_cast<WORD>(offset);
                    memcpy(pNewDevNames.GetString() + offset, p2->pPortName,
                        portLength * sizeof(TCHAR));
                    pNewDevNames->wDefault = 0;

                    // Set the new hDevMode and hDevNames.
                    *pHDevMode = newDevMode;
                    *pHDevNames = newDevNames;

                    result = true;
                }
            }
        }

        ::ClosePrinter(printer);
    }

    return result;
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
        SetPrinterFromDevMode(GetDeviceName(), GetDevMode());
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
std::vector<CString> CClassicPrintDlg::FindPrinters() const
{
    std::vector<CString> printerNames;

    DWORD bufferSize = 0;
    DWORD printerCount = 0;
    DWORD level = 2;        // for PRINTER_INFO_2
    ::SetLastError(0);

    // Identify size of PRINTER_INFO_2 buffer required.
    ::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
        NULL, level, NULL, 0, &bufferSize, &printerCount);

    // Check for ERROR_INSUFFICIENT_BUFFER
    if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    {
        // Allocate the size of our vector.
        std::vector<BYTE> buffer(bufferSize);

        // Fill the buffer. The buffer is actually an array of PRINTER_INFO_2.
        VERIFY(::EnumPrinters(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS,
            NULL, level, &buffer.front(), bufferSize, &bufferSize, &printerCount));

        // Do we have any printers?
        if (printerCount != 0)
        {
            // Get our array of PRINTER_INFO_2
            PRINTER_INFO_2* pInfo = reinterpret_cast<PRINTER_INFO_2*>(&buffer.front());

            for (DWORD i = 0; i < printerCount; i++, pInfo++)
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

CDevMode CClassicPrintDlg::GetDevMode() const
{
    if (GetApp()->GetHDevMode().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    if (GetApp()->GetHDevMode().Get() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());
    return CDevMode(GetApp()->GetHDevMode());
}

CDevNames CClassicPrintDlg::GetDevNames() const
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    if (GetApp()->GetHDevNames().Get() == 0)
        throw CResourceException(GetApp()->MsgPrintFound());
    return CDevNames(GetApp()->GetHDevNames());
}

CString CClassicPrintDlg::GetDriverName() const
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    CString str;
    if (GetApp()->GetHDevNames().Get() != 0)
        str = GetDevNames().GetDriverName();
    return str;
}

CString CClassicPrintDlg::GetDeviceName() const
{
    if (GetApp()->GetHDevNames().Get() == 0)
        GetApp()->UpdateDefaultPrinter();
    CString str;
    if (GetApp()->GetHDevNames().Get() != 0)
        str = GetDevNames().GetDeviceName();
    return str;
}

CString CClassicPrintDlg::GetPortName() const
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
CDC CClassicPrintDlg::GetPrinterDC() const
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

int CClassicPrintDlg::GetPrintToFile() const
{
    return m_printToFile;
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

// Retrieves the status from the printer's PRINTER_INFO_2.
DWORD CClassicPrintDlg::GetPrinterStatus(LPCTSTR printerName) const
{
    DWORD status = 0;
    HANDLE printer = 0;

    if (::OpenPrinter(const_cast<LPTSTR>(printerName), &printer, 0))
    {
        // Create PRINTER_INFO_2 structure.
        DWORD bytesNeeded = 0;
        ::GetPrinter(printer, 2, NULL, 0, &bytesNeeded);
        std::vector<BYTE> infoBuffer(bytesNeeded);

        // Fill the PRINTER_INFO_2 structure.
        if (::GetPrinter(printer, 2, &infoBuffer.front(), bytesNeeded, &bytesNeeded))
        {
            PRINTER_INFO_2* printerInfo2 = reinterpret_cast<PRINTER_INFO_2*>(&infoBuffer.front());
            status = printerInfo2->Status;
        }
        ::ClosePrinter(printer);
    }

    return status;
}

int CClassicPrintDlg::GetRadio() const
{
    return m_radio;
}

// Call this function after calling DoModal to determine whether to print
// all pages in the document.
bool CClassicPrintDlg::IsPrintAll() const
{
    return (!IsPrintRange() && !IsPrintSelection());
}

// Call this function after calling DoModal to determine whether the printer
// should collate all printed copies of the document.
bool CClassicPrintDlg::IsCollate() const
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

bool CClassicPrintDlg::IsPrintToFile() const
{
    return m_printToFile != 0;
}

void CClassicPrintDlg::SetCollate(bool isCollate)
{
    m_collate = isCollate ? 1 : 0;
}

void CClassicPrintDlg::SetCopies(int copies)
{
    m_copies = copies;
}

void CClassicPrintDlg::SetFromPage(int fromPage)
{
    m_fromPage = fromPage;
}

void CClassicPrintDlg::SetPrintToFile(bool isPrintToFile)
{
    m_printToFile = isPrintToFile ? 1 : 0;
}

void CClassicPrintDlg::SetRadio(int radio)
{
    m_radio = radio;
}

void CClassicPrintDlg::SetToPage(int toPage)
{
    m_toPage = toPage;
}


BOOL CClassicPrintDlg::OnComboSelection()
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

        m_type = GetDriverName();
        m_where = GetPortName();
        UpdateStatusText();
    }

    ::SendMessage(m_owner, UWM_SETDEFAULTOPTIONS, 0, 0);
    UpdateData(m_dx, SENDTOCONTROL);
    return TRUE;
}

// Called when the Close button is pressed.
void CClassicPrintDlg::OnCancel()
{
    // Ignore Cancel button if the Properties dialog is displayed.
    if (!m_isPropertiesDisplayed)
        CDialog::OnCancel();
}

// Called when the dialog is closed.
void CClassicPrintDlg::OnClose()
{
    // Ignore close request if the Properties dialog is displayed.
    if (!m_isPropertiesDisplayed)
        CDialog::OnClose();
}

// Called when a button on the dialog is presseed, or
// a combox selection is made.
BOOL CClassicPrintDlg::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDB_PRINTPROPERTIES:    return OnPrintProperties();

    case IDB_RADIOALL:           // Intentionally blank
    case IDB_RADIORANGE:         // Intentionally blank
    case IDB_RADIOSELECTION:     return OnRadioSelection(id);
    }

    UINT msg = HIWORD(wparam);
    switch (msg)
    {
    case CBN_SELCHANGE:    return OnComboSelection();
    }

    return FALSE;
}

// Called before the modal dialog is displayed.
BOOL CClassicPrintDlg::OnInitDialog()
{
    // Set the dialog's PortName and DriverName text.
    HGLOBAL hDevMode = 0;
    HGLOBAL hDevNames = 0;
    if (CreateGlobalHandles(GetDeviceName(), &hDevMode, &hDevNames))
    {
        CDevNames pDevNames(hDevNames);

        m_type = pDevNames.GetDriverName();
        m_where = pDevNames.GetPortName();
    }
    if (hDevMode != 0)
        ::GlobalFree(hDevMode);
    if (hDevNames != 0)
        ::GlobalFree(hDevNames);

    UpdateStatusText();

    // Update the dialog.
    UpdateData(m_dx, SENDTOCONTROL);

    std::vector<CString> names = FindPrinters();
    std::vector<CString>::iterator it;
    for (it = names.begin(); it != names.end(); ++it)
    {
        m_comboBox.AddString(*it);
    }

    int item = m_comboBox.FindString(0, GetDeviceName());
    m_comboBox.SetCurSel(item);

    return TRUE;
}

// Called when the OK button is pressed.
void CClassicPrintDlg::OnOK()
{
    // Ignore OK button if the Properties dialog is displayed.
    if (!m_isPropertiesDisplayed)
    {
        UpdateData(m_dx, READFROMCONTROL);
        GetApp()->UpdatePrinterMemory(m_hDevMode, m_hDevNames);
        m_hDevMode = 0;
        m_hDevNames = 0;

        CDialog::OnOK();
    }
}

// Called when the properties button on the print dialog is pressed.
BOOL CClassicPrintDlg::OnPrintProperties()
{
    assert(m_owner != 0);

    // Ignore Properties button if the Properties dialog is already displayed.
    if (!m_isPropertiesDisplayed)
    {
        m_isPropertiesDisplayed = true;
        // Get the printer name.
        CString dev = GetDeviceName();
        LPTSTR deviceName = const_cast<LPTSTR>(dev.c_str());

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

        ::SendMessage(m_owner, UWM_SETDEFAULTOPTIONS, 0, 0);
        UpdateData(m_dx, SENDTOCONTROL);

        m_isPropertiesDisplayed = false;
    }
    return TRUE;
}

// Called when a radio button is selected
BOOL CClassicPrintDlg::OnRadioSelection(UINT id)
{
    CheckRadioButton(IDB_RADIOALL, IDB_RADIOSELECTION, id);
    UpdateData(m_dx, READFROMCONTROL);
    return TRUE;
}

// Sets the DEVMODE parameters of the specified printer.
bool CClassicPrintDlg::SetPrinterFromDevMode(LPCTSTR deviceName, LPDEVMODE pDevMode)
{
    assert(deviceName);
    assert(pDevMode);

    // Retrieve the printer handle with PRINTER_ALL_ACCESS if we can.
    HANDLE printer;
    PRINTER_DEFAULTS printerDefaults;
    ZeroMemory(&printerDefaults, sizeof(printerDefaults));
    printerDefaults.DesiredAccess = PRINTER_ALL_ACCESS;
    if (::OpenPrinter(const_cast<LPTSTR>(deviceName), &printer, &printerDefaults) == FALSE)
        if (::OpenPrinter(const_cast<LPTSTR>(deviceName), &printer, 0) == FALSE)
            throw CWinException(_T("Failed to get printer handle."));;

    // Determine the size of the printerInfo buffer.
    DWORD bufferSize = 0;
    SetLastError(0);
    VERIFY(::GetPrinter(printer, 2, 0, 0, &bufferSize) == FALSE);
    if ((GetLastError() != ERROR_INSUFFICIENT_BUFFER) || (bufferSize == 0))
        throw CWinException(_T("Failed to get printer info buffer size."));

    // Allocate a buffer for the PRINTER_INFO_2.
    std::vector<BYTE> infoBuffer(bufferSize);
    PRINTER_INFO_2* printerInfo2 = reinterpret_cast<PRINTER_INFO_2*>(&infoBuffer.front());

    // Update the printer.
    if (::GetPrinter(printer, 2, &infoBuffer.front(), bufferSize, &bufferSize))
    {
        printerInfo2->pDevMode = pDevMode;
        ::SetPrinter(printer, 2, &infoBuffer.front(), 0);
        return true;
    }

    return false;
}

// Retrieves the printer status and stores a text result.
void CClassicPrintDlg::UpdateStatusText()
{
    // Refer to the description of PRINTER_INFO_2 in the
    // windows API documentation for possible status values.
    DWORD status = GetPrinterStatus(GetDeviceName());
    CString statusText;
    if (status == 0)                      statusText = _T("Ready");
    if (status & PRINTER_STATUS_PAUSED)   statusText = _T("Paused");
    if (status & PRINTER_STATUS_ERROR)    statusText = _T("Error");
    if (status & PRINTER_STATUS_OFFLINE)  statusText = _T("Offline");
    if (status & PRINTER_STATUS_BUSY)     statusText = _T("Busy");
    if (status & PRINTER_STATUS_PRINTING) statusText = _T("Printing");
    if (status & PRINTER_STATUS_NOT_AVAILABLE) statusText = _T("Not available");
    m_status = statusText;
}
