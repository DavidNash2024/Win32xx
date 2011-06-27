// Win32++   Pre-release Version 7.2
// Released: N/A
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2011  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
// gdi.h
//  Declaration of the CDC class, and CBitmapInfoPtr class

// The CDC class provides a device context, along with the various associated
//  objects such as Bitmaps, Brushes, Bitmaps, Fonts and Pens. This class
//  handles the creation, selection, de-selection and deletion of these objects
//  automatically. It also automatically deletes or releases the device context
//  itself as appropriate. Any failure to create the new GDI object throws an
//  exception.
// 
// The CDC class is sufficient for most GDI programming needs. Sometimes 
//  however we need to have the GDI object seperated from the device context.
//  Wrapper classes for GDI objects are provided for this purpose. The classes 
//  are CBitmap, CBrush, CFont, CPalette, CPen and CRgn. These classes 
//  automatically delete the GDI resouce assigned to them when their destructor 
//  is called. These wrapper class objects can be attached to the CDC as
//  shown below. 
//
// Coding Exampe without CDC ...
//	HDC hdcClient = ::GetDC(m_hWnd);
//  HDC hdcMem = ::CreateCompatibleDC(hdcClient);
//  HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(255,0,0);
//  HPEN hPenOld = (HPEN)::SelectObject(hdcMem, hPen);
//	::MoveToEx(hdcMem, 0, 0, NULL);
//  ::LineTo(hdcMem, 50, 50);
//	::BitBlt(hdcClient, 0, 0, cx, cy, hdcMem, 0, 0);
//  ::SelectObject(hdcMem, hPenOld);
//  ::DeleteObject(hPen);
//  ::DeleteDC(hdcMem);
//	::ReleaseDC(m_hWnd, hdcClient);
//
// Coding Example with CDC ...
//	CClientDC dcClient(this)
//  CMemDC dcMem(&dcClient);
//  CMemDC.CreatePen(PS_SOLID, 1, RGB(255,0,0);
//	CMemDC.MoveTo(0, 0);
//  CMemDC.LineTo(50, 50);
//	dcClient.BitBlt(0, 0, cx, cy, &CMemDC, 0, 0);
//
// Coding Example with CDC and CPen
//	CClientDC dcClient(this)
//  CPen MyPen(PS_SOLID, 1, RGB(255,0,0));
//  CMemDC CMemDC(&dcClient);
//  CMemDC.AttachPen(MyPen);
//	CMemDC.MoveTo(0, 0);
//  CMemDC.LineTo(50, 50);
//	dcClient.BitBlt(0, 0, cx, cy, &CMemDC, 0, 0);
//
// When the CDC object drops out of scope, it's destructor is called, cleaning up
//  any GDI objects it created, as well as the device context.
// When the CPen object drops out of scope, it's destructor is called, deleting
//  the associated GDI object (HPEN).
//
// Notes:
//  * A device context assigned to a CDC object will be released or deleted, unless
//     it is detached.
//  * A GDI object created by one of the CDC member functions will be deleted when
//     the CDC object is destroyed, unless it is detached.
//  * The GDI objects attached to the CDC (by using AttachPen for example) are not
//     deleted when the CDC goes out of scope. Only GDI objects created by the CDC
//     are automatically deleted.
//  * GDI objects belonging to the GDI classes (eg. CPen) are automatically deleted
//     when the class object goes out of scope.
//  * The various AttachXXX functions can attach either a gdi handle (eg HPEN) or
//     a gdi class object (eg CPen).
//  * A GDI object (with the exception of regions) can only be selected into one
//     device context at a time.
//  * Set the region's shape before selecting it into a DC.

// The CBitmapInfoPtr class is a convienient wrapper for the BITMAPINFO structure.
// The size of the BITMAPINFO structure is dependant on the type of HBITMAP, and its
// space needs to be allocated dynamically. CBitmapInfoPtr automatically allocates
// and deallocates the memory for the structure. A CBitmapInfoPtr object can be
// used anywhere in place of a LPBITMAPINFO. LPBITMAPINFO is used in functions like
// GetDIBits and SetDIBits.
//
// Coding example ...
//  CDC MemDC = CreateCompatibleDC(NULL);
//  CBitmapInfoPtr pbmi(hBitmap);
//  MemDC.GetDIBits(hBitmap, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);

#ifndef _WIN32XX_GDI_H_
#define _WIN32XX_GDI_H_

#include "wincore.h"

// Disable macros from Windowsx.h
#undef CopyRgn

namespace Win32xx
{

	/////////////////////////////////////////////////////////////////
	// Declarations for some global functions in the Win32xx namespace
	//
#ifndef _WIN32_WCE
	void GrayScaleBitmap(HBITMAP hbmSource);
	void TintBitmap(HBITMAP hbmSource, int cRed, int cGreen, int cBlue);
	HIMAGELIST CreateDisabledImageList(HIMAGELIST himlNormal);
#endif


	///////////////////////////////////////////////
	// Declarations for the CBitmap class
	//
	class CBitmap
	{
	  public:
		struct DataMembers	// A structure that contains the data members for CBitmap
		{
			HBITMAP hBitmap;
			long	Count;
		};

		CBitmap();
		CBitmap(HBITMAP hBitmap);
		CBitmap(LPCTSTR lpstr);
		CBitmap(int nID);
		CBitmap(const CBitmap& rhs);
		operator HBITMAP() const;
		void operator = (HBITMAP hBitmap);
		CBitmap& operator = (const CBitmap& rhs);
		~CBitmap();

		void Attach( HBITMAP hBitmap );
		HBITMAP Detach();
		HBITMAP GetBitmap() const;

		// Create and load methods
		HBITMAP LoadBitmap(LPCTSTR lpszName);
		HBITMAP LoadBitmap(int nID);
		HBITMAP LoadImage(LPCTSTR lpszName, int cxDesired, int cyDesired, UINT fuLoad);
		HBITMAP LoadImage(UINT nID, int cxDesired, int cyDesired, UINT fuLoad);
		void LoadOEMBitmap(UINT nIDBitmap);
		void CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitsPerPixel, LPCVOID lpBits);
		void CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight);

		// Attributes
		BITMAP GetBitmapData() const;
		void CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, LPVOID* ppvBits, HANDLE hSection, DWORD dwOffset);

#ifndef _WIN32_WCE
		int GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const;
		int SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse);
		HBITMAP CreateMappedBitmap(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0);
		void CreateBitmapIndirect(LPBITMAP lpBitmap);
		CSize GetBitmapDimensionEx() const;
		CSize SetBitmapDimensionEx(int nWidth, int nHeight);

		// DIB support
		HBITMAP CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* lpbmih, DWORD dwInit, LPCVOID lpbInit, CONST BITMAPINFO* lpbmi, UINT uColorUse);
#endif // !_WIN32_WCE

	private:
		void Release();
		DataMembers* m_pData;					// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CBrush class
	//
	class CBrush
	{
	  public:
		struct DataMembers	// A structure that contains the data members for CBrush
		{
			HBRUSH hBrush;
			long	Count;
		};

		CBrush();
		CBrush(HBRUSH hBrush);
		CBrush(COLORREF crColor);
		CBrush(const CBrush& rhs);
		operator HBRUSH() const;
		void operator = (HBRUSH hBrush);
		CBrush& operator = (const CBrush& rhs);
		~CBrush();

		void Attach(HBRUSH hBrush);
		HBRUSH Detach();
		HBRUSH GetBrush() const;

		void CreateSolidBrush(COLORREF crColor);
		void CreatePatternBrush(HBITMAP hBitmap);
		void CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT nUsage);
		LOGBRUSH GetLogBrush() const;

#ifndef _WIN32_WCE
		void CreateHatchBrush(int nIndex, COLORREF crColor);
		void CreateBrushIndirect(LPLOGBRUSH lpLogBrush);
#endif // !defined(_WIN32_WCE)

	  private:
		void Release();
		DataMembers* m_pData;						// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CFont class
	//
	class CFont
	{
	public:
		struct DataMembers	// A structure that contains the data members for CFont
		{
			HFONT hFont;
			long	Count;
		};

		CFont();
		CFont(HFONT hFont);
		CFont(const CFont& rhs);
		operator HFONT() const;
		void operator = (HFONT hFont);
		CFont& operator = (const CFont& rhs);
		~CFont();

		void Attach(HFONT hFont);
		HFONT Detach();
		HFONT GetFont() const;

		// Create methods
		void CreateFontIndirect(const LOGFONT* lpLogFont);
		LOGFONT GetLogFont() const;
		void CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC = NULL, BOOL bBold = FALSE, BOOL bItalic = FALSE);
		void CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hDC = NULL);

#ifndef _WIN32_WCE
		void CreateFont(int nHeight, int nWidth, int nEscapement,
				int nOrientation, int nWeight, DWORD dwItalic, DWORD dwUnderline,
				DWORD dwStrikeOut, DWORD dwCharSet, DWORD dwOutPrecision,
				DWORD dwClipPrecision, DWORD dwQuality, DWORD dwPitchAndFamily,
				LPCTSTR lpszFacename);
#endif // #ifndef _WIN32_WCE

	  private:
		void Release();
		DataMembers* m_pData;				// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CPalette class
	//
	class CPalette
	{
	  public:
		struct DataMembers	// A structure that contains the data members for CPalette
		{
			HPALETTE hPalette;
			long	Count;
		};

		CPalette();
		CPalette(HPALETTE hPalette);
		CPalette(const CPalette& rhs);
		operator HPALETTE() const;
		void operator = (HPALETTE hPalette);
		CPalette& operator = (const CPalette& rhs);
		~CPalette();

		void Attach(HPALETTE hPalette);
		HPALETTE Detach();
		HPALETTE GetPalette() const;

		// Create methods
		void CreatePalette(LPLOGPALETTE lpLogPalette);

#ifndef _WIN32_WCE
		void CreateHalftonePalette(HDC hDC);
#endif // !_WIN32_WCE

		// Attributes
		int GetEntryCount() const;
		UINT GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const;
		UINT SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);

		// Operations
#ifndef _WIN32_WCE
		BOOL ResizePalette(UINT nNumEntries);
		void AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors);
#endif // !_WIN32_WCE

		UINT GetNearestPaletteIndex (COLORREF crColor) const;

	  private:
		void Release();
		DataMembers* m_pData;					// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CPen class
	//
	class CPen
	{
	public:
		struct DataMembers	// A structure that contains the data members for CPen
		{
			HPEN hPen;
			long Count;
		};

		CPen();
		CPen(HPEN hPen);
		CPen(int nPenStyle, int nWidth, COLORREF crColor);
#ifndef _WIN32_WCE
		CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
#endif // !_WIN32_WCE
		CPen(const CPen& rhs);
		operator HPEN() const;
		void operator= (HPEN hPen);
		CPen& operator = (const CPen& rhs);
		~CPen();

		void Attach(HPEN hPen);
		HPEN Detach();
		HPEN GetPen() const;

		void CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
		void CreatePenIndirect(LPLOGPEN lpLogPen);
		LOGPEN GetLogPen() const;

#ifndef _WIN32_WCE
		void ExtCreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL);
		EXTLOGPEN GetExtLogPen() const;
#endif // !_WIN32_WCE

	  private:
		void Release();
		DataMembers* m_pData;				// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CRgn class
	//
	class CRgn
	{
	  public:
		struct DataMembers	// A structure that contains the data members for CRgn
		{
			HRGN hRgn;
			long Count;
		};

		CRgn();
		CRgn(HRGN hRgn);
		CRgn(const CRgn& rhs);
		operator HRGN() const;
		void operator = (HRGN hRgn);
		CRgn& operator = (const CRgn& rhs);
		~CRgn ();

		void Attach(HRGN hRgn);
		HRGN Detach();
		HRGN GetRgn() const;

		// Create methods
		void CreateRectRgn(int x1, int y1, int x2, int y2);
		void CreateRectRgnIndirect(const RECT& rc);

#ifndef _WIN32_WCE
		void CreateEllipticRgn(int x1, int y1, int x2, int y2);
		void CreateEllipticRgnIndirect(const RECT& rc);
		void CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode);
		void CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode);
		void CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3);
		void CreateFromPath(HDC hDC);
#endif // !_WIN32_WCE

		void CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData);

		// Operations
		void SetRectRgn(int x1, int y1, int x2, int y2);
		void SetRectRgn(const RECT& rc);
		int CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode);
		int CombineRgn(HRGN hRgnSrc, int nCombineMode);
		int CopyRgn(HRGN hRgnSrc);
		BOOL EqualRgn(HRGN hRgn) const;
		int OffsetRgn(int x, int y);
		int OffsetRgn(POINT& pt);
		int GetRgnBox(RECT& rc) const;
		BOOL PtInRegion(int x, int y) const;
		BOOL PtInRegion(POINT& pt) const;
		BOOL RectInRegion(const RECT& rc) const;
		int GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const;

	  private:
		void Release();
		DataMembers* m_pData;				// pointer to the class's data members
	};


	///////////////////////////////////////////////
	// Declarations for the CDC class
	//
	class CDC
	{
		friend class CWinApp;
		friend class CWnd;

	public:
		struct DataMembers	// A structure that contains the data members for CDC
		{
			CBitmap Bitmap;
			CBrush	Brush;
			CFont	Font;
			CPalette Palette;
			CPen	Pen;
			CRgn	Rgn;
			HDC		hDC;
			HBITMAP hBitmapOld;
			HBRUSH	hBrushOld;
			HFONT	hFontOld;
			HPALETTE hPaletteOld;
			HPEN	hPenOld;
			long	Count;
			BOOL	bRemoveHDC;
			HWND	hWnd;
		};

		CDC();									// Constructs a new CDC without assigning a HDC
		CDC(HDC hDC, HWND hWnd = 0);			// Assigns a HDC to a new CDC
		CDC(const CDC& rhs);					// Constructs a new copy of the CDC
		virtual ~CDC();
		operator HDC() const { return m_pData->hDC; }	// Converts a CDC to a HDC
		CDC& operator = (const CDC& rhs);		// Assigns a CDC to an existing CDC
		
		void AttachDC(HDC hDC, HWND hWnd = 0);
		void Destroy();
		HDC  DetachDC();
		HDC GetHDC() const { return m_pData->hDC; }
		static CDC* FromHandle(HDC hDC);

#ifndef _WIN32_WCE
		void operator = (const HDC hDC);
#endif

		// Initialization
		BOOL CreateCompatibleDC(CDC* pDC);
		BOOL CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE* pInitData);
		int GetDeviceCaps(int nIndex) const;
#ifndef _WIN32_WCE
		BOOL CreateIC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE* pInitData);
#endif

		// Create and Select Bitmaps
		void AttachBitmap(HBITMAP hBitmap);
		HBITMAP CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, LPCVOID pvColors);
		HBITMAP CreateCompatibleBitmap(CDC* pDC, int cx, int cy);
		HBITMAP CreateDIBSection(CDC* pDC, const BITMAPINFO& bmi, UINT iUsage, LPVOID *ppvBits,
										HANDLE hSection, DWORD dwOffset);
		HBITMAP DetachBitmap();
		HBITMAP GetBitmap() const;
		BITMAP  GetBitmapInfo() const;
		HBITMAP LoadBitmap(UINT nID);
		HBITMAP LoadBitmap(LPCTSTR lpszName);
		HBITMAP LoadImage(UINT nID, int cxDesired, int cyDesired, UINT fuLoad);
		HBITMAP LoadImage(LPCTSTR lpszName, int cxDesired, int cyDesired, UINT fuLoad);
		HBITMAP LoadOEMBitmap(UINT nIDBitmap); // for OBM_/OCR_/OIC

#ifndef _WIN32_WCE
		HBITMAP CreateBitmapIndirect(LPBITMAP pBitmap);
		HBITMAP CreateDIBitmap(CDC* pDC, const BITMAPINFOHEADER& bmih, DWORD fdwInit, LPCVOID lpbInit,
										BITMAPINFO& bmi, UINT fuUsage);
		HBITMAP CreateMappedBitmap(UINT nIDBitmap, UINT nFlags /*= 0*/, LPCOLORMAP lpColorMap /*= NULL*/, int nMapSize /*= 0*/);
#endif

		// Create and Select Brushes
		void AttachBrush(HBRUSH hBrush);
		HBRUSH CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT iUsage);
		HBRUSH CreatePatternBrush(HBITMAP hbmp);
		HBRUSH CreateSolidBrush(COLORREF rbg);
		HBRUSH DetachBrush();
		HBRUSH GetBrush() const;
		LOGBRUSH GetBrushInfo() const;

#ifndef _WIN32_WCE
		HBRUSH CreateBrushIndirect(LPLOGBRUSH& pLogBrush);
		HBRUSH CreateHatchBrush(int fnStyle, COLORREF rgb);
#endif

		// Create and Select Fonts
		void AttachFont(HFONT hFont);
		HFONT CreateFontIndirect(const LOGFONT& lf);
		HFONT DetachFont();
		HFONT GetFont() const;
		LOGFONT GetFontInfo() const;

#ifndef _WIN32_WCE
		HFONT CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int fnWeight,
  							DWORD fdwItalic, DWORD fdwUnderline, DWORD fdwStrikeOut, DWORD fdwCharSet,
  							DWORD fdwOutputPrecision, DWORD fdwClipPrecision, DWORD fdwQuality,
  							DWORD fdwPitchAndFamily, LPCTSTR lpszFace);
#endif

		// Create and Select Palettes
		void AttachPalette(HPALETTE hPalette);
		HPALETTE CreatePalette(LPLOGPALETTE lpLogPalette);	
		HPALETTE DetachPalette();
		HPALETTE GetPalette() const;
#ifndef _WIN32_WCE
		HPALETTE CreateHalftonePalette();
