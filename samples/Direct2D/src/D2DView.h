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
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();
        (*ppInterfaceToRelease) = NULL;
    }
}


///////////////////////////////////////////////
// CD2DView manages the window that renders the
// Direct2D images.
class CD2DView : public CWnd
{
public:
    CD2DView();
    virtual ~CD2DView();

protected:
    // Virtual functions that override base class functions
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnDestroy();
    virtual LRESULT OnPaint(UINT, WPARAM, LPARAM);
    virtual void    PreCreate(CREATESTRUCT&cs);
    virtual void    PreRegisterClass(WNDCLASS& wc);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CD2DView(const CD2DView&);                // Disable copy construction
    CD2DView& operator = (const CD2DView&);   // Disable assignment operator

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
