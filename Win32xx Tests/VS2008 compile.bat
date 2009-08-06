REM: A batch program to rebuild the Win32++ samples using VS2008 (Express).
REM: The contents of the log file is erased 

REM: Set the Paths
@set PATH=c:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;c:\Program Files\Microsoft Visual Studio 9.0\VC\BIN;c:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools;c:\WINDOWS\Microsoft.NET\Framework\v3.5;c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;c:\Program Files\Microsoft Visual Studio 9.0\VC\VCPackages;%PATH%



REM: Remove old files before we start
call clean
pushd "..\Networking"
call clean
popd


REM: Change the directory to the parent
pushd ..


REM: Compile the code
devenv /build Debug Browser\ProjectFiles\Browser_2008.sln /projectconfig Debug 				> "Win32xx Tests\VS2008.log"
devenv /build Release Browser\ProjectFiles\Browser_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug ControlBars\ProjectFiles\ControlBars_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release ControlBars\ProjectFiles\ControlBars_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug Dialog\ProjectFiles\Dialog_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Dialog\ProjectFiles\Dialog_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug DlgSubclass\ProjectFiles\DlgSubClass_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release DlgSubclass\ProjectFiles\DlgSubClass_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug DockContainer\ProjectFiles\ContainerDocking_2008.sln /projectconfig Debug		>>"Win32xx Tests\VS2008.log"
devenv /build Release DockContainer\ProjectFiles\ContainerDocking_2008.sln /projectconfig Release	>>"Win32xx Tests\VS2008.log"
devenv /build Debug DockSimple\ProjectFiles\Docking_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release DockSimple\ProjectFiles\Docking_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug DockTabbedMDI\ProjectFiles\TabbedMDI_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release DockTabbedMDI\ProjectFiles\TabbedMDI_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug DocViewForm\ProjectFiles\DocViewForm_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release DocViewForm\ProjectFiles\DocViewForm_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug DX\ProjectFiles\DX_2008.sln /projectconfig Debug					>>"Win32xx Tests\VS2008.log"
devenv /build Release DX\ProjectFiles\DX_2008.sln /projectconfig Release				>>"Win32xx Tests\VS2008.log"
devenv /build Debug Explorer\ProjectFiles\Explorer_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release Explorer\ProjectFiles\Explorer_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug FastGDI\ProjectFiles\FastGDI_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release FastGDI\ProjectFiles\FastGDI_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Frame\ProjectFiles\Frame_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Frame\ProjectFiles\Frame_2008.sln /projectconfig Release				>>"Win32xx Tests\VS2008.log"
devenv /build Debug MDIDemo\ProjectFiles\MDIDemo_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release MDIDemo\ProjectFiles\MDIDemo_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug MDIFrame\ProjectFiles\MDIFrame_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release MDIFrame\ProjectFiles\MDIFrame_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Networking\ClientDlg\Client_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Networking\ClientDlg\Client_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Networking\ServerDlg\Server_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Networking\ServerDlg\Server_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug NotePad\ProjectFiles\Notepad_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release NotePad\ProjectFiles\Notepad_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Performance\ProjectFiles\Performance_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release Performance\ProjectFiles\Performance_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug Picture\ProjectFiles\Picture_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Picture\ProjectFiles\Picture_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug PropertySheet\ProjectFiles\PropertySheet_2008.sln /projectconfig Debug		>>"Win32xx Tests\VS2008.log"
devenv /build Release PropertySheet\ProjectFiles\PropertySheet_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug Scribble\ProjectFiles\Scribble_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release Scribble\ProjectFiles\Scribble_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Simple\ProjectFiles\Simple_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Simple\ProjectFiles\Simple_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Splitter\ProjectFiles\Splitter_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release Splitter\ProjectFiles\Splitter_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Subclass\ProjectFiles\Subclass_2008.sln /projectconfig Debug			>>"Win32xx Tests\VS2008.log"
devenv /build Release Subclass\ProjectFiles\Subclass_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug TabDemo\ProjectFiles\TabDemo_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release TabDemo\ProjectFiles\TabDemo_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug TabDialogDemo\ProjectFiles\TabDialogDemo_2008.sln /projectconfig Debug		>>"Win32xx Tests\VS2008.log"
devenv /build Release TabDialogDemo\ProjectFiles\TabDialogDemo_2008.sln /projectconfig Release		>>"Win32xx Tests\VS2008.log"
devenv /build Debug Themes\ProjectFiles\Themes_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Themes\ProjectFiles\Themes_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"
devenv /build Debug Threads\ProjectFiles\Threads_2008.sln /projectconfig Debug				>>"Win32xx Tests\VS2008.log"
devenv /build Release Threads\ProjectFiles\Threads_2008.sln /projectconfig Release			>>"Win32xx Tests\VS2008.log"


