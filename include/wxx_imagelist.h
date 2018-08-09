// Win32++   Version 8.5.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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
// wxx_imagelist.h
//  Declaration of the CImageList class

// The CImageList class manages an image list.
// An image list is a collection of images of the same size, each of which
// can be referred to by its index. Image lists are used to efficiently manage
// large sets of icons or bitmaps. All images in an image list are contained
// in a single, wide bitmap in screen device format. An image list can also
// include a monochrome bitmap that contains masks that are used to draw
// images transparently.


#ifndef _WIN32XX_IMAGELIST_H_
#define _WIN32XX_IMAGELIST_H_


#include "wxx_appcore0.h"
#include "wxx_rect.h"


namespace Win32xx
{
    // Forward declaration
    class CBitmap;

    ///////////////////////////////////////
    // The CImageList class which provides the functionality of image lists.
    // An image list is a collection of images of the same size, each of
    // which can be referred to by its index.
    class CImageList
    {
        friend class CWinApp;

    public:
        //Construction
        CImageList();
        CImageList(HIMAGELIST hImages);
        CImageList(const CImageList& rhs);
        CImageList& operator = (const CImageList& rhs);
        void operator = (const HIMAGELIST rhs);
        ~CImageList();

        //Initialization
        BOOL Create(int cx, int cy, UINT flags, int initial, int grow);
        BOOL Create(UINT bitmapID, int cx, int grow, COLORREF mask);
        BOOL Create(LPCTSTR pBitmapID, int cx, int grow, COLORREF mask);
        BOOL Create(HIMAGELIST hImages);

#ifndef _WIN32_WCE
        BOOL CreateDisabledImageList(HIMAGELIST hNormal);
#endif

        //Operations
        int Add(HBITMAP hImage, HBITMAP hMask) const;
        int Add(HBITMAP hImage, COLORREF mask) const;
        int Add(HICON hIcon) const;
        void Attach(HIMAGELIST hImages);
        BOOL BeginDrag(int image, CPoint hotSpot) const;
		BOOL Copy(int dst, int src, UINT flags /*= ILCF_MOVE*/) const;
        void DeleteImageList();
        HIMAGELIST Detach();
        BOOL DragEnter(HWND hLock, CPoint point) const;
        BOOL DragLeave(HWND hLock) const;
        BOOL DragMove(CPoint point) const;
        BOOL DragShowNolock(BOOL show) const;
        BOOL Draw(HDC hdc, int image, POINT point, UINT style) const;
        BOOL DrawEx(HDC hdc, int image, POINT pt, SIZE sz, COLORREF bckgnd, COLORREF frgnd, UINT style) const;
        BOOL DrawIndirect(IMAGELISTDRAWPARAMS* pILDParams) const;
		void EndDrag() const;
		HICON ExtractIcon(int index) const;
		void Read(LPSTREAM pStream);
        BOOL Remove(int image) const;
        BOOL Replace(int image, HBITMAP hImage, HBITMAP hMask) const;
        int Replace(int image, HICON hIcon) const;
		BOOL Write(LPSTREAM pStream) const;

        //Attributes
		COLORREF GetBkColor() const;
		HIMAGELIST GetDragImage(POINT* pPoint, POINT* pHotspot) const;
		HIMAGELIST GetHandle() const;
        HICON GetIcon(int image, UINT flags) const;
        CSize GetIconSize() const;
        int GetImageCount() const;
        BOOL GetImageInfo(int image, IMAGEINFO* pImageInfo) const;
		COLORREF SetBkColor(COLORREF bckgnd) const;
		BOOL SetDragCursorImage(int drag, int dxHotspot, int dyHotspot) const;

        //Operators
        operator HIMAGELIST () const;

    private:
        void AddToMap() const;
        void Release();
        BOOL RemoveFromMap() const;

        CIml_Data* m_pData;
    };


    ///////////////////////////////////////
    // Definitions for the CImageList class
    //

    inline CImageList::CImageList()
    {
        m_pData = new CIml_Data;
    }


    inline CImageList::CImageList(HIMAGELIST hImages)
    {
        m_pData = new CIml_Data;
        Attach(hImages);
    }


    // Note: A copy of a CImageList is a clone of the original.
    //       Both objects manipulate the one HIMAGELIST.
    inline CImageList::CImageList(const CImageList& rhs)
    {
        m_pData = rhs.m_pData;
        InterlockedIncrement(&m_pData->count);
    }


