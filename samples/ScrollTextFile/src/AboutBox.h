/* (10-08-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (AboutBox.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2017, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

	Contents Description: Declaration of the basic AboutBox dialog for
	a typical application using the Win32++ Windows interface classes,
	Copyright c) 2005-2017 David Nash, under permissions granted therein.

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
                by the 1997-1999 JPL NPP project for C++ programming.

*******************************************************************************/

#ifndef ABOUT_BOX_H_DEFINED
#define ABOUT_BOX_H_DEFINED

/*============================================================================*/
        class
AboutBox        : public CDialog                                        /*

*-----------------------------------------------------------------------------*/
{
	  // public declarations
	public:
		AboutBox();
		~AboutBox();
		
		void    SetAboutBoxInfo(LPCTSTR info) { m_sAboutBoxInfo = info;}

	  // protected declarations
	protected:
		virtual BOOL DialogProc(UINT, WPARAM, LPARAM);

	  // private declarations
	private:
		CString m_sAboutBoxInfo; // about the program, credits, etc.
};
/*----------------------------------------------------------------------------*/
#endif // ABOUT_BOX_H_DEFINED

