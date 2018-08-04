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
    void  LoadDocRegistry(LPCTSTR keyName);
    void  SaveDocRegistry(LPCTSTR keyName);
    DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);

    BOOL  GetCheckA() { return m_checkA; }
    void  SetCheckA(BOOL isChecked) { m_checkA = isChecked; }

    BOOL  GetCheckB() { return m_checkB; }
    void  SetCheckB(BOOL isChecked) { m_checkB = isChecked; }

    BOOL  GetCheckC() { return m_checkC; }
    void  SetCheckC(BOOL isChecked) { m_checkC = isChecked; }

    UINT  GetRadio() { return m_radio; }
    void  SetRadio(UINT radio) { m_radio = radio; }

private:
    BOOL  m_checkA;
    BOOL  m_checkB;
    BOOL  m_checkC;
    UINT  m_radio;
};

#endif //SDI_DOC_H
