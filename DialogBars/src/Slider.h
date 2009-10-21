//////////////////////////////////////////////
// Slider.h

#ifndef SLIDER_H
#define SLIDER_H


class CSlider : public CWnd
{
public:
	CSlider() {}
	virtual ~CSlider() {}

	virtual void OnInitDialog();
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void SetSlider(int nPos);
};


#endif
