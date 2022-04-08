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

    bool  GetCheckA() { return m_isCheckA; }
    void  SetCheckA(bool isChecked) { m_isCheckA = isChecked; }

    bool  GetCheckB() { return m_isCheckB; }
    void  SetCheckB(bool isChecked) { m_isCheckB = isChecked; }

    bool  GetCheckC() { return m_isCheckC; }
    void  SetCheckC(bool isChecked) { m_isCheckC = isChecked; }

    UINT  GetRadio() { return m_radio; }
    void  SetRadio(UINT radio) { m_radio = radio; }

private:
    bool  m_isCheckA;
    bool  m_isCheckB;
    bool  m_isCheckC;
    DWORD m_radio;
};

#endif //SDI_DOC_H
