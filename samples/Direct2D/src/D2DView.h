/////////////////////////////
// D2DView.h
//

#ifndef _D2DVIEW_H_
#define _D2DVIEW_H_


#include <d2d1.h>
#include <d2d1helper.h>


template<class Interface>
inline void SafeRelease( Interface **ppInterfaceToRelease )
{
    if (*ppInterfaceToRelease != nullptr)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = nullptr;
    }
}


///////////////////////////////////////////////
// CD2DView manages the window that renders the
// Direct2D images.
class CD2DView : public CWnd
{
public:
    CD2DView();
    virtual ~CD2DView() override;

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual void    OnDestroy() override;
    virtual LRESULT OnDpiChanged(UINT, WPARAM, LPARAM);
    virtual LRESULT OnPaint(UINT, WPARAM, LPARAM) override;
    virtual void    PreCreate(CREATESTRUCT&cs) override;
    virtual void    PreRegisterClass(WNDCLASS& wc) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CD2DView(const CD2DView&) = delete;
    CD2DView& operator=(const CD2DView&) = delete;

    // Message handlers
    LRESULT OnDisplayChange(UINT, WPARAM, LPARAM);
    LRESULT OnSize(UINT, WPARAM wparam, LPARAM lparm);

    HRESULT CreateDeviceResources();
    void    DiscardDeviceResources();
    HRESULT OnRender();
    void    OnResize(UINT width, UINT height);

    // Member variables
    ID2D1HwndRenderTarget* m_pRenderTarget;
    ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
    ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
};

#endif // _D2DVIEW_H_
