///////////////////////////////////////
// FormView.h

#ifndef SDI_VIEW_H
#define SDI_VIEW_H


class CFormDoc;

// Declaration of the CFormView class
class CFormView : public CDialog
{
public:
	CFormView(UINT nResID);
	virtual ~CFormView();
	virtual HWND Create(HWND hParent);

	CFormDoc& GetDoc();

	BOOL OnButton();
	BOOL OnCheckA();
	BOOL OnCheckB();
	BOOL OnCheckC();
	BOOL OnRangeOfIDs(UINT nIDFirst, UINT nIDLast, UINT nIDClicked);

protected:
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnCancel();
	virtual void OnClose();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

private:


	CResizer m_Resizer;

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	// Inheriting these classes from the base class is optional, but the derived
	// classes can be customized if required.
	class CRadioA : public CButton {};
	class CRadioB : public CButton {};
	class CRadioC : public CButton {};
	class CCheckA : public CButton {};
	class CCheckB : public CButton {};
	class CCheckC : public CButton {};
	class CMyButton : public CButton {};
	class CMyListBox : public CListBox {};
	class CMyEdit : public CEdit {};
	class CMyRichEdit : public CRichEdit {};
	class CMyPicture : public CStatic {};
	class CMyStatus : public CStatic {};
	class COK : public CButton {};
	class CGroup : public CWnd {};
	
	CRadioA		m_RadioA; 
	CRadioB		m_RadioB;
	CRadioC		m_RadioC;
	CCheckA		m_CheckA;
	CCheckB		m_CheckB;
	CCheckC		m_CheckC;
	CMyButton	m_Button;
	CMyListBox	m_ListBox;
	CMyEdit		m_Edit;
	CMyRichEdit	m_RichEdit;
	CMyPicture	m_Picture;
	CMyStatus		m_Status;
	COK			m_OK;
	CGroup		m_Group;

};

#endif //SDI_VIEW_H
