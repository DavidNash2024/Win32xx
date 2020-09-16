///////////////////////////////////////
// FormDoc.h

// Based on code provided by Lynn Allan


#ifndef SDI_DOC_H
#define SDI_DOC_H


//////////////////////////////////////////////////////////////
// CDoc holds the application's data. It inherits from CObject
// to perform data serialization to and from the archive.
class CFormDoc
{
public:
    CFormDoc();
    virtual ~CFormDoc();
    void  LoadSettings(LPCTSTR keyName);
    void  SaveSettings(LPCTSTR keyName);

    BOOL  GetCheckA() { return m_isCheckA; }
    void  SetCheckA(BOOL IsChecked) { m_isCheckA = IsChecked; }

    BOOL  GetCheckB() { return m_isCheckB; }
    void  SetCheckB(BOOL IsChecked) { m_isCheckB = IsChecked; }

    BOOL  GetCheckC() { return m_isCheckC; }
    void  SetCheckC(BOOL IsChecked) { m_isCheckC = IsChecked; }

    UINT  GetRadio() { return m_radio; }
    void  SetRadio(UINT radio) { m_radio = radio; }

private:
    DWORD  m_isCheckA;
    DWORD  m_isCheckB;
    DWORD  m_isCheckC;
    DWORD  m_radio;
};

#endif //SDI_DOC_H
