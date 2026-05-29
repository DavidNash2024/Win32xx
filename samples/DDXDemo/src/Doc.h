/////////////////////////////////////////
// Doc.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_DOC_H
#define SDI_DOC_H

////////////////////////////////////////////////////////
// The CDoc class manages the document data, which is
// stored in the registry.
class CDoc    : public CObject
{
public:
    CDoc();
    virtual ~CDoc() override = default;

    void    LoadDocRegistry(LPCWSTR keyName);
    void    SaveDocRegistry(LPCWSTR keyName);
    BOOL    RegQueryBOOLValue(CRegKey& key, LPCWSTR pName);
    DWORD   RegQueryDWORDValue(CRegKey& key, LPCWSTR pName);
    CString RegQueryStringValue(CRegKey& key, LPCWSTR pName);
    SYSTEMTIME RegQuerySYSTEMTIMEValue(CRegKey &key, LPCWSTR pName);
    BOOL    GetCheckA() const {return m_checkVal[0];}
    void    SetCheckA(BOOL IsChecked) { m_checkVal[0] = IsChecked;}

    BOOL    GetCheckB() const {return m_checkVal[1];}
    void    SetCheckB(BOOL IsChecked) { m_checkVal[1] = IsChecked;}

    BOOL    GetCheckC() const {return m_checkVal[2];}
    void    SetCheckC(BOOL IsChecked) { m_checkVal[2] = IsChecked;}

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

    LPCWSTR GetLPWSTR() const {return m_LPWSTRVal;}
    void SetLPWSTR(LPCWSTR val) {StrCopy(m_LPWSTRVal, val, 256); }

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
    BOOL        m_checkVal[3];
    UINT        m_radioA;
    CString     m_listBoxVal;
    CString     m_comboBoxVal;
    CString     m_editVal;
    CString     m_richEditVal;
    wchar_t     m_LPWSTRVal[256];
    SYSTEMTIME  m_dateSysTime;
    SYSTEMTIME  m_calDateSysTime;
};

#endif //SDI_DOC_H