#endif
			
		// Create and Select Pens
		void AttachPen(HPEN hPen);
		HPEN CreatePen(int nStyle, int nWidth, COLORREF rgb);
		HPEN CreatePenIndirect(LPLOGPEN pLogPen);
		HPEN DetachPen();
		HPEN GetPen() const;
		LOGPEN GetPenInfo() const;

		// Create Select Regions
		HRGN CreateRectRgn(int left, int top, int right, int bottom);
		HRGN CreateRectRgnIndirect(const RECT& rc);
		HRGN CreateFromData(const XFORM* Xform, DWORD nCount, const RGNDATA *pRgnData);
		HRGN GetRgn() const;
#ifndef _WIN32_WCE
		HRGN CreateEllipticRgn(int left, int top, int right, int bottom);
		HRGN CreateEllipticRgnIndirect(const RECT& rc);
		HRGN CreatePolygonRgn(LPPOINT ppt, int cPoints, int fnPolyFillMode);
		HRGN CreatePolyPolygonRgn(LPPOINT ppt, LPINT pPolyCounts, int nCount, int fnPolyFillMode);
#endif

		// Wrappers for WinAPI functions

		// Point and Line Drawing Functions
		CPoint GetCurrentPosition() const;
		CPoint MoveTo(int x, int y) const;
		CPoint MoveTo(POINT pt) const;
		BOOL LineTo(int x, int y) const;
		BOOL LineTo(POINT pt) const;
		COLORREF GetPixel(int x, int y) const;
		COLORREF GetPixel(POINT pt) const;
		COLORREF SetPixel(int x, int y, COLORREF crColor) const;
		COLORREF SetPixel(POINT pt, COLORREF crColor) const;
#ifndef _WIN32_WCE
		BOOL Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
		BOOL Arc(RECT& rc, POINT ptStart, POINT ptEnd) const;
		BOOL ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
		BOOL ArcTo(RECT& rc, POINT ptStart, POINT ptEnd) const;
		BOOL AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle) const;
		int  GetArcDirection() const;
		int  SetArcDirection(int nArcDirection) const;
		BOOL PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount) const;
		BOOL Polyline(LPPOINT lpPoints, int nCount) const;
		BOOL PolyPolyline(const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount) const;
		BOOL PolylineTo(const POINT* lpPoints, int nCount) const;
		BOOL PolyBezier(const POINT* lpPoints, int nCount) const;
		BOOL PolyBezierTo(const POINT* lpPoints, int nCount) const;
		BOOL SetPixelV(int x, int y, COLORREF crColor) const;
		BOOL SetPixelV(POINT pt, COLORREF crColor) const;
#endif

		// Shape Drawing Functions
		void DrawFocusRect(const RECT& rc) const;
		BOOL Ellipse(int x1, int y1, int x2, int y2) const;
		BOOL Ellipse(const RECT& rc) const;
		BOOL Polygon(LPPOINT lpPoints, int nCount) const;
		BOOL Rectangle(int x1, int y1, int x2, int y2) const;
		BOOL Rectangle(const RECT& rc) const;
		BOOL RoundRect(int x1, int y1, int x2, int y2, int nWidth, int nHeight) const;
		BOOL RoundRect(const RECT& rc, int nWidth, int nHeight) const;
		
#ifndef _WIN32_WCE
		BOOL Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
		BOOL Chord(const RECT& rc, POINT ptStart, POINT ptEnd) const;
		BOOL Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const;
		BOOL Pie(const RECT& rc, POINT ptStart, POINT ptEnd) const;
		BOOL PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount) const;
#endif

		// Fill and Image Drawing functions
		BOOL FillRect(const RECT& rc, HBRUSH hbr) const;
		BOOL InvertRect(const RECT& rc) const;
		BOOL DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags) const;
		BOOL DrawEdge(const RECT& rc, UINT nEdge, UINT nFlags) const;
		BOOL DrawFrameControl(const RECT& rc, UINT nType, UINT nState) const;
		BOOL FillRgn(HRGN hrgn, HBRUSH hbr) const;
		void GradientFill(COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical);
		void SolidFill(COLORREF Color, const RECT& rc);
		
#ifndef _WIN32_WCE
		BOOL DrawIcon(int x, int y, HICON hIcon) const;
		BOOL DrawIcon(POINT point, HICON hIcon) const;
		BOOL FrameRect(const RECT& rc, HBRUSH hbr) const;
		BOOL PaintRgn(HRGN hrgn) const;
#endif

		// Bitmap Functions
		void DrawBitmap(int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask);
		int  StretchDIBits(int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth,
			           int nSrcHeight, CONST VOID *lpBits, BITMAPINFO& bi, UINT iUsage, DWORD dwRop) const;
		BOOL PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop) const;
		BOOL BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop) const;
		BOOL StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop) const;
		
#ifndef _WIN32_WCE
		int  GetDIBits(HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage) const;
		int  SetDIBits(HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, LPBITMAPINFO lpbi, UINT fuColorUse) const;
		int  GetStretchBltMode() const;
		int  SetStretchBltMode(int iStretchMode) const;
		BOOL FloodFill(int x, int y, COLORREF crColor) const;
		BOOL ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType) const;
#endif

		// Clipping Functions
		int  ExcludeClipRect(int Left, int Top, int Right, int BottomRect);
		int  ExcludeClipRect(const RECT& rc);
		int  GetClipBox(RECT& rc);			
		int  GetClipRgn(HRGN hrgn);				
		int  IntersectClipRect(int Left, int Top, int Right, int Bottom);
		int  IntersectClipRect(const RECT& rc);
		BOOL RectVisible(const RECT& rc);
		int  SelectClipRgn(HRGN hrgn);
 
#ifndef _WIN32_WCE
		int  ExtSelectClipRgn(HRGN hrgn, int fnMode);
		int  OffsetClipRgn(int nXOffset, int nYOffset);
		BOOL PtVisible(int X, int Y);
#endif

        // Co-ordinate Functions
#ifndef _WIN32_WCE
		BOOL DPtoLP(LPPOINT lpPoints, int nCount)  const;
		BOOL DPtoLP(RECT& rc)  const;
		BOOL LPtoDP(LPPOINT lpPoints, int nCount)  const;
		BOOL LPtoDP(RECT& rc)  const;
#endif

		// Layout Functions
		DWORD GetLayout() const;
		DWORD SetLayout(DWORD dwLayout) const;

		// Mapping functions
#ifndef _WIN32_WCE
		int  GetMapMode() const;
		int  SetMapMode(int nMapMode) const;
		BOOL GetViewportOrgEx(LPPOINT lpPoint)  const;
		BOOL SetViewportOrgEx(int x, int y, LPPOINT lpPoint = NULL) const;
		BOOL SetViewportOrgEx(POINT point, LPPOINT lpPointRet = NULL) const;
		BOOL OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint = NULL) const;
		BOOL GetViewportExtEx(LPSIZE lpSize)  const;
		BOOL SetViewportExtEx(int x, int y, LPSIZE lpSize) const;
		BOOL SetViewportExtEx(SIZE size, LPSIZE lpSizeRet) const;
		BOOL ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const;
		BOOL OffsetWindowOrg(int nWidth, int nHeight, LPPOINT lpPoint) const;
		BOOL GetWindowExtEx(LPSIZE lpSize)  const;
		BOOL SetWindowExtEx(int x, int y, LPSIZE lpSize) const;
		BOOL SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const;
		BOOL ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const;
		BOOL GetWindowOrgEx(LPPOINT lpPoint)  const;
		BOOL SetWindowOrgEx(int x, int y, LPPOINT lpPoint) const;
		BOOL SetWindowOrgEx(POINT point, LPPOINT lpPointRet) const;
		BOOL OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint) const;
#endif

		// Printer Functions
		int StartDoc(LPDOCINFO lpDocInfo) const;
		int EndDoc() const;
		int StartPage() const;
		int EndPage() const;
		int AbortDoc() const;
		int SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const;

		// Text Functions
		int  DrawText(LPCTSTR lpszString, int nCount, LPRECT lprc, UINT nFormat) const;
		BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lprc, LPCTSTR lpszString, int nCount = -1, LPINT lpDxWidths = NULL) const;
		COLORREF GetBkColor() const;
		int  GetBkMode() const;
		UINT GetTextAlign() const;
		int  GetTextFace(int nCount, LPTSTR lpszFacename) const;
		COLORREF GetTextColor() const;		
		BOOL  GetTextMetrics(TEXTMETRIC& Metrics) const;
		COLORREF SetBkColor(COLORREF crColor) const;
		int  SetBkMode(int iBkMode) const;	
		UINT  SetTextAlign(UINT nFlags) const;
		COLORREF SetTextColor(COLORREF crColor) const;

#ifndef _WIN32_WCE
		int   DrawTextEx(LPTSTR lpszString, int nCount, LPRECT lprc, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams) const;
		CSize GetTabbedTextExtent(LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const;
		int   GetTextCharacterExtra() const;
		CSize GetTextExtentPoint32(LPCTSTR lpszString, int nCount) const;
		BOOL  GrayString(HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight) const;
		int   SetTextCharacterExtra(int nCharExtra) const;
		int   SetTextJustification(int nBreakExtra, int nBreakCount) const;
		CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) const;
		BOOL  TextOut(int x, int y, LPCTSTR lpszString, int nCount = -1) const;
#endif

	private:
		void AddToMap();
		static CDC* AddTempHDC(HDC hDC, HWND hWnd);
		void Release();
		void RemoveCurrentBitmap();
		void RemoveCurrentBrush();
		void RemoveCurrentFont();
		void RemoveCurrentPalette();
		void RemoveCurrentPen();
		void RemoveCurrentRgn();
		BOOL RemoveFromMap();
		CCriticalSection m_Lock;

		DataMembers* m_pData;		// pointer to the class's data members
	};

	class CClientDC : public CDC
	{
	public:
		CClientDC(CWnd* pWnd)
		{
			if (pWnd) assert(pWnd->IsWindow());
			HWND hWnd = pWnd? pWnd->GetHwnd() : GetDesktopWindow();
			AttachDC(::GetDC(hWnd), hWnd);			
		}
	};

	class CMemDC : public CDC
	{
	public:
		CMemDC(CDC* pDC)
		{
			if (pDC) assert(pDC->GetHDC());
			HDC hDC = pDC? pDC->GetHDC() : NULL;
			AttachDC(::CreateCompatibleDC(hDC));
		}
	};

	class CPaintDC : public CDC
	{
	public:
		CPaintDC(CWnd* pWnd)
		{
			assert(pWnd->IsWindow());
			m_hWnd = pWnd->GetHwnd();
			AttachDC(::BeginPaint(pWnd->GetHwnd(), &m_ps), m_hWnd);
		}

		~CPaintDC()	{ ::EndPaint(m_hWnd, &m_ps); }

	private:
		HWND m_hWnd;
		PAINTSTRUCT m_ps;
	};

	class CWindowDC : public CDC
	{
	public:
		CWindowDC(CWnd* pWnd)
		{
			if (pWnd) assert(pWnd->IsWindow());
			HWND hWnd = pWnd? pWnd->GetHwnd() : GetDesktopWindow();
			AttachDC(::GetWindowDC(hWnd), hWnd);
		}
	};
	
#ifndef _WIN32_WCE
	class CMetaFileDC : public CDC
	{
	public:
		CMetaFileDC() : m_hMF(0), m_hEMF(0) {}
		~CMetaFileDC() 
		{
			if (m_hMF) ::CloseMetaFile(GetHDC());
			if (m_hEMF) ::CloseEnhMetaFile(GetHDC());
		}
		void Create(LPCTSTR lpszFilename = NULL) { AttachDC(::CreateMetaFile(lpszFilename)); }
		void CreateEnhanced(CDC* pDCRef, LPCTSTR lpszFileName, LPCRECT lpBounds, LPCTSTR lpszDescription)
		{
			HDC hDC = pDCRef? pDCRef->GetHDC() : NULL;
			::CreateEnhMetaFile(hDC, lpszFileName, lpBounds, lpszDescription);
			assert(GetHDC());
		}
		HMETAFILE Close() {	return ::CloseMetaFile(GetHDC()); }
		HENHMETAFILE CloseEnhanced() { return ::CloseEnhMetaFile(GetHDC()); }

	private:
		HMETAFILE m_hMF;
		HENHMETAFILE m_hEMF; 
	};
#endif
	

	///////////////////////////////////////////////
	// Declarations for the CBitmapInfoPtr class
	// The CBitmapInfoPtr class is a convienient wrapper for the BITMAPINFO structure.
	class CBitmapInfoPtr
	{
	public:
		CBitmapInfoPtr(HBITMAP hbm)
		{
			BITMAP bmSource;
			::GetObject(hbm, sizeof(BITMAP), &bmSource);

			// Convert the color format to a count of bits.
			WORD cClrBits = (WORD)(bmSource.bmPlanes * bmSource.bmBitsPixel);
			if (cClrBits == 1) 	     cClrBits = 1;
			else if (cClrBits <= 4)  cClrBits = 4;
			else if (cClrBits <= 8)  cClrBits = 8;
			else if (cClrBits <= 16) cClrBits = 16;
			else if (cClrBits <= 24) cClrBits = 24;
			else                     cClrBits = 32;

			// Allocate memory for the BITMAPINFO structure.
			UINT uQuadSize = (cClrBits == 24)? 0 : sizeof(RGBQUAD) * (int)(1 << cClrBits);
			m_bmi.assign(sizeof(BITMAPINFOHEADER) + uQuadSize, 0);
			m_pbmiArray = (LPBITMAPINFO) &m_bmi[0];

			m_pbmiArray->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
			m_pbmiArray->bmiHeader.biHeight		= bmSource.bmHeight;
			m_pbmiArray->bmiHeader.biWidth		= bmSource.bmWidth;
			m_pbmiArray->bmiHeader.biPlanes		= bmSource.bmPlanes;
			m_pbmiArray->bmiHeader.biBitCount	= bmSource.bmBitsPixel;
			m_pbmiArray->bmiHeader.biCompression = BI_RGB;
			if (cClrBits < 24)
				m_pbmiArray->bmiHeader.biClrUsed = (1<<cClrBits);
		}
		LPBITMAPINFO get() const { return m_pbmiArray; }
		operator LPBITMAPINFO() const { return m_pbmiArray; }
		LPBITMAPINFO operator->() const { return m_pbmiArray; }

	private:
		CBitmapInfoPtr(const CBitmapInfoPtr&);				// Disable copy construction
		CBitmapInfoPtr& operator = (const CBitmapInfoPtr&);	// Disable assignment operator
		LPBITMAPINFO m_pbmiArray;
		std::vector<byte> m_bmi;
	};

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
	///////////////////////////////////////////////
	// Declarations for the CBitmap class
	//
	inline CBitmap::CBitmap()
	{
		m_pData = new DataMembers;
		m_pData->hBitmap = 0;
		m_pData->Count = 1L;
	}

	inline CBitmap::CBitmap(HBITMAP hBitmap)
	{
		m_pData = new DataMembers;
		m_pData->hBitmap = hBitmap;
		m_pData->Count = 1L;
	}

	inline CBitmap::CBitmap(LPCTSTR lpszName)
	{
		m_pData = new DataMembers;
		m_pData->hBitmap = 0;
		LoadBitmap(lpszName);
		m_pData->Count = 1L;
	}

	inline CBitmap::CBitmap(int nID)
	{
		m_pData = new DataMembers;
		m_pData->hBitmap = 0;
		LoadBitmap(nID);
		m_pData->Count = 1L;
	}

	inline CBitmap::CBitmap(const CBitmap& rhs)
	// Note: A copy of a CBitmap is a clone of the original.
	//       Both objects manipulate the one HBITMAP.	
	{
		assert(m_pData);
		m_pData->hBitmap = rhs.m_pData->hBitmap;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline CBitmap::operator HBITMAP() const
	{
		assert(m_pData);
		return m_pData->hBitmap;
	}

	inline void CBitmap::operator = (HBITMAP hBitmap)
	{
		assert(m_pData);
		assert (m_pData->hBitmap == NULL);
		m_pData->hBitmap = hBitmap;
	}

	inline CBitmap& CBitmap::operator = ( const CBitmap& rhs )
	// Note: A copy of a CBitmap is a clone of the original.
	//       Both objects manipulate the one HBITMAP.	
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}


	inline CBitmap::~CBitmap()
	{
		Release();
	}

	inline void CBitmap::Attach(HBITMAP hBitmap)
	// Attaches a HBITMAP to the CBitmap object.
	// The HBITMAP will be automatically deleted when the destructor is called unless it is detached.
	{
		assert(m_pData);
		if (m_pData->hBitmap != NULL && m_pData->hBitmap != hBitmap)
			::DeleteObject(m_pData->hBitmap);
		m_pData->hBitmap = hBitmap;
	}

	inline HBITMAP CBitmap::Detach()
	// Detaches a HBITMAP from the CBitmap object.
	{
		assert(m_pData);
		assert(m_pData->hBitmap);
		HBITMAP hBitmap = m_pData->hBitmap;
		m_pData->hBitmap = NULL;
		return hBitmap;
	}

	inline HBITMAP CBitmap::GetBitmap() const
	// Returns the attached HBITMAP.
	{
		assert(m_pData);
		return m_pData->hBitmap;
	}

	inline HBITMAP CBitmap::LoadBitmap(int nID)
	// Loads a bitmap from a resource using the resource ID.
	{
		return LoadBitmap(MAKEINTRESOURCE(nID));
	}

	inline HBITMAP CBitmap::LoadBitmap(LPCTSTR lpszName)
	// Loads a bitmap from a resource using the resource string.
	{
		assert(GetApp());
		assert(m_pData);
		if (m_pData->hBitmap != NULL)
			::DeleteObject(m_pData->hBitmap);

		m_pData->hBitmap = LoadImage(lpszName, 0, 0, LR_DEFAULTCOLOR);

		return m_pData->hBitmap;
	}

	inline HBITMAP CBitmap::LoadImage(UINT nID, int cxDesired, int cyDesired, UINT fuLoad)
	// Loads a bitmap from a resource using the resource ID.
	{
		return LoadImage(MAKEINTRESOURCE(nID), cxDesired, cyDesired, fuLoad);
	}

	inline HBITMAP CBitmap::LoadImage(LPCTSTR lpszName, int cxDesired, int cyDesired, UINT fuLoad)
	// Loads a bitmap from a resource using the resource string.	
	{
		assert(GetApp());
		assert(m_pData);
		if (m_pData->hBitmap != NULL)
			::DeleteObject(m_pData->hBitmap);

		m_pData->hBitmap = (HBITMAP)::LoadImage(GetApp()->GetResourceHandle(), lpszName, IMAGE_BITMAP, cxDesired, cyDesired, fuLoad);	
		assert(m_pData->hBitmap);
		return m_pData->hBitmap;
	}

	inline void CBitmap::LoadOEMBitmap(UINT nIDBitmap) // for OBM_/OCR_/OIC_
	// Loads a predefined bitmap
	// Predefined bitmaps include: OBM_BTNCORNERS, OBM_BTSIZE, OBM_CHECK, OBM_CHECKBOXES, OBM_CLOSE, OBM_COMBO
	//  OBM_DNARROW, OBM_DNARROWD, OBM_DNARROWI, OBM_LFARROW, OBM_LFARROWD, OBM_LFARROWI, OBM_MNARROW,OBM_OLD_CLOSE 
	//  OBM_OLD_DNARROW, OBM_OLD_LFARROW, OBM_OLD_REDUCE, OBM_OLD_RESTORE, OBM_OLD_RGARROW, OBM_OLD_UPARROW 
	//  OBM_OLD_ZOOM, OBM_REDUCE, OBM_REDUCED, OBM_RESTORE, OBM_RESTORED, OBM_RGARROW, OBM_RGARROWD, OBM_RGARROWI 
	//  OBM_SIZE, OBM_UPARROW, OBM_UPARROWD, OBM_UPARROWI, OBM_ZOOM, OBM_ZOOMD 
	{
		assert(m_pData);
		if (m_pData->hBitmap != NULL)
			::DeleteObject(m_pData->hBitmap);

		m_pData->hBitmap = ::LoadBitmap(NULL, MAKEINTRESOURCE(nIDBitmap));
		assert(m_pData->hBitmap);
	}

