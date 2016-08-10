/* (06-Aug-16) [Tab/Indent: 8/8][Line/Box: 80/74]               (MRUFrame.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the CMRUFrame class for
	applications using the Win32++ Windows interface classes, Copyright
	(c) 2005-2016 David Nash, under permissions granted therein.

	This class derives from the CFrame class and extends the scope and
	functionality of the Most Recently Used (MRU) list.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and to the alterations,
	additions, deletions, and other treatments of materials that may have
	been extracted from the cited sources.  Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
	of the subject class and any associated documentation composed by
	the author, to utilize this material, free of charge and without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, as well as that of David Nash
		and Win32++, together with the respective permission
		conditions shall be included in all copies or substantial
		portions of this material so copied, modified, merged,
		published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.
	Citation of the author's work should be included in such usages.

	Special Conventions:

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of the CMRUFrame class

*******************************************************************************/
#ifndef CMRUFRAME_H
#define CMRUFRAME_H

/*============================================================================*/
	class
CMRUFrame : public CFrame                                               /*

	Extend the CFrame class to add serialization of the MRU list and to
	allow public access to, and manipulation of, MRU strings. MRU strings
	that are not valid file paths are discarded by Validate().
*-----------------------------------------------------------------------------*/
{
	public:
		CMRUFrame();
		virtual ~CMRUFrame();

		  // method members
		virtual CString AccessMRUEntry(UINT nIndex);
		virtual void	AddMRUEntry(LPCTSTR szMRUEntry)
				    { CFrame::AddMRUEntry(szMRUEntry);}
		virtual void    EmptyMRUList();
		virtual UINT    GetMRUSize() { return GetMRUEntries().size();}
		virtual void	RemoveMRUEntry(LPCTSTR szMRUEntry)
				    {CFrame::RemoveMRUEntry(szMRUEntry);}
		virtual void	ValidateMRU();

	  // Protected Declarations
	protected:
		virtual void 	ConnectMRU(UINT nMaxMRU = 0);
		virtual void    Serialize(CArchive &ar);

	  // Private Declarations
	private:
		size_t		m_nMaxMRU;	// maximum MRU entries, this app
		UINT    	m_nSchema;      // serialized data schema

};
/*----------------------------------------------------------------------------*/
#endif  // CMRUFRAME_H
