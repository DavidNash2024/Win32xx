///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


// Declaration of the CViewDialog class
class CViewDialog : public CDialog
{
public:
	CViewDialog(UINT nResID);
	virtual ~CViewDialog();

protected:
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:
	void AppendText(int nID, LPCTSTR szText);
	void OnButton();
	void OnCheck1();
	void OnCheck2();
	void OnCheck3();
	void OnRangeOfRadioIDs(UINT nIDFirst, UINT nIDLast, UINT nIDClicked);

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