#ifndef _WIN32_WCE
		inline HBITMAP CBitmap::CreateMappedBitmap(UINT nIDBitmap, UINT nFlags /*= 0*/, LPCOLORMAP lpColorMap /*= NULL*/, int nMapSize /*= 0*/)
		// Creates a new bitmap using the bitmap data and colors specified by the bitmap resource and the color mapping information.
		{			
			assert(GetApp());
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateMappedBitmap(GetApp()->GetResourceHandle(), nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize);
			assert(m_pData->hBitmap);
			return m_pData->hBitmap;
		}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateBitmap(int nWidth, int nHeight, UINT nPlanes, UINT nBitsPerPixel, LPCVOID lpBits)
		// Creates a bitmap with the specified width, height, and color format (color planes and bits-per-pixel).	
		{
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateBitmap(nWidth, nHeight, nPlanes, nBitsPerPixel, lpBits);
			assert(m_pData->hBitmap);
		}

#ifndef _WIN32_WCE
		inline void CBitmap::CreateBitmapIndirect(LPBITMAP lpBitmap)
		// Creates a bitmap with the width, height, and color format specified in the BITMAP structure.	
		{
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateBitmapIndirect(lpBitmap);
			assert(m_pData->hBitmap);
		}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateCompatibleBitmap(HDC hDC, int nWidth, int nHeight)
		// Creates a bitmap compatible with the device that is associated with the specified device context.
		{
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
			assert(m_pData->hBitmap);
		}

		// Attributes
		inline BITMAP CBitmap::GetBitmapData() const
		// Retrieves the BITMAP structure
		{
			assert(m_pData);
			assert(m_pData->hBitmap != NULL);
			BITMAP bmp = {0};
			::GetObject(m_pData->hBitmap, sizeof(BITMAP), &bmp);
			return bmp;
		}

#ifndef _WIN32_WCE
		inline CSize CBitmap::GetBitmapDimensionEx() const
		// Retrieves the dimensions of a compatible bitmap. 
		// The retrieved dimensions must have been set by the SetBitmapDimensionEx function.	
		{
			assert(m_pData);
			assert(m_pData->hBitmap != NULL);
			CSize Size;
			::GetBitmapDimensionEx(m_pData->hBitmap, &Size);
			return Size;
		}

		inline CSize CBitmap::SetBitmapDimensionEx(int nWidth, int nHeight)
		// The SetBitmapDimensionEx function assigns preferred dimensions to a bitmap. 
		// These dimensions can be used by applications; however, they are not used by the system.		
		{
			assert(m_pData);
			assert(m_pData->hBitmap != NULL);
			CSize Size;
			::SetBitmapDimensionEx(m_pData->hBitmap, nWidth, nHeight, Size);
			return Size;
		}

		// DIB support
		inline HBITMAP CBitmap::CreateDIBitmap(HDC hDC, CONST BITMAPINFOHEADER* lpbmih, DWORD dwInit, CONST VOID* lpbInit, CONST BITMAPINFO* lpbmi, UINT uColorUse)
		// Creates a compatible bitmap (DDB) from a DIB and, optionally, sets the bitmap bits.	
		{
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateDIBitmap(hDC, lpbmih, dwInit, lpbInit, lpbmi, uColorUse);
			assert(m_pData->hBitmap);
			return m_pData->hBitmap;
		}
#endif // !_WIN32_WCE

		inline void CBitmap::CreateDIBSection(HDC hDC, CONST BITMAPINFO* lpbmi, UINT uColorUse, VOID** ppvBits, HANDLE hSection, DWORD dwOffset)
		// Creates a DIB that applications can write to directly. The function gives you a pointer to the location of the bitmap bit values. 
		// You can supply a handle to a file-mapping object that the function will use to create the bitmap, or you can let the system allocate the memory for the bitmap.		
		{
			assert(m_pData);
			if (m_pData->hBitmap != NULL)
				::DeleteObject(m_pData->hBitmap);

			m_pData->hBitmap = ::CreateDIBSection(hDC, lpbmi, uColorUse, ppvBits, hSection, dwOffset);
			assert(m_pData->hBitmap);
		}

#ifndef _WIN32_WCE
		inline int CBitmap::GetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines,  LPVOID lpvBits, LPBITMAPINFO lpbmi, UINT uColorUse) const
		// Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format.
		{
			assert(m_pData);
			assert(m_pData->hBitmap != NULL);
			return ::GetDIBits(hDC, m_pData->hBitmap, uStartScan, cScanLines,  lpvBits, lpbmi, uColorUse);
		}

		inline int CBitmap::SetDIBits(HDC hDC, UINT uStartScan, UINT cScanLines, CONST VOID* lpvBits, CONST BITMAPINFO* lpbmi, UINT uColorUse)
		// Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB.	
		{
			assert(m_pData);
			assert(m_pData->hBitmap != NULL);
			return ::SetDIBits(hDC, m_pData->hBitmap, uStartScan, cScanLines, lpvBits, lpbmi, uColorUse);
		}
#endif // !_WIN32_WCE

		inline void CBitmap::Release()
		{
			assert(m_pData);
			BOOL bSucceeded = TRUE;

			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				if (m_pData->hBitmap != NULL)
					bSucceeded = ::DeleteObject(m_pData->hBitmap);

				delete m_pData;
				m_pData = 0;
			}

			assert(bSucceeded);
		}


	///////////////////////////////////////////////
	// Definitions of the CBrush class
	//
	inline CBrush::CBrush()
	{
		m_pData = new DataMembers;
		m_pData->hBrush = 0;
		m_pData->Count = 1L;
	}

	inline CBrush::CBrush(HBRUSH hBrush)
	{
		m_pData = new DataMembers;
		m_pData->hBrush = hBrush;
		m_pData->Count = 1L;
	}

	inline CBrush::CBrush(COLORREF crColor)
	{
		m_pData = new DataMembers;
		m_pData->hBrush = ::CreateSolidBrush(crColor);
		assert (m_pData->hBrush);
		m_pData->Count = 1L;
	}

	inline CBrush::CBrush(const CBrush& rhs)
	// Note: A copy of a CBrush is a clone of the original.
	//       Both objects manipulate the one HBRUSH.
	{
		assert(m_pData);
		m_pData->hBrush = rhs.m_pData->hBrush;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline CBrush::operator HBRUSH() const
	{
		assert(m_pData);
		return m_pData->hBrush;
	}

	inline CBrush& CBrush::operator = ( const CBrush& rhs )
	// Note: A copy of a CBrush is a clone of the original.
	//       Both objects manipulate the one HBRUSH.	
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}


	inline CBrush::~CBrush()
	{
		Release();
	}

	inline void CBrush::Attach(HBRUSH hBrush)
	// Attaches a HBRUSH to the CBrush object.
	// The HBRUSH will be automatically deleted when the destructor is called unless it is detached.	
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL && m_pData->hBrush != hBrush)
			::DeleteObject(m_pData->hBrush);
		m_pData->hBrush = hBrush;
	}

	inline HBRUSH CBrush::Detach()
	// Detaches a HBRUSH from the CBrush object
	{
		assert(m_pData);
		HBRUSH hBrush = m_pData->hBrush;
		m_pData->hBrush = NULL;
		return hBrush;
	}

	inline HBRUSH CBrush::GetBrush() const
	{
		assert(m_pData);
		return m_pData->hBrush;
	}

	inline void CBrush::CreateSolidBrush(COLORREF crColor)
	// Creates a logical brush that has the specified solid color.	
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL)
			::DeleteObject(m_pData->hBrush);

		m_pData->hBrush = ::CreateSolidBrush(crColor);
		assert (m_pData->hBrush);
	}

#ifndef _WIN32_WCE
	inline void CBrush::CreateHatchBrush(int nIndex, COLORREF crColor)
	// Creates a logical brush that has the specified hatch pattern and color.
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL)
			::DeleteObject(m_pData->hBrush);

		m_pData->hBrush = ::CreateHatchBrush(nIndex, crColor);
		assert (m_pData->hBrush);
	}

	inline void CBrush::CreateBrushIndirect(LPLOGBRUSH lpLogBrush)
	// Creates a logical brush from style, color, and pattern specified in the LOGPRUSH struct.
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL)
			::DeleteObject(m_pData->hBrush);

		m_pData->hBrush = ::CreateBrushIndirect(lpLogBrush);
		assert (m_pData->hBrush);
	}
#endif // !defined(_WIN32_WCE)

	inline void CBrush::CreatePatternBrush(HBITMAP hBitmap)
	// Creates a logical brush with the specified bitmap pattern. The bitmap can be a DIB section bitmap, 
	// which is created by the CreateDIBSection function, or it can be a device-dependent bitmap.	
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL)
			::DeleteObject(m_pData->hBrush);

		m_pData->hBrush = ::CreatePatternBrush(hBitmap);
		assert (m_pData->hBrush);
	}

	inline void CBrush::CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT nUsage)
	// Creates a logical brush that has the pattern specified by the device-independent bitmap (DIB).	
	{
		assert(m_pData);
		if (m_pData->hBrush != NULL)
			::DeleteObject(m_pData->hBrush);

		m_pData->hBrush = ::CreateDIBPatternBrushPt(lpPackedDIB, nUsage);
		assert (m_pData->hBrush);
	}

	inline LOGBRUSH CBrush::GetLogBrush() const
	// Retrieves the LOGBRUSH structure that defines the style, color, and pattern of a physical brush.	
	{
		assert(m_pData);
		assert(m_pData->hBrush != NULL);
		LOGBRUSH LogBrush = {0};
		::GetObject (m_pData->hBrush, sizeof(LOGBRUSH), &LogBrush);
		return LogBrush;
	}

	inline void CBrush::Release()
	{
		assert(m_pData);
		BOOL bSucceeded = TRUE;

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hBrush != NULL)
				bSucceeded = ::DeleteObject(m_pData->hBrush);

			delete m_pData;
			m_pData = 0;
		}

		assert(bSucceeded);
	}


	///////////////////////////////////////////////
	// Definitions of the CFont class
	//
	inline CFont::CFont()
	{
		m_pData = new DataMembers;
		m_pData->hFont = 0;
		m_pData->Count = 1L;
	}

	inline CFont::CFont(HFONT hFont)
	{
		m_pData = new DataMembers;
		m_pData->hFont = hFont;
		m_pData->Count = 1L;
	}

	inline CFont::CFont(const CFont& rhs)
	// Note: A copy of a CFont is a clone of the original.
	//       Both objects manipulate the one HFONT.	
	{
		assert(m_pData);
		m_pData->hFont = rhs.m_pData->hFont;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline void CFont::operator = (HFONT hFont)
	{
		assert(m_pData);
		assert (m_pData->hFont == NULL);
		m_pData->hFont = hFont;
	}

	inline CFont::operator HFONT() const
	{
		assert(m_pData);
		return m_pData->hFont;
	}

	inline CFont& CFont::operator = ( const CFont& rhs )
	// Note: A copy of a CFont is a clone of the original.
	//       Both objects manipulate the one HFONT.
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	inline CFont::~CFont()
	{
		Release();
	}

	inline void CFont::Attach(HFONT hFont)
	// Attaches a HFONT to the CFont object.
	// The HFONT will be automatically deleted when the destructor is called unless it is detached.	
	{
		assert(m_pData);
		if (m_pData->hFont != NULL && m_pData->hFont != hFont)
			::DeleteObject(m_pData->hFont);
		m_pData->hFont = hFont;
	}

	inline HFONT CFont::Detach()
	// Detaches a HFONT from the CFont object	
	{
		assert(m_pData);
		HFONT hFont = m_pData->hFont;
		m_pData->hFont = NULL;
		return hFont;
	}

	inline HFONT CFont::GetFont() const
	{
		assert(m_pData);
		return m_pData->hFont;
	}

	inline void CFont::CreateFontIndirect(const LOGFONT* lpLogFont)
	// Creates a logical font that has the specified characteristics.	
	{
		assert(m_pData);
		if (m_pData->hFont != NULL)
			::DeleteObject(m_pData->hFont);
		
		m_pData->hFont = ::CreateFontIndirect(lpLogFont);
		assert(m_pData->hFont);
	}

	inline void CFont::CreatePointFont(int nPointSize, LPCTSTR lpszFaceName, HDC hDC /*= NULL*/, BOOL bBold /*= FALSE*/, BOOL bItalic /*= FALSE*/)
	// Creates a font of a specified typeface and point size.	
	{
		LOGFONT logFont = { 0 };
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfHeight = nPointSize;

		lstrcpy(logFont.lfFaceName, lpszFaceName);

		if (bBold)
			logFont.lfWeight = FW_BOLD;
		if (bItalic)
			logFont.lfItalic = (BYTE)TRUE;

		CreatePointFontIndirect(&logFont, hDC);
	}

	inline void CFont::CreatePointFontIndirect(const LOGFONT* lpLogFont, HDC hDC /* = NULL*/)
	// Creates a font of a specified typeface and point size.
	// This function automatically converts the height in lfHeight to logical units using the specified device context.	
	{
		HDC hDC1 = (hDC != NULL) ? hDC : ::GetDC(HWND_DESKTOP);

		// convert nPointSize to logical units based on hDC
		LOGFONT logFont = *lpLogFont;

#ifndef _WIN32_WCE
		POINT pt = { 0, 0 };
		pt.y = ::MulDiv(::GetDeviceCaps(hDC1, LOGPIXELSY), logFont.lfHeight, 720);   // 72 points/inch, 10 decipoints/point
		::DPtoLP(hDC1, &pt, 1);

		POINT ptOrg = { 0, 0 };
		::DPtoLP(hDC1, &ptOrg, 1);

		logFont.lfHeight = -abs(pt.y - ptOrg.y);
#else // CE specific
		// DP and LP are always the same on CE
		logFont.lfHeight = -abs(((::GetDeviceCaps(hDC1, LOGPIXELSY)* logFont.lfHeight)/ 720));
#endif // _WIN32_WCE

		if (hDC == NULL)
			::ReleaseDC (NULL, hDC1);

		CreateFontIndirect (&logFont);
	}

#ifndef _WIN32_WCE

	inline void CFont::CreateFont(int nHeight, int nWidth, int nEscapement,
			int nOrientation, int nWeight, DWORD dwItalic, DWORD dwUnderline,
			DWORD dwStrikeOut, DWORD dwCharSet, DWORD dwOutPrecision,
			DWORD dwClipPrecision, DWORD dwQuality, DWORD dwPitchAndFamily,
			LPCTSTR lpszFacename)
	// Creates a logical font with the specified characteristics.
	{
		assert(m_pData);
		if (m_pData->hFont != NULL)
			::DeleteObject(m_pData->hFont);

		m_pData->hFont = ::CreateFont(nHeight, nWidth, nEscapement,
			nOrientation, nWeight, dwItalic, dwUnderline, dwStrikeOut,
			dwCharSet, dwOutPrecision, dwClipPrecision, dwQuality,
			dwPitchAndFamily, lpszFacename);
		assert(m_pData->hFont);
	}
#endif // #ifndef _WIN32_WCE

	inline LOGFONT CFont::GetLogFont() const
	// Retrieves the Logfont structure that contains font attributes.	
	{
		assert(m_pData);
		assert(m_pData->hFont != NULL);
		LOGFONT LogFont = {0};
		::GetObject(m_pData->hFont, sizeof(LOGFONT), &LogFont);
		return LogFont;
	}

	inline void CFont::Release()
	{
		assert(m_pData);
		BOOL bSucceeded = TRUE;

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hFont != NULL)
				bSucceeded = ::DeleteObject(m_pData->hFont);

			delete m_pData;
			m_pData = 0;
		}

		assert(bSucceeded);
	}


	///////////////////////////////////////////////
	// Definitions of the CPalette class
	//
	inline CPalette::CPalette()
	{
		m_pData = new DataMembers;
		m_pData->hPalette = 0;
		m_pData->Count = 1L;
	}

	inline CPalette::CPalette(HPALETTE hPalette)
	{
		m_pData = new DataMembers;
		m_pData->hPalette = hPalette;
		m_pData->Count = 1L;
	}

	inline CPalette::CPalette(const CPalette& rhs)
	// Note: A copy of a CPalette is a clone of the original.
	//       Both objects manipulate the one HPalette.	
	{
		assert(m_pData);
		m_pData->hPalette = rhs.m_pData->hPalette;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline CPalette::operator HPALETTE() const
	{
		assert(m_pData);
		return m_pData->hPalette;
	}

	inline void CPalette::operator = (HPALETTE hPalette)
	{
		assert(m_pData);
		assert (m_pData->hPalette == NULL);
		m_pData->hPalette = hPalette;
	}

	inline CPalette& CPalette::operator = (const CPalette& rhs)
	// Note: A copy of a CPalette is a clone of the original.
	//       Both objects manipulate the one HPalette.	
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}


	inline CPalette::~CPalette ()
	{
		Release();
	}

	inline void CPalette::Attach(HPALETTE hPalette)
	// Attaches a HPALETTE to the CPalette object.
	// The HPALETTE will be automatically deleted when the destructor is called unless it is detached.
	{
		assert(m_pData);
		if (m_pData->hPalette != NULL && m_pData->hPalette != hPalette)
			::DeleteObject(m_pData->hPalette);
		m_pData->hPalette = hPalette;
	}

	inline HPALETTE CPalette::Detach()
	// Detaches a HPALETTE from the CPalette object	
	{
		assert(m_pData);
		HPALETTE hPalette = m_pData->hPalette;
		m_pData->hPalette = NULL;
		return hPalette;
	}

	inline HPALETTE CPalette::GetPalette() const
	{
		assert(m_pData);
		return m_pData->hPalette;
	}

	inline void CPalette::CreatePalette(LPLOGPALETTE lpLogPalette)
	// Creates a logical palette from the information in the specified LOGPALETTE structure.	
	{
		assert(m_pData);
		if (m_pData->hPalette != NULL)
			::DeleteObject(m_pData->hPalette);

		m_pData->hPalette = ::CreatePalette (lpLogPalette);
		assert(m_pData->hPalette);
	}