    // Note: A copy of a CImageList is a clone of the original.
    inline CImageList& CImageList::operator = (const CImageList& rhs)
    {
        if (this != &rhs)
        {
            InterlockedIncrement(&rhs.m_pData->count);
            Release();
            m_pData = rhs.m_pData;
        }

        return *this;
    }


    inline void CImageList::operator = (const HIMAGELIST rhs)
    {
        Attach(rhs);
    }


    inline CImageList::~CImageList()
    {
        Release();
    }


    // Store the HIMAGELIST and CImageList pointer in the HIMAGELIST map
    inline void CImageList::AddToMap() const
    {
        assert( &GetApp() );
        assert(m_pData->hImageList);

        GetApp().AddCImlData(m_pData->hImageList, m_pData);
    }


    inline BOOL CImageList::RemoveFromMap() const
    {
        BOOL Success = FALSE;

        if ( &GetApp() )
        {
            // Allocate an iterator for our CImageList data
            std::map<HIMAGELIST, CIml_Data*, CompareHIMAGELIST>::iterator m;

            CWinApp& App = GetApp();
            CThreadLock mapLock(App.m_wndLock);
            m = App.m_mapCImlData.find(m_pData->hImageList);
            if (m != App.m_mapCImlData.end())
            {
                // Erase the CImageList data entry from the map
                App.m_mapCImlData.erase(m);
                Success = TRUE;
            }

        }

        return Success;
    }


    // Adds an image or images to an image list, generating a mask from the specified bitmap.
    // The hbmMask parameter can be NULL.
    inline int CImageList::Add(HBITMAP hImage, HBITMAP hMask) const
    {
        assert(m_pData);
        assert (m_pData->hImageList);
        return ImageList_Add(m_pData->hImageList, hImage, hMask );
    }


    // Adds an image or images to an image list, using the specified color as the mask.
    inline int CImageList::Add(HBITMAP hImage, COLORREF mask) const
    {
        assert(m_pData);
        assert (m_pData->hImageList);
        return ImageList_AddMasked(m_pData->hImageList, hImage, mask);
    }


    // Adds an Icon to the image list
    inline int CImageList::Add(HICON hIcon) const
    {
        assert(m_pData);
        assert (m_pData->hImageList);

        // Append the icon to the image list
        return ImageList_ReplaceIcon(m_pData->hImageList, -1, hIcon);
    }


    // Attaches an existing ImageList to this CImageList
    // hImageList can be NULL
    inline void CImageList::Attach(HIMAGELIST hImages)
    {
        assert(m_pData);

        if (hImages != m_pData->hImageList)
        {
            // Release any existing ImageList
            if (m_pData->hImageList)
            {
                Release();
                m_pData = new CIml_Data;
            }

            if (hImages)
            {
                // Add the image list to this CImageList
                CIml_Data* pCImlData = GetApp().GetCImlData(hImages);
                if (pCImlData)
                {
                    delete m_pData;
                    m_pData = pCImlData;
                    InterlockedIncrement(&m_pData->count);
                }
                else
                {
                    m_pData->hImageList = hImages;
                    AddToMap();
                }
            }
        }
    }


    // Begins dragging an image.
    inline BOOL CImageList::BeginDrag(int image, CPoint hotSpot) const
    {
        assert(m_pData);
        assert(m_pData->hImageList);
        return ImageList_BeginDrag(m_pData->hImageList, image, hotSpot.x, hotSpot.y);
    }

	// Copies images within the image list.
	// Possible flags: ILCF_MOVE, CLCF_SWAP 
	inline BOOL CImageList::Copy(int Dest, int Src, UINT flags /*= ILCF_MOVE*/) const
	{
		assert(m_pData);
		assert(m_pData->hImageList);
		return ImageList_Copy(*this, Dest, *this, Src, flags);
	}


