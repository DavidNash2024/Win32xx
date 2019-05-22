/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CDoc class declaration. This class
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

    Declaration of the CDoc class

*******************************************************************************/

#ifndef SDI_DOC_H
#define SDI_DOC_H

/*============================================================================*/
    class
CDoc    : public CObject                        /*

    Declaration of the CDoc class
*-----------------------------------------------------------------------------*/
{
    public:
        CDoc();
        virtual ~CDoc();
        void    LoadDocRegistry(LPCTSTR szKeyName);
        void    SaveDocRegistry(LPCTSTR szKeyName);
        BOOL    RegQueryBOOLValue(CRegKey& key, LPCTSTR pName);
        DWORD   RegQueryDWORDValue(CRegKey& key, LPCTSTR pName);
        CString RegQueryStringValue(CRegKey& key, LPCTSTR pName);
        SYSTEMTIME RegQuerySYSTEMTIMEValue(CRegKey &key, LPCTSTR pName);
        BOOL    GetCheckA() const {return m_iCheckA;}
        void    SetCheckA(BOOL IsChecked) { m_iCheckA = IsChecked;}

        BOOL    GetCheckB() const {return m_iCheckB;}
        void    SetCheckB(BOOL IsChecked) { m_iCheckB = IsChecked;}

        BOOL    GetCheckC() const {return m_iCheckC;}
        void    SetCheckC(BOOL IsChecked) { m_iCheckC = IsChecked;}

        UINT    GetRadio() const {return m_iRadioA;}
        void    SetRadio(UINT radio) { m_iRadioA = radio;}

        const CString& GetListBoxS() const {return m_sListBox;}
        void    SetListBoxS(const CString &s){ m_sListBox = s;}

        int GetListBoxX() const {return m_iListBox;}
        void    SetListBoxX(int index){m_iListBox = index;}

        const CString& GetComboBoxS() const {return m_sComboBox;}
        void    SetComboBoxS(const CString &s){ m_sComboBox = s;}

        int GetComboBoxX() const {return m_iComboBox;}
        void    SetComboBoxX(int index){m_iComboBox = index;}

        const CString& GetEditBox() const {return m_sString;}
        void    SetEditBox(const CString &s){m_sString = s;}

        const CString& GetRichEditBox() const {return m_sRichEdit;}
        void    SetRichEditBox(const CString &s){ m_sRichEdit = s;}

        BYTE    GetByte() const {return m_iByte;}
        void    SetByte(BYTE val) {m_iByte = val;}

        short   GetShort() const {return m_iShort;}
        void    SetShort(short val) {m_iShort = val;}

        int     GetInt() const {return m_iInt;}
        void    SetInt(int val) {m_iInt = val;}

        UINT    GetUINT() const {return m_iUINT;}
        void    SetUINT(UINT val) {m_iUINT = val;}

        long    GetLong() {return m_iLong;}
        void    SetLong(long val) {m_iLong = val;}

        DWORD   GetULong() const {return m_ULong;}
        void    SetULong(DWORD val) {m_ULong = val;}

        float   GetFloat() const {return m_fFloat;}
        void    SetFloat(float val) {m_fFloat = val;}

        double  GetDouble() const {return m_dDouble;}
        void    SetDouble(double val) {m_dDouble = val;}

        const CString& GetString() const {return m_sString;}
        void     SetString(const CString& val) {m_sString = val;}

        LPCTSTR GetLPTSTR() const {return m_LPTSTR;}
        void SetLPTSTR(LPCTSTR val) {StrCopy(m_LPTSTR, val, 256); }

        int GetSlider() const {return m_iSlider;}
        void    SetSlider(int val) {m_iSlider = val;}

        SYSTEMTIME GetDateTime() const {return m_stDateTime;}
        void    SetDateTime(const SYSTEMTIME &st){m_stDateTime = st;}

        SYSTEMTIME GetMoCalendar() const {return m_stMoCalendar;}
        void    SetMoCalendar(const SYSTEMTIME &st){m_stMoCalendar = st;}

    private:
        BYTE    m_iByte;
        short   m_iShort;
        int     m_iInt;
        int     m_iComboBox;
        int     m_iListBox;
        int     m_iSlider;
        UINT    m_iUINT;
        long    m_iLong;
        DWORD   m_ULong;
        float   m_fFloat;
        double  m_dDouble;
        BOOL    m_iCheckA;
        BOOL    m_iCheckB;
        BOOL    m_iCheckC;
        UINT    m_iRadioA;
        CString m_sListBox;
        CString m_sComboBox;
        CString m_sString;
        CString m_sRichEdit;
        TCHAR   m_LPTSTR[256];
        SYSTEMTIME m_stDateTime;
        SYSTEMTIME m_stMoCalendar;
};

/*----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
