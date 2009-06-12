//////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include "../../Win32++/wincore.h"
#include <d3d9.h>  // See the note above


// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class CView : public CWnd
{
public:
	CView() : m_pD3D(NULL), m_pd3dDevice(NULL), m_pVB(NULL) {}
	virtual ~CView();
	virtual HRESULT InitD3D(HWND hWnd);
	virtual HRESULT InitGeometry();
	virtual void Render();
	virtual VOID SetupMatrices();

protected:
	virtual	void OnCreate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPDIRECT3D9       m_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9 m_pd3dDevice;		// Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;      // Buffer to hold vertices
};


#endif