    // Creates a new image list.
    // Possible flag values:
    // ILC_COLOR    Use the default behavior if none of the other ILC_COLOR* flags is specified.
    // ILC_COLOR4   Use a 4-bit (16-color) device-independent bitmap (DIB) section as the bitmap for the image list.
    // ILC_COLOR8   Use an 8-bit DIB section. The colors used for the color table are the same colors as the halftone palette.
    // ILC_COLOR16  Use a 16-bit (32/64k-color) DIB section.
    // ILC_COLOR24  Use a 24-bit DIB section.
    // ILC_COLOR32  Use a 32-bit DIB section.
    // ILC_COLORDDB Use a device-dependent bitmap.
    // ILC_MASK     Use a mask. The image list contains two bitmaps, one of which is a monochrome bitmap used as a mask.
    //              If this value is not included, the image list contains only one bitmap.
    inline BOOL CImageList::Create(int cx, int cy, UINT flags, int initial, int grow)
    {
        assert(m_pData);

        HIMAGELIST himlNew = ImageList_Create(cx, cy, flags, initial, grow);

        if (himlNew)
        {
        //  m_pData->hImageList = himlNew;
        //  AddToMap();

            Attach(himlNew);
            m_pData->isManagedHiml = TRUE;
        }

        return ( himlNew != 0 );
    }


    // Creates a new image list.
    // cx       The width of each image.
    // nGrow    The number of images by which the image list can grow when the system needs to make room for new images.
    // crMask   The color used to generate a mask. Each pixel of this color in the specified bitmap is changed to black,
    //          and the corresponding bit in the mask is set to 1. If this parameter is the CLR_NONE value, no mask is generated.
    inline BOOL CImageList::Create(UINT bitmapID, int cx, int grow, COLORREF mask)
    {
        assert(m_pData);
        assert(NULL == m_pData->hImageList);

        LPCTSTR lpszBitmapID = MAKEINTRESOURCE (bitmapID);
        return Create(lpszBitmapID, cx, grow, mask);
    }


    // Creates a new image list.
    //
    // cx       The width of each image.
    // nGrow    The number of images by which the image list can grow when the system needs to make room for new images.
    // crMask   The color used to generate a mask. Each pixel of this color in the specified bitmap is changed to black,
    //          and the corresponding bit in the mask is set to 1. If this parameter is the CLR_NONE value, no mask is generated.
    inline BOOL CImageList::Create(LPCTSTR pResourceName, int cx, int grow, COLORREF mask)
    {
        assert(m_pData);
        assert(NULL == m_pData->hImageList);
        HIMAGELIST himlNew = ImageList_LoadBitmap(GetApp().GetInstanceHandle(), pResourceName, cx, grow, mask);

        if (himlNew)
        {
            Attach(himlNew);
            m_pData->isManagedHiml = TRUE;
        }

        return ( himlNew!= 0 );
    }


    // Creates a duplicate ImageList
    inline BOOL CImageList::Create(HIMAGELIST hImages)
    {
        assert(m_pData);
        HIMAGELIST himlCopy = ImageList_Duplicate(hImages);

        if (himlCopy)
        {
            Attach(himlCopy);
            m_pData->isManagedHiml = TRUE;
        }

        return ( himlCopy!= 0 );
    }


    // Destroys an image list.
    inline void CImageList::DeleteImageList()
    {
        assert(m_pData);
        if (m_pData->hImageList != 0)
        {
            ImageList_Destroy(Detach());
        }
    }


    // Detaches the HIMAGELIST from all CImageList objects.
    inline HIMAGELIST CImageList::Detach()
    {
        assert(m_pData);
        HIMAGELIST hImageList = m_pData->hImageList;
        RemoveFromMap();
        m_pData->hImageList = 0;

        if (m_pData->count > 0)
        {
            if (InterlockedDecrement(&m_pData->count) == 0)
            {
                delete m_pData;
            }
        }

        m_pData = new CIml_Data;

        return hImageList;
    }


    // Displays the drag image at the specified position within the window.
    inline BOOL CImageList::DragEnter(HWND hLock, CPoint point) const
    {
        assert(m_pData->hImageList);
        return ImageList_DragEnter(hLock, point.x, point.y);
    }


    // Unlocks the specified window and hides the drag image, allowing the window to be updated.
    inline BOOL CImageList::DragLeave(HWND hLock) const
    {
        return ImageList_DragLeave(hLock);
    }


    // Moves the image that is being dragged during a drag-and-drop operation.
    // This function is typically called in response to a WM_MOUSEMOVE message.
    inline BOOL CImageList::DragMove(CPoint point) const
    {
        return ImageList_DragMove(point.x, point.y);
    }


    // Shows or hides the drag image during a drag operation, without locking the window.
    inline BOOL CImageList::DragShowNolock(BOOL show) const
    {
        return ImageList_DragShowNolock(show);
    }