REM: Copy the executables to the testing directories
if exist "Win32xx Tests\VS2008" rmdir /s /q "Win32xx Tests\VS2008"
mkdir "Win32xx Tests\VS2008"
mkdir "Win32xx Tests\VS2008\Debug"

copy Browser\ProjectFiles\Debug\Browser.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy ControlBars\ProjectFiles\Debug\ControlBars.exe         "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Dialog\ProjectFiles\Debug\Dialog.exe                   "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DlgSubclass\ProjectFiles\Debug\DlgSubClass.exe         "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DockContainer\ProjectFiles\Debug\ContainerDocking.exe  "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DockSimple\ProjectFiles\Debug\Docking.exe              "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DockTabbedMDI\ProjectFiles\Debug\TabbedMDI.exe         "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DocViewForm\ProjectFiles\Debug\DocViewForm.exe         "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy DX\ProjectFiles\Debug\DX.exe                           "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Explorer\ProjectFiles\Debug\Explorer.exe               "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy FastGDI\ProjectFiles\Debug\FastGDI.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Frame\ProjectFiles\Debug\Frame.exe                     "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy MDIDemo\ProjectFiles\Debug\MDIDemo.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy MDIFrame\ProjectFiles\Debug\MDIFrame.exe               "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Networking\ClientDlg\Debug\Client.exe                  "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Networking\ServerDlg\Debug\Server.exe                  "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy NotePad\ProjectFiles\Debug\Notepad.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Performance\ProjectFiles\Debug\Performance.exe         "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Picture\ProjectFiles\Debug\Picture.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy PropertySheet\ProjectFiles\Debug\PropertySheet.exe     "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Scribble\ProjectFiles\Debug\Scribble.exe               "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Simple\ProjectFiles\Debug\Simple.exe                   "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Splitter\ProjectFiles\Debug\Splitter.exe               "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Subclass\ProjectFiles\Debug\Subclass.exe               "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy TabDemo\ProjectFiles\Debug\TabDemo.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy TabDialogDemo\ProjectFiles\Debug\TabDialogDemo.exe     "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Themes\ProjectFiles\Debug\Themes.exe                   "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"
copy Threads\ProjectFiles\Debug\Threads.exe                 "Win32xx Tests\VS2008\Debug"    >>"Win32xx Tests\VS2008.log"

mkdir "Win32xx Tests\VS2008\Release"

copy Browser\ProjectFiles\Release\Browser.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy ControlBars\ProjectFiles\Release\ControlBars.exe         "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Dialog\ProjectFiles\Release\Dialog.exe                   "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DlgSubclass\ProjectFiles\Release\DlgSubClass.exe         "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DockContainer\ProjectFiles\Release\ContainerDocking.exe  "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DockSimple\ProjectFiles\Release\Docking.exe              "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DockTabbedMDI\ProjectFiles\Release\TabbedMDI.exe         "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DocViewForm\ProjectFiles\Release\DocViewForm.exe         "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy DX\ProjectFiles\Release\DX.exe                           "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Explorer\ProjectFiles\Release\Explorer.exe               "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy FastGDI\ProjectFiles\Release\FastGDI.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Frame\ProjectFiles\Release\Frame.exe                     "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy MDIDemo\ProjectFiles\Release\MDIDemo.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy MDIFrame\ProjectFiles\Release\MDIFrame.exe               "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Networking\ClientDlg\Release\Client.exe                  "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Networking\ServerDlg\Release\Server.exe                  "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy NotePad\ProjectFiles\Release\Notepad.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Performance\ProjectFiles\Release\Performance.exe         "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Picture\ProjectFiles\Release\Picture.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy PropertySheet\ProjectFiles\Release\PropertySheet.exe     "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Scribble\ProjectFiles\Release\Scribble.exe               "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Simple\ProjectFiles\Release\Simple.exe                   "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Splitter\ProjectFiles\Release\Splitter.exe               "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Subclass\ProjectFiles\Release\Subclass.exe               "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy TabDemo\ProjectFiles\Release\TabDemo.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy TabDialogDemo\ProjectFiles\Release\TabDialogDemo.exe     "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Themes\ProjectFiles\Release\Themes.exe                   "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"
copy Threads\ProjectFiles\Release\Threads.exe                 "Win32xx Tests\VS2008\Release"    >>"Win32xx Tests\VS2008.log"

REM: Pop the directory change off the stack
popd

                                          