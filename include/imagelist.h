// Win32++   Version 7.4.1
// Not officially released yet.
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2014  David Nash
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
// imagelist.h
//  Declaration of the CImageList class

// The CImageList class manages an image list.
// An image list is a collection of images of the same size, each of which
// can be referred to by its index. Image lists are used to efficiently manage
// large sets of icons or bitmaps. All images in an image list are contained
// in a single, wide bitmap in screen device format. An image list can also 
// include a monochrome bitmap that contains masks that are used to draw
// images transparently.
 
#if !defined(_WIN32XX_IMAGELIST_H_)
#define _WIN32XX_IMAGELIST_H_


#include "wincore.h"


namespace Win32xx
{

	///////////////////////////////////////
	// Declaration of the CImageList class, which manages ImageLists.
	// An image list is a collection of images of the same size, each of
	//  which can be referred to by its index.
	//
	class CImageList
	{
		friend class CWinApp;

	public:
		//Construction
		CImageList();
		~CImageList();

		//Initialization
		BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
		BOOL Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
		BOOL Create(LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask);
#if (_WIN32_IE >= 0x0400)
		BOOL Create(CImageList* pImageList);
#endif

		//Operations
		int Add(CBitmap* pbmImage, CBitmap* pbmMask) ;
		int Add(CBitmap* pbmImage, COLORREF crMask);
		int Add(HICON hIcon);
		void Attach(HIMAGELIST hImageList);
		BOOL BeginDrag(int nImage, CPoint ptHotSpot) const;
		void DeleteImageList();
		HIMAGELIST Detach();
		BOOL DragEnter(CWnd* pWndLock, CPoint point) const;
		BOOL DragLeave(CWnd* pWndLock) const;
		BOOL DragMove(CPoint pt) const;
		BOOL DragShowNolock(BOOL bShow) const;
		BOOL Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) const;
		BOOL DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz, COLORREF clrBk, COLORREF clrFg, UINT nStyle) const;
		BOOL Remove(int nImage) const;
		BOOL Replace(int nImage, CBitmap* pbmImage,  CBitmap* pbmMask) const;
		int Replace(int nImage, HICON hIcon) const;
		HIMAGELIST GetHandle() const;
		static CImageList* FromHandle(HIMAGELIST hImageList);

		//Attributes
		HICON GetIcon(int iImage, UINT nFlags) const;
		BOOL GetIconSize(int* cx, int* cy) const;
		int GetImageCount() const;
		BOOL GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const;

		//Operators
		operator HIMAGELIST () const;

	private:
		CImageList(const CImageList&);				// Disable copy construction
		CImageList& operator = (const CImageList&);	// Disable assignment operator
		void AddToMap();
		BOOL RemoveFromMap();

		HIMAGELIST m_hImageList;
		BOOL m_IsTmpImageList;
	};


	///////////////////////////////////////
	// Definitions for the CImageList class
	//

	inline CImageList::CImageList() : m_hImageList(0), m_IsTmpImageList(FALSE)
	{
	}

	inline CImageList::~CImageList()
	{
		if (m_hImageList)
		{
			if (!m_IsTmpImageList)
			{
				::ImageList_Destroy(m_hImageList);
			}

			RemoveFromMap();
		}
	}

	inline void CImageList::AddToMap()
	// Store the HIMAGELIST and CImageList pointer in the HIMAGELIST map
	{
		assert( GetApp() );
		assert(m_hImageList);

		GetApp()->m_csMapLock.Lock();
		GetApp()->m_mapHIMAGELIST.insert(std::make_pair(m_hImageList, this));
		GetApp()->m_csMapLock.Release();
	}

	inline CImageList* CImageList::FromHandle(HIMAGELIST hImageList)
	// Returns the CImageList object associated with the ImageList handle (HIMAGELIST).
	{
		assert( GetApp() );

		CImageList* pImageList = GetApp()->GetCImageListFromMap(hImageList);
		if ((hImageList != 0) && (pImageList == 0))
		{
			pImageList = GetApp()->AddTmpImageList(hImageList);
			pImageList->AddToMap();
			::PostMessage(0, UWM_CLEANUPTEMPS, 0, 0);
		}

		return pImageList;
	}

	inline BOOL CImageList::RemoveFromMap()
	{
		BOOL Success = FALSE;

		if (GetApp())
		{
			// Allocate an iterator for our HIMAGELIST map
			std::map<HIMAGELIST, CImageList*, CompareHIMAGELIST>::iterator m;

			CWinApp* pApp = GetApp();
			if (pApp)
			{
				// Erase the CImageList pointer entry from the map
				pApp->m_csMapLock.Lock();
				for (m = pApp->m_mapHIMAGELIST.begin(); m != pApp->m_mapHIMAGELIST.end(); ++m)
				{
					if (this == m->second)
					{
						pApp->m_mapHIMAGELIST.erase(m);
						Success = TRUE;
						break;
					}
				}

				pApp->m_csMapLock.Release();
			}
		}

		return Success;
	}

	inline int CImageList::Add(CBitmap* pbmImage, CBitmap* pbmMask)
	// Adds an image or images to an image list. The pbmMask parameter can be NULL.
	{
		assert (m_hImageList);
		assert (pbmImage);
		HBITMAP hbmMask = pbmMask? (HBITMAP)*pbmMask : 0;
		return ImageList_Add(m_hImageList, (HBITMAP)*pbmImage, hbmMask );
	}

	inline int CImageList::Add(CBitmap* pbmImage, COLORREF crMask)
	// Adds an image or images to an image list, generating a mask from the specified bitmap.
	{
		assert (m_hImageList);
		assert (pbmImage);
		return ImageList_AddMasked(m_hImageList, (HBITMAP)*pbmImage, crMask);
	}

	inline int CImageList::Add(HICON hIcon)
	// Adds an Icon to the image list
	{
		assert (m_hImageList);

		// Append the icon to the image list
		return ImageList_ReplaceIcon(m_hImageList, -1, hIcon);
	}

	inline void CImageList::Attach(HIMAGELIST hImageList)
	// Attaches an existing ImageList to this CImageList
	{
		assert(hImageList);
		assert( 0 == m_hImageList );
		assert( 0 == GetApp()->GetCImageListFromMap(hImageList) );

		m_hImageList = hImageList;
		AddToMap();
	}

	inline BOOL CImageList::BeginDrag(int nImage, CPoint ptHotSpot) const
	// Begins dragging an image.
	{
		assert(m_hImageList);
		return ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
	}

	inline BOOL CImageList::Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow)
	// Creates a new image list.
	//
	// Possible flag values:
	// ILC_COLOR	Use the default behavior if none of the other ILC_COLOR* flags is specified.
	// ILC_COLOR4	Use a 4-bit (16-color) device-independent bitmap (DIB) section as the bitmap for the image list.
	// ILC_COLOR8	Use an 8-bit DIB section. The colors used for the color table are the same colors as the halftone palette.
	// ILC_COLOR16	Use a 16-bit (32/64k-color) DIB section.
	// ILC_COLOR24	Use a 24-bit DIB section.
	// ILC_COLOR32  Use a 32-bit DIB section.
	// ILC_COLORDDB	Use a device-dependent bitmap.
	// ILC_MASK		Use a mask. The image list contains two bitmaps, one of which is a monochrome bitmap used as a mask.
	//				If this value is not included, the image list contains only one bitmap.
	{
		assert(NULL == m_hImageList);
		m_hImageList = ImageList_Create(cx, cy, nFlags, nInitial, nGrow);

		if (m_hImageList)
			AddToMap();

		return ( m_hImageList!= 0 );
	}

	inline BOOL CImageList::Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
	// Creates a new image list.
	//
	// cx		The width of each image.
	// nGrow	The number of images by which the image list can grow when the system needs to make room for new images.
	// crMask	The color used to generate a mask. Each pixel of this color in the specified bitmap is changed to black,
	//			and the corresponding bit in the mask is set to 1. If this parameter is the CLR_NONE value, no mask is generated.
	{
		assert(NULL == m_hImageList);

		LPCTSTR lpszBitmapID = MAKEINTRESOURCE (nBitmapID);
		return Create(lpszBitmapID, cx, nGrow, crMask);
	}

	inline BOOL CImageList::Create(LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask)
	// Creates a new image list.
	//
	// cx		The width of each image.
	// nGrow	The number of images by which the image list can grow when the system needs to make room for new images.
	// crMask	The color used to generate a mask. Each pixel of this color in the specified bitmap is changed to black,
	//			and the corresponding bit in the mask is set to 1. If this parameter is the CLR_NONE value, no mask is generated.
	{
		assert(NULL == m_hImageList);
		m_hImageList = ImageList_LoadBitmap(GetApp()->GetInstanceHandle(), lpszBitmapID, cx, nGrow, crMask);

		if (m_hImageList)
			AddToMap();

		return ( m_hImageList!= 0 );
	}

