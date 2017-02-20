///////////////////////////////////////
// CViewDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


// Declaration of the CViewDialog class
class CViewDialog : public CDialog
{
public:
	CViewDialog(UINT nResID);
	virtual ~CViewDialog();

protected:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	void AppendText(int nID, LPCTSTR szText);
	BOOL OnButton();
	BOOL OnCheck1();
	BOOL OnCheck2();
	BOOL OnCheck3();
	BOOL OnRangeOfRadioIDs(UINT nIDFirst, UINT nIDLast, UINT nIDClicked);

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

// Declaration of the CContainDialog class
class CContainDialog : public CDockContainer
{
public:
	CContainDialog();
	~CContainDialog() {}

private:
	CViewDialog m_ViewDialog;
};

// Declaration of the CDockDialog class
class CDockDialog : public CDocker
{
public:
	CDockDialog();
	virtual ~CDockDialog() {}

private:
	CContainDialog m_View;

};

#endif //MYDIALOG_H
