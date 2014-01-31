//////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include <d3d9.h>  // See the note above

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};


class CView : public CWnd, public CThread
{
public:
	CView() : m_pD3D(NULL), m_pd3dDevice(NULL), m_pVB(NULL), m_pParent(NULL) {}
	virtual ~CView();
	virtual HRESULT InitD3D(HWND hWnd);
	virtual HRESULT InitGeometry();
	virtual void Render();
	virtual void SetupDefaultRenderStates();
	virtual void SetupMatrices();

protected:
	// CThread member function overrides
	virtual void StartThread(CWnd* pParent);
	virtual BOOL InitInstance();
	virtual int MessageLoop();

	// CWnd member function overrides
	virtual HWND Create(CWnd* pParent = 0);
	virtual	void OnCreate();
	virtual void OnDestroy();
	virtual void PreCreate(CREATESTRUCT &cs);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LPDIRECT3D9       m_pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9 m_pd3dDevice;		// Our rendering device
	LPDIRECT3DVERTEXBUFFER9 m_pVB;      // Buffer to hold vertices
	D3DPRESENT_PARAMETERS m_d3dpp;
	CWnd* m_pParent;
};


#endif
