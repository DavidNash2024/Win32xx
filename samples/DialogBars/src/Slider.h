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
    virtual LRESULT OnMessageReflect(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void SetSlider(int pos);
};


#endif
