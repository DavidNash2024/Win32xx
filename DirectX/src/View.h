//////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include <d3d9.h>  // See the note above
//#include "MainFrm.h"

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
//#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class CView : public CWnd, public CThread
{
public:
	CView() : m_pD3D(NULL), m_pd3dDevice(NULL), m_pVB(NULL), m_hwndParent(NULL) {}
	virtual ~CView();
	virtual HRESULT InitD3D(HWND hWnd);
	virtual HRESULT InitGeometry();
	virtual void Render();
	virtual VOID SetupMatrices();

protected:
	// CThread member function overrides
	virtual void StartThread(HWND hwndParent);
	virtual BOOL InitInstance();
	virtual int MessageLoop();

	// CWnd member function overrides
	virtual HWND Create(HWND hWndParent = 0);
	virtual	void OnCreate();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPDIRECT3D9       m_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9 m_pd3dDevice;		// Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;      // Buffer to hold vertices
	HWND m_hwndParent;
};


#endif
