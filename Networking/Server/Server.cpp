
#include "DialogApp.h"
#include "Server.h"

/////////////////////////////////////////
// Definitions of the CClientSocket class
//
void CClientSocket::OnClose()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientClose(this);
}

void CClientSocket::OnReceive()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnClientReceive(this);
}


/////////////////////////////////////////
// Definitions of the CListenSocket class
//
void CListenSocket::OnAccept()
{
	CSvrDialog& Dialog = ((CDialogApp*)GetApp())->GetDialog();
	Dialog.OnListenAccept();
}





