/* (10-08-2014) [Tab/Indent: 8/8][Line/Box: 80/74]            (CColorChoice.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of a generic CColorChoice popup
	dialog class for applications using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein. Information on the use of this class appears in the
	CColorChoice.cpp file.

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

	Special Conventions:

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.
		
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.		

********************************************************************************

	Declaration of the CColorChoice class

*******************************************************************************/

#ifndef CCOLORCHOICE_H
#define CCOLORCHOICE_H

#include "ColorDefs.h"
#include "CListBoxDlg.h"

/*******************************************************************************

	Local struct and enum definitions                               */

  // usage-color pairs forming the m_ColorTable array
struct ctl_color {CString  usage;  COLORREF color;};

/******************************************************************************/
	class
CColorChoice   : public CColorDialog					/*

*-----------------------------------------------------------------------------*/
{
	public:
		CColorChoice();
		virtual ~CColorChoice(){}
		
		virtual BOOL	AddColorChoice(const CString&, COLORREF);
			void    ClearColorTable(){m_ColorTable.clear();}
			UINT    DeleteEntry(UINT index);
		virtual INT_PTR DoModal(HWND hWndOwner = 0);
			CBrush&	GetBrush(UINT index);
			COLORREF GetColor(UINT idx)
				     { return m_ColorTable[idx].color;}
			UINT	GetTableSize(){ return m_ColorTable.size();}
			CString& GetUsage(UINT idx)
				     { return  m_ColorTable[idx].usage;}
		virtual void    SetColorTable(UINT idx, COLORREF rgb)
				     { m_ColorTable[idx].color = rgb;}
		virtual void    SetUsage(UINT idx, const CString& s)
				     { m_ColorTable[idx].usage = s;}
		virtual CListBoxDlg& GetListBoxDlg(){return m_LBDlg;}
			void	SetBoxTitle(LPCTSTR title) {m_sBoxTitle = title;}

	protected:

		virtual void 	OnHelpButton();	
		virtual BOOL 	OnInitDialog();	
		virtual void    SetWindowTitle() const
				    {SetWindowText(m_sBoxTitle);}	
		virtual	void	Serialize(CArchive &ar);

	private:
		void InitCustomColors();		

		  // private data
		CBrush          m_br;		// must have object persistence
		CListBoxDlg     m_LBDlg;	// the list box dialog
		CString 	m_sBoxTitle;	// the color dialog box title
		std::vector<ctl_color> m_ColorTable;   // usage-color pairs
};
/*----------------------------------------------------------------------------*/
#endif  // CCOLORCHOICE_H
