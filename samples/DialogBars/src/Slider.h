/////////////////////////////
// Slider.h
//

#ifndef MYSLIDER_H
#define MYSLIDER_H

/////////////////////////////////////
// Declaration of the CMySlider class
//
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