    // Draws an image list item in the specified device context.
    inline BOOL CImageList::Draw(HDC hdc, int image, POINT point, UINT style) const
    {
        assert(m_pData->hImageList);
        return ImageList_Draw(m_pData->hImageList, image, hdc , point.x, point.y, style);
    }


    // Draws an image list item in the specified device context. The function uses the specified drawing style
    // and blends the image with the specified color.
    // nStyle can have the following values:
    // ILD_BLEND25, ILD_FOCUS
    //   Draws the image, blending 25 percent with the blend color specified by rgbFg.
    //   This value has no effect if the image list does not contain a mask.
    // ILD_BLEND50, ILD_SELECTED, ILD_BLEND
    //   Draws the image, blending 50 percent with the blend color specified by rgbFg.
    //   This value has no effect if the image list does not contain a mask.
    // ILD_MASK     Draws the mask.
    // ILD_NORMAL   Draws the image using the background color for the image list. If the
    //              background color is the CLR_NONE value, the image is drawn transparently using the mask.
    // ILD_TRANSPARENT  Draws the image transparently using the mask, regardless of the background color.
    //                  This value has no effect if the image list does not contain a mask.
    // ILD_IMAGE    If the overlay does not require a mask to be drawn set the ILD_IMAGE flag.
    //              This causes ImageList_DrawEx to just draw the image, ignoring the mask.
    inline BOOL CImageList::DrawEx(HDC hdc, int image, POINT pt, SIZE sz, COLORREF bckgnd, COLORREF frgnd, UINT style) const
    {
        assert(m_pData->hImageList);
        return ImageList_DrawEx(m_pData->hImageList, image, hdc, pt.x, pt.y, sz.cx, sz.cy, bckgnd, frgnd, style);
    }


    // Draws an image list image based on an IMAGELISTDRAWPARAMS structure.
    inline BOOL CImageList::DrawIndirect(IMAGELISTDRAWPARAMS* pimldp) const
    {
        assert(m_pData->hImageList);
        return ImageList_DrawIndirect(pimldp);
    }

	// Ends a drag operation.
	inline void CImageList::EndDrag() const
	{
		assert(m_pData->hImageList);
		ImageList_EndDrag();
	}

	
	// Calls the GetIcon function to create an icon or cursor based on an image and mask in an image list
	inline HICON CImageList::ExtractIcon(int index) const
	{
		assert(m_pData->hImageList);
		return ImageList_ExtractIcon(GetApp().GetResourceHandle(), *this, index);
	}


	// Retrieves the current background color for an image list.
	inline COLORREF CImageList::GetBkColor() const
	{
		assert(m_pData->hImageList);
		return ImageList_GetBkColor(*this);
	}


	// Retrieves the temporary image list that is used for the drag image.
	// pHotspot - Pointer to a POINT structure that receives the offset of the
	// drag image relative to the drag position.Can be NULL.

	inline HIMAGELIST CImageList::GetDragImage(POINT* pPoint, POINT* pHotspot) const
	{
		assert(m_pData->hImageList);
		return ImageList_GetDragImage(pPoint, pHotspot);
	}


	// Returns the HIMAGELIST assigned to this CImageList.
	inline HIMAGELIST CImageList::GetHandle() const
	{
		return m_pData->hImageList;
	}


    // Creates an icon from an image and mask in an image list.
    inline HICON CImageList::GetIcon(int iImage, UINT nFlags) const
    {
        assert(m_pData->hImageList);
        return ImageList_GetIcon(m_pData->hImageList, iImage, nFlags);
    }


    // Retrieves the dimensions of images in an image list.
    inline CSize CImageList::GetIconSize() const
    {
        assert(m_pData->hImageList);
        int cx = 0;
        int cy = 0;
        ImageList_GetIconSize(m_pData->hImageList, &cx, &cy);
        return CSize(cx, cy);
    }


    // Retrieves the number of images in an image list.
    inline int CImageList::GetImageCount() const
    {
        assert(m_pData->hImageList);
        return ImageList_GetImageCount(m_pData->hImageList);
    }


    // Retrieves information about an image.
    inline BOOL CImageList::GetImageInfo(int image, IMAGEINFO* pImageInfo) const
    {
        assert(m_pData->hImageList);
		assert(pImageInfo);
        return ImageList_GetImageInfo(m_pData->hImageList, image, pImageInfo);
    }


