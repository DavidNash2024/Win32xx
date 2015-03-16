//////////////////////////////////////////////
// ColourDialog.h

#ifndef COLOURDIALOG_H
#define COLOURDIALOG_H


class CColourDialog : public CDialog
{
public:
    CColourDialog(UINT nResID, CBitmap& bmImage);
    virtual ~CColourDialog();
	void CreateImagePreviews();
	
protected:
	virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL    OnInitDialog();
	virtual void    OnOK();
	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnGrayScale();
	LRESULT OnHScroll(WPARAM wParam, LPARAM lParam);
	void    PaintPreview();

	// CWnd object for the dialog controls to attach to
	CWnd m_RedSlider;
	CWnd m_GreenSlider;
	CWnd m_BlueSlider;
	CWnd m_RedEdit;
	CWnd m_GreenEdit;
	CWnd m_BlueEdit;
	CWnd m_Preview;

	CBitmap m_bmImage;
	CBitmap m_bmPreview;
	CBitmap m_bmPreviewOrig;
	int m_cRed;
	int m_cGreen;
	int m_cBlue;
};


#endif // COLOURDIALOG_H

