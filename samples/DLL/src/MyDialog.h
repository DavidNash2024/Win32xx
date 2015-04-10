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
	virtual void OnOK();

private:
	BOOL OnButton();
	BOOL OnRadio1();
	BOOL OnRadio2();
	BOOL OnRadio3();
	BOOL OnCheck1();
	BOOL OnCheck2();
	BOOL OnCheck3();

	HMODULE m_hInstRichEdit;
};

#endif //MYDIALOG_H
