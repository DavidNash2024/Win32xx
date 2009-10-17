#ifndef SLIDER_H
#define SLIDER_H

#include "../../Win32++/wincore.h"

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
