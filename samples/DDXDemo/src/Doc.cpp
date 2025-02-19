/* (20-Oct-2024) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CDoc class implementation. This class
    interfaces the document, held in the registry, with the remainder of the
    DDX/DDV Demonstration program. This class is a modified version of that
    found in the FormDocView sample distributed with the Win32++ Windows
    interface classes. The modified sample program was based on code provided
    by Lynn Allan. This demo extends the given sample by application of
    Dialog Data Exchange and Validation (DDX/DDV).

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

    Acknowledgement: The author would like to thank and acknowledge the advice,
    critical review, insight, and assistance provided by David Nash in the
    development of this work.

*******************************************************************************/

#include "stdafx.h"
#include "Doc.h"

/*============================================================================*/
    CDoc::
CDoc()                                                                  /*

    Construct the basic document object.
*-----------------------------------------------------------------------------*/
{
      //Initialize the values of ALL controls using DDX/DDV.  These are
      // overwritten by saved values from the registry after initial
      // startup, but it is important to have valid values for the
      // initial startup.
    m_byteVal       = 10;
    m_shortVal      = 0;
    m_intVal        = 0;
    m_UINTVal       = 10;
    m_longVal       = 0L;
    m_ULongVal      = 10;
    m_floatVal      = 0.0;
    m_doubleVal     = 0.0;
    m_LPWSTRVal[0]  = L'\0';
    m_checkVal[0]     = FALSE;
    m_checkVal[1]     = FALSE;
    m_checkVal[2]     = FALSE;
    m_radioA        = 0;
    m_listBoxIndx   = 0;
    m_comboBoxIndx  = 0;
    m_sliderVal     = 0;
    CTime t = CTime::GetCurrentTime();
    t.GetAsSystemTime(m_dateSysTime);
    m_calDateSysTime = m_dateSysTime;
}

/*============================================================================*/
    void CDoc::
LoadDocRegistry(LPCWSTR keyName)                                            /*

    Load the saved document value parameters from the registry from the
    'HKEY_CURRENT_USER\Software\keyName\Document Settings' key.
*-----------------------------------------------------------------------------*/
{
    CRegKey key;
    CString strKey;
    strKey << L"Software\\" << keyName << L"\\Document Settings";
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
    {
        m_byteVal   = static_cast<BYTE>(RegQueryDWORDValue(key, L"Byte1"));
        m_shortVal  = static_cast<short>(RegQueryDWORDValue(key, L"Short1"));
        m_intVal    = RegQueryDWORDValue(key, L"Int1");
        m_UINTVal   = RegQueryDWORDValue(key, L"UINT1");
        m_longVal   = RegQueryDWORDValue(key, L"Long1");
        m_ULongVal  = RegQueryDWORDValue(key, L"ULong1");
        CString s   = RegQueryStringValue(key, L"Float1");
        LPWSTR p;
        m_floatVal  = static_cast<float>(_tcstod(s.c_str(), &p));
        s           = RegQueryStringValue(key, L"Double1");
        m_doubleVal = _tcstod(s, &p);
        s           = RegQueryStringValue(key, L"LPWSTR1");
        StrCopy(m_LPWSTRVal, s.c_str(), 256);
        m_checkVal[0] = RegQueryBOOLValue(key,   L"CheckA");
        m_checkVal[1] = RegQueryBOOLValue(key,   L"CheckB");
        m_checkVal[2] = RegQueryBOOLValue(key,   L"CheckC");
        m_radioA      = RegQueryDWORDValue(key,  L"Radio");
        m_editVal     = RegQueryStringValue(key, L"Edit1");
        m_richEditVal = RegQueryStringValue(key, L"RichEdit1");
        m_listBoxVal  = RegQueryStringValue(key, L"ListBox1");
        m_listBoxIndx = RegQueryDWORDValue(key,  L"ListBox1x");
        m_comboBoxVal = RegQueryStringValue(key, L"ComboBox1");
        m_comboBoxIndx = RegQueryDWORDValue(key, L"ComboBox1x");
        m_sliderVal   = RegQueryDWORDValue(key,  L"Slider1");
        m_dateSysTime = RegQuerySYSTEMTIMEValue(key, L"MyDateTime");
        m_calDateSysTime = RegQuerySYSTEMTIMEValue(key, L"MoCalendar1");
    }
}

/*============================================================================*/
    BOOL CDoc::
