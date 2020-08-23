/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
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

    Programming Notes: The programming standards roughly follow those
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

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
      // initil startup.
    m_byteVal       = 10;
    m_shortVal      = 0;
    m_intVal        = 0;
    m_UINTVal       = 10;
    m_longVal       = 0L;
    m_ULongVal      = 10;
    m_floatVal      = 0.0;
    m_doubleVal     = 0.0;
    m_LPTSTRVal[0]  = _T('\0');
    m_checkAVal     = FALSE;
    m_checkBVal     = FALSE;
    m_checkCVal     = FALSE;
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
LoadDocRegistry(LPCTSTR keyName)                                            /*

    Load the saved document value parameters from the registry from the
    'HKEY_CURRENT_USER\Software\keyName\Document Settings' key.
*-----------------------------------------------------------------------------*/
{
    CRegKey key;
    CString strKey = _T("Software\\") + (CString)keyName +
        _T("\\Document Settings");
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
    {
        m_byteVal   = static_cast<BYTE>(RegQueryDWORDValue(key, _T("Byte1")));
        m_shortVal  = static_cast<short>(RegQueryDWORDValue(key, _T("Short1")));
        m_intVal    = RegQueryDWORDValue(key, _T("Int1"));
        m_UINTVal   = RegQueryDWORDValue(key, _T("UINT1"));
        m_longVal   = RegQueryDWORDValue(key, _T("Long1"));
        m_ULongVal  = RegQueryDWORDValue(key, _T("ULong1"));
        CString s   = RegQueryStringValue(key, _T("Float1"));
        LPTSTR p;
        m_floatVal  = static_cast<float>(_tcstod(s.c_str(), &p));
        s           = RegQueryStringValue(key, _T("Double1"));
        m_doubleVal = _tcstod(s, &p);
        s           = RegQueryStringValue(key, _T("LPTSTR1"));
        StrCopy(m_LPTSTRVal, s.c_str(), 256);
        m_checkAVal = RegQueryBOOLValue(key,   _T("CheckA"));
        m_checkBVal = RegQueryBOOLValue(key,   _T("CheckB"));
        m_checkCVal = RegQueryBOOLValue(key,   _T("CheckC"));
        m_radioA    = RegQueryDWORDValue(key,  _T("Radio"));
        m_editVal   = RegQueryStringValue(key, _T("Edit1"));
        m_richEditVal = RegQueryStringValue(key, _T("RichEdit1"));
        m_listBoxVal  = RegQueryStringValue(key, _T("ListBox1"));
        m_listBoxIndx = RegQueryDWORDValue(key,  _T("ListBox1x"));
        m_comboBoxVal = RegQueryStringValue(key, _T("ComboBox1"));
        m_comboBoxIndx = RegQueryDWORDValue(key,  _T("ComboBox1x"));
        m_sliderVal   = RegQueryDWORDValue(key,  _T("Slider1"));
        m_dateSysTime = RegQuerySYSTEMTIMEValue(key, _T("MyDateTime"));
        m_calDateSysTime = RegQuerySYSTEMTIMEValue(key, _T("MoCalendar1"));
    }
}

/*============================================================================*/
    BOOL CDoc::
RegQueryBOOLValue(CRegKey& key, LPCTSTR pName)                              /*

    Return the BOOL value of a specified value pName found in the
    currently open registry key. Here, the boolean value is stored in a
    DWORD slot, rather than a bool-sized slot.
*-----------------------------------------------------------------------------*/
{
    return RegQueryDWORDValue(key, pName) & 1;
}

/*============================================================================*/
    DWORD CDoc::
RegQueryDWORDValue(CRegKey& key, LPCTSTR pName)                             /*

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
RegQuerySYSTEMTIMEValue(CRegKey& key, LPCTSTR pName)                        /*

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
RegQueryStringValue(CRegKey &key, LPCTSTR pName)                            /*

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
        return _T("");
}

/*============================================================================*/
    void CDoc::
SaveDocRegistry(LPCTSTR keyName)                                          /*

    Write document value parameters into the registry key labeled
    'HKEY_CURRENT_USER\Software\keyName\Document Settings'.
*-----------------------------------------------------------------------------*/
{
    CString strKey = _T("Software\\") + (CString)keyName  +
        _T("\\Document Settings");
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, NULL);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        key.SetDWORDValue(_T("CheckA"),     m_checkAVal);
        key.SetDWORDValue(_T("CheckB"),     m_checkBVal);
        key.SetDWORDValue(_T("CheckC"),     m_checkCVal);
        key.SetDWORDValue(_T("Radio"),      m_radioA);
        key.SetDWORDValue(_T("Byte1"),      m_byteVal);
        key.SetDWORDValue(_T("Short1"),     m_shortVal);
        key.SetDWORDValue(_T("Int1"),       m_intVal);
        key.SetDWORDValue(_T("UINT1"),      m_UINTVal);
        key.SetDWORDValue(_T("Long1"),      m_longVal);
        key.SetDWORDValue(_T("ULong1"),     m_ULongVal);
        CString s;
        s.Format(_T("%.*g"), FLT_DIG, m_floatVal);
        key.SetStringValue(_T("Float1"),    s.c_str());
        s.Format(_T("%.*g"), FLT_DIG, m_doubleVal);
        key.SetStringValue(_T("Double1"),   s.c_str());
        key.SetStringValue(_T("LPTSTR1"),   m_LPTSTRVal);
        key.SetStringValue(_T("Edit1"),     m_editVal.c_str());
        key.SetStringValue(_T("RichEdit1"), m_richEditVal.c_str());
        key.SetStringValue(_T("ListBox1"),  m_listBoxVal.c_str());
        key.SetDWORDValue(_T("ListBox1x"),  m_listBoxIndx);
        key.SetStringValue(_T("ComboBox1"), m_comboBoxVal.c_str());
        key.SetDWORDValue(_T("ComboBox1x"), m_comboBoxIndx);
        key.SetDWORDValue(_T("Slider1"),    m_sliderVal);
        ULONG size = sizeof(SYSTEMTIME);
        key.SetBinaryValue(_T("MyDateTime"), static_cast<void*>(&m_dateSysTime),
            size);
        key.SetBinaryValue(_T("MoCalendar1"), static_cast<void*>(&m_calDateSysTime),
            size);
    }
}/*---------------------------------------------------------------------------*/

