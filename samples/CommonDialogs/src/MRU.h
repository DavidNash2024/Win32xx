/* (03-Mar-2015) [Tab/Indent: 8/8][Line/Box: 80/74]                    (MRU.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: CMRU class declaration.  This class is
	used to load, display, edit, and  save the Most Recently Used (MRU)
	list, commonly used to contain the files used by an application.
	This class has been adapted from the Win32++ Windows interface classes,
	Copyright (c) 2005-2016 David Nash, under permissions granted therein.
	It offers an alternate to the MRU operations found in the CFrame class
	of Win32++ for those not wishing to use the system's registry, but
	prefer a separate application parameter file.

        Caveats: The copyright displayed above extends only to the author's
	original contributions to the subject class, and  to the alterations,
	additions, deletions, and  other treatments of materials that may have
	been extracted from cited sources. Unaltered portions of those
	materials retain their original copyright status. The author hereby
	grants permission to any person obtaining a copy of this treatment
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

	Declaration of the CMRU class

*******************************************************************************/

#ifndef GPP_MRU_H_FILE
#define GPP_MRU_H_FILE


/*============================================================================*/
        class
CMRU : public CObject							/*

	Declaration of the Most-Recently-Used (MRU) item list handler class
	that can be archived via Serialize().
**----------------------------------------------------------------------------*/
{
	  // Public Declarations
        public:
		  // construction/destruction
 		CMRU();
		virtual ~CMRU(){}

		  // method members
		virtual CString AccessMRUEntry(UINT nIndex);
		virtual void 	AddMRUEntry(LPCTSTR szMRUEntry);
		virtual void 	ConnectMRU(CFrame*, UINT nMaxMRU = 0);
		virtual void    EmptyMRUList();
		virtual CString GetMRUEntry(UINT nIndex);
		virtual UINT    GetSize() {return m_vMRUEntries.size();}
		virtual void 	RemoveMRUEntry(LPCTSTR szMRUEntry);
		virtual void 	UpdateMRUMenu();
		virtual void	ValidateMRU(void);

	  // Protected Declarations
	protected:
		virtual void    Serialize(CArchive &ar);

	  // Private Declarations
	private:
		std::vector<CString> m_vMRUEntries; // MRU array entries
		size_t		m_nMaxMRU;	// maximum MRU entries, this app
		CFrame         *m_theFrame;     // frame containing main menu

		static const size_t  m_nMaximumMRUSlots;  // slots upper limit
		static const CString m_emptyMRUListLabel; // MRU locator label
};
/*----------------------------------------------------------------------------*/
#endif // GPP_MRU_H_FILE


