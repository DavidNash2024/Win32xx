REM: A batch program to rebuild the Win32++ samples using VS2008 (Express).
REM: The contents of the log file is erased 

::SetPaths
@set PATH=c:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;c:\Program Files\Microsoft Visual Studio 9.0\VC\BIN;c:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools;c:\WINDOWS\Microsoft.NET\Framework\v3.5;c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;c:\Program Files\Microsoft Visual Studio 9.0\VC\VCPackages;%PATH%

::Cleanup
call clean
pushd "Networking"
call clean
popd

::Compile code
devenv /build Debug Browser\ProjectFiles\Browser_2008.sln /projectconfig Debug 				> TestVS2008.log
devenv /build Release Browser\ProjectFiles\Browser_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug ControlBars\ProjectFiles\ControlBars_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release ControlBars\ProjectFiles\ControlBars_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug Dialog\ProjectFiles\Dialog_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Dialog\ProjectFiles\Dialog_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug DlgSubclass\ProjectFiles\DlgSubClass_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release DlgSubclass\ProjectFiles\DlgSubClass_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug DockContainer\ProjectFiles\ContainerDocking_2008.sln /projectconfig Debug		>>TestVS2008.log
devenv /build Release DockContainer\ProjectFiles\ContainerDocking_2008.sln /projectconfig Release	>>TestVS2008.log
devenv /build Debug DockSimple\ProjectFiles\Docking_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release DockSimple\ProjectFiles\Docking_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug DockTabbedMDI\ProjectFiles\TabbedMDI_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release DockTabbedMDI\ProjectFiles\TabbedMDI_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug DocViewForm\ProjectFiles\DocViewForm_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release DocViewForm\ProjectFiles\DocViewForm_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug DX\ProjectFiles\DX_2008.sln /projectconfig Debug					>>TestVS2008.log
devenv /build Release DX\ProjectFiles\DX_2008.sln /projectconfig Release				>>TestVS2008.log
devenv /build Debug Explorer\ProjectFiles\Explorer_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release Explorer\ProjectFiles\Explorer_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug FastGDI\ProjectFiles\FastGDI_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release FastGDI\ProjectFiles\FastGDI_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Frame\ProjectFiles\Frame_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Frame\ProjectFiles\Frame_2008.sln /projectconfig Release				>>TestVS2008.log
devenv /build Debug MDIDemo\ProjectFiles\MDIDemo_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release MDIDemo\ProjectFiles\MDIDemo_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug MDIFrame\ProjectFiles\MDIFrame_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release MDIFrame\ProjectFiles\MDIFrame_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Networking\ClientDlg\Client_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Networking\ClientDlg\Client_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Networking\ServerDlg\Server_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Networking\ServerDlg\Server_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug NotePad\ProjectFiles\Notepad_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release NotePad\ProjectFiles\Notepad_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Performance\ProjectFiles\Performance_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release Performance\ProjectFiles\Performance_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug Picture\ProjectFiles\Picture_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Picture\ProjectFiles\Picture_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug PropertySheet\ProjectFiles\PropertySheet_2008.sln /projectconfig Debug		>>TestVS2008.log
devenv /build Release PropertySheet\ProjectFiles\PropertySheet_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug Scribble\ProjectFiles\Scribble_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release Scribble\ProjectFiles\Scribble_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Simple\ProjectFiles\Simple_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Simple\ProjectFiles\Simple_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Splitter\ProjectFiles\Splitter_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release Splitter\ProjectFiles\Splitter_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Subclass\ProjectFiles\Subclass_2008.sln /projectconfig Debug			>>TestVS2008.log
devenv /build Release Subclass\ProjectFiles\Subclass_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug TabDemo\ProjectFiles\TabDemo_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release TabDemo\ProjectFiles\TabDemo_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug TabDialogDemo\ProjectFiles\TabDialogDemo_2008.sln /projectconfig Debug		>>TestVS2008.log
devenv /build Release TabDialogDemo\ProjectFiles\TabDialogDemo_2008.sln /projectconfig Release		>>TestVS2008.log
devenv /build Debug Themes\ProjectFiles\Themes_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Themes\ProjectFiles\Themes_2008.sln /projectconfig Release			>>TestVS2008.log
devenv /build Debug Threads\ProjectFiles\Threads_2008.sln /projectconfig Debug				>>TestVS2008.log
devenv /build Release Threads\ProjectFiles\Threads_2008.sln /projectconfig Release			>>TestVS2008.log
