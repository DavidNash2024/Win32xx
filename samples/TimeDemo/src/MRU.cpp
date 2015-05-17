/* (03-Mar-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                  (MRU.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2015, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                                                                              |
===============================================================================*

	Contents Description: CMRU class implementation.  This class is
	used to load, display, edit, and  save the Most Recently Used (MRU)
	list, commonly used to contain the files used by an application.
	This class has been adapted from the Win32++ Windows interface classes,
	Copyright (c) 2005-2015 David Nash, under permissions granted therein.
	It offers an alternate to the MRU operations found in the CFrame class
	of Win32++ for those not wishing to use the system's registry, but
	prefer a separate application parameter file.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from cited sources. Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any persion obtaining a copy of this treatment
	of the subject class and  any associated documentation composed by
	the author, to utilize this material, free of charge and  without
	restriction or limitation, subject to the following conditions:

		The above copyright notice, together with the respective
		permission conditions shall be included in all copies  or
		substantial portions of this material so copied, modified,
		merged, published, distributed, or otherwise held by others.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.
	In no event shall the authors or copyright holders be liable for any
	claim, damages, or other liability, whether in an action of contract,
	tort or otherwise, arising from, out of, or in connection with, these
	materials, the use thereof, or any other other dealings therewith.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CMRU class

*******************************************************************************/

#include "MRU.h"
#include <io.h>

/*******************************************************************************

	Class static constant initialization				*/

  // maximum possible number of MRU slots.  The actual number an app will have
  // (less than or equal to this) is set in the ConnectMRU() method.
const	size_t  CMRU::m_nMaximumMRUSlots = 16;
  // The empty MRU list label: each app using this class should insert this
  // label in the leftmost submenu at the point the MRU list is to appear
  // and having the numeric identifier IDW_FILE_MRU_FILE1.
const   CString CMRU::m_emptyMRUListLabel = _T("Recent Files");

/*******************************************************************************

	CMRU Class Implementation

*=============================================================================*/
	CMRU::
CMRU()             							/*

	Construct an empty MRU object.
*-----------------------------------------------------------------------------*/
{
	m_vMRUEntries.clear();
	m_nMaxMRU  = 0;
	m_theFrame = NULL;
}

/*============================================================================*/
	CMRU::
~CMRU()             							/*

	Destroy the MRU object.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CString CMRU::
AccessMRUEntry(UINT nIndex)                          			/*

	Return a MRU CString entry given its nIndex. If the nIndex is not
	within the MRU list bounds, return an empty string.
*-----------------------------------------------------------------------------*/
{
	CString s = _T("");
	if (nIndex < m_vMRUEntries.size())
		s = m_vMRUEntries[nIndex];

	return s;
}

/*============================================================================*/
	void CMRU::
AddMRUEntry(LPCTSTR szMRUEntry)                                         /*

	Add the szMRUEntry string to the Most Recently Used (MRU) menu at the
	top of the menu.  Eliminate list members if they exceed the specified
	bound on number of entries.
*-----------------------------------------------------------------------------*/
{
	  // Erase possible duplicate entries from vector
	RemoveMRUEntry(szMRUEntry);
	  // Insert the entry at the beginning of the vector
	m_vMRUEntries.insert(m_vMRUEntries.begin(), szMRUEntry);
	  // Delete excessive MRU entries
	if (m_vMRUEntries.size() > m_nMaxMRU)
		m_vMRUEntries.erase(m_vMRUEntries.begin() + m_nMaxMRU,
		    m_vMRUEntries.end());
	  // display the changed MRU menu
	UpdateMRUMenu();
}

/*============================================================================*/
	void CMRU::
ConnectMRU(CFrame *frame, UINT nMaxMRU /*= 0*/)				/*

	Register the frame and  the maximum number of elements nMaxMRU of the
	MRU list to be shown. This method may be invoked at any time after
	the frame has been constructed and  before any of the other operations
	of this object are invoked.  The constructor of the frame object is
	a good place for this invocation.
*-----------------------------------------------------------------------------*/
{
	m_theFrame = frame;
	m_nMaxMRU  = MIN(nMaxMRU, m_nMaximumMRUSlots);
	m_vMRUEntries.clear();
}

/*============================================================================*/
	void CMRU::
EmptyMRUList()             						/*

	Remove all entries from the MRU list and  display the default list
	on the MRU menu.
*-----------------------------------------------------------------------------*/
{
	m_vMRUEntries.clear();
	UpdateMRUMenu();
}

/*============================================================================*/
	CString CMRU::
GetMRUEntry(UINT nIndex)                          			/*

	Return a MRU CString entry given its nIndex, and  move that entry to
	the top of the list, provided the nIndex is within the MRU list's
	bounds. If not, return an empty string and  do not insert it in
	the list.
*-----------------------------------------------------------------------------*/
{
	CString s = AccessMRUEntry(nIndex);
	if (!s.IsEmpty())
		AddMRUEntry(s);
	return s;
}

/*============================================================================*/
	void CMRU::
RemoveMRUEntry(LPCTSTR szMRUEntry)                          		/*

	Remove the szMRUEntry entry from the MRU list if it is in the list.
*-----------------------------------------------------------------------------*/
{
	std::vector<CString>::iterator it;
	for (it = m_vMRUEntries.begin(); it != m_vMRUEntries.end(); ++it)
	{
		if ((*it) == szMRUEntry)
		{
			m_vMRUEntries.erase(it);
			break;
		}
	}
	UpdateMRUMenu();
}

/*============================================================================*/
	void CMRU::
