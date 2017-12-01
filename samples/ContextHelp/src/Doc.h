///////////////////////////////////////
// Doc.h

// Based on code provided by Lynn Allan


#ifndef SDI_DOC_H
#define SDI_DOC_H


// Declaration of the CDoc class
class CDoc
{
public:
    CDoc();
    virtual ~CDoc();
    void  LoadDocRegistry(LPCTSTR szKeyName);
    void  SaveDocRegistry(LPCTSTR szKeyName);
    DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);

    BOOL  GetCheckA() { return m_CheckA; }
    void  SetCheckA(BOOL IsChecked) { m_CheckA = IsChecked; }

    BOOL  GetCheckB() { return m_CheckB; }
    void  SetCheckB(BOOL IsChecked) { m_CheckB = IsChecked; }

    BOOL  GetCheckC() { return m_CheckC; }
    void  SetCheckC(BOOL IsChecked) { m_CheckC = IsChecked; }

    UINT  GetRadio() { return m_Radio; }
    void  SetRadio(UINT radio) { m_Radio = radio; }

private:
    BOOL  m_CheckA;
    BOOL  m_CheckB;
    BOOL  m_CheckC;
    UINT  m_Radio;
};

#endif //SDI_DOC_H
