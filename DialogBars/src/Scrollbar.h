#ifndef SCROLLBAR_H
#define SCROLLBAR_H


class CScrollbar : public CWnd
{
public:
	CScrollbar();
	virtual ~CScrollbar() {}
	
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnInitDialog();
	virtual void SetScroll(int nPos);

private:
	SCROLLINFO m_si;
};


#endif