RegQueryBOOLValue(CRegKey& key, LPCWSTR pName)                              /*

    Return the BOOL value of a specified value pName found in the
    currently open registry key. Here, the boolean value is stored in a
    DWORD slot, rather than a bool-sized slot.
*-----------------------------------------------------------------------------*/
{
    return RegQueryDWORDValue(key, pName) & 1;
}

/*============================================================================*/
    DWORD CDoc::
RegQueryDWORDValue(CRegKey& key, LPCWSTR pName)                             /*

    Return the DWORD value of a specified value pName found in the
    currently open registry key.
*-----------------------------------------------------------------------------*/
{
    DWORD dw;
    if (ERROR_SUCCESS == key.QueryDWORDValue(pName, dw))
        return dw;
    else
        return 0;
}

/*============================================================================*/
    SYSTEMTIME CDoc::
RegQuerySYSTEMTIMEValue(CRegKey& key, LPCWSTR pName)                        /*

    Return the SYSTEMTIME value of a specified value pName found in the
    currently open registry key.
*-----------------------------------------------------------------------------*/
{
    SYSTEMTIME value;
    ULONG size = sizeof(SYSTEMTIME);
    if (ERROR_SUCCESS != key.QueryBinaryValue(pName, &value, &size))
    {
        CTime t = CTime::GetCurrentTime();
        t.GetAsSystemTime(value);
    }
    return value;
}

/*============================================================================*/
    CString CDoc::
RegQueryStringValue(CRegKey &key, LPCWSTR pName)                            /*

    Return the CString value of a specified value pName found in the
    currently open registry key.
*-----------------------------------------------------------------------------*/
{
    ULONG len = 256;
    CString sValue;
    if (ERROR_SUCCESS == key.QueryStringValue(pName, sValue.GetBuffer(255), &len))
    {
        sValue.ReleaseBuffer();
        return sValue;
    }
    else
        return L"";
}

/*============================================================================*/
    void CDoc::
SaveDocRegistry(LPCWSTR keyName)                                          /*

    Write document value parameters into the registry key labeled
    'HKEY_CURRENT_USER\Software\keyName\Document Settings'.
*-----------------------------------------------------------------------------*/
{
    CString strKey;
    strKey << L"Software\\" << keyName << L"\\Document Settings";
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, nullptr, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, nullptr, nullptr);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        key.SetDWORDValue(L"CheckA",     m_checkVal[0]);
        key.SetDWORDValue(L"CheckB",     m_checkVal[1]);
        key.SetDWORDValue(L"CheckC",     m_checkVal[2]);
        key.SetDWORDValue(L"Radio",      m_radioA);
        key.SetDWORDValue(L"Byte1",      m_byteVal);
        key.SetDWORDValue(L"Short1",     m_shortVal);
        key.SetDWORDValue(L"Int1",       m_intVal);
        key.SetDWORDValue(L"UINT1",      m_UINTVal);
        key.SetDWORDValue(L"Long1",      m_longVal);
        key.SetDWORDValue(L"ULong1",     m_ULongVal);
        CString s;
        s.Format(L"%.*g", FLT_DIG, m_floatVal);
        key.SetStringValue(L"Float1",    s.c_str());
        s.Format(L"%.*g", FLT_DIG, m_doubleVal);
        key.SetStringValue(L"Double1",   s.c_str());
        key.SetStringValue(L"LPWSTR1",   m_LPWSTRVal);
        key.SetStringValue(L"Edit1",     m_editVal.c_str());
        key.SetStringValue(L"RichEdit1", m_richEditVal.c_str());
        key.SetStringValue(L"ListBox1",  m_listBoxVal.c_str());
        key.SetDWORDValue(L"ListBox1x",  m_listBoxIndx);
        key.SetStringValue(L"ComboBox1", m_comboBoxVal.c_str());
        key.SetDWORDValue(L"ComboBox1x", m_comboBoxIndx);
        key.SetDWORDValue(L"Slider1",    m_sliderVal);
        ULONG size = sizeof(SYSTEMTIME);
        key.SetBinaryValue(L"MyDateTime", static_cast<void*>(&m_dateSysTime),
            size);
        key.SetBinaryValue(L"MoCalendar1", static_cast<void*>(&m_calDateSysTime),
            size);
    }
}/*---------------------------------------------------------------------------*/

