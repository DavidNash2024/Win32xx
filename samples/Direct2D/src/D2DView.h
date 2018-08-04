
#ifndef _D2DVIEW_H_
#define _D2DVIEW_H_


#include <d2d1.h>
#include <d2d1helper.h>


template<class Interface>
inline void SafeRelease( Interface **ppInterfaceToRelease )
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}


class CD2DView : public CWnd
{
public:
    CD2DView();
    virtual ~CD2DView();
    
    HRESULT CreateDeviceResources();
    void    DiscardDeviceResources();
    LRESULT OnDisplayChange(UINT, WPARAM, LPARAM);
    HRESULT OnRender();
    LRESULT OnSize(UINT, WPARAM wparam, LPARAM lparm);
    void    OnResize(UINT width, UINT height);

protected:
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnDestroy();
    virtual LRESULT OnPaint(UINT, WPARAM, LPARAM);
    virtual void PreCreate(CREATESTRUCT&cs);
    virtual void PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

#endif // _D2DVIEW_H_
