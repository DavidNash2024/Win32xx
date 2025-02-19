///////////////////////////////////////
// StaticImage.h
//

#ifndef _STATIC_IMAGE_H_
#define _STATIC_IMAGE_H_

#include "resource.h"

//////////////////////////////////////////////
// CStaticImage manages the bitmap image
// displayed by CFormView.
class CStaticImage : public CStatic
{
public:
    CStaticImage() = default;
    virtual ~CStaticImage() override = default;

protected:
    virtual void OnAttach() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    void DpiScaleImage();
    LRESULT OnAfterDpiChange(UINT msg, WPARAM wparam, LPARAM lparam);

    CBitmap   m_patternImage;
};

#endif // _STATIC_IMAGE_H_
