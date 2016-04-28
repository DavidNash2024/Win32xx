/* (15-Sep-2015) [Tab/Indent: 8/8][Line/Box: 80/74]            (ContextHelp.h) *
********************************************************************************
|                                                                              |
|                     Copyright (c) 2016, Robert C. Tausworthe                 |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
*==============================================================================*

	Contents: Generic ContextHelp class declaration file. This class
	provides the means for accessing help topics contained in compiled HTML
	(.chm) documents to applications that use the Win32++ WINAPI interface,
	Copyright (c) 2005-2016 David Nash, under permissions granted therein.
	Consult the ContextHelp.cpp file for further usage information.

        Caveats: The copyright displayed above extends only to the author's
        treatment of material that may have been extracted from cited sources.
	Unaltered portions of those materials retain their original copyright
	status. The author's additions and modifications may be used or altered
	by others for non-commercial use only without permission.  However,
	citation of the author's work should be included in such usages.

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
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	ContextHelp Class Declaration

*******************************************************************************/

#ifndef CONTEXTHELP_H
#define CONTEXTHELP_H

#include <vector>

  // the help message structure definition
struct help_message {UINT nID; CString topic;};

/*============================================================================*/
	class
ContextHelp    : public CDialog						/*

	A class to provide access to documentation of features contained in a
	.chm file based on selection of contextual items found within an app.
*-----------------------------------------------------------------------------*/
{
	public:
		ContextHelp();
		virtual ~ContextHelp(){}
		
		virtual BOOL    AddHelpTopic(UINT nID, const CString& topic);
		virtual void	DefaultTopics(void);
		virtual BOOL	InitiateContextHelp(void);
		virtual BOOL    IsActive() { return m_bHelpActive;}
		virtual BOOL    LookupAndDisplay(const CString& topic);
		virtual BOOL	OnHelp(const CString &topic);
		virtual BOOL    OnHelp(WPARAM wParam, UINT maxMRU = 0);
		virtual BOOL    OnHelpID(UINT nID, UINT maxMRU = 0);
		virtual  void	SetCredits(const CString &s){ m_sCredits = s;}
		virtual void    SetHelpFile(const CString& s)
				    { m_sContextHelpFile = s;}

	protected:
		virtual BOOL	OnHelpAbout(void);
		virtual BOOL 	OnInitDialog();

	private:
		  // private data
		CString 	m_sCredits;
		BOOL    	m_bHelpActive;
		CString         m_sContextHelpFile;  // name of the help file
		HWND		m_hHadFocus;    // focus window on context help
		std::vector<help_message> m_help_table; // iD vs topic array

};
/*----------------------------------------------------------------------------*/
#endif // CONTEXTHELP_H