#if (_WIN32_IE >= 0x0400)
	inline BOOL CImageList::Create(CImageList* pImageList)
	// Creates a duplicate ImageList
	{
		assert(pImageList);
		m_hImageList = ImageList_Duplicate(pImageList->m_hImageList);

		if (m_hImageList)
			AddToMap();

		return ( m_hImageList!= 0 );
	}
#endif

	inline void CImageList::DeleteImageList()
	// Destroys an image list.
	{
		RemoveFromMap();
		
		ImageList_Destroy(m_hImageList);
		m_hImageList = 0;
	}

	inline HIMAGELIST CImageList::Detach()
	// Detaches the HIMAGELIST from this CImageList. If the HIMAGELIST is not detached it will be
	// destroyed when this CImageList is deconstructed.
	{
		assert(m_hImageList);
		HIMAGELIST hImageList = m_hImageList;
		m_hImageList = 0;
		
		RemoveFromMap();

		return hImageList;
	}

	inline BOOL CImageList::DragEnter(CWnd* pWndLock, CPoint point) const
	// Displays the drag image at the specified position within the window.
	{
		assert(m_hImageList);
		return ImageList_DragEnter(*pWndLock, point.x, point.y);
	}

	inline BOOL CImageList::DragLeave(CWnd* pWndLock) const
	// Unlocks the specified window and hides the drag image, allowing the window to be updated.
	{
		return ImageList_DragLeave(*pWndLock);
	}

	inline BOOL CImageList::DragMove(CPoint pt) const
	// Moves the image that is being dragged during a drag-and-drop operation.
	// This function is typically called in response to a WM_MOUSEMOVE message.
	{
		return ImageList_DragMove(pt.x, pt.y);
	}

	inline BOOL CImageList::DragShowNolock(BOOL bShow) const
	// Shows or hides the drag image during a drag operation, without locking the window.
	{
		return ImageList_DragShowNolock(bShow);
	}

	inline BOOL CImageList::Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) const
	// Draws an image list item in the specified device context.
	{
		assert(m_hImageList);
		return ImageList_Draw(m_hImageList, nImage, pDC->GetHDC() , pt.x, pt.y, nStyle);
	}

	inline BOOL CImageList::DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz, COLORREF clrBk, COLORREF clrFg, UINT nStyle) const
	// Draws an image list item in the specified device context. The function uses the specified drawing style
	// and blends the image with the specified color.
	//
	// nStyle can have the following values:
	// ILD_BLEND25, ILD_FOCUS
	//   Draws the image, blending 25 percent with the blend color specified by rgbFg.
	// 	 This value has no effect if the image list does not contain a mask.
	// ILD_BLEND50, ILD_SELECTED, ILD_BLEND
	//   Draws the image, blending 50 percent with the blend color specified by rgbFg.
	//   This value has no effect if the image list does not contain a mask.
	// ILD_MASK 	Draws the mask.
	// ILD_NORMAL	Draws the image using the background color for the image list. If the
	//				background color is the CLR_NONE value, the image is drawn transparently using the mask.
	// ILD_TRANSPARENT 		Draws the image transparently using the mask, regardless of the background color.
	//						This value has no effect if the image list does not contain a mask.
	// ILD_IMAGE	If the overlay does not require a mask to be drawn set the ILD_IMAGE flag.
	//				This causes ImageList_DrawEx to just draw the image, ignoring the mask.
	{
		assert(m_hImageList);
		return ImageList_DrawEx(m_hImageList, nImage, pDC->GetHDC() , pt.x, pt.y, sz.cx, sz.cy, clrBk, clrFg, nStyle);
	}

	inline HICON CImageList::GetIcon(int iImage, UINT nFlags) const
	// Creates an icon from an image and mask in an image list.
	{
		assert(m_hImageList);
		return ImageList_GetIcon(m_hImageList, iImage, nFlags);
	}

	inline BOOL CImageList::GetIconSize(int* cx, int* cy) const
	// Retrieves the dimensions of images in an image list.
	{
		assert(m_hImageList);
		return ImageList_GetIconSize(m_hImageList, cx, cy);
	}

	inline int CImageList::GetImageCount() const
	// Retrieves the number of images in an image list.
	{
		assert(m_hImageList);
		return ImageList_GetImageCount(m_hImageList);
	}

	inline BOOL CImageList::GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const
	// Retrieves information about an image.
	{
		assert(m_hImageList);
		return ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
	}

	inline BOOL CImageList::Remove(int nImage) const
	// Removes an image from an image list.
	{
		assert(m_hImageList);
		return ImageList_Remove(m_hImageList, nImage);
	}

	inline BOOL CImageList::Replace(int nImage, CBitmap* pbmImage,  CBitmap* pbmMask) const
	// Replaces an image in an image list with a new image.
	{
		assert (m_hImageList);
		return ImageList_Replace(m_hImageList, nImage, *pbmImage, *pbmMask);
	}

	inline int CImageList::Replace(int nImage, HICON hIcon) const
	// Replaces an image with an icon or cursor.
	{
		assert (m_hImageList);
		return ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
	}

	inline HIMAGELIST CImageList::GetHandle() const
	// Returns the HIMAGELIST assigned to this CImageList
	{
		return m_hImageList;
	}

	inline CImageList::operator HIMAGELIST () const
	// Retrieves the image list's handle.
	{
		return m_hImageList;
	}


}	// namespace Win32xx

#endif	// _WIN32XX_MENU_H_

