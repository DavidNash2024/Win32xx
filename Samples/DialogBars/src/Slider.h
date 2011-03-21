//////////////////////////////////////////////
// MySlider.h

#ifndef MYSLIDER_H
#define MYSLIDER_H


class CMySlider : public CSlider
{
public:
	CMySlider() {}
	virtual ~CMySlider() {}

	virtual void OnInitialUpdate();
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetSlider(int nPos);
};


#endif
