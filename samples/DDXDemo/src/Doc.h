/* (12-Jun-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                    (Doc.h) *
********************************************************************************
|                                                                              |
|                      Author: Robert C. Tausworthe, 2020                      |
|                                                                              |
===============================================================================*

    Contents Description: The DDXDemo CDoc class declaration. This class
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

#ifndef SDI_DOC_H
#define SDI_DOC_H

/*============================================================================*/
    class
CDoc    : public CObject                                                    /*

    Declaration of the CDoc class of the App-Frame-Doc-View architecture.
*-----------------------------------------------------------------------------*/
{
    public:
        CDoc();
        virtual ~CDoc() {}

        void    LoadDocRegistry(LPCTSTR keyName);
        void    SaveDocRegistry(LPCTSTR keyName);
        BOOL    RegQueryBOOLValue(CRegKey& key, LPCTSTR pName);
        DWORD   RegQueryDWORDValue(CRegKey& key, LPCTSTR pName);
        CString RegQueryStringValue(CRegKey& key, LPCTSTR pName);
        SYSTEMTIME RegQuerySYSTEMTIMEValue(CRegKey &key, LPCTSTR pName);
        BOOL    GetCheckA() const {return m_checkAVal;}
        void    SetCheckA(BOOL IsChecked) { m_checkAVal = IsChecked;}

        BOOL    GetCheckB() const {return m_checkBVal;}
        void    SetCheckB(BOOL IsChecked) { m_checkBVal = IsChecked;}

        BOOL    GetCheckC() const {return m_checkCVal;}
        void    SetCheckC(BOOL IsChecked) { m_checkCVal = IsChecked;}

        UINT    GetRadio() const {return m_radioA;}
        void    SetRadio(UINT radio) { m_radioA = radio;}

        const CString& GetListBoxS() const {return m_listBoxVal;}
        void    SetListBoxS(const CString &s){ m_listBoxVal = s;}

        int     GetListBoxX() const {return m_listBoxIndx;}
        void    SetListBoxX(int index){m_listBoxIndx = index;}

        const CString& GetComboBoxS() const {return m_comboBoxVal;}
        void    SetComboBoxS(const CString &s){ m_comboBoxVal = s;}

        int     GetComboBoxX() const {return m_comboBoxIndx;}
        void    SetComboBoxX(int index){m_comboBoxIndx = index;}

        const CString& GetEditBox() const {return m_editVal;}
        void    SetEditBox(const CString &s){m_editVal = s;}

        const CString& GetRichEditBox() const {return m_richEditVal;}
        void    SetRichEditBox(const CString &s){ m_richEditVal = s;}

        BYTE    GetByte() const {return m_byteVal;}
        void    SetByte(BYTE val) {m_byteVal = val;}

        short   GetShort() const {return m_shortVal;}
        void    SetShort(short val) {m_shortVal = val;}

        int     GetInt() const {return m_intVal;}
        void    SetInt(int val) {m_intVal = val;}

        UINT    GetUINT() const {return m_UINTVal;}
        void    SetUINT(UINT val) {m_UINTVal = val;}

        long    GetLong() {return m_longVal;}
        void    SetLong(long val) {m_longVal = val;}

        DWORD   GetULong() const {return m_ULongVal;}
        void    SetULong(DWORD val) {m_ULongVal = val;}

        float   GetFloat() const {return m_floatVal;}
        void    SetFloat(float val) {m_floatVal = val;}

        double  GetDouble() const {return m_doubleVal;}
        void    SetDouble(double val) {m_doubleVal = val;}

        const CString& GetString() const {return m_editVal;}
        void     SetString(const CString& val) {m_editVal = val;}

        LPCTSTR GetLPTSTR() const {return m_LPTSTRVal;}
        void SetLPTSTR(LPCTSTR val) {StrCopy(m_LPTSTRVal, val, 256); }

        int     GetSlider() const {return m_sliderVal;}
        void    SetSlider(int val) {m_sliderVal = val;}

        SYSTEMTIME GetDateTime() const {return m_dateSysTime;}
        void    SetDateTime(const SYSTEMTIME &st){m_dateSysTime = st;}

        SYSTEMTIME GetMoCalendar() const {return m_calDateSysTime;}
        void    SetMoCalendar(const SYSTEMTIME &st){m_calDateSysTime = st;}

    private:
        BYTE        m_byteVal;
        short       m_shortVal;
        int         m_intVal;
        int         m_comboBoxIndx;
        int         m_listBoxIndx;
        int         m_sliderVal;
        UINT        m_UINTVal;
        long        m_longVal;
        DWORD       m_ULongVal;
        float       m_floatVal;
        double      m_doubleVal;
        BOOL        m_checkAVal;
        BOOL        m_checkBVal;
        BOOL        m_checkCVal;
        UINT        m_radioA;
        CString     m_listBoxVal;
        CString     m_comboBoxVal;
        CString     m_editVal;
        CString     m_richEditVal;
        TCHAR       m_LPTSTRVal[256];
        SYSTEMTIME  m_dateSysTime;
        SYSTEMTIME  m_calDateSysTime;
};
/*----------------------------------------------------------------------------*/
#endif //SDI_DOC_H