#ifndef _WIN32_WCE
	inline void CPalette::CreateHalftonePalette(HDC hDC)
	// Creates a halftone palette for the specified device context (DC).	
	{
		assert(m_pData);
		assert(hDC != NULL);
		if (m_pData->hPalette != NULL)
			::DeleteObject(m_pData->hPalette);

		m_pData->hPalette = ::CreateHalftonePalette(hDC);
		assert(m_pData->hPalette);
	}
#endif // !_WIN32_WCE

	inline int CPalette::GetEntryCount() const
	// Retrieve the number of entries in the palette.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		WORD nEntries = 0;
		::GetObject(m_pData->hPalette, sizeof(WORD), &nEntries);
		return (int)nEntries;
	}

	inline UINT CPalette::GetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors) const
	// Retrieves a specified range of palette entries from the palette.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		return ::GetPaletteEntries(m_pData->hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	inline UINT CPalette::SetPaletteEntries(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	// Sets RGB (red, green, blue) color values and flags in a range of entries in the palette.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		return ::SetPaletteEntries(m_pData->hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

#ifndef _WIN32_WCE
	inline void CPalette::AnimatePalette(UINT nStartIndex, UINT nNumEntries, LPPALETTEENTRY lpPaletteColors)
	// Replaces entries in the palette.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		::AnimatePalette(m_pData->hPalette, nStartIndex, nNumEntries, lpPaletteColors);
	}

	inline BOOL CPalette::ResizePalette(UINT nNumEntries)
	//  Increases or decreases the size of the palette based on the specified value.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		return ::ResizePalette(m_pData->hPalette, nNumEntries);
	}
#endif // !_WIN32_WCE

	inline UINT CPalette::GetNearestPaletteIndex(COLORREF crColor) const
	// Retrieves the index for the entry in the palette most closely matching a specified color value.	
	{
		assert(m_pData);
		assert(m_pData->hPalette != NULL);
		return ::GetNearestPaletteIndex(m_pData->hPalette, crColor);
	}

	inline void CPalette::Release()
	{
		assert(m_pData);
		BOOL bSucceeded = TRUE;

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hPalette != NULL)
				bSucceeded = ::DeleteObject(m_pData->hPalette);

			delete m_pData;
			m_pData = 0;
		}

		assert(bSucceeded);
	}

	///////////////////////////////////////////////
	// Declarations for the CPen class
	//
	inline CPen::CPen()
	{
		m_pData = new DataMembers;
		m_pData->hPen = 0;
		m_pData->Count = 1L;
	}
	inline CPen::CPen(int nPenStyle, int nWidth, COLORREF crColor)
	{
		m_pData = new DataMembers;
		m_pData->hPen = ::CreatePen(nPenStyle, nWidth, crColor);
		m_pData->Count = 1L;

		assert(m_pData->hPen);
	}

#ifndef _WIN32_WCE
	inline CPen::CPen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount /*= 0*/, const DWORD* lpStyle /*= NULL*/)
	{
		m_pData = new DataMembers;
		m_pData->hPen = ::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle);
		m_pData->Count = 1L;

		assert(m_pData->hPen);
	}
#endif // !_WIN32_WCE

	inline CPen::CPen(const CPen& rhs)
	// Note: A copy of a CPen is a clone of the original.
	//       Both objects manipulate the one HPEN.	
	{
		assert(m_pData);
		m_pData->hPen = rhs.m_pData->hPen;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline void CPen::operator = (HPEN hPen)
	{
		assert(m_pData);
		assert (m_pData->hPen == NULL);
		m_pData->hPen = hPen;
	}

	inline CPen::operator HPEN () const
	{
		assert(m_pData);
		return m_pData->hPen;
	}

	inline CPen& CPen::operator = (const CPen& rhs)
	// Note: A copy of a CPen is a clone of the original.
	//       Both objects manipulate the one HPEN.	
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}


	inline CPen::~CPen()
	{
		Release();
	}

	inline void CPen::Attach(HPEN hPen)
	// Attaches a HPEN to the CPen object.
	// The HPEN will be automatically deleted when the destructor is called unless it is detached.
	{
		assert(m_pData);
		if (m_pData->hPen != NULL && m_pData->hPen != hPen)
			::DeleteObject(m_pData->hPen);
		m_pData->hPen = hPen;
	}

	inline HPEN CPen::Detach()
	// Detaches a HPEN from the CPen object	
	{
		assert(m_pData);
		HPEN hPen = m_pData->hPen;
		m_pData->hPen = NULL;
		return hPen;
	}

	inline HPEN CPen::GetPen() const
	{
		assert(m_pData);
		return m_pData->hPen;
	}

	inline void CPen::CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
	// Creates a logical pen that has the specified style, width, and color.	
	{
		assert(m_pData);
		if (m_pData->hPen != NULL)
			::DeleteObject(m_pData->hPen);

		m_pData->hPen = ::CreatePen(nPenStyle, nWidth, crColor);
		assert(m_pData->hPen);
	}

	inline void CPen::CreatePenIndirect(LPLOGPEN lpLogPen)
	// Creates a logical cosmetic pen that has the style, width, and color specified in a structure.
	{
		assert(m_pData);
		if (m_pData->hPen != NULL)
			::DeleteObject(m_pData->hPen);

		m_pData->hPen = ::CreatePenIndirect(lpLogPen);
		assert(m_pData->hPen);
	}

	inline LOGPEN CPen::GetLogPen() const
	{
		// Retrieves the LOGPEN struct that specifies the pen's style, width, and color.
		assert(m_pData);
		assert(m_pData->hPen != NULL);

		LOGPEN LogPen = {0};
		::GetObject(m_pData->hPen, sizeof(LOGPEN), &LogPen);
		return LogPen;
	}

#ifndef _WIN32_WCE
	inline void CPen::ExtCreatePen(int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount /* = 0*/, const DWORD* lpStyle /*= NULL*/)
	// Creates a logical cosmetic or geometric pen that has the specified style, width, and brush attributes.
	{
		assert(m_pData);
		if (m_pData->hPen != NULL)
			::DeleteObject(m_pData->hPen);

		m_pData->hPen = ::ExtCreatePen(nPenStyle, nWidth, pLogBrush, nStyleCount, lpStyle);
		assert(m_pData->hPen);
	}

	inline EXTLOGPEN CPen::GetExtLogPen() const
	// Retrieves the EXTLOGPEN struct that specifies the pen's style, width, color and brush attributes.	
	{
		assert(m_pData);
		assert(m_pData->hPen != NULL);

		EXTLOGPEN ExLogPen = {0};
		::GetObject(m_pData->hPen, sizeof(EXTLOGPEN), &ExLogPen);
		return ExLogPen;
	}
#endif // !_WIN32_WCE

	inline void CPen::Release()
	{
		assert(m_pData);
		BOOL bSucceeded = TRUE;

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hPen != NULL)
				bSucceeded = ::DeleteObject(m_pData->hPen);

			delete m_pData;
			m_pData = 0;
		}

		assert(bSucceeded);
	}


	///////////////////////////////////////////////
	// Definitions of the CRgn class
	//
	inline CRgn::CRgn()
	{
		m_pData = new DataMembers;
		m_pData->hRgn = 0;
		m_pData->Count = 1L;
	}

	inline CRgn::CRgn(HRGN hRgn)
	{
		m_pData = new DataMembers;
		m_pData->hRgn = hRgn;
		m_pData->Count = 1L;
	}

	inline CRgn::CRgn(const CRgn& rhs)
	// Note: A copy of a CRgn is a clone of the original.
	//       Both objects manipulate the one HRGN.	
	{
		m_pData->hRgn = rhs.m_pData->hRgn;
		m_pData->Count = rhs.m_pData->Count;

		InterlockedIncrement(&m_pData->Count);
	}

	inline CRgn::operator HRGN() const
	{
		assert(m_pData);
		return m_pData->hRgn;
	}

	inline void CRgn::operator = (HRGN hRgn)
	{
		assert(m_pData);
		assert(m_pData->hRgn == NULL);
		m_pData->hRgn = hRgn;
	}

	inline CRgn& CRgn::operator = ( const CRgn& rhs )
	// Note: A copy of a CRgn is a clone of the original.
	//       Both objects manipulate the one HRGN.
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}


	inline CRgn::~CRgn()
	{
		Release();
	}

	inline void CRgn::Attach(HRGN hRgn)
	// Attaches a HRGN to the CRgn object.
	// The HRGN will be automatically deleted when the destructor is called unless it is detached.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL && m_pData->hRgn != hRgn)
			::DeleteObject(m_pData->hRgn);
		m_pData->hRgn = hRgn;
	}

	inline HRGN CRgn::Detach()
	// Detaches a HRGN from the CRgn object
	{
		assert(m_pData);
		HRGN hRgn = m_pData->hRgn;
		m_pData->hRgn = NULL;
		return hRgn;
	}

	inline HRGN CRgn::GetRgn() const
	{
		assert(m_pData);
		return m_pData->hRgn;
	}

	inline void CRgn::CreateRectRgn(int x1, int y1, int x2, int y2)
	// Creates a rectangular region.
	{	
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreateRectRgn(x1, y1, x2, y2);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreateRectRgnIndirect(const RECT& rc)
	// Creates a rectangular region.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreateRectRgnIndirect(&rc);
		assert (m_pData->hRgn);
	}

#ifndef _WIN32_WCE
	inline void CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2)
	// Creates an elliptical region.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreateEllipticRgn(x1, y1, x2, y2);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreateEllipticRgnIndirect(const RECT& rc)
	// Creates an elliptical region.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreateEllipticRgnIndirect(&rc);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreatePolygonRgn(LPPOINT lpPoints, int nCount, int nMode)
	// Creates a polygonal region.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreatePolygonRgn(lpPoints, nCount, nMode);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreatePolyPolygonRgn(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount, int nPolyFillMode)
	// Creates a region consisting of a series of polygons. The polygons can overlap.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, nPolyFillMode);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
	// Creates a rectangular region with rounded corners.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
		assert (m_pData->hRgn);
	}

	inline void CRgn::CreateFromPath(HDC hDC)
	// Creates a region from the path that is selected into the specified device context. 
	// The resulting region uses device coordinates.	
	{
		assert(m_pData);
		assert(hDC != NULL);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::PathToRegion(hDC);
		assert(m_pData->hRgn);
	}

#endif // !_WIN32_WCE

	inline void CRgn::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
	// Creates a region from the specified region and transformation data.
	{
		assert(m_pData);
		if (m_pData->hRgn != NULL)
			::DeleteObject(m_pData->hRgn);

		m_pData->hRgn = ::ExtCreateRegion(lpXForm, nCount, pRgnData);
		assert(m_pData->hRgn);
	}

	inline void CRgn::SetRectRgn(int x1, int y1, int x2, int y2)
	// converts the region into a rectangular region with the specified coordinates.	
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		::SetRectRgn(m_pData->hRgn, x1, y1, x2, y2);
	}

	inline void CRgn::SetRectRgn(const RECT& rc)
	// converts the region into a rectangular region with the specified coordinates.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		::SetRectRgn(m_pData->hRgn, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline int CRgn::CombineRgn(HRGN hRgnSrc1, HRGN hRgnSrc2, int nCombineMode)
	// Combines two sepcified regions and stores the result.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::CombineRgn(m_pData->hRgn, hRgnSrc1, hRgnSrc2, nCombineMode);
	}

	inline int CRgn::CombineRgn(HRGN hRgnSrc, int nCombineMode)
	// Combines the sepcified region with the current region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::CombineRgn(m_pData->hRgn, m_pData->hRgn, hRgnSrc, nCombineMode);
	}

	inline int CRgn::CopyRgn(HRGN hRgnSrc)
	// Assigns the specified region to the current region.	
	{
		assert(m_pData);
		assert(m_pData->hRgn == NULL);
		return ::CombineRgn(m_pData->hRgn, hRgnSrc, NULL, RGN_COPY);
	}

	inline BOOL CRgn::EqualRgn(HRGN hRgn) const
	// Checks the two specified regions to determine whether they are identical.	
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::EqualRgn(m_pData->hRgn, hRgn);
	}

	inline int CRgn::OffsetRgn(int x, int y)
	// Moves a region by the specified offsets.	
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::OffsetRgn(m_pData->hRgn, x, y);
	}

	inline int CRgn::OffsetRgn(POINT& pt)
	// Moves a region by the specified offsets.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::OffsetRgn(m_pData->hRgn, pt.x, pt.y);
	}

	inline int CRgn::GetRgnBox(RECT& rc) const
	// Retrieves the bounding rectangle of the specified region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::GetRgnBox(m_pData->hRgn, &rc);
	}

	inline int CRgn::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
	// Fills the specified buffer with data describing a region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return (int)::GetRegionData(m_pData->hRgn, nDataSize, lpRgnData);
	}

	inline BOOL CRgn::PtInRegion(int x, int y) const
	// Determines whether the specified point is inside the specified region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::PtInRegion(m_pData->hRgn, x, y);
	}

	inline BOOL CRgn::PtInRegion(POINT& pt) const
	// Determines whether the specified point is inside the specified region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::PtInRegion(m_pData->hRgn, pt.x, pt.y);
	}

	inline BOOL CRgn::RectInRegion(const RECT& rc) const
	// Determines whether the specified rect is inside the specified region.
	{
		assert(m_pData);
		assert(m_pData->hRgn != NULL);
		return ::RectInRegion(m_pData->hRgn, &rc);
	}

	inline void CRgn::Release()
	{
		assert(m_pData);
		BOOL bSucceeded = TRUE;

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hRgn != NULL)
				bSucceeded = ::DeleteObject(m_pData->hRgn);

			delete m_pData;
			m_pData = 0;
		}

		assert(bSucceeded);
	}


	///////////////////////////////////////////////
	// Definitions of the CDC class
	//
	inline CDC::CDC()
	{
		// Allocate memory for our data members
		m_pData = new DataMembers;

		// Assign values to our data members
		m_pData->hDC = 0;
		m_pData->hBitmapOld = 0;
		m_pData->hBrushOld = 0;
		m_pData->hFontOld = 0;
		m_pData->hPaletteOld = 0;
		m_pData->hPenOld = 0;
		m_pData->Count = 1L;
		m_pData->bRemoveHDC = TRUE;
		m_pData->hWnd = 0;
	}

	inline CDC::CDC(HDC hDC, HWND hWnd /*= 0*/)
	// This constructor assigns an existing HDC to the CDC
	// The HDC WILL be released or deleted when the CDC object is destroyed
	// The hWnd paramter is only used in WindowsCE. It specifies the HWND of a Window or 
	// Window Client DC

	// Note: this constructor permits a call like this:
	// CDC MyCDC = SomeHDC;
	//  or
	// CDC MyCDC = ::CreateCompatibleDC(SomeHDC);
	//  or
	// CDC MyCDC = ::GetDC(SomeHWND);	
	{
		UNREFERENCED_PARAMETER(hWnd);
		assert(hDC);

		CDC* pDC = GetApp()->GetCDCFromMap(hDC);
		if (pDC)
		{
			m_pData = pDC->m_pData;
			InterlockedIncrement(&m_pData->Count);			
		}
		else
		{
			// Allocate memory for our data members
			m_pData = new DataMembers;

			// Assign values to our data members
			m_pData->hDC = hDC;
			m_pData->hBitmapOld = 0;
			m_pData->hBrushOld = 0;
			m_pData->hFontOld = 0;
			m_pData->hPaletteOld = 0;
			m_pData->hPenOld = 0;
			m_pData->Count = 1L;
			m_pData->bRemoveHDC = TRUE;
#ifndef _WIN32_WCE
			m_pData->hWnd = ::WindowFromDC(hDC);
#else
			m_pData->hWnd = hWnd;
#endif
			if (m_pData->hWnd)
				AddToMap();
		}
	}

