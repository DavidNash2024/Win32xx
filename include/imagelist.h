// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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

		struct DataMembers	// A structure that contains the data members
		{
			HIMAGELIST	hImageList;
			BOOL		IsTmpImageList;
			long		Count;
		};

	public:
		//Construction
		CImageList();
		CImageList(const CImageList& rhs);
		CImageList& operator = (const CImageList& rhs);
		void operator = (const HIMAGELIST hImageLIst);
		~CImageList();

		//Initialization
		BOOL Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow);
		BOOL Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask);
		BOOL Create(LPCTSTR lpszBitmapID, int cx, int nGrow, COLORREF crMask);
#if (_WIN32_IE >= 0x0400)
		BOOL Create(CImageList* pImageList);
#endif

#ifndef _WIN32_WCE
		BOOL CreateDisabledImageList(CImageList* pimlNormal);
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
		BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pimldp);
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
		void AddToMap();
		void Release();
		BOOL RemoveFromMap();

		DataMembers* m_pData; 
	};


	///////////////////////////////////////
	// Definitions for the CImageList class
	//

	inline CImageList::CImageList()
	{
		m_pData = new DataMembers;
		m_pData->hImageList = 0;
		m_pData->Count = 1L;
		m_pData->IsTmpImageList = FALSE;
	}

	inline CImageList::CImageList(const CImageList& rhs)
	// Note: A copy of a CImageList is a clone of the original.
	//       Both objects manipulate the one HIMAGELIST.
	{
		m_pData = rhs.m_pData;
		InterlockedIncrement(&m_pData->Count);
	}

	inline CImageList& CImageList::operator = (const CImageList& rhs)
	// Note: A copy of a CImageList is a clone of the original.
	{
		if (this != &rhs)
		{
			InterlockedIncrement(&rhs.m_pData->Count);
			Release();
			m_pData = rhs.m_pData;
		}

		return *this;
	}

	inline void CImageList::operator = (const HIMAGELIST hImageLIst)
	{
		Attach(hImageLIst);
	}

	inline CImageList::~CImageList()
	{
		Release();
	}

	inline void CImageList::AddToMap()
	// Store the HIMAGELIST and CImageList pointer in the HIMAGELIST map
	{
		assert( GetApp() );
		assert(m_pData->hImageList);

		GetApp()->m_csMapLock.Lock();
		GetApp()->m_mapHIMAGELIST.insert(std::make_pair(m_pData->hImageList, this));
		GetApp()->m_csMapLock.Release();
	}

	inline CImageList* CImageList::FromHandle(HIMAGELIST hImageList)
	// Returns the CImageList object associated with the ImageList handle (HIMAGELIST).
	{
		assert( GetApp() );

		// Find an existing permanent CImageList from the map
		CImageList* pImageList = GetApp()->GetCImageListFromMap(hImageList);
		if ((0 != hImageList) && (0 == pImageList))
		{
			// Find any existing temporary CImageList for the HIMAGELIST
			TLSData* pTLSData = GetApp()->SetTlsData();
			std::map<HIMAGELIST, ImageListPtr, CompareHIMAGELIST>::iterator m;
			m = pTLSData->TmpImageLists.find(hImageList);
	
			if (m != pTLSData->TmpImageLists.end())
				pImageList = m->second.get();

			if (!pImageList)
			{
				pImageList = new CImageList;
				pImageList->m_pData->hImageList = hImageList;
				pImageList->m_pData->IsTmpImageList = TRUE;
				pTLSData->TmpImageLists.insert(std::make_pair(hImageList, pImageList));

				::PostMessage(0, UWM_CLEANUPTEMPS, 0, 0);
			}
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
		assert(m_pData);
		assert (m_pData->hImageList);
		assert (pbmImage);
		HBITMAP hbmMask = pbmMask? (HBITMAP)*pbmMask : 0;
		return ImageList_Add(m_pData->hImageList, (HBITMAP)*pbmImage, hbmMask );
	}

	inline int CImageList::Add(CBitmap* pbmImage, COLORREF crMask)
	// Adds an image or images to an image list, generating a mask from the specified bitmap.
	{
		assert(m_pData);
		assert (m_pData->hImageList);
		assert (pbmImage);
		return ImageList_AddMasked(m_pData->hImageList, (HBITMAP)*pbmImage, crMask);
	}

	inline int CImageList::Add(HICON hIcon)
	// Adds an Icon to the image list
	{
		assert(m_pData);
		assert (m_pData->hImageList);

		// Append the icon to the image list
		return ImageList_ReplaceIcon(m_pData->hImageList, -1, hIcon);
	}

	inline void CImageList::Attach(HIMAGELIST hImageList)
	// Attaches an existing ImageList to this CImageList
	{
		assert(hImageList);
		assert(m_pData);
		assert( 0 == m_pData->hImageList );

		CImageList* pImageList = GetApp()->GetCImageListFromMap(hImageList);
		if (pImageList)
		{
			delete m_pData;
			m_pData = pImageList->m_pData;
			InterlockedIncrement(&m_pData->Count);
		}
		else
		{
			m_pData->hImageList = hImageList;
			AddToMap();
		}
	}

	inline BOOL CImageList::BeginDrag(int nImage, CPoint ptHotSpot) const
	// Begins dragging an image.
	{
		assert(m_pData);
		assert(m_pData->hImageList);
		return ImageList_BeginDrag(m_pData->hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
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
		assert(m_pData);
		assert(NULL == m_pData->hImageList);
		m_pData->hImageList = ImageList_Create(cx, cy, nFlags, nInitial, nGrow);

		if (m_pData->hImageList)
			AddToMap();

		return ( m_pData->hImageList!= 0 );
	}

	inline BOOL CImageList::Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
	// Creates a new image list.
	//
	// cx		The width of each image.
	// nGrow	The number of images by which the image list can grow when the system needs to make room for new images.
	// crMask	The color used to generate a mask. Each pixel of this color in the specified bitmap is changed to black,
	//			and the corresponding bit in the mask is set to 1. If this parameter is the CLR_NONE value, no mask is generated.
	{
		assert(m_pData);
		assert(NULL == m_pData->hImageList);

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
		assert(m_pData);
		assert(NULL == m_pData->hImageList);
		m_pData->hImageList = ImageList_LoadBitmap(GetApp()->GetInstanceHandle(), lpszBitmapID, cx, nGrow, crMask);

		if (m_pData->hImageList)
			AddToMap();

		return ( m_pData->hImageList!= 0 );
	}

#if (_WIN32_IE >= 0x0400)
	inline BOOL CImageList::Create(CImageList* pImageList)
	// Creates a duplicate ImageList
	{
		assert(pImageList);
		assert(m_pData);
		m_pData->hImageList = ImageList_Duplicate(pImageList->GetHandle());

		if (m_pData->hImageList != 0)
			AddToMap();

		return ( m_pData->hImageList!= 0 );
	}
#endif

	inline void CImageList::DeleteImageList()
	// Destroys an image list.
	{
		assert(m_pData);
		if (m_pData->hImageList != 0)
		{
			RemoveFromMap();
			
			ImageList_Destroy(m_pData->hImageList);
			m_pData->hImageList = 0;
		}
	}

	inline HIMAGELIST CImageList::Detach()
	// Detaches the HIMAGELIST from this CImageList. If the HIMAGELIST is not detached it will be
	// destroyed when this CImageList is deconstructed.
	{
		assert(m_pData);
		assert(m_pData->hImageList);
		HIMAGELIST hImageList = m_pData->hImageList;

		if (m_pData->Count > 0)
		{
			if (InterlockedDecrement(&m_pData->Count) == 0)
			{
				RemoveFromMap();
				delete m_pData;
			}
		}

		// Assign values to our data members
		m_pData = new DataMembers;
		m_pData->hImageList = 0;
		m_pData->Count = 1L;
		m_pData->IsTmpImageList = FALSE;	

		return hImageList;
	}

	inline BOOL CImageList::DragEnter(CWnd* pWndLock, CPoint point) const
	// Displays the drag image at the specified position within the window.
	{
		assert(m_pData->hImageList);
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
		assert(m_pData->hImageList);
		return ImageList_Draw(m_pData->hImageList, nImage, pDC->GetHDC() , pt.x, pt.y, nStyle);
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
		assert(m_pData->hImageList);
		return ImageList_DrawEx(m_pData->hImageList, nImage, pDC->GetHDC() , pt.x, pt.y, sz.cx, sz.cy, clrBk, clrFg, nStyle);
	}

	inline BOOL CImageList::DrawIndirect(IMAGELISTDRAWPARAMS* pimldp)
	// Draws an image list image based on an IMAGELISTDRAWPARAMS structure.
	{
		assert(m_pData->hImageList);
		return ImageList_DrawIndirect(pimldp);
	}

	inline HICON CImageList::GetIcon(int iImage, UINT nFlags) const
	// Creates an icon from an image and mask in an image list.
	{
		assert(m_pData->hImageList);
		return ImageList_GetIcon(m_pData->hImageList, iImage, nFlags);
	}

	inline BOOL CImageList::GetIconSize(int* cx, int* cy) const
	// Retrieves the dimensions of images in an image list.
	{
		assert(m_pData->hImageList);
		return ImageList_GetIconSize(m_pData->hImageList, cx, cy);
	}

	inline int CImageList::GetImageCount() const
	// Retrieves the number of images in an image list.
	{
		assert(m_pData->hImageList);
		return ImageList_GetImageCount(m_pData->hImageList);
	}

	inline BOOL CImageList::GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const
	// Retrieves information about an image.
	{
		assert(m_pData->hImageList);
		return ImageList_GetImageInfo(m_pData->hImageList, nImage, pImageInfo);
	}

	inline BOOL CImageList::Remove(int nImage) const
	// Removes an image from an image list.
	{
		assert(m_pData->hImageList);
		return ImageList_Remove(m_pData->hImageList, nImage);
	}

	inline BOOL CImageList::Replace(int nImage, CBitmap* pbmImage,  CBitmap* pbmMask) const
	// Replaces an image in an image list with a new image.
	{
		assert (m_pData->hImageList);
		return ImageList_Replace(m_pData->hImageList, nImage, *pbmImage, *pbmMask);
	}

	inline int CImageList::Replace(int nImage, HICON hIcon) const
	// Replaces an image with an icon or cursor.
	{
		assert (m_pData->hImageList);
		return ImageList_ReplaceIcon(m_pData->hImageList, nImage, hIcon);
	}

	inline HIMAGELIST CImageList::GetHandle() const
	// Returns the HIMAGELIST assigned to this CImageList
	{
		return m_pData->hImageList;
	}

	inline CImageList::operator HIMAGELIST () const
	// Retrieves the image list's handle.
	{
		return m_pData->hImageList;
	}

	inline void CImageList::Release()
	{
		assert(m_pData);

		if (InterlockedDecrement(&m_pData->Count) == 0)
		{
			if (m_pData->hImageList != NULL)
			{
				if (!m_pData->IsTmpImageList)
				{
					ImageList_Destroy(m_pData->hImageList);
					RemoveFromMap();
				}
			}

			delete m_pData;
			m_pData = 0;
		}
	}


#ifndef _WIN32_WCE

	inline BOOL CImageList::CreateDisabledImageList(CImageList* pimlNormal)
	// Creates a gray scale image list from the specified color image list.
	{
		assert(NULL == m_pData->hImageList);
		assert(pimlNormal);
	
		int nCount = pimlNormal->GetImageCount();
		if (nCount > 0)
		{
			int cx, cy;
			pimlNormal->GetIconSize(&cx, &cy);

			// Create the disabled ImageList
			Create(cx, cy, ILC_COLOR24 | ILC_MASK, nCount, 0);

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
				pimlNormal->Draw(&MemDC, i, CPoint(0,0), ILD_NORMAL);

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
				CBitmap Bitmap = MemDC.DetachBitmap();
				Add(&Bitmap, crMask);
			}
		}

		return ( m_pData->hImageList!= 0 );
	}


#endif
}	// namespace Win32xx

#endif	// _WIN32XX_MENU_H_