	// Reads the image list from a stream.
	inline void CImageList::Read(LPSTREAM pStream)
	{
		assert(m_pData->hImageList);
		HIMAGELIST himl =  ImageList_Read(pStream);

		if (himl)
		{
			m_pData->isManagedHiml = TRUE;
			Attach(himl);
		}
	}


    // Removes an image from an image list.
    inline BOOL CImageList::Remove(int image) const
    {
        assert(m_pData->hImageList);
        return ImageList_Remove(m_pData->hImageList, image);
    }


    // Replaces an image in an image list with a new image.
    inline BOOL CImageList::Replace(int image, HBITMAP hImage, HBITMAP hMask) const
    {
        assert (m_pData->hImageList);
        return ImageList_Replace(m_pData->hImageList, image, hImage, hMask);
    }


    // Replaces an image with an icon or cursor.
    inline int CImageList::Replace(int image, HICON hIcon) const
    {
        assert (m_pData->hImageList);
        return ImageList_ReplaceIcon(m_pData->hImageList, image, hIcon);
    }


	// Writes the image list to a stream.
	inline BOOL CImageList::Write(LPSTREAM pStream) const
	{
		assert(m_pData->hImageList);
		return ImageList_Write(*this, pStream);
	}


	// Sets the background color for an image list.This function only works if you add an icon
	// or use ImageList_AddMasked with a black and white bitmap. Without a mask, the entire image
	// is drawn; hence the background color is not visible.
	inline COLORREF CImageList::SetBkColor(COLORREF bckgnd) const
	{
		assert(m_pData->hImageList);
		return ImageList_SetBkColor(*this, bckgnd);
	}

	
	// Creates a new drag image by combining the specified image (typically a mouse cursor image)
	// with the current drag image.
	inline BOOL CImageList::SetDragCursorImage(int drag, int dxHotspot, int dyHotspot) const
	{
		assert(m_pData->hImageList);
		return ImageList_SetDragCursorImage(*this, drag, dxHotspot, dyHotspot);
	}


    // Retrieves the image list's handle.
    inline CImageList::operator HIMAGELIST () const

    {
        return m_pData->hImageList;
    }


    inline void CImageList::Release()
    {
        assert(m_pData);

        if (InterlockedDecrement(&m_pData->count) == 0)
        {
            if (m_pData->hImageList != NULL)
            {
                if (m_pData->isManagedHiml)
                {
                    ImageList_Destroy(m_pData->hImageList);
                }

                RemoveFromMap();
            }

            delete m_pData;
            m_pData = 0;
        }
    }


#ifndef _WIN32_WCE

    // Creates a gray scale image list from the specified color image list.
    inline BOOL CImageList::CreateDisabledImageList(HIMAGELIST hNormal)
    {
        assert(NULL == m_pData->hImageList);
        assert(hNormal);

        int count = ImageList_GetImageCount(hNormal);
        if (count > 0)
        {
            int cx, cy;
            ImageList_GetIconSize(hNormal, &cx, &cy);

            // Create the disabled ImageList
            Create(cx, cy, ILC_COLOR24 | ILC_MASK, count, 0);

            // Process each image in the ImageList
            for (int i = 0 ; i < count; ++i)
            {
                CClientDC DesktopDC(NULL);
                CMemDC memDC(NULL);
				memDC.CreateCompatibleBitmap(DesktopDC, cx, cx);
                CRect rc;
                rc.SetRect(0, 0, cx, cx);

                // Set the mask color to grey for the new ImageList
                COLORREF mask = RGB(200, 200, 200);
                if (GetDeviceCaps(DesktopDC, BITSPIXEL) < 24)
                {
                    HPALETTE hPal = reinterpret_cast<HPALETTE>(GetCurrentObject(DesktopDC, OBJ_PAL));
                    UINT index = GetNearestPaletteIndex(hPal, mask);
                    if (index != CLR_INVALID)
						mask = PALETTEINDEX(index);
                }
				memDC.SolidFill(mask, rc);

                // Draw the image on the memory DC
                ImageList_Draw(hNormal, i, memDC, 0, 0, ILD_NORMAL);

                // Detach the bitmap so we can use it.
                CBitmap bitmap = memDC.DetachBitmap();
                bitmap.ConvertToDisabled(mask);
                Add(bitmap, mask);
            }
        }

        return ( m_pData->hImageList != 0 );
    }

#endif

}   // namespace Win32xx

#endif  // _WIN32XX_IMAGELIST_H_

