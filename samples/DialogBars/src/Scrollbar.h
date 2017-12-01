//////////////////////////////////////////////
// CMyScrollBar.h

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H


class CMyScrollBar : public CScrollBar
{
public:
    CMyScrollBar();
    virtual ~CMyScrollBar() {}

    virtual LRESULT OnHScroll(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnMessageReflect(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnInitialUpdate();
    virtual void SetScroll(int nPos);

private:
    SCROLLINFO m_si;
};


#endif
