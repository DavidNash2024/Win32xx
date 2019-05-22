/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (Doc.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CDoc class implementation. This class
    interfaces the document, held in the registry, with the remainder of the
    DDX/DDV Demonstration program. This class is a modified version of that
    found in the FormDocView sample distributed with the Win32++ Windows
    interface classes, Copyright (c) 2005-2015 David Nash, used under
    permissions granted therein. The modified sample program was based on
    code provided by Lynn Allan. This demo extends the given sample by
    application of Dialog Data Exchange and Validation (DDX/DDV).

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and  to the alterations,
    additions, deletions, and  other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and  any associated documentation composed by
    the author, to utilize this material, free of charge and  without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Special Conventions:

    Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.
        
    Acknowledgement:
    The author would like to thank and acknowledge the advice, critical
    review, insight, and assistance provided by David Nash in the development
    of this work.       

********************************************************************************

    Implementation of the CDoc class

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
    m_iByte     = 10;  
    m_iShort    = 0;
    m_iInt      = 0;
    m_iUINT     = 10;  
    m_iLong     = 0L;
    m_ULong     = 10;  
    m_fFloat    = 0.0;
    m_dDouble   = 0.0;
    m_LPTSTR[0] = _T('\0');
    m_iCheckA   = FALSE;
    m_iCheckB   = FALSE;
    m_iCheckC   = FALSE;
    m_iRadioA   = 0;
    m_iListBox  = 0;
    m_iComboBox = 0;
    m_iSlider   = 0;
    CTime::GetCurrentTime().GetAsSystemTime(m_stDateTime);
    m_stMoCalendar = m_stDateTime;
}

/*============================================================================*/
    CDoc::
~CDoc()                                                                 /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
    void CDoc::
LoadDocRegistry(LPCTSTR szKeyName)                                      /*

    Load the saved document value parameters from the registry from the
    'HKEY_CURRENT_USER\Software\szKeyName\Document Settings' key.
*-----------------------------------------------------------------------------*/
{
    CRegKey key;
    CString strKey = _T("Software\\") + (CString)szKeyName +
        _T("\\Document Settings");
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_READ))
    {
        m_iByte     = static_cast<BYTE>(RegQueryDWORDValue(key, _T("Byte1")));
        m_iShort    = static_cast<short>(RegQueryDWORDValue(key, _T("Short1")));
        m_iInt      = RegQueryDWORDValue(key, _T("Int1"));
        m_iUINT     = RegQueryDWORDValue(key, _T("UINT1"));
        m_iLong     = RegQueryDWORDValue(key, _T("Long1"));
        m_ULong     = RegQueryDWORDValue(key, _T("ULong1"));
        CString s   = RegQueryStringValue(key, _T("Float1"));
        LPTSTR p;
        m_fFloat    = static_cast<float>(_tcstod(s.c_str(), &p));
        s           = RegQueryStringValue(key, _T("Double1"));
        m_dDouble   = _tcstod(s, &p);
        s           = RegQueryStringValue(key, _T("LPTSTR1"));
        StrCopy(m_LPTSTR, s.c_str(), 256);
        m_iCheckA   = RegQueryBOOLValue(key,   _T("CheckA"));
        m_iCheckB   = RegQueryBOOLValue(key,   _T("CheckB"));
        m_iCheckC   = RegQueryBOOLValue(key,   _T("CheckC"));
        m_iRadioA   = RegQueryDWORDValue(key,  _T("Radio"));
        m_sString   = RegQueryStringValue(key, _T("Edit1"));
        m_sRichEdit = RegQueryStringValue(key, _T("RichEdit1"));
        m_sListBox  = RegQueryStringValue(key, _T("ListBox1"));
        m_iListBox  = RegQueryDWORDValue(key,  _T("ListBox1x"));
        m_sComboBox = RegQueryStringValue(key, _T("ComboBox1"));
        m_iComboBox = RegQueryDWORDValue(key,  _T("ComboBox1x"));
        m_iSlider   = RegQueryDWORDValue(key,  _T("Slider1"));
        m_stDateTime = RegQuerySYSTEMTIMEValue(key, _T("MyDateTime"));
        m_stMoCalendar = RegQuerySYSTEMTIMEValue(key, _T("MoCalendar1"));
    }
}

/*============================================================================*/
    BOOL CDoc::
RegQueryBOOLValue(CRegKey& key, LPCTSTR pName)              /*

    Return the BOOL value of a specified value pName found in the
    currently open registry key. Here, the boolean value is stored in a
    DWORD slot, rather than a bool-sized slot.
*-----------------------------------------------------------------------------*/
{
    return RegQueryDWORDValue(key, pName) & 1;
}

/*============================================================================*/
    DWORD CDoc::
RegQueryDWORDValue(CRegKey& key, LPCTSTR pName)             /*

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
RegQuerySYSTEMTIMEValue(CRegKey& key, LPCTSTR pName)            /*

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
RegQueryStringValue(CRegKey &key, LPCTSTR pName)            /*

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
SaveDocRegistry(LPCTSTR szKeyName)                                      /*

    Write document value parameters into the registry key labeled
    'HKEY_CURRENT_USER\Software\szKeyName\Document Settings'.
*-----------------------------------------------------------------------------*/
{
    CString strKey = _T("Software\\") + (CString)szKeyName  +
        _T("\\Document Settings");
    CRegKey key;
    key.Create(HKEY_CURRENT_USER, strKey, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, NULL);
      // Create() closes the key handle, so we have to reopen it
    if (ERROR_SUCCESS == key.Open(HKEY_CURRENT_USER, strKey, KEY_WRITE))
    {
        key.SetDWORDValue(_T("CheckA"),     m_iCheckA);
        key.SetDWORDValue(_T("CheckB"),     m_iCheckB);
        key.SetDWORDValue(_T("CheckC"),     m_iCheckC);
        key.SetDWORDValue(_T("Radio"),      m_iRadioA);
        key.SetDWORDValue(_T("Byte1"),      m_iByte);
        key.SetDWORDValue(_T("Short1"),     m_iShort);
        key.SetDWORDValue(_T("Int1"),       m_iInt);
        key.SetDWORDValue(_T("UINT1"),      m_iUINT);
        key.SetDWORDValue(_T("Long1"),      m_iLong);
        key.SetDWORDValue(_T("ULong1"),     m_ULong);
        CString s;
        s.Format(_T("%.*g"), FLT_DIG, m_fFloat);
        key.SetStringValue(_T("Float1"),    s.c_str());
        s.Format(_T("%.*g"), FLT_DIG, m_dDouble);
        key.SetStringValue(_T("Double1"),   s.c_str());
        key.SetStringValue(_T("LPTSTR1"),   m_LPTSTR);
        key.SetStringValue(_T("Edit1"),     m_sString.c_str());
        key.SetStringValue(_T("RichEdit1"), m_sRichEdit.c_str());
        key.SetStringValue(_T("ListBox1"),  m_sListBox.c_str());
        key.SetDWORDValue(_T("ListBox1x"),  m_iListBox);
        key.SetStringValue(_T("ComboBox1"), m_sComboBox.c_str());
        key.SetDWORDValue(_T("ComboBox1x"), m_iComboBox);
        key.SetDWORDValue(_T("Slider1"),    m_iSlider);
        ULONG size = sizeof(SYSTEMTIME);
        key.SetBinaryValue(_T("MyDateTime"), static_cast<void*>(&m_stDateTime),
            size);
        key.SetBinaryValue(_T("MoCalendar1"), static_cast<void*>(&m_stMoCalendar),
            size);
    }
}