#ifndef _WIN32_WCE
	inline void CDC::operator = (const HDC hDC)
	// Note: this assignment operater permits a call like this:
	// CDC MyCDC;
	// MyCDC = SomeHDC;	
	{
		AttachDC(hDC);
	}
#endif

	inline CDC::CDC(const CDC& rhs)	// Copy constructor
	// The copy constructor is called when a temporary copy of the CDC needs to be created.
	// This can happen when a CDC is passed by value in a function call. Each CDC copy manages
	// the same Device Context and GDI objects.	
	{
		m_pData = rhs.m_pData;
		InterlockedIncrement(&m_pData->Count);
	}

	inline CDC& CDC::operator = (const CDC& rhs)
	// Note: A copy of a CDC is a clone of the original.
	//       Both objects manipulate the one HDC	
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	inline CDC::~CDC ()
	{
		Release();
	}

	inline void CDC::AddToMap()
	// Store the HDC and CDC pointer in the HDC map
	{
		assert( GetApp() );
		assert(m_pData->hDC);
		GetApp()->m_csMapLock.Lock();

		assert(m_pData->hDC);
		assert(!GetApp()->GetCDCFromMap(m_pData->hDC));

		GetApp()->m_mapHDC.insert(std::make_pair(m_pData->hDC, this));
		GetApp()->m_csMapLock.Release();
	}

	inline void CDC::AttachDC(HDC hDC, HWND hWnd /* = 0*/)
	// Attaches a HDC to the CDC object.
	// The HDC will be automatically deleted or released when the destructor is called.
	// The hWnd parameter is only used on WindowsCE. It specifies the HWND of a Window or 
	// Window Client DC
	{
		UNREFERENCED_PARAMETER(hWnd);
		assert(m_pData);
		assert(0 == m_pData->hDC);
		assert(hDC);

		CDC* pDC = GetApp()->GetCDCFromMap(hDC);
		if (pDC)
		{
			delete m_pData;
			m_pData = pDC->m_pData;
			InterlockedIncrement(&m_pData->Count);			
		}
		else
		{
			m_pData->hDC = hDC;

#ifndef _WIN32_WCE
			m_pData->hWnd = ::WindowFromDC(hDC);
#else
			m_pData->hWnd = hWnd;
#endif

			if (m_pData->hWnd == 0)
				AddToMap();
		}
	}

	inline HDC CDC::DetachDC()
	// Detaches the HDC from this object. 
	{
		assert(m_pData);
		assert(m_pData->hDC);

		m_Lock.Lock();
		HDC hDC = m_pData->hDC;

		if (m_pData->Count)
		{
			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				if (m_pData->hDC)
				{
					// Delete any GDI objects belonging to this CDC
					if (m_pData->hPenOld)	::SelectObject(m_pData->hDC, m_pData->hPenOld);
					if (m_pData->hBrushOld)	::SelectObject(m_pData->hDC, m_pData->hBrushOld);
					if (m_pData->hBitmapOld)::SelectObject(m_pData->hDC, m_pData->hBitmapOld);
					if (m_pData->hFontOld)	::SelectObject(m_pData->hDC, m_pData->hFontOld);
					if (m_pData->hPaletteOld) ::SelectObject(m_pData->hDC, m_pData->hPaletteOld);
				}

				delete m_pData;
				RemoveFromMap();
			}
		}

		m_Lock.Release();

		// Allocate memory for our data members
		m_pData = new DataMembers;

		// Assign values to our data members
		m_pData->hDC = 0;
		m_pData->hBitmapOld = 0;
		m_pData->hBrushOld = 0;
		m_pData->hFontOld = 0;
		m_pData->hPaletteOld = 0;
		m_pData->hPenOld = 0;
		m_pData->Count = 1L;
		m_pData->bRemoveHDC = TRUE;
		m_pData->hWnd = 0;

		return hDC;
	}

	// Initialization
	inline BOOL CDC::CreateCompatibleDC(CDC* pDC)
	// Returns a memory device context (DC) compatible with the specified device.	
	{
		assert(m_pData->hDC == NULL);
		HDC hdcSource = (pDC == NULL)? NULL : pDC->GetHDC();	
		HDC hDC = ::CreateCompatibleDC(hdcSource);
		if (hDC)
		{
			m_pData->hDC = hDC;
			AddToMap();
		}
		return (BOOL)hDC;
	}

	inline BOOL CDC::CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE* pInitData)
	// Returns a device context (DC) for a device using the specified name.	
	{
		assert(m_pData->hDC == NULL);
		HDC hDC = ::CreateDC(lpszDriver, lpszDevice, lpszOutput, pInitData);
		if (hDC)
		{
			m_pData->hDC = hDC;
			AddToMap();
		}
		return (BOOL)hDC;
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::CreateIC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, const DEVMODE* pInitData)
	{
		assert(m_pData->hDC == NULL);
		HDC hDC = ::CreateIC(lpszDriver, lpszDevice, lpszOutput, pInitData);
		if (hDC)
		{
			m_pData->hDC = hDC;
			AddToMap();
		}
		return (BOOL)hDC;
	}
#endif
	
	inline void CDC::DrawBitmap(int x, int y, int cx, int cy, HBITMAP hbmImage, COLORREF clrMask)
	// Draws the specified bitmap to the specified DC using the mask colour provided as the transparent colour
	// Suitable for use with a Window DC or a memory DC
	{
		// Create the Image memory DC
		CMemDC dcImage(this);
		dcImage.SetBkColor(clrMask);
		dcImage.AttachBitmap(hbmImage);

		// Create the Mask memory DC
		CMemDC dcMask(this);
        dcMask.CreateBitmap(cx, cy, 1, 1, NULL);
		dcMask.BitBlt(0, 0, cx, cy, &dcImage, 0, 0, SRCCOPY);

		// Mask the image to 'this' DC
		BitBlt(x, y, cx, cy, &dcImage, 0, 0, SRCINVERT);
		BitBlt(x, y, cx, cy, &dcMask, 0, 0, SRCAND);
		BitBlt(x, y, cx, cy, &dcImage, 0, 0, SRCINVERT);
	}

	inline CDC* CDC::FromHandle(HDC hDC)
	// Returns the CDC object associated with the device context handle
	// If a CDC object doesn't already exist, a temporary CDC object is created.
	// The HDC belonging to a temporary CDC is not released or destroyed when the 
	//  temporary CDC is deconstructed.
	{
		assert( GetApp() );
		CDC* pDC = GetApp()->GetCDCFromMap(hDC);
		if (pDC == 0)
		{
			pDC = GetApp()->AddTmpDC(hDC);
			pDC->m_pData->bRemoveHDC = FALSE;
		}
		return pDC;
	}

	inline CDC* CDC::AddTempHDC(HDC hDC, HWND hWnd)
	// Returns the CDC object associated with the device context handle
	// The HDC is removed when the CDC is destroyed 
	{
		assert( GetApp() );
		CDC* pDC = GetApp()->AddTmpDC(hDC);
		pDC->m_pData->bRemoveHDC = TRUE;
		pDC->m_pData->hWnd = hWnd;
		return pDC;
	}

	inline void CDC::GradientFill(COLORREF Color1, COLORREF Color2, const RECT& rc, BOOL bVertical)
	// An efficient color gradient filler compatible with all Windows operating systems
	{
		int Width = rc.right - rc.left;
		int Height = rc.bottom - rc.top;

		int r1 = GetRValue(Color1);
		int g1 = GetGValue(Color1);
		int b1 = GetBValue(Color1);

		int r2 = GetRValue(Color2);
		int g2 = GetGValue(Color2);
		int b2 = GetBValue(Color2);

		COLORREF OldBkColor = GetBkColor();

		if (bVertical)
		{
			for(int i=0; i < Width; ++i)
			{
				int r = r1 + (i * (r2-r1) / Width);
				int g = g1 + (i * (g2-g1) / Width);
				int b = b1 + (i * (b2-b1) / Width);
				SetBkColor(RGB(r, g, b));
				CRect line( i + rc.left, rc.top, i + 1 + rc.left, rc.top+Height);
				ExtTextOut(0, 0, ETO_OPAQUE, line, NULL, 0, 0);
			}
		}
		else
		{
			for(int i=0; i < Height; ++i)
			{
				int r = r1 + (i * (r2-r1) / Height);
				int g = g1 + (i * (g2-g1) / Height);
				int b = b1 + (i * (b2-b1) / Height);
				SetBkColor(RGB(r, g, b));
				CRect line(rc.left, i + rc.top, rc.left+Width, i + 1 + rc.top);
				ExtTextOut(0, 0, ETO_OPAQUE, line, NULL, 0, 0);
			}
		}

		SetBkColor(OldBkColor);
	}

	inline void CDC::Release()
	{
		m_Lock.Lock();

		if (m_pData->Count)
		{
			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				Destroy();
				delete m_pData;
				m_pData = 0;
			}
		}
		
		m_Lock.Release();
	}

	inline BOOL CDC::RemoveFromMap()
	{
		assert( GetApp() );
		BOOL Success = FALSE;

		// Allocate an iterator for our HDC map
		std::map<HDC, CDC*, CompareHDC>::iterator m;

		CWinApp* pApp = GetApp();
		if (pApp)
		{
			// Erase the CDC pointer entry from the map
			pApp->m_csMapLock.Lock();
			for (m = pApp->m_mapHDC.begin(); m != pApp->m_mapHDC.end(); ++m)
			{
				if (this == m->second)
				{
					pApp->m_mapHDC.erase(m);
					Success = TRUE;
					break;
				}
			}

			pApp->m_csMapLock.Release();
		}

		return Success;
	}

	inline void CDC::RemoveCurrentBitmap()
	{
		if (m_pData->hBitmapOld)
		{
			HBITMAP hBitmap = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->hBitmapOld);
			if (hBitmap == m_pData->Bitmap)
			{
				// Only delete the bitmap objects we create
				m_pData->Bitmap.Detach();
				DeleteObject(hBitmap);
			}
		}
	}

	inline void CDC::RemoveCurrentBrush()
	{
		if (m_pData->hBrushOld)
		{
			HBRUSH hBrush = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->hBrushOld);
			if (hBrush == m_pData->Brush)
			{
				// Only delete the brush objects we create
				m_pData->Brush.Detach();
				DeleteObject(hBrush);
			}
		}
	}

	inline void CDC::RemoveCurrentFont()
	{
		if (m_pData->hFontOld)
		{
			HFONT hFont = (HFONT)::SelectObject(m_pData->hDC, m_pData->hFontOld);
			if (hFont == m_pData->Font)
			{
				// Only delete the font objects we create
				m_pData->Font.Detach();
				DeleteObject(hFont);
			}
		}
	}

	inline void CDC::RemoveCurrentPalette()
	{
		if (m_pData->hPaletteOld)
		{
			HPALETTE hPalette = (HPALETTE)::SelectObject(m_pData->hDC, m_pData->hPaletteOld);
			if (hPalette == m_pData->Palette)
			{
				// Only delete the pen objects we create
				m_pData->Palette.Detach();
				DeleteObject(hPalette);
			}
		}
	}

	inline void CDC::RemoveCurrentPen()
	{
		if (m_pData->hPenOld)
		{
			HPEN hPen = (HPEN)::SelectObject(m_pData->hDC, m_pData->hPenOld);
			if (hPen == m_pData->Pen)
			{
				// Only delete the pen objects we create
				m_pData->Pen.Detach();
				DeleteObject(hPen);
			}
		}
	}

	inline void CDC::RemoveCurrentRgn()
	{
		if (m_pData->Rgn)
		{
			// Only delete the region objects we create
			HRGN hRgn = m_pData->Rgn.Detach();
			DeleteObject(hRgn);
		}
	}

	inline void CDC::SolidFill(COLORREF Color, const RECT& rc)
	// Fills a rectangle with a solid color
	{
		COLORREF OldColor = SetBkColor(Color);
		ExtTextOut(0, 0, ETO_OPAQUE, &rc, NULL, 0, 0);
		SetBkColor(OldColor);
	}

	// Bitmap functions
	inline void CDC::AttachBitmap(HBITMAP hBitmap)
	// Use this to attach an existing bitmap.
	{
		assert(m_pData->hDC);
		assert(hBitmap);

		RemoveCurrentBitmap();
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, hBitmap);
	}

	inline HBITMAP CDC::CreateCompatibleBitmap(CDC* pDC, int cx, int cy)
	// Creates a compatible bitmap and selects it into the device context.
	{
		assert(m_pData->hDC);
		assert(pDC);
		RemoveCurrentBitmap();

		m_pData->Bitmap.CreateCompatibleBitmap(pDC->GetHDC(), cx, cy);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

	inline HBITMAP CDC::CreateBitmap(int cx, int cy, UINT Planes, UINT BitsPerPixel, LPCVOID pvColors)
	// Creates a bitmap and selects it into the device context.
	{
		assert(m_pData->hDC);
		RemoveCurrentBitmap();

		m_pData->Bitmap.CreateBitmap(cx, cy, Planes, BitsPerPixel, pvColors);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

#ifndef _WIN32_WCE
	inline HBITMAP CDC::CreateBitmapIndirect (LPBITMAP pBitmap)
	// Creates a bitmap and selects it into the device context.
	{

		assert(m_pData->hDC);
		RemoveCurrentBitmap();

		m_pData->Bitmap.CreateBitmapIndirect(pBitmap);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

	inline HBITMAP CDC::CreateDIBitmap(CDC* pDC, const BITMAPINFOHEADER& bmih, DWORD fdwInit, LPCVOID lpbInit,
										BITMAPINFO& bmi,  UINT fuUsage)
	// Creates a bitmap and selects it into the device context.
	{
		assert(m_pData->hDC);
		assert(pDC);
		RemoveCurrentBitmap();

		m_pData->Bitmap.CreateDIBitmap(pDC->GetHDC(), &bmih, fdwInit, lpbInit, &bmi, fuUsage);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}
#endif

	inline HBITMAP CDC::CreateDIBSection(CDC* pDC, const BITMAPINFO& bmi, UINT iUsage, LPVOID *ppvBits,
										HANDLE hSection, DWORD dwOffset)
	// Creates a bitmap and selects it into the device context.
	{
		assert(m_pData->hDC);
		assert(pDC);
		RemoveCurrentBitmap();

		m_pData->Bitmap.CreateDIBSection(pDC->GetHDC(), &bmi, iUsage, ppvBits, hSection, dwOffset);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

	inline void CDC::Destroy()
	// Deletes or releases the device context and returns the CDC object to its
	// default state, ready for reuse.
	{
		if (m_pData->hDC)
		{
			// Delete any GDI objects belonging to this CDC
			if (m_pData->hPenOld)	::SelectObject(m_pData->hDC, m_pData->hPenOld);
			if (m_pData->hBrushOld)	::SelectObject(m_pData->hDC, m_pData->hBrushOld);
			if (m_pData->hBitmapOld)::SelectObject(m_pData->hDC, m_pData->hBitmapOld);
			if (m_pData->hFontOld)	::SelectObject(m_pData->hDC, m_pData->hFontOld);
			if (m_pData->hPaletteOld) ::SelectObject(m_pData->hDC, m_pData->hPaletteOld);

			if (m_pData->bRemoveHDC)
			{
				// We need to release a Window DC, and delete a memory DC
				if (m_pData->hWnd)
					::ReleaseDC(m_pData->hWnd, m_pData->hDC);
				else
					if (!::DeleteDC(m_pData->hDC))
						::ReleaseDC(NULL, m_pData->hDC);

				m_pData->hDC = 0; 
				m_pData->hWnd = 0;
				m_pData->bRemoveHDC = TRUE;
				m_pData->hPenOld = 0;
				m_pData->hBrushOld = 0;
				m_pData->hBitmapOld = 0;
				m_pData->hFontOld = 0;
				m_pData->hPaletteOld = 0;
			}
		}

		RemoveFromMap();
	}

	inline HBITMAP CDC::DetachBitmap()
	// Use this to detach the bitmap from the HDC.
	{
		assert(m_pData->hDC);

		HBITMAP hBitmap = 0;	
		if(m_pData->hBitmapOld)
		{
			hBitmap = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->hBitmapOld);
			if (hBitmap == m_pData->Bitmap)
				m_pData->Bitmap.Detach();
		}

		m_pData->hBitmapOld = NULL;
		return hBitmap;
	}

	inline HBITMAP CDC::GetBitmap() const
	// Retrieves the current bitmap
	{
		return m_pData->Bitmap;
	}

	inline BITMAP CDC::GetBitmapInfo() const
	// Retrieves the BITMAP information for the current HBITMAP.
	{
		assert(m_pData->hDC);

		HBITMAP hbm = (HBITMAP)::GetCurrentObject(m_pData->hDC, OBJ_BITMAP);
		BITMAP bm = {0};
		::GetObject(hbm, sizeof(bm), &bm);
		return bm;
	}

	inline HBITMAP CDC::LoadBitmap(UINT nID)
	// Loads a bitmap from the resource and selects it into the device context
	{
		return LoadBitmap(MAKEINTRESOURCE(nID));
	}

	inline HBITMAP CDC::LoadBitmap(LPCTSTR lpszName)
	// Loads a bitmap from the resource and selects it into the device context
	{
		assert(m_pData->hDC);
		assert(GetApp());
		m_pData->Bitmap.LoadBitmap(lpszName);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

	inline HBITMAP CDC::LoadImage(UINT nID, int cxDesired, int cyDesired, UINT fuLoad)
	// Loads a bitmap from the resource and selects it into the device context
	{
		return LoadImage(nID, cxDesired, cyDesired, fuLoad);
	}

	inline HBITMAP CDC::LoadImage(LPCTSTR lpszName, int cxDesired, int cyDesired, UINT fuLoad)
	// Loads a bitmap from the resource and selects it into the device context
	{
		assert(m_pData->hDC);
		assert(GetApp());
		m_pData->Bitmap.LoadImage(lpszName, cxDesired, cyDesired, fuLoad);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

	inline HBITMAP CDC::LoadOEMBitmap(UINT nIDBitmap) // for OBM_/OCR_/OIC_
	// Loads a predefined system bitmap and selects it into the device context
	{
		assert(m_pData->hDC);
		m_pData->Bitmap.LoadOEMBitmap(nIDBitmap);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}

#ifndef _WIN32_WCE
	inline HBITMAP CDC::CreateMappedBitmap(UINT nIDBitmap, UINT nFlags /*= 0*/, LPCOLORMAP lpColorMap /*= NULL*/, int nMapSize /*= 0*/)
	// creates and selects a new bitmap using the bitmap data and colors specified by the bitmap resource and the color mapping information.
	{
		assert(m_pData->hDC);
		m_pData->Bitmap.CreateMappedBitmap(nIDBitmap, (WORD)nFlags, lpColorMap, nMapSize);
		m_pData->hBitmapOld = (HBITMAP)::SelectObject(m_pData->hDC, m_pData->Bitmap);
		return m_pData->Bitmap;
	}
#endif // !_WIN32_WCE


	// Brush functions
	inline void CDC::AttachBrush(HBRUSH hBrush)
	// Use this to attach an existing HBRUSH or CBrush.
	{
		assert(m_pData->hDC);
		assert(hBrush);
		RemoveCurrentBrush();
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, hBrush);
	}

#ifndef _WIN32_WCE
	inline HBRUSH CDC::CreateBrushIndirect(LPLOGBRUSH& pLogBrush)
	// Creates the brush and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentBrush();

		m_pData->Brush.CreateBrushIndirect(pLogBrush);
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->Brush);
		return m_pData->Brush;
	}

	inline HBRUSH CDC::CreateHatchBrush(int fnStyle, COLORREF rgb)
	// Creates the brush and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentBrush();

		m_pData->Brush.CreateHatchBrush(fnStyle, rgb);
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->Brush);
		return m_pData->Brush;
	}
