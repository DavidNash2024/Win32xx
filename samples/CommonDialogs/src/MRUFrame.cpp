/* (06-Aug-16) [Tab/Indent: 8/8][Line/Box: 80/74]               (MRUFrame.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CMRUFrame class for
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

	Implementation of the CMRUFrame class

*******************************************************************************/

#include "stdafx.h"
#include "MRUFrame.h"

static const UINT current_MRU_schema = 1;

/*============================================================================*/
	CMRUFrame::
CMRUFrame()                                                             /*

	Contstruct the MRU frame.
*-----------------------------------------------------------------------------*/
{
    	m_nSchema = -1;
	m_nMaxMRU  = 0;
	EmptyMRUList();
}

/*============================================================================*/
	CMRUFrame::
~CMRUFrame()                                                             /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CString CMRUFrame::
AccessMRUEntry(UINT nIndex)                          			/*

	Return a MRU CString entry given its nIndex. If the nIndex is not
	within the MRU list bounds, return an empty string.
*-----------------------------------------------------------------------------*/
{
	CString s; // empty on construction
	if (nIndex < GetMRUSize())
		s = GetMRUEntries()[nIndex];

	return s;
}

/*============================================================================*/
	void CMRUFrame::
ConnectMRU(UINT nMaxMRU)						/*

	Register the maximum number of elements nMaxMRU of the MRU list to be
	shown.  If there is no current registry key name, enter an obligatory
	dummy in order to set the nMaxMRU value into CFrame.
*-----------------------------------------------------------------------------*/
{
	static const CString keyname = _T("CMRUFrame");
	CString currentkey = GetRegistryKeyName();
	if (currentkey != keyname)
		LoadRegistrySettings(keyname);

	m_nMaxMRU  = MIN(nMaxMRU, 16);
	LoadRegistryMRUSettings(m_nMaxMRU);
	  // throw away any MRU entries from the registry
	EmptyMRUList();
}

/*============================================================================*/
	void CMRUFrame::
EmptyMRUList()                                                          /*

	Remove all entries from the MRU list and  display the empty list
	on the MRU menu.
*-----------------------------------------------------------------------------*/
{
	  // use a separate list to get the entries
	const std::vector<CString>& MRUEntries = GetMRUEntries();
	  // then remove items from the MRU list one by one
	std::vector<CString>::const_iterator it;
	for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
		RemoveMRUEntry(*it);
	  // sync the on-screen MRU list
	UpdateMRUMenu();
}

/*============================================================================*/
	void CMRUFrame::
Serialize(CArchive &ar)                                                 /*

	Serialize or deserialize the MRU list to from the archive ar, depending
	on the sense of IsStoring(). This class has its own m_nSchema.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
        	ar << current_MRU_schema;
		  // record the number of entries to write
		size_t i, nMRU = GetMRUSize();
		ar << nMRU;
		  // save this many entries (don't use copied list)
		for (i = 0; i < nMRU; ++i)
		{
			ar << GetMRUEntries()[i];
		}
	}
        else    // recovering
        {
            	ar >> m_nSchema;
            	if (m_nSchema != current_MRU_schema)
			return;

		  // read MRU values from archive
		EmptyMRUList();
		  // use dummy vector in case an exception occurs
		std::vector<CString> vMRUEntries;
		  // extract all the MRU entries that were archived to preserve
		  // the archive for subsequent use
		size_t i, nMRU;
		ar >> nMRU; // the number of entries to read in
		CString s;
		for (i = 0; i < nMRU; ++i)
		{
			ar >> s;
			if (i < m_nMaxMRU)  // keep only those within the limit
				vMRUEntries.push_back(s);
		}
		  // all successfully read in, so store them LIFO order into
		  // the MRU list for proper display
		for (i = 0; i < nMRU; ++i)
		{
			s = vMRUEntries[nMRU - 1 - i];
			AddMRUEntry(s);
		}
		  // sync the on-screen MRU list
		UpdateMRUMenu();
	}
	  // handle any serialization of the base class
	CFrame::Serialize(ar);
}

/*============================================================================*/
	void CMRUFrame::
ValidateMRU()                                                 		/*
	Validate the that the MRU list entries, if there are any, correspond
	to actual file paths. Remove any that do not.
*-----------------------------------------------------------------------------*/
{
	  // get a copy of the MRU list entries
	const std::vector<CString>& MRUEntries = GetMRUEntries();
	  // check them one by one as a valid file path
	std::vector<CString>::const_iterator it;
	for (it = MRUEntries.begin(); it != MRUEntries.end(); ++it)
	{
		  // check whether the current entry exists
		CString s = *it;
		if (_taccess(s.c_str(), 4) != 0)
		{
#ifdef _DEBUG
			::MessageBox(NULL, s.c_str(), _T("Removing invalid path."),
			    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
#endif
			RemoveMRUEntry(s);
		}
	}
	  // sync the on-screen MRU list
	UpdateMRUMenu();
}

