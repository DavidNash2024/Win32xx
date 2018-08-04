///////////////////////////////////////
// FormDoc.h

// Based on code provided by Lynn Allan


#ifndef SDI_DOC_H
#define SDI_DOC_H


// Declaration of the CFormDoc class
class CFormDoc
{
public:
    CFormDoc();
    virtual ~CFormDoc();
    void  LoadDocRegistry(LPCTSTR keyName);
    void  SaveDocRegistry(LPCTSTR keyName);
    DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);

    BOOL  GetCheckA() { return m_isCheckA; }
    void  SetCheckA(BOOL IsChecked) { m_isCheckA = IsChecked; }

    BOOL  GetCheckB() { return m_isCheckB; }
    void  SetCheckB(BOOL IsChecked) { m_isCheckB = IsChecked; }

    BOOL  GetCheckC() { return m_isCheckC; }
    void  SetCheckC(BOOL IsChecked) { m_isCheckC = IsChecked; }

    UINT  GetRadio() { return m_radio; }
    void  SetRadio(UINT radio) { m_radio = radio; }

private:
    BOOL  m_isCheckA;
    BOOL  m_isCheckB;
    BOOL  m_isCheckC;
    UINT  m_radio;
};

#endif //SDI_DOC_H