Serialize(CArchive &ar)                                               	/*

	Serialize or deserialize the MRU list to from the archive ar, depending
	on the sense of IsStoring().   If the archive has encountered errors at
	entry, do not execute. Also, if invoked in the read mode but the archive
	is not opened for reading, do not execute. If invoked in the write mode,
	but the archive is not open, throw an exception.
*-----------------------------------------------------------------------------*/
{
	  // perform loading or storing
        if (ar.IsStoring())
        {
                  // each item serialized is written to the archive
                  // file as a char stream of the proper length,
                  // preceded by that length. In some cases, other forms os
		  // data are saved, from which the primary items are then
                  // reconstructed.

		  // record the number of entries to write
		size_t i, nMRU = m_vMRUEntries.size();
		ar << nMRU;
		  // save this many entries
		for (i = 0; i < nMRU; ++i)
		{
			ar << m_vMRUEntries[i];
		}

	}
        else    // recovering
        {
                  // each item deserialized from the archive is
                  // retrieved by first reading its length and  then
                  // loading in that number of bytes into the data
                  // item saved in the archive, as above. Some items require
                  // additional converstion procedures.
		m_vMRUEntries.clear();
		std::vector<CString> vMRUEntries;
		  // load all archived MRU entries, used or not,
		  // to preserve archive for other users
		size_t i, nMRU;
		ar >> nMRU; // the number of them
		CString s;
		for (i = 0; i < nMRU; ++i)
		{
			ar >> s;
			if (i < m_nMaxMRU)  // record only those within bounds
				vMRUEntries.push_back(s);
		}

		  // successfully read in, so store them
		m_vMRUEntries = vMRUEntries;
		UpdateMRUMenu();
	}
}

/*============================================================================*/
	void CMRU::
UpdateMRUMenu()                          				/*

	Update the MRU menu with the current Most Recently Used (MRU) list.
	Display the MRU entries in the leftmost submenu of the CFrame's main
	menu, at the IDW_FILE_MRU_FILE1 location of that submenu, in the same
	order they occur in the current MRU CString vector.
*-----------------------------------------------------------------------------*/
{
	  // find in the leftmost submenu (i.e., the one with index 0)
	CMenu fileMenu = m_theFrame->GetFrameMenu().GetSubMenu(0);
	  // compute the index of the last entry in the MRU list
	int iLast = (int)MIN(m_vMRUEntries.size(), m_nMaxMRU) -  1;
	  // if there is no leftmost submenu, or if there are no entries to
	  // post, or if we cannot modify the first entry to indicate an empty
	  // MRU list, we cannot proceed
	if (!fileMenu.GetHandle())
	{	  // just refresh the frame menu bar and  leave
		m_theFrame->DrawMenuBar();
		return;
	}
	  // insert the empty MRU list label in the top slot
	fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
	    IDW_FILE_MRU_FILE1, m_emptyMRUListLabel.c_str());

	  // remove all the other MRU Menu entries
	for (int i = IDW_FILE_MRU_FILE2; i <= IDW_FILE_MRU_FILE1 +
	    (int)m_nMaxMRU; ++i)
		fileMenu.DeleteMenu(i, MF_BYCOMMAND);
	  // if the list is not empty, there's more to do
	if (iLast >= 0)
	{
		  // create the MRU "show" list, which contains only strings
		  // of limited length, chars removed at the midpoint, as needed
		int 	maxlen = MAX_MENU_STRING - 10,
			mid    = maxlen / 2;
		CString strMRUShow[m_nMaximumMRUSlots];
		for (int i = 0; i <= iLast; i++)
		{
			CString s = m_vMRUEntries[i];
			if (s.GetLength() > maxlen)
			{
				  // eliminate middle if too long
				s.Delete(mid, s.GetLength() - maxlen);
				s.Insert(mid, _T("..."));
			}
			// Prefix with its number
			CString v;
			v.Format(_T("%d "), i + 1);
			strMRUShow[i] = v + s;
		}

		  // display the MRU items: start by replacing the first item
		  // in the the list with the last MRU item
		fileMenu.ModifyMenu(IDW_FILE_MRU_FILE1, MF_BYCOMMAND,
		    IDW_FILE_MRU_FILE1 + iLast, strMRUShow[iLast]);
		  // now insert the remaining items in reverse order, starting
		  // at the next-to-iLast entry and  pushing all the others
		  // down in the menu (entries thus end up in the correct order)
		for (int j = iLast - 1 ; j >= 0; iLast--, j--)
			fileMenu.InsertMenu(IDW_FILE_MRU_FILE1 + iLast,
			    MF_BYCOMMAND, IDW_FILE_MRU_FILE1 + j,
			    strMRUShow[j]);
	}
	  // refresh the frame menu bar and  leave
	m_theFrame->DrawMenuBar();
}

/*============================================================================*/
	void CMRU::
ValidateMRU()								/*

	Validate the that the MRU list entries, if there are any, correspond
	to actual file paths. Remove any that do not.
*-----------------------------------------------------------------------------*/
{
	  // search the MRU list in reverse so as not to cause reshuffling
	std::vector<CString>::reverse_iterator it;
	for (it = m_vMRUEntries.rbegin(); it != m_vMRUEntries.rend(); ++it)
	{
		  // check whether the current entry exists, or is gone
		CString s = (*it);
#if defined(UNICODE) || defined(_UNICODE)
		bool gone = (_waccess(s.c_str(), 4) != 0);
#else
		bool gone = (_access(s.c_str(), 4) != 0);
#endif
		  // if it is not there, remove the item from the list
		if (gone)
		{
	::MessageBox(NULL, s.c_str(), _T("removing"),
	    MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
			  // reverse iterators must be altered to the
			  // proper forward iterator
			m_vMRUEntries.erase((it - 1).base());
		}
	}
	UpdateMRUMenu();
}

