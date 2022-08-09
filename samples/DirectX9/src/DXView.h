/////////////////////////////
// DXView.h
//

#ifndef DXVIEW_H
#define DXVIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include <d3d9.h>           // See the note above
#include "DXThread.h"       // Declares the CDXThread class


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};


////////////////////////////////////////////
// CDXView is the view window of CMainFrame.
// It runs in its own thread, and renders the directX image.
//
class CDXView : public CWnd
{
public:
    CDXView();
    virtual ~CDXView();
    void Render();

protected:
    // Virtual functions that override base class functions
    virtual HWND Create(HWND parent = 0);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CDXView(const CDXView&);                // Disable copy construction
    CDXView& operator = (const CDXView&);   // Disable assignment operator

    // Member functions
    HRESULT InitD3D(HWND hWnd);
    HRESULT InitGeometry();
    void SetupDefaultRenderStates();
    void SetupMatrices();

    // Member variables
    LPDIRECT3D9             m_pD3D;           // Used to create the D3DDevice
    LPDIRECT3DDEVICE9       m_pd3dDevice;     // Our rendering device
    LPDIRECT3DVERTEXBUFFER9 m_pVB;            // Buffer to hold vertices
    D3DPRESENT_PARAMETERS   m_d3dpp;
    CDXThread               m_DXThread;
    CEvent                  m_windowCreated;
};


#endif
