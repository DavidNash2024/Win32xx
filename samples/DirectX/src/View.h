//////////////////////////////////////////////
// View.h

#ifndef VIEW_H
#define VIEW_H

// NOTE: for MS compilers you will need the DirectX SDK v9
//       for Dev-C++ you will need the DirecX v9.0c DevPak

#include <d3d9.h>			// See the note above


#define UWM_RESIZE     (WM_APP + 0x0001)  // Message - a copy of WM_SIZE info forwarded on to the DirectX window.

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};


///////////////////////////////////////////////////////
// Definition of the CDXView class
//  CDXView is the view window of CMainFrame. It is the
//   parent window of CDX.
//
//  Note: this class has two nested classes:
//  CDX			- the child window that renders the DirectX scene
//  CDXThread	- A seperate thread with the DirectX message loop

class CDXView : public CWnd
{
	// CDX nested class within CDXView
	class CDX : public CWnd
	{
	public:
		CDX();
		virtual ~CDX();

		HRESULT InitD3D(HWND hWnd);
		HRESULT InitGeometry();
		void Render();
		void SetupDefaultRenderStates();
		void SetupMatrices();

	protected:
		virtual	int  OnCreate(CREATESTRUCT& cs);
		virtual void OnDestroy();
		virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		LPDIRECT3D9       m_pD3D;			// Used to create the D3DDevice
		LPDIRECT3DDEVICE9 m_pd3dDevice;		// Our rendering device
		LPDIRECT3DVERTEXBUFFER9 m_pVB;      // Buffer to hold vertices
		D3DPRESENT_PARAMETERS m_d3dpp;
	};
	
	// CDXThread nested class within CDXView
	class CDXThread : public CWinThread
	{
	public:
		CDXThread() {}
		virtual ~CDXThread()  {	}

		virtual BOOL InitInstance();
		virtual int MessageLoop();

	private:
		CDX* m_pDX;		// DirectX window
	};

public:
	CDXView() {}
	virtual ~CDXView();

	CDX& GetDX() const { return const_cast<CDX&>(m_DX); }

protected:
	virtual int OnCreate(CREATESTRUCT& cs);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CDX m_DX;
	CDXThread m_DXThread;
};


#endif
