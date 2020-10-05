/////////////////////////////
// Doc.h
//

#ifndef SDI_DOC_H
#define SDI_DOC_H


/////////////////////////////////////////////////////////////////
// CDoc holds the application's data. It stores the application's
// settings in the registry.
class CDoc
{
public:
    CDoc();
    virtual ~CDoc();
    void  LoadSettings(LPCTSTR keyName);
    void  SaveSettings(LPCTSTR keyName);

    BOOL  GetCheckA() { return m_checkA; }
    void  SetCheckA(BOOL isChecked) { m_checkA = isChecked; }

    BOOL  GetCheckB() { return m_checkB; }
    void  SetCheckB(BOOL isChecked) { m_checkB = isChecked; }

    BOOL  GetCheckC() { return m_checkC; }
    void  SetCheckC(BOOL isChecked) { m_checkC = isChecked; }

    UINT  GetRadio() { return m_radio; }
    void  SetRadio(UINT radio) { m_radio = radio; }

private:
    DWORD  m_checkA;
    DWORD  m_checkB;
    DWORD  m_checkC;
    DWORD  m_radio;
};

#endif //SDI_DOC_H
