///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H



// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
	CMyDialog(UINT nResID);
	virtual ~CMyDialog();

protected:
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnEraseBkgnd(CDC& dc);
	virtual void OnOK();

private:
	BOOL OnButton();
	BOOL OnRadio1();
	BOOL OnRadio2();
	BOOL OnRadio3();
	BOOL OnCheck1();
	BOOL OnCheck2();
	BOOL OnCheck3();

	CResizer m_Resizer;

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done here to keep the IDE's class view tidy.
	class CRadioA : public CButton {};
	class CRadioB : public CButton {};
	class CRadioC : public CButton {};
	class CCheckA : public CButton {};
	class CCheckB : public CButton {};
	class CCheckC : public CButton {};
	class CButton1 : public CButton {};
	class CRichEdit1 : public CRichEdit {};
	class CRichEdit2 : public CRichEdit {};
	
	CRadioA		m_RadioA; 
	CRadioB		m_RadioB;
	CRadioC		m_RadioC;
	CCheckA		m_CheckA;
	CCheckB		m_CheckB;
	CCheckC		m_CheckC;
	CButton1	m_Button;
	CRichEdit1	m_RichEdit1;
	CRichEdit2	m_RichEdit2;
};

#endif //MYDIALOG_H