#endif

	inline HBRUSH CDC::CreateDIBPatternBrushPt(LPCVOID lpPackedDIB, UINT iUsage)
	// Creates the brush and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentBrush();

		m_pData->Brush.CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->Brush);
		return m_pData->Brush;
	}

	inline HBRUSH CDC::CreatePatternBrush(HBITMAP hbmp)
	// Creates the brush and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentBrush();

		m_pData->Brush.CreatePatternBrush(hbmp);
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->Brush);
		return m_pData->Brush;
	}

	inline HBRUSH CDC::CreateSolidBrush(COLORREF rgb)
	// Creates the brush and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentBrush();

		m_pData->Brush.CreateSolidBrush(rgb);
		m_pData->hBrushOld = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->Brush);
		return m_pData->Brush;
	}

	inline HBRUSH CDC::DetachBrush()
	// Use this to detach the brush from the HDC.
	{
		assert(m_pData->hDC);

		HBRUSH hBrush = 0;
		if (m_pData->hBrushOld)
		{
			hBrush = (HBRUSH)::SelectObject(m_pData->hDC, m_pData->hBrushOld);
			if (hBrush == m_pData->Brush)
				m_pData->Brush.Detach();
		}
		
		m_pData->hBrushOld = NULL;
		return hBrush;
	}

	inline HBRUSH CDC::GetBrush() const
	// Retrieves the current brush information
	{
		return m_pData->Brush;
	}

	inline LOGBRUSH CDC::GetBrushInfo() const
	// Retrieves the current brush information
	{
		assert(m_pData->hDC);

		HBRUSH hBrush = (HBRUSH)::GetCurrentObject(m_pData->hDC, OBJ_BRUSH);
		LOGBRUSH lBrush = {0};
		::GetObject(hBrush, sizeof(lBrush), &lBrush);
		return lBrush;
	}


	// Font functions
	inline void CDC::AttachFont(HFONT hFont)
	// Use this to attach an existing HFONT or CFont.
	{
		assert(m_pData->hDC);
		assert(hFont);

		RemoveCurrentFont();
		m_pData->hFontOld = (HFONT)::SelectObject(m_pData->hDC, hFont);
	}

#ifndef _WIN32_WCE
	inline HFONT CDC::CreateFont (
					int nHeight,               // height of font
  					int nWidth,                // average character width
  					int nEscapement,           // angle of escapement
  					int nOrientation,          // base-line orientation angle
  					int fnWeight,              // font weight
  					DWORD fdwItalic,           // italic attribute option
  					DWORD fdwUnderline,        // underline attribute option
  					DWORD fdwStrikeOut,        // strikeout attribute option
  					DWORD fdwCharSet,          // character set identifier
  					DWORD fdwOutputPrecision,  // output precision
  					DWORD fdwClipPrecision,    // clipping precision
  					DWORD fdwQuality,          // output quality
  					DWORD fdwPitchAndFamily,   // pitch and family
  					LPCTSTR lpszFace           // typeface name
 					)

	// Creates a logical font with the specified characteristics.
	{
		assert(m_pData->hDC);
		RemoveCurrentFont();

		m_pData->Font.CreateFont (nHeight, nWidth, nEscapement, nOrientation, fnWeight,
								fdwItalic, fdwUnderline, fdwStrikeOut, fdwCharSet,
								fdwOutputPrecision, fdwClipPrecision, fdwQuality,
								fdwPitchAndFamily, lpszFace);

		m_pData->hFontOld = (HFONT)::SelectObject(m_pData->hDC, m_pData->Font);
		return m_pData->Font;
	}
#endif

	inline HFONT CDC::CreateFontIndirect(const LOGFONT& lf)
	// Creates a logical font and selects it into the device context.
	{
		assert(m_pData->hDC);
		RemoveCurrentFont();

		m_pData->Font.CreateFontIndirect(&lf);
		m_pData->hFontOld = (HFONT)::SelectObject(m_pData->hDC, m_pData->Font);
		return m_pData->Font;
	}

	inline HFONT CDC::DetachFont()
	// Use this to detach the font from the HDC.
	{
		assert(m_pData->hDC);

		HFONT hFont = 0;
		if (m_pData->hFontOld)
		{
			hFont = (HFONT)::SelectObject(m_pData->hDC, m_pData->hFontOld);
			if (m_pData->Font == hFont)
				m_pData->Font.Detach();
		}

		m_pData->hFontOld = NULL;
		return hFont;
	}

	inline HFONT CDC::GetFont() const
	// Retrieves the current font
	{
		return m_pData->Font;
	}

	inline LOGFONT CDC::GetFontInfo() const
	// Retrieves the current font information.
	{
		assert(m_pData->hDC);

		HFONT hFont = (HFONT)::GetCurrentObject(m_pData->hDC, OBJ_FONT);
		LOGFONT lFont = {0};
		::GetObject(hFont, sizeof(lFont), &lFont);
		return lFont;
	}

	// Palette functions
	inline void CDC::AttachPalette(HPALETTE hPalette)
	// Use this to attach an existing HPALETTE or CPalette.
	{
		assert(m_pData->hDC);
		assert(hPalette);
		RemoveCurrentPalette();

		m_pData->hPaletteOld = (HPALETTE)::SelectObject(m_pData->hDC, hPalette);
	}
	
	inline HPALETTE CDC::CreatePalette(LPLOGPALETTE lpLogPalette)
	// Creates the palatte and selects it into the device context.
	{
		assert(m_pData->hDC);
		RemoveCurrentPalette();

		m_pData->Palette.CreatePalette(lpLogPalette);
		m_pData->hPaletteOld = (HPALETTE)::SelectObject(m_pData->hDC, m_pData->Palette);
		return m_pData->Palette;
	}

	inline HPALETTE CDC::DetachPalette()
	// Use this to detach the palette from the device context.
	{
		assert(m_pData->hDC);
		HPALETTE hPalette = 0;

		if (m_pData->hPaletteOld)
		{
			hPalette = (HPALETTE)::SelectObject(m_pData->hDC, m_pData->hPaletteOld);
			if (hPalette == m_pData->Palette)
				m_pData->Palette.Detach();
		}

		m_pData->hPaletteOld = NULL;
		return hPalette;
	}

	inline HPALETTE CDC::GetPalette() const
	// Retrieves the current palette
	{
		return m_pData->Palette;
	}

#ifndef _WIN32_WCE
	inline HPALETTE CDC::CreateHalftonePalette()
	// Creates a halftone palette and selects it into the device context. 
	{
		assert(m_pData->hDC);
		RemoveCurrentPalette();

		m_pData->Palette.CreateHalftonePalette(m_pData->hDC);
		m_pData->hPaletteOld = (HPALETTE)::SelectObject(m_pData->hDC, m_pData->Palette);
		return m_pData->Palette;
	}
#endif


	// Pen functions
	inline void CDC::AttachPen(HPEN hPen)
	// Use this to attach an existing HPEN or CPen.
	{
		assert(m_pData->hDC);
		assert(hPen);
		RemoveCurrentPen();

		m_pData->hPenOld = (HPEN)::SelectObject(m_pData->hDC, hPen);
	}

	inline HPEN CDC::CreatePen (int nStyle, int nWidth, COLORREF rgb)
	// Creates the pen and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentPen();

		m_pData->Pen.CreatePen(nStyle, nWidth, rgb);
		m_pData->hPenOld = (HPEN)::SelectObject(m_pData->hDC, m_pData->Pen);
		return m_pData->Pen;
	}

	inline HPEN CDC::CreatePenIndirect (LPLOGPEN pLogPen)
	// Creates the pen and selects it into the device context
	{
		assert(m_pData->hDC);
		RemoveCurrentPen();

		m_pData->Pen.CreatePenIndirect(pLogPen);
		m_pData->hPenOld = (HPEN)::SelectObject(m_pData->hDC, m_pData->Pen);
		return m_pData->Pen;
	}

	inline HPEN CDC::DetachPen()
	// Use this to detach the pen from the device context.
	{
		assert(m_pData->hDC);
		HPEN hPen = 0;

		if (m_pData->hPenOld)
		{
			hPen = (HPEN)::SelectObject(m_pData->hDC, m_pData->hPenOld);
			if (hPen == m_pData->Pen)
				m_pData->Pen.Detach();
		}

		m_pData->hPenOld = NULL;
		return hPen;
	}

	inline LOGPEN CDC::GetPenInfo() const
	// Retrieves the current pen information as a LOGPEN
	{
		assert(m_pData->hDC);

		HPEN hPen = (HPEN)::GetCurrentObject(m_pData->hDC, OBJ_PEN);
		LOGPEN lPen = {0};
		::GetObject(hPen, sizeof(lPen), &lPen);
		return lPen;
	}

	// Region functions
	inline HRGN CDC::CreateRectRgn(int left, int top, int right, int bottom)
	// Creates a rectangular region from the rectangle co-ordinates.
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreateRectRgn(left, top, right, bottom);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::CreateRectRgnIndirect(const RECT& rc)
	// Creates a rectangular region from the rectangle co-ordinates.
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreateRectRgnIndirect(rc);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::CreateFromData(const XFORM* Xform, DWORD nCount, const RGNDATA *pRgnData)
	// Creates a region from the specified region data and tranformation data.
	// Notes: GetRegionData can be used to get a region's data
	//        If the XFROM pointer is NULL, the identity transformation is used.
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreateFromData(Xform, nCount, pRgnData);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::GetRgn() const
	// Retrieves the current palette
	{
		return m_pData->Rgn;
	}

#ifndef _WIN32_WCE
	inline HRGN CDC::CreateEllipticRgn(int left, int top, int right, int bottom)
	// Creates the ellyiptical region from the bounding rectangle co-ordinates
	// and selects it into the device context	
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreateEllipticRgn(left, top, right, bottom);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::CreateEllipticRgnIndirect(const RECT& rc)
	// Creates the ellyiptical region from the bounding rectangle co-ordinates
	// and selects it into the device context	
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreateEllipticRgnIndirect(rc);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::CreatePolygonRgn(LPPOINT ppt, int cPoints, int fnPolyFillMode)
	// Creates the polygon region from the array of points and selects it into
	// the device context. The polygon is presumed closed	
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreatePolygonRgn(ppt, cPoints, fnPolyFillMode);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}

	inline HRGN CDC::CreatePolyPolygonRgn(LPPOINT ppt, LPINT pPolyCounts, int nCount, int fnPolyFillMode)
	// Creates the polygon region from a series of polygons.The polygons can overlap.	
	{
		assert(m_pData->hDC);
		RemoveCurrentRgn();

		m_pData->Rgn.CreatePolyPolygonRgn(ppt, pPolyCounts, nCount, fnPolyFillMode);
		::SelectClipRgn(m_pData->hDC, m_pData->Rgn);
		return m_pData->Rgn;
	}
