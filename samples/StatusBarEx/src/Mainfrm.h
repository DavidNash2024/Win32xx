/* [Tab/Indent: 8/8][Line/Box: 80/74]	                          (MainFrm.h) *
********************************************************************************

	Contents Description: Declaration of the CMainFrame class for a sample
	program demonstrating the use of the status bar using the Win32++ 
	Windows framework, Copyright (c) 2005-2017 David Nash.

	This particular program was adapted from the StatusBar sample progam
	in the Win32++ distribution by Robert C. Tausworthe to illustrate the 
	definition of the status bar makeup by way of a data structure, in a
	similar fashion as that appearing in the Microsoft Foundation Classes 
	(MFC), and to override the Win32++ normal statusbar definition of four 
	parts to any number suitable for an application.

	The code changes to the StatusBar sample program only occur in the 
	CMainFrame and MyStatusBar classes. However, the entire program has
	been reformatted for better readability, per the Programming Notes,
	below.

	These materials are provided "as is", without warranty of any kind,
	express or implied, including but not limited to: warranties of
	merchantability, fitness for a particular purpose, and non-infringement.

	Programming Notes:
                The programming conventions used here roughly follow those 
		established by the 1997-1999 Jet Propulsion Laboratory Deep 
		Space Network Planning and Preparation Subsystem project for 
		C++ programming.

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "CustomStatusBar.h"

/*============================================================================*/
	class 
CMainFrame : public CFrame						/*

	Declaration of the CMainFrame class.
*-----------------------------------------------------------------------------*/
{
	public:
		CMainFrame();
		virtual ~CMainFrame();

		BOOL	OnFileExit();
		BOOL	OnFileOpen();
		BOOL	OnFilePrint();
		BOOL	OnFileSave();

	protected:
		virtual CStatusBar& GetStatusBar() const
			  { return const_cast<CustomStatusBar&>
			      (m_CustomStatusBar);
			  }
		virtual BOOL	OnCommand(WPARAM, LPARAM);
		virtual int	OnCreate(CREATESTRUCT& cs);
		virtual void	OnInitialUpdate();
		virtual LRESULT OnNotify(WPARAM, LPARAM);
		virtual void	PreCreate(CREATESTRUCT&);
		virtual void	SetStatusIndicators();
		virtual void	SetupToolBar();
		virtual LRESULT WndProc(UINT, WPARAM, LPARAM);

	private:
		CView		m_View;
		CustomStatusBar	m_CustomStatusBar;
};
/*----------------------------------------------------------------------------*/
#endif //MAINFRM_H

