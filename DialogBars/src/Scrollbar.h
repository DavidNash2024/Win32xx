//////////////////////////////////////////////
// CMyScrollbar.h

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H


class CMyScrollbar : public CScrollbar
{
public:
	CMyScrollbar();
	virtual ~CMyScrollbar() {}
	
	virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnInitDialog();
	virtual void SetScroll(int nPos);

private:
	SCROLLINFO m_si;
};


#endif
