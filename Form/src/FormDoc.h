///////////////////////////////////////
// SdiDoc.h

// Based on code provided by Lynn Allan


#ifndef SDI_DOC_H
#define SDI_DOC_H


// Declaration of the CFormDoc class
class CFormDoc //: public CDocument
{
public:
	CFormDoc();
	virtual ~CFormDoc();
	void  LoadDocRegistry(LPCTSTR szKeyName);
	void  SaveDocRegistry(LPCTSTR szKeyName);
	DWORD GetRegDwordFromOpenKey(HKEY hKey, LPCTSTR pName);

	BOOL  GetCheckA(void) { return m_bCheckA; }
	void  SetCheckA(BOOL bCheck) { m_bCheckA = bCheck; }

	BOOL  GetCheckB(void) { return m_bCheckB; }
	void  SetCheckB(BOOL bCheck) { m_bCheckB = bCheck; }

	BOOL  GetCheckC(void) { return m_bCheckC; }
	void  SetCheckC(BOOL bCheck) { m_bCheckC = bCheck; }

	UINT  GetRadio(void) { return m_radio; }
	void  SetRadio(UINT radio) { m_radio = radio; }

private:
	BOOL  m_bCheckA;
	BOOL  m_bCheckB;
	BOOL  m_bCheckC;

	UINT  m_radio;


};

#endif //SDI_DOC_H