#endif


	// Wrappers for WinAPI functions

	inline int CDC::GetDeviceCaps (int nIndex) const
	// Retrieves device-specific information for the specified device.
	{
		assert(m_pData->hDC);
		return ::GetDeviceCaps(m_pData->hDC, nIndex);
	}

	// Clipping functions
	inline int CDC::ExcludeClipRect(int Left, int Top, int Right, int BottomRect)
	// Creates a new clipping region that consists of the existing clipping region minus the specified rectangle.
	{
		assert(m_pData->hDC);
		return ::ExcludeClipRect(m_pData->hDC, Left, Top, Right, BottomRect);
	}

	inline int CDC::ExcludeClipRect(const RECT& rc)
	// Creates a new clipping region that consists of the existing clipping region minus the specified rectangle.
	{
		assert(m_pData->hDC);
		return ::ExcludeClipRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline int CDC::GetClipBox (RECT& rc)
	// Retrieves the dimensions of the tightest bounding rectangle that can be drawn around the current visible area on the device.
	{
		assert(m_pData->hDC);
		return ::GetClipBox(m_pData->hDC, &rc);
	}

	inline int CDC::GetClipRgn(HRGN hrgn)
	// Retrieves a handle identifying the current application-defined clipping region for the specified device context.
	// hrgn: A handle to an existing region before the function is called. 
	//       After the function returns, this parameter is a handle to a copy of the current clipping region.
	{
		assert(m_pData->hDC);
		return ::GetClipRgn(m_pData->hDC, hrgn);
	}

	inline int CDC::IntersectClipRect(int Left, int Top, int Right, int Bottom)
	// Creates a new clipping region from the intersection of the current clipping region and the specified rectangle.
	{
		assert(m_pData->hDC);
		return ::IntersectClipRect(m_pData->hDC, Left, Top, Right, Bottom);
	}
	
	inline int CDC::IntersectClipRect(const RECT& rc)
	// Creates a new clipping region from the intersection of the current clipping region and the specified rectangle.
	{
		assert(m_pData->hDC);
		return ::IntersectClipRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL CDC::RectVisible(const RECT& rc)
	// Determines whether any part of the specified rectangle lies within the clipping region of a device context.
	{
		assert(m_pData->hDC);
		return ::RectVisible (m_pData->hDC, &rc);
	}

	inline int CDC::SelectClipRgn(HRGN hRegion)
	// Selects a region as the current clipping region for the specified device context.
	// Note: Only a copy of the selected region is used.
	//       To remove a device-context's clipping region, specify a NULL region handle.
	{
		assert(m_pData->hDC);
		return ::SelectClipRgn(m_pData->hDC, hRegion);
	}

#ifndef _WIN32_WCE
	inline int CDC::ExtSelectClipRgn(HRGN hrgn, int fnMode)
	// Combines the specified region with the current clipping region using the specified mode.
	{
		assert(m_pData->hDC);
		return ::ExtSelectClipRgn(m_pData->hDC, hrgn, fnMode);
	}

	inline BOOL CDC::PtVisible(int X, int Y)
	// Determines whether the specified point is within the clipping region of a device context.
	{
		assert(m_pData->hDC);
		return ::PtVisible (m_pData->hDC, X, Y);
	}

	inline int CDC::OffsetClipRgn(int nXOffset, int nYOffset)
	// Moves the clipping region of a device context by the specified offsets.
	{
		assert(m_pData->hDC);
		return ::OffsetClipRgn (m_pData->hDC, nXOffset, nYOffset);
	}
#endif

	// Point and Line Drawing Functions
	inline CPoint CDC::GetCurrentPosition() const
	// Returns the current "MoveToEx" position.
	{
		assert(m_pData->hDC);
		CPoint pt;
		::MoveToEx(m_pData->hDC, 0, 0, &pt);
		::MoveToEx(m_pData->hDC, pt.x, pt.y, NULL);
		return pt;
	}

	inline CPoint CDC::MoveTo(int x, int y) const
	// Updates the current position to the specified point.
	{
		assert(m_pData->hDC);
		return ::MoveToEx(m_pData->hDC, x, y, NULL);
	}

	inline CPoint CDC::MoveTo(POINT pt) const
	// Updates the current position to the specified point
	{
		assert(m_pData->hDC);
		return ::MoveToEx(m_pData->hDC, pt.x, pt.y, NULL);
	}

	inline BOOL CDC::LineTo(int x, int y) const
	// Draws a line from the current position up to, but not including, the specified point.
	{
		assert(m_pData->hDC);
		return ::LineTo(m_pData->hDC, x, y);
	}

	inline BOOL CDC::LineTo(POINT pt) const
	// Draws a line from the current position up to, but not including, the specified point.
	{
		assert(m_pData->hDC);
		return ::LineTo(m_pData->hDC, pt.x, pt.y);
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Arc(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	// Draws an elliptical arc.
	{
		assert(m_pData->hDC);
		return ::Arc(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL CDC::Arc(RECT& rc, POINT ptStart, POINT ptEnd) const
	// Draws an elliptical arc.
	{
		assert(m_pData->hDC);
		return ::Arc(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL CDC::ArcTo(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	// Draws an elliptical arc.
	{
		assert(m_pData->hDC);
		return ::ArcTo(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL CDC::ArcTo(RECT& rc, POINT ptStart, POINT ptEnd) const
	// Draws an elliptical arc.
	{
		assert(m_pData->hDC);
		return ::ArcTo (m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL CDC::AngleArc(int x, int y, int nRadius, float fStartAngle, float fSweepAngle) const
	// Draws a line segment and an arc.
	{
		assert(m_pData->hDC);
		return ::AngleArc(m_pData->hDC, x, y, nRadius, fStartAngle, fSweepAngle);
	}

	inline int CDC::GetArcDirection() const
	// Retrieves the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE ).
	{
		assert(m_pData->hDC);
		return ::GetArcDirection(m_pData->hDC);
	}

	inline int CDC::SetArcDirection(int nArcDirection) const
	// Sets the current arc direction ( AD_COUNTERCLOCKWISE or AD_CLOCKWISE ).
	{
		assert(m_pData->hDC);
		return ::SetArcDirection(m_pData->hDC, nArcDirection);
	}

	inline BOOL CDC::PolyDraw(const POINT* lpPoints, const BYTE* lpTypes, int nCount) const
	// Draws a set of line segments and Bzier curves.
	{
		assert(m_pData->hDC);
		return ::PolyDraw(m_pData->hDC, lpPoints, lpTypes, nCount);
	}

	inline BOOL CDC::Polyline(LPPOINT lpPoints, int nCount) const
	// Draws a series of line segments by connecting the points in the specified array.
	{
		assert(m_pData->hDC);
		return ::Polyline(m_pData->hDC, lpPoints, nCount);
	}

	inline BOOL CDC::PolyPolyline(const POINT* lpPoints, const DWORD* lpPolyPoints, int nCount) const
	// Draws multiple series of connected line segments.
	{
		assert(m_pData->hDC);
		return ::PolyPolyline(m_pData->hDC, lpPoints, lpPolyPoints, nCount);
	}

	inline BOOL CDC::PolylineTo(const POINT* lpPoints, int nCount) const
	// Draws one or more straight lines.
	{
		assert(m_pData->hDC);
		return ::PolylineTo(m_pData->hDC, lpPoints, nCount);
	}
	inline BOOL CDC::PolyBezier(const POINT* lpPoints, int nCount) const
	// Draws one or more Bzier curves.
	{
		assert(m_pData->hDC);
		return ::PolyBezier(m_pData->hDC, lpPoints, nCount);
	}

	inline BOOL CDC::PolyBezierTo(const POINT* lpPoints, int nCount) const
	// Draws one or more Bzier curves.
	{
		assert(m_pData->hDC);
		return ::PolyBezierTo(m_pData->hDC, lpPoints, nCount );
	}

	inline COLORREF CDC::GetPixel(int x, int y) const
	// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates.
	{
		assert(m_pData->hDC);
		return ::GetPixel(m_pData->hDC, x, y);
	}

	inline COLORREF CDC::GetPixel(POINT pt) const
	// Retrieves the red, green, blue (RGB) color value of the pixel at the specified coordinates.
	{
		assert(m_pData->hDC);
		return ::GetPixel(m_pData->hDC, pt.x, pt.y);
	}

	inline COLORREF CDC::SetPixel (int x, int y, COLORREF crColor) const
	// Sets the pixel at the specified coordinates to the specified color.
	{
		assert(m_pData->hDC);
		return ::SetPixel(m_pData->hDC, x, y, crColor);
	}

	inline COLORREF CDC::SetPixel(POINT pt, COLORREF crColor) const
	// Sets the pixel at the specified coordinates to the specified color.
	{
		assert(m_pData->hDC);
		return ::SetPixel(m_pData->hDC, pt.x, pt.y, crColor);
	}

	inline BOOL CDC::SetPixelV(int x, int y, COLORREF crColor) const
	// Sets the pixel at the specified coordinates to the closest approximation of the specified color.
	{
		assert(m_pData->hDC);
		return ::SetPixelV(m_pData->hDC, x, y, crColor);
	}

	inline BOOL CDC::SetPixelV(POINT pt, COLORREF crColor) const
	// Sets the pixel at the specified coordinates to the closest approximation of the specified color.
	{
		assert(m_pData->hDC);
		return ::SetPixelV(m_pData->hDC, pt.x, pt.y, crColor);
	}
#endif

	// Shape Drawing Functions
	inline void CDC::DrawFocusRect(const RECT& rc) const
	// Draws a rectangle in the style used to indicate that the rectangle has the focus.
	{
		assert(m_pData->hDC);
		::DrawFocusRect(m_pData->hDC, &rc);
	}

	inline BOOL CDC::Ellipse(int x1, int y1, int x2, int y2) const
	// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
	{
		assert(m_pData->hDC);
		return ::Ellipse(m_pData->hDC, x1, y1, x2, y2);
	}

	inline BOOL CDC::Ellipse(const RECT& rc) const
	// Draws an ellipse. The center of the ellipse is the center of the specified bounding rectangle.
	{
		assert(m_pData->hDC);
		return ::Ellipse(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL CDC::Polygon(LPPOINT lpPoints, int nCount) const
	// Draws a polygon consisting of two or more vertices connected by straight lines.
	{
		assert(m_pData->hDC);
		return ::Polygon(m_pData->hDC, lpPoints, nCount);
	}

	inline BOOL CDC::Rectangle(int x1, int y1, int x2, int y2) const
	// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
	{
		assert(m_pData->hDC);
		return ::Rectangle(m_pData->hDC, x1, y1, x2, y2);
	}

	inline BOOL CDC::Rectangle(const RECT& rc) const
	// Draws a rectangle. The rectangle is outlined by using the current pen and filled by using the current brush.
	{
		assert(m_pData->hDC);
		return ::Rectangle(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom);
	}

	inline BOOL CDC::RoundRect(int x1, int y1, int x2, int y2, int nWidth, int nHeight) const
	// Draws a rectangle with rounded corners.
	{
		assert(m_pData->hDC);
		return ::RoundRect(m_pData->hDC, x1, y1, x2, y2, nWidth, nHeight);
	}
	inline BOOL CDC::RoundRect(const RECT& rc, int nWidth, int nHeight) const
	// Draws a rectangle with rounded corners.
	{
		assert(m_pData->hDC);
		return ::RoundRect(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom, nWidth, nHeight );
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::Chord(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant).
	{
		assert(m_pData->hDC);
		return ::Chord(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL CDC::Chord(const RECT& rc, POINT ptStart, POINT ptEnd) const
	// Draws a chord (a region bounded by the intersection of an ellipse and a line segment, called a secant).
	{
		assert(m_pData->hDC);
		return ::Chord(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL CDC::Pie(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) const
	// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
	{
		assert(m_pData->hDC);
		return ::Pie(m_pData->hDC, x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline BOOL CDC::Pie(const RECT& rc, POINT ptStart, POINT ptEnd) const
	// Draws a pie-shaped wedge bounded by the intersection of an ellipse and two radials.
	{
		assert(m_pData->hDC);
		return ::Pie(m_pData->hDC, rc.left, rc.top, rc.right, rc.bottom,
			ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}

	inline BOOL CDC::PolyPolygon(LPPOINT lpPoints, LPINT lpPolyCounts, int nCount) const
	// Draws a series of closed polygons.
	{
		assert(m_pData->hDC);
		return ::PolyPolygon(m_pData->hDC, lpPoints, lpPolyCounts, nCount);
	}
#endif

	// Fill and 3D Drawing functions
	inline BOOL CDC::FillRect(const RECT& rc, HBRUSH hbr) const
	// Fills a rectangle by using the specified brush.
	{
		assert(m_pData->hDC);
		return (BOOL)::FillRect(m_pData->hDC, &rc, hbr);
	}

	inline BOOL CDC::InvertRect(const RECT& rc) const
	// Inverts a rectangle in a window by performing a logical NOT operation on the color values for each pixel in the rectangle's interior.
	{
		assert(m_pData->hDC);
		return ::InvertRect( m_pData->hDC, &rc);
	}

	inline BOOL CDC::DrawIconEx(int xLeft, int yTop, HICON hIcon, int cxWidth, int cyWidth, UINT istepIfAniCur, HBRUSH hbrFlickerFreeDraw, UINT diFlags) const
	// draws an icon or cursor, performing the specified raster operations, and stretching or compressing the icon or cursor as specified.
	{
		assert(m_pData->hDC);
		return ::DrawIconEx(m_pData->hDC, xLeft, yTop, hIcon, cxWidth, cyWidth, istepIfAniCur, hbrFlickerFreeDraw, diFlags);
	}

	inline BOOL CDC::DrawEdge(const RECT& rc, UINT nEdge, UINT nFlags) const
	// Draws one or more edges of rectangle.
	{
		assert(m_pData->hDC);
		return ::DrawEdge(m_pData->hDC, (LPRECT)&rc, nEdge, nFlags);
	}

	inline BOOL CDC::DrawFrameControl(const RECT& rc, UINT nType, UINT nState) const
	// Draws a frame control of the specified type and style.
	{
		assert(m_pData->hDC);
		return ::DrawFrameControl(m_pData->hDC, (LPRECT)&rc, nType, nState);
	}

	inline BOOL CDC::FillRgn(HRGN hrgn, HBRUSH hbr) const
	// Fills a region by using the specified brush.
	{
		assert(m_pData->hDC);
		return ::FillRgn(m_pData->hDC, hrgn, hbr);
	}

#ifndef _WIN32_WCE
	inline BOOL CDC::DrawIcon(int x, int y, HICON hIcon) const
	// Draws an icon or cursor.
	{
		assert(m_pData->hDC);
		return ::DrawIcon(m_pData->hDC, x, y, hIcon);
	}

	inline BOOL CDC::DrawIcon(POINT pt, HICON hIcon) const
	// Draws an icon or cursor.
	{
		assert(m_pData->hDC);
		return ::DrawIcon(m_pData->hDC, pt.x, pt.y, hIcon);
	}

	inline BOOL CDC::FrameRect(const RECT& rc, HBRUSH hbr) const
	// Draws a border around the specified rectangle by using the specified brush.
	{
		assert(m_pData->hDC);
		return (BOOL)::FrameRect(m_pData->hDC, &rc, hbr);
	}

	inline BOOL CDC::PaintRgn(HRGN hrgn) const
	// Paints the specified region by using the brush currently selected into the device context.
	{
		assert(m_pData->hDC);
		return ::PaintRgn(m_pData->hDC, hrgn);
	}
#endif

	// Bitmap Functions
	inline int CDC::StretchDIBits(int XDest, int YDest, int nDestWidth, int nDestHeight, int XSrc, int YSrc, int nSrcWidth,
		           int nSrcHeight, CONST VOID *lpBits, BITMAPINFO& bi, UINT iUsage, DWORD dwRop) const
	// Copies the color data for a rectangle of pixels in a DIB to the specified destination rectangle.
	{
		assert(m_pData->hDC);
		return ::StretchDIBits(m_pData->hDC, XDest, YDest, nDestWidth, nDestHeight, XSrc, YSrc, nSrcWidth, nSrcHeight, lpBits, &bi, iUsage, dwRop);
	}

	inline BOOL CDC::PatBlt(int x, int y, int nWidth, int nHeight, DWORD dwRop) const
	// Paints the specified rectangle using the brush that is currently selected into the device context.
	{
		assert(m_pData->hDC);
		return ::PatBlt(m_pData->hDC, x, y, nWidth, nHeight, dwRop);
	}

	inline BOOL CDC::BitBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop) const
	// Performs a bit-block transfer of the color data corresponding to a rectangle of pixels from the specified source device context into a destination device context.
	{
		assert(m_pData->hDC);
		assert(pSrcDC);
		return ::BitBlt(m_pData->hDC, x, y, nWidth, nHeight, pSrcDC->GetHDC(), xSrc, ySrc, dwRop);
	}

	inline BOOL CDC::StretchBlt(int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, DWORD dwRop) const
	// Copies a bitmap from a source rectangle into a destination rectangle, stretching or compressing the bitmap to fit the dimensions of the destination rectangle, if necessary.
	{
		assert(m_pData->hDC);
		assert(pSrcDC);
		return ::StretchBlt(m_pData->hDC, x, y, nWidth, nHeight, pSrcDC->GetHDC(), xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
	}

#ifndef _WIN32_WCE
	inline int CDC::GetDIBits(HBITMAP hbmp, UINT uStartScan, UINT cScanLines, LPVOID lpvBits, LPBITMAPINFO lpbi, UINT uUsage) const
	// Retrieves the bits of the specified compatible bitmap and copies them into a buffer as a DIB using the specified format.
	{
		assert(m_pData->hDC);
		return ::GetDIBits(m_pData->hDC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, uUsage);
	}

	inline int CDC::SetDIBits(HBITMAP hbmp, UINT uStartScan, UINT cScanLines, CONST VOID *lpvBits, LPBITMAPINFO lpbi, UINT fuColorUse) const
	// Sets the pixels in a compatible bitmap (DDB) using the color data found in the specified DIB.
	{
		assert(m_pData->hDC);
		return ::SetDIBits(m_pData->hDC, hbmp, uStartScan, cScanLines, lpvBits, lpbi, fuColorUse);
	}

	inline int CDC::GetStretchBltMode() const
	// Retrieves the current stretching mode.
	// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK
	{
		assert(m_pData->hDC);
		return ::GetStretchBltMode(m_pData->hDC);
	}

	inline int CDC::SetStretchBltMode(int iStretchMode) const
	// Sets the stretching mode.
	// Possible modes: BLACKONWHITE, COLORONCOLOR, HALFTONE, STRETCH_ANDSCANS, STRETCH_DELETESCANS, STRETCH_HALFTONE, STRETCH_ORSCANS, WHITEONBLACK	
	{
		assert(m_pData->hDC);
		return ::SetStretchBltMode(m_pData->hDC, iStretchMode);
	}

	inline BOOL CDC::FloodFill(int x, int y, COLORREF crColor) const
	// Fills an area of the display surface with the current brush.
	{
		assert(m_pData->hDC);
		return ::FloodFill(m_pData->hDC, x, y, crColor);
	}

	inline BOOL CDC::ExtFloodFill(int x, int y, COLORREF crColor, UINT nFillType) const
	// Fills an area of the display surface with the current brush.
	// Fill type: FLOODFILLBORDER or FLOODFILLSURFACE
	{
		assert(m_pData->hDC);
		return ::ExtFloodFill(m_pData->hDC, x, y, crColor, nFillType );
	}

	// co-ordinate functions
	inline BOOL CDC::DPtoLP(LPPOINT lpPoints, int nCount) const
	// Converts device coordinates into logical coordinates.
	{
		assert(m_pData->hDC);
		return ::DPtoLP(m_pData->hDC, lpPoints, nCount);
	}

	inline BOOL CDC::DPtoLP(RECT& rc) const
	// Converts device coordinates into logical coordinates.
	{
		assert(m_pData->hDC);
		return ::DPtoLP(m_pData->hDC, (LPPOINT)&rc, 2);
	}

	inline BOOL CDC::LPtoDP(LPPOINT lpPoints, int nCount) const
	// Converts logical coordinates into device coordinates.
	{
		assert(m_pData->hDC);
		return ::LPtoDP(m_pData->hDC, lpPoints, nCount);
	}

	inline BOOL CDC::LPtoDP(RECT& rc) const
	// Converts logical coordinates into device coordinates.
	{
		assert(m_pData->hDC);
		return ::LPtoDP(m_pData->hDC, (LPPOINT)&rc, 2);
	}

#endif

	// Layout Functions
	inline DWORD CDC::GetLayout() const
	// Returns the layout of a device context (LAYOUT_RTL and LAYOUT_BITMAPORIENTATIONPRESERVED).
	{
#if defined(WINVER) && defined(GetLayout) && (WINVER >= 0x0500)
		return ::GetLayout(m_pData->hDC);
#else
		return 0;
#endif
	}

	inline DWORD CDC::SetLayout(DWORD dwLayout) const
	// changes the layout of a device context (DC).
	// dwLayout values:  LAYOUT_RTL or LAYOUT_BITMAPORIENTATIONPRESERVED
	{
#if defined(WINVER) && defined (SetLayout) && (WINVER >= 0x0500)
		// Sets the layout of a device context
		return ::SetLayout(m_pData->hDC, dwLayout);
#else
		UNREFERENCED_PARAMETER(dwLayout); // no-op
		return 0;
#endif
	}

	// Mapping Functions
#ifndef _WIN32_WCE
	inline int CDC::GetMapMode()  const
	// Rretrieves the current mapping mode.
	// Possible modes: MM_ANISOTROPIC, MM_HIENGLISH, MM_HIMETRIC, MM_ISOTROPIC, MM_LOENGLISH, MM_LOMETRIC, MM_TEXT, and MM_TWIPS.
	{
		assert(m_pData->hDC);
		return ::GetMapMode(m_pData->hDC);
	}

	inline BOOL CDC::GetViewportOrgEx(LPPOINT lpPoint)  const
	// Retrieves the x-coordinates and y-coordinates of the viewport origin for the device context.
	{
		assert(m_pData->hDC);
		return ::GetViewportOrgEx(m_pData->hDC, lpPoint);
	}

	inline int CDC::SetMapMode(int nMapMode) const
	// Sets the mapping mode of the specified device context.
	{
		assert(m_pData->hDC);
		return ::SetMapMode(m_pData->hDC, nMapMode);
	}

	inline BOOL CDC::SetViewportOrgEx(int x, int y, LPPOINT lpPoint /* = NULL */) const
	// Specifies which device point maps to the window origin (0,0).
	{
		assert(m_pData->hDC);
		return ::SetViewportOrgEx(m_pData->hDC, x, y, lpPoint);
	}

	inline BOOL CDC::SetViewportOrgEx(POINT point, LPPOINT lpPointRet /* = NULL */) const
	// Specifies which device point maps to the window origin (0,0).
	{
		assert(m_pData->hDC);
		return SetViewportOrgEx(point.x, point.y, lpPointRet);
	}

	inline BOOL CDC::OffsetViewportOrgEx(int nWidth, int nHeight, LPPOINT lpPoint /* = NULL */) const
	// Modifies the viewport origin for the device context using the specified horizontal and vertical offsets.
	{
		assert(m_pData->hDC);
		return ::OffsetViewportOrgEx(m_pData->hDC, nWidth, nHeight, lpPoint);
	}

	inline BOOL CDC::GetViewportExtEx(LPSIZE lpSize)  const
	// Retrieves the x-extent and y-extent of the current viewport for the device context.
	{
		assert(m_pData->hDC);
		return ::GetViewportExtEx(m_pData->hDC, lpSize);
	}

	inline BOOL CDC::SetViewportExtEx(int x, int y, LPSIZE lpSize ) const
	// Sets the horizontal and vertical extents of the viewport for the device context by using the specified values.
	{
		assert(m_pData->hDC);
		return ::SetViewportExtEx(m_pData->hDC, x, y, lpSize);
	}

	inline BOOL CDC::SetViewportExtEx(SIZE size, LPSIZE lpSizeRet ) const
	// Sets the horizontal and vertical extents of the viewport for the device context by using the specified values.
	{
		assert(m_pData->hDC);
		return SetViewportExtEx(size.cx, size.cy, lpSizeRet);
	}

	inline BOOL CDC::ScaleViewportExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize ) const
	// Modifies the viewport for the device context using the ratios formed by the specified multiplicands and divisors.
	{
		assert(m_pData->hDC);
		return ::ScaleViewportExtEx(m_pData->hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}

	inline BOOL CDC::GetWindowOrgEx(LPPOINT lpPoint) const
	// Retrieves the x-coordinates and y-coordinates of the window origin for the device context.
	{
		assert(m_pData->hDC);
		return ::GetWindowOrgEx(m_pData->hDC, lpPoint);
	}

	inline BOOL CDC::SetWindowOrgEx(int x, int y, LPPOINT lpPoint ) const
	// Specifies which window point maps to the viewport origin (0,0).
	{
		assert(m_pData->hDC);
		return ::SetWindowOrgEx(m_pData->hDC, x, y, lpPoint);
	}

	inline BOOL CDC::SetWindowOrgEx(POINT point, LPPOINT lpPointRet ) const
	// Specifies which window point maps to the viewport origin (0,0).
	{
		assert(m_pData->hDC);
		return SetWindowOrgEx(point.x, point.y, lpPointRet);
	}

	inline BOOL CDC::OffsetWindowOrgEx(int nWidth, int nHeight, LPPOINT lpPoint ) const
	// Modifies the window origin for the device context using the specified horizontal and vertical offsets.
	{
		assert(m_pData->hDC);
		return ::OffsetWindowOrgEx(m_pData->hDC, nWidth, nHeight, lpPoint);
	}

	inline BOOL CDC::GetWindowExtEx(LPSIZE lpSize)  const
	// Retrieves the x-extent and y-extent of the window for the device context.
	{
		assert(m_pData->hDC);
		return ::GetWindowExtEx(m_pData->hDC, lpSize);
	}

	inline BOOL CDC::SetWindowExtEx(int x, int y, LPSIZE lpSize ) const
	// Sets the horizontal and vertical extents of the window for the device context by using the specified values.
	{
		assert(m_pData->hDC);
		return ::SetWindowExtEx(m_pData->hDC, x, y, lpSize);
	}

	inline BOOL CDC::SetWindowExtEx(SIZE size, LPSIZE lpSizeRet) const
	// Sets the horizontal and vertical extents of the window for the device context by using the specified values.
	{
		assert(m_pData->hDC);
		return SetWindowExtEx(size.cx, size.cy, lpSizeRet);
	}

	inline BOOL CDC::ScaleWindowExtEx(int xNum, int xDenom, int yNum, int yDenom, LPSIZE lpSize) const
	// Modifies the window for the device context using the ratios formed by the specified multiplicands and divisors.
	{
		assert(m_pData->hDC);
		return ::ScaleWindowExtEx(m_pData->hDC, xNum, xDenom, yNum, yDenom, lpSize);
	}
#endif

	// Printer Functions
	inline int CDC::StartDoc(LPDOCINFO lpDocInfo) const
	// Starts a print job.
	{
		assert(m_pData->hDC);
		return ::StartDoc(m_pData->hDC, lpDocInfo);
	}

	inline int CDC::EndDoc() const
	// Ends a print job.
	{
		assert(m_pData->hDC);
		return ::EndDoc(m_pData->hDC);
	}

	inline int CDC::StartPage() const
	// Prepares the printer driver to accept data.
	{
		assert(m_pData->hDC);
		return ::StartPage(m_pData->hDC);
	}

	inline int CDC::EndPage() const
	// Notifies the device that the application has finished writing to a page.
	{
		assert(m_pData->hDC);
		return ::EndPage(m_pData->hDC);
	}

	inline int CDC::AbortDoc() const
	// Stops the current print job and erases everything drawn since the last call to the StartDoc function.
	{
		assert(m_pData->hDC);
		return ::AbortDoc(m_pData->hDC);
	}

	inline int CDC::SetAbortProc(BOOL (CALLBACK* lpfn)(HDC, int)) const
	// Sets the application-defined abort function that allows a print job to be canceled during spooling.
	{
		assert(m_pData->hDC);
		return ::SetAbortProc(m_pData->hDC, lpfn);
	}

	// Text Functions
	inline BOOL CDC::ExtTextOut(int x, int y, UINT nOptions, LPCRECT lprc, LPCTSTR lpszString, int nCount /*= -1*/, LPINT lpDxWidths /*=NULL*/) const
	// Draws text using the currently selected font, background color, and text color
	{
		assert(m_pData->hDC);

		if (nCount == -1)
			nCount = lstrlen (lpszString);

		return ::ExtTextOut(m_pData->hDC, x, y, nOptions, lprc, lpszString, nCount, lpDxWidths );
	}

	inline int CDC::DrawText(LPCTSTR lpszString, int nCount, LPRECT lprc, UINT nFormat) const
	// Draws formatted text in the specified rectangle
	{
		assert(m_pData->hDC);
		return ::DrawText(m_pData->hDC, lpszString, nCount, lprc, nFormat );
	}

	inline UINT CDC::GetTextAlign() const
	// Retrieves the text-alignment setting
	// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP
	{
		assert(m_pData->hDC);
		return ::GetTextAlign(m_pData->hDC);
	}

	inline UINT CDC::SetTextAlign(UINT nFlags) const
	// Sets the text-alignment setting
	// Values: TA_BASELINE, TA_BOTTOM, TA_TOP, TA_CENTER, TA_LEFT, TA_RIGHT, TA_RTLREADING, TA_NOUPDATECP, TA_UPDATECP	
	{
		assert(m_pData->hDC);
		return ::SetTextAlign(m_pData->hDC, nFlags);
	}

	inline int CDC::GetTextFace(int nCount, LPTSTR lpszFacename) const
	// Retrieves the typeface name of the font that is selected into the device context	
	{
		assert(m_pData->hDC);
		return ::GetTextFace(m_pData->hDC, nCount, lpszFacename);
	}

	inline BOOL CDC::GetTextMetrics(TEXTMETRIC& Metrics) const
	// Fills the specified buffer with the metrics for the currently selected font	
	{
		assert(m_pData->hDC);
		return ::GetTextMetrics(m_pData->hDC, &Metrics);
	}

	inline COLORREF CDC::GetBkColor() const
	// Returns the current background color
	{
		assert(m_pData->hDC);
		return ::GetBkColor(m_pData->hDC);
	}

	inline COLORREF CDC::SetBkColor(COLORREF crColor) const
	// Sets the current background color to the specified color value
	{
		assert(m_pData->hDC);
		return ::SetBkColor(m_pData->hDC, crColor);
	}

	inline COLORREF CDC::GetTextColor() const
	// Retrieves the current text color
	{
		assert(m_pData->hDC);
		return ::GetTextColor(m_pData->hDC);
	}

	inline COLORREF CDC::SetTextColor(COLORREF crColor) const
	// Sets the current text color
	{
		assert(m_pData->hDC);
		return ::SetTextColor(m_pData->hDC, crColor);
	}

	inline int CDC::GetBkMode() const
	// returns the current background mix mode (OPAQUE or TRANSPARENT)
	{
		assert(m_pData->hDC);
		return ::GetBkMode(m_pData->hDC);
	}

	inline int CDC::SetBkMode(int iBkMode) const
	// Sets the current background mix mode (OPAQUE or TRANSPARENT)
	{
		assert(m_pData->hDC);
		return ::SetBkMode(m_pData->hDC, iBkMode);
	}

#ifndef _WIN32_WCE
	inline int CDC::DrawTextEx(LPTSTR lpszString, int nCount, LPRECT lprc, UINT nFormat, LPDRAWTEXTPARAMS lpDTParams) const
	// Draws formatted text in the specified rectangle with more formatting options
	{
		assert(m_pData->hDC);
		return ::DrawTextEx(m_pData->hDC, lpszString, nCount, lprc, nFormat, lpDTParams);
	}

	inline CSize CDC::GetTextExtentPoint32(LPCTSTR lpszString, int nCount) const
	// Computes the width and height of the specified string of text
	{
		assert(m_pData->hDC);
		CSize sz;
		::GetTextExtentPoint32(m_pData->hDC, lpszString, nCount, &sz);
		return sz;
	}

	inline CSize CDC::GetTabbedTextExtent(LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions) const
	// Computes the width and height of a character string
	{
		assert(m_pData->hDC);
		DWORD dwSize = ::GetTabbedTextExtent(m_pData->hDC, lpszString, nCount, nTabPositions, lpnTabStopPositions );
		CSize sz(dwSize);
		return sz;
	}

	inline BOOL CDC::GrayString(HBRUSH hBrush, GRAYSTRINGPROC lpOutputFunc, LPARAM lpData, int nCount, int x, int y, int nWidth, int nHeight) const
	// Draws gray text at the specified location
	{
		assert(m_pData->hDC);
		return ::GrayString(m_pData->hDC, hBrush, lpOutputFunc, lpData, nCount, x, y, nWidth, nHeight);
	}

	inline int CDC::SetTextJustification(int nBreakExtra, int nBreakCount) const
	// Specifies the amount of space the system should add to the break characters in a string of text
	{
		assert(m_pData->hDC);
		return ::SetTextJustification(m_pData->hDC, nBreakExtra, nBreakCount);
	}

	inline int CDC::GetTextCharacterExtra() const
	// Retrieves the current intercharacter spacing for the device context
	{
		assert(m_pData->hDC);
		return ::GetTextCharacterExtra(m_pData->hDC);
	}

	inline int CDC::SetTextCharacterExtra(int nCharExtra) const
	// Sets the intercharacter spacing
	{
		assert(m_pData->hDC);
		return ::SetTextCharacterExtra(m_pData->hDC, nCharExtra);
	}

	inline CSize CDC::TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount, int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin) const
	// Writes a character string at a specified location, expanding tabs to the values specified in an array of tab-stop positions
	{
		assert(m_pData->hDC);
		DWORD dwSize = ::TabbedTextOut(m_pData->hDC, x, y, lpszString, nCount, nTabPositions, lpnTabStopPositions, nTabOrigin );
		CSize sz(dwSize);
		return sz;
	}

	inline BOOL CDC::TextOut(int x, int y, LPCTSTR lpszString, int nCount/* = -1*/) const
	// Writes a character string at the specified location
	{
		assert(m_pData->hDC);
		if (nCount == -1)
			nCount = lstrlen (lpszString);

		return ::TextOut(m_pData->hDC, x, y, lpszString, nCount);
	}

#endif



	/////////////////////////////////////////////////////////////////
	// Definitions for some global functions in the Win32xx namespace
	//

#ifndef _WIN32_WCE
	inline void TintBitmap (HBITMAP hbmSource, int cRed, int cGreen, int cBlue)
	// Modifies the colour of the supplied Device Dependant Bitmap, by the colour
	// correction values specified. The correction values can range from -255 to +255.
	// This function gains its speed by accessing the bitmap colour information
	// directly, rather than using GetPixel/SetPixel.
	{
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);
		pbmi->bmiHeader.biBitCount = 24;

		// Create the reference DC for GetDIBits to use
		CMemDC MemDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* pByteArray = &vBits[0];

		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
		UINT nWidthBytes = pbmi->bmiHeader.biSizeImage/pbmi->bmiHeader.biHeight;

		// Ensure sane colour correction values
		cBlue  = MIN(cBlue, 255);
		cBlue  = MAX(cBlue, -255);
		cRed   = MIN(cRed, 255);
		cRed   = MAX(cRed, -255);
		cGreen = MIN(cGreen, 255);
		cGreen = MAX(cGreen, -255);

		// Pre-calculate the RGB modification values
		int b1 = 256 - cBlue;
		int g1 = 256 - cGreen;
		int r1 = 256 - cRed;

		int b2 = 256 + cBlue;
		int g2 = 256 + cGreen;
		int r2 = 256 + cRed;

		// Modify the colour
		int yOffset = 0;
		int xOffset;
		int Index;
		for (int Row=0; Row < pbmi->bmiHeader.biHeight; Row++)
		{
			xOffset = 0;

			for (int Column=0; Column < pbmi->bmiHeader.biWidth; Column++)
			{
				// Calculate Index
				Index = yOffset + xOffset;

				// Adjust the colour values
				if (cBlue > 0)
					pByteArray[Index]   = (BYTE)(cBlue + (((pByteArray[Index] *b1)) >>8));
				else if (cBlue < 0)
					pByteArray[Index]   = (BYTE)((pByteArray[Index] *b2) >>8);

				if (cGreen > 0)
					pByteArray[Index+1] = (BYTE)(cGreen + (((pByteArray[Index+1] *g1)) >>8));
				else if (cGreen < 0)
					pByteArray[Index+1] = (BYTE)((pByteArray[Index+1] *g2) >>8);

				if (cRed > 0)
					pByteArray[Index+2] = (BYTE)(cRed + (((pByteArray[Index+2] *r1)) >>8));
				else if (cRed < 0)
					pByteArray[Index+2] = (BYTE)((pByteArray[Index+2] *r2) >>8);

				// Increment the horizontal offset
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
	}

	inline void GrayScaleBitmap(HBITMAP hbmSource)
	{
		// Create our LPBITMAPINFO object
		CBitmapInfoPtr pbmi(hbmSource);

		// Create the reference DC for GetDIBits to use
		CMemDC MemDC(NULL);

		// Use GetDIBits to create a DIB from our DDB, and extract the colour data
		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, NULL, pbmi, DIB_RGB_COLORS);
		std::vector<byte> vBits(pbmi->bmiHeader.biSizeImage, 0);
		byte* pByteArray = &vBits[0];

		MemDC.GetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
		UINT nWidthBytes = pbmi->bmiHeader.biSizeImage/pbmi->bmiHeader.biHeight;

		int yOffset = 0;
		int xOffset;
		int Index;

		for (int Row=0; Row < pbmi->bmiHeader.biHeight; Row++)
		{
			xOffset = 0;

			for (int Column=0; Column < pbmi->bmiHeader.biWidth; Column++)
			{
				// Calculate Index
				Index = yOffset + xOffset;

				BYTE byGray = (BYTE) ((pByteArray[Index] + pByteArray[Index+1]*6 + pByteArray[Index+2] *3)/10);
				pByteArray[Index]   = byGray;
				pByteArray[Index+1] = byGray;
				pByteArray[Index+2] = byGray;

				// Increment the horizontal offset
				xOffset += pbmi->bmiHeader.biBitCount >> 3;
			}

			// Increment vertical offset
			yOffset += nWidthBytes;
		}

		// Save the modified colour back into our source DDB
		MemDC.SetDIBits(hbmSource, 0, pbmi->bmiHeader.biHeight, pByteArray, pbmi, DIB_RGB_COLORS);
	}

	inline HIMAGELIST CreateDisabledImageList(HIMAGELIST himlNormal)
	// Returns a greyed image list, created from hImageList
	{
		int cx, cy;
		int nCount = ImageList_GetImageCount(himlNormal);
		if (0 == nCount)
			return NULL;

		ImageList_GetIconSize(himlNormal, &cx, &cy);

		// Create the disabled ImageList
		HIMAGELIST himlDisabled = ImageList_Create(cx, cy, ILC_COLOR24 | ILC_MASK, nCount, 0);

		// Process each image in the ImageList
		for (int i = 0 ; i < nCount; ++i)
		{
			CClientDC DesktopDC(NULL);
			CMemDC MemDC(NULL);
			MemDC.CreateCompatibleBitmap(&DesktopDC, cx, cx);
			CRect rc;
			rc.SetRect(0, 0, cx, cx);

			// Set the mask color to grey for the new ImageList
			COLORREF crMask = RGB(200, 199, 200);
			if ( GetDeviceCaps(DesktopDC, BITSPIXEL) < 24)
			{
				HPALETTE hPal = (HPALETTE)GetCurrentObject(DesktopDC, OBJ_PAL);
				UINT Index = GetNearestPaletteIndex(hPal, crMask);
				if (Index != CLR_INVALID) crMask = PALETTEINDEX(Index);
			}

			MemDC.SolidFill(crMask, rc);

			// Draw the image on the memory DC
			ImageList_SetBkColor(himlNormal, crMask);
			ImageList_Draw(himlNormal, i, MemDC, 0, 0, ILD_NORMAL);

			// Convert colored pixels to gray
			for (int x = 0 ; x < cx; ++x)
			{
				for (int y = 0; y < cy; ++y)
				{
					COLORREF clr = ::GetPixel(MemDC, x, y);

					if (clr != crMask)
					{
						BYTE byGray = (BYTE) (95 + (GetRValue(clr) *3 + GetGValue(clr)*6 + GetBValue(clr))/20);
						MemDC.SetPixel(x, y, RGB(byGray, byGray, byGray));
					}

				}
			}

			// Detach the bitmap so we can use it.
			HBITMAP hbm = MemDC.DetachBitmap();
			ImageList_AddMasked(himlDisabled, hbm, crMask);
			::DeleteObject(hbm);
		}

		return himlDisabled;
	}
#endif


} // namespace Win32xx

#endif // _WIN32XX_GDI_H_

