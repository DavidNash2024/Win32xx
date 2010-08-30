//////////////////////////////////////////////
// MySlider.h

#ifndef MYSLIDER_H
#define MYSLIDER_H


class CMySlider : public CWnd
{
public:
	CMySlider() {}
	virtual ~CMySlider() {}

	virtual void OnInitDialog();
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetSlider(int nPos);
};


#endif
