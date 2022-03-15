/////////////////////////////
// View.h
//

#ifndef VIEW_H
#define VIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include <d3d9.h>           // See the note above


#define UWM_RESIZE     (WM_APP + 0x0001)  // Message - a copy of WM_SIZE info forwarded on to the DirectX window.

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};


///////////////////////////////////////////////////////
// CDXView is the view window of CMainFrame. It is the
// parent window of CDX.
//
// Note: this class has two nested classes:
//  CDX         - the child window that renders the DirectX scene
//  CDXThread   - A separate thread with the DirectX message loop
class CDXView : public CWnd
{
    // CDX nested class within CDXView
    class CDX : public CWnd
    {
    public:
        CDX();
        virtual ~CDX();
        void Render();

    protected:
        // Virtual functions that override base class functions
        virtual int     OnCreate(CREATESTRUCT& cs);
        virtual void    OnDestroy();
        virtual LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
        virtual void    PreCreate(CREATESTRUCT& cs);
        virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        HRESULT InitD3D(HWND hWnd);
        HRESULT InitGeometry();
        void SetupDefaultRenderStates();
        void SetupMatrices();

        // Member variables
        LPDIRECT3D9       m_pD3D;           // Used to create the D3DDevice
        LPDIRECT3DDEVICE9 m_pd3dDevice;     // Our rendering device
        LPDIRECT3DVERTEXBUFFER9 m_pVB;      // Buffer to hold vertices
        D3DPRESENT_PARAMETERS m_d3dpp;
    };

    // CDXThread nested class within CDXView
    class CDXThread : public CWinThread
    {
    public:
        CDXThread() : m_pDX(0) {}
        virtual ~CDXThread() {}

    protected:
        // Virtual functions that override base class functions
        virtual BOOL InitInstance();
        virtual int MessageLoop();

    private:
        CDX* m_pDX;     // DirectX window
    };

public:
    CDXView() {}
    virtual ~CDXView();

    CDX& GetDX() const { return const_cast<CDX&>(m_dx); }

protected:
    // Virtual functions that override base class functions
    virtual int OnCreate(CREATESTRUCT& cs);

    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Message handlers
    LRESULT OnSize(UINT msg, WPARAM wparam, LPARAM lparam);
	
	// Member variables
    CDX m_dx;
    CDXThread m_dxThread;
};


#endif
