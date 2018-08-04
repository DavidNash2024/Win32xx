//////////////////////////////////////////////
// CMyScrollBar.h

#ifndef MYSCROLLBAR_H
#define MYSCROLLBAR_H


class CMyScrollBar : public CScrollBar
{
public:
    CMyScrollBar();
    virtual ~CMyScrollBar() {}

    virtual LRESULT OnHScroll(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnInitialUpdate();
    virtual void SetScroll(int pos);

private:
    SCROLLINFO m_si;
};


#endif
