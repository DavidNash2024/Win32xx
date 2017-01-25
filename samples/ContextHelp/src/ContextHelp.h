///////////////////////////////////////////////////////////////////////////////
//	Declaration of CContextHelp class. This class creates a window to display
//  a .chm help file.
//	
//	CContextHelp implements the following functions:
//	AddHelpTopic.  		Adds a <UINT, CString> pair to a map of known topics.
//	CreateHtmlHelp.		Creates a HtmlHelp window.
//	ShowHelpTopic. 		Creates a HtmlHelp window and displays the specified topic.
//	GetHelpFilePath.	Retrieves the pathname of the chm help file.
//	SetHelpFilePath		Set the pathname of the chm help file
//
//	The .chm help file is typically placed in a subdirectory of the user's
//	APPDATA environmental path. 
//
// 	Be sure to load the htmlhelp.h and htmlhelp.lib precisely as
// 	described in Adding Compiled HTML Help to MinGW Win32 Apps.html.
// 	Add the link to the libhtmlhelp.a to use this function
// 	(linker parameter -lhtmlhelp in DevC++). Ignore the compiler
// 	warning message ".drectve `-defaultlib:uuid.lib ' unrecognized".
// 	For other compilers, add the htmlhelp.lib to the linker list.
//
//  How to use CContexHelp
//  1) Use SetHelpFilePath to specify the chm help file.
//  2) Use AddHelpTopic to add a <UINT, CString> pair to the map of known topics.
//  3) Use ShowHelpTopic to display the help for a specified topic.
//
// 	Acknowledgement:
//	  I wish to thank  Robert C. Tausworthe for providing the original
// 	  code and for his ongoing assistance in developing this sample.


#ifndef CONTEXTHELP_H
#define CONTEXTHELP_H



/////////////////////////////////////////////////////////
// CContextHelp Class Declaration
//
//  A class used to open a help file and display the specified topic.
//

class CContextHelp : public CWnd
{
	public:
		CContextHelp();
		virtual ~CContextHelp(); 
		
		virtual void	AddHelpTopic(UINT nID, LPCTSTR topic);
		virtual HWND	CreateHtmlHelp(HWND hwndCaller, LPCTSTR szString, UINT uCommand, DWORD dwData);
		virtual void	ShowHelpTopic(UINT nID);
		virtual void	ShowHelpTopic(LPCTSTR topic);

		const CString&	GetHelpFilePath() const				{ return m_HelpFilePath; }
		void			SetHelpFilePath(LPCTSTR chmName)	{ m_HelpFilePath = chmName; }

	private:
		CString		m_HelpFilePath;				// Help file path
		std::map<UINT, CString> m_HelpTopics;	// Map of help topics

};

#endif // CONTEXTHELP_H
