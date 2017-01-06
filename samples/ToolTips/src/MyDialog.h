///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


class CBalloonToolTip : public CToolTip
{
public:
	CBalloonToolTip() {}
	virtual ~CBalloonToolTip() {}

protected:
	virtual void PreCreate(CREATESTRUCT& cs)
	{
		CToolTip::PreCreate(cs);
		cs.style |= TTS_BALLOON;	// Add the balloon style
	}

};


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
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();

private:
	BOOL OnButton();
	BOOL OnRadio1();
	BOOL OnRadio2();
	BOOL OnRadio3();
	BOOL OnCheck1();
	BOOL OnCheck2();
	BOOL OnCheck3();

	CRichEdit m_RichEdit;
	CEdit m_Edit;
	CBalloonToolTip m_BubbleTT;
	CToolTip m_TT;
	CString m_str;
	HICON m_hInfo;		// Icon used by tooltips for the RichEdit and Edit controls
};

#endif //MYDIALOG_H
