REM: A batch program to rebuild the Win32++ samples using VS2005 (Express).
REM: The contents of the log file is erased
REM: The contents of the VS2005 directory will be erased

::SetPaths
@set PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files\Microsoft Visual Studio 8\VC\BIN;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\bin;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files\Microsoft Visual Studio 8\VC\BIN;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\bin;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%


::Cleanup
call clean
call CleanTutorials
pushd "..\Networking"
call clean
popd
RMDIR /S /Q "..\StaticLibrary\Frame\Debug"
RMDIR /S /Q "..\StaticLibrary\Frame\Release"

REM: Change the directory to the parent
pushd ..
if exist "Win32xx Tests\VS2005" rmdir /s /q "Win32xx Tests\VS2005"


::Compile code
devenv /build Debug Browser\ProjectFiles\Browser_2005.sln /projectconfig Debug 				> "Win32xx Tests\VS2005.log"
devenv /build Release Browser\ProjectFiles\Browser_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Dialog\ProjectFiles\Dialog_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Dialog\ProjectFiles\Dialog_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug DialogDemo\ProjectFiles\DialogDemo_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release DialogDemo\ProjectFiles\DialogDemo_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug DialogBarsDemo\ProjectFiles\DialogBarsDemo_2005.sln /projectconfig Debug		>>"Win32xx Tests\VS2005.log"
devenv /build Release DialogBarsDemo\ProjectFiles\DialogBarsDemo_2005.sln /projectconfig Release	>>"Win32xx Tests\VS2005.log"
devenv /build Debug DockContainer\ProjectFiles\ContainerDocking_2005.sln /projectconfig Debug		>>"Win32xx Tests\VS2005.log"
devenv /build Release DockContainer\ProjectFiles\ContainerDocking_2005.sln /projectconfig Release	>>"Win32xx Tests\VS2005.log"
devenv /build Debug DockSimple\ProjectFiles\Docking_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release DockSimple\ProjectFiles\Docking_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug DockTabbedMDI\ProjectFiles\TabbedMDI_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release DockTabbedMDI\ProjectFiles\TabbedMDI_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug DocViewForm\ProjectFiles\DocViewForm_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release DocViewForm\ProjectFiles\DocViewForm_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug DX\ProjectFiles\DX_2005.sln /projectconfig Debug					>>"Win32xx Tests\VS2005.log"
devenv /build Release DX\ProjectFiles\DX_2005.sln /projectconfig Release				>>"Win32xx Tests\VS2005.log"
devenv /build Debug Explorer\ProjectFiles\Explorer_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Explorer\ProjectFiles\Explorer_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug FastGDI\ProjectFiles\FastGDI_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release FastGDI\ProjectFiles\FastGDI_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Frame\ProjectFiles\Frame_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Frame\ProjectFiles\Frame_2005.sln /projectconfig Release				>>"Win32xx Tests\VS2005.log"
devenv /build Debug MDIDemo\ProjectFiles\MDIDemo_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release MDIDemo\ProjectFiles\MDIDemo_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug MDIFrame\ProjectFiles\MDIFrame_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release MDIFrame\ProjectFiles\MDIFrame_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Networking\ClientDlg\Client_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Networking\ClientDlg\Client_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Networking\ServerDlg\Server_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Networking\ServerDlg\Server_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug NotePad\ProjectFiles\Notepad_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release NotePad\ProjectFiles\Notepad_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Performance\ProjectFiles\Performance_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Performance\ProjectFiles\Performance_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Picture\ProjectFiles\Picture_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Picture\ProjectFiles\Picture_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug PropertySheet\ProjectFiles\PropertySheet_2005.sln /projectconfig Debug		>>"Win32xx Tests\VS2005.log"
devenv /build Release PropertySheet\ProjectFiles\PropertySheet_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Scribble\ProjectFiles\Scribble_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Scribble\ProjectFiles\Scribble_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Simple\ProjectFiles\Simple_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Simple\ProjectFiles\Simple_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Splitter\ProjectFiles\Splitter_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Splitter\ProjectFiles\Splitter_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Subclass\ProjectFiles\Subclass_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Subclass\ProjectFiles\Subclass_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug TabDemo\ProjectFiles\TabDemo_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release TabDemo\ProjectFiles\TabDemo_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug TabDialogDemo\ProjectFiles\TabDialogDemo_2005.sln /projectconfig Debug		>>"Win32xx Tests\VS2005.log"
devenv /build Release TabDialogDemo\ProjectFiles\TabDialogDemo_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Themes\ProjectFiles\Themes_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Themes\ProjectFiles\Themes_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"
devenv /build Debug Threads\ProjectFiles\Threads_2005.sln /projectconfig Debug				>>"Win32xx Tests\VS2005.log"
devenv /build Release Threads\ProjectFiles\Threads_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"

mkdir "Win32xx Tests\VS2005"
mkdir "Win32xx Tests\VS2005\Debug"

copy Browser\ProjectFiles\Debug\Browser.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Dialog\ProjectFiles\Debug\Dialog.exe                   "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DialogBarsDemo\ProjectFiles\Debug\DialogBarsDemo.exe   "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DialogDemo\ProjectFiles\Debug\DialogDemo.exe           "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DockContainer\ProjectFiles\Debug\ContainerDocking.exe  "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DockSimple\ProjectFiles\Debug\Docking.exe              "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DockTabbedMDI\ProjectFiles\Debug\TabbedMDI.exe         "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DocViewForm\ProjectFiles\Debug\DocViewForm.exe         "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy DX\ProjectFiles\Debug\DX.exe                           "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Explorer\ProjectFiles\Debug\Explorer.exe               "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy FastGDI\ProjectFiles\Debug\FastGDI.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Frame\ProjectFiles\Debug\Frame.exe                     "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy MDIDemo\ProjectFiles\Debug\MDIDemo.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy MDIFrame\ProjectFiles\Debug\MDIFrame.exe               "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Networking\ClientDlg\Debug\Client.exe                  "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Networking\ServerDlg\Debug\Server.exe                  "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy NotePad\ProjectFiles\Debug\Notepad.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Performance\ProjectFiles\Debug\Performance.exe         "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Picture\ProjectFiles\Debug\Picture.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy PropertySheet\ProjectFiles\Debug\PropertySheet.exe     "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Scribble\ProjectFiles\Debug\Scribble.exe               "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Simple\ProjectFiles\Debug\Simple.exe                   "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Splitter\ProjectFiles\Debug\Splitter.exe               "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Subclass\ProjectFiles\Debug\Subclass.exe               "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy TabDemo\ProjectFiles\Debug\TabDemo.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy TabDialogDemo\ProjectFiles\Debug\TabDialogDemo.exe     "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Themes\ProjectFiles\Debug\Themes.exe                   "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"
copy Threads\ProjectFiles\Debug\Threads.exe                 "Win32xx Tests\VS2005\Debug"    >>"Win32xx Tests\VS2005.log"

mkdir "Win32xx Tests\VS2005\Release"

copy Browser\ProjectFiles\Release\Browser.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Dialog\ProjectFiles\Release\Dialog.exe                   "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DialogBarsDemo\ProjectFiles\Release\DialogBarsDemo.exe   "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DialogDemo\ProjectFiles\Release\DialogDemo.exe           "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DockContainer\ProjectFiles\Release\ContainerDocking.exe  "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DockSimple\ProjectFiles\Release\Docking.exe              "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DockTabbedMDI\ProjectFiles\Release\TabbedMDI.exe         "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DocViewForm\ProjectFiles\Release\DocViewForm.exe         "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy DX\ProjectFiles\Release\DX.exe                           "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Explorer\ProjectFiles\Release\Explorer.exe               "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy FastGDI\ProjectFiles\Release\FastGDI.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Frame\ProjectFiles\Release\Frame.exe                     "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy MDIDemo\ProjectFiles\Release\MDIDemo.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy MDIFrame\ProjectFiles\Release\MDIFrame.exe               "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Networking\ClientDlg\Release\Client.exe                  "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Networking\ServerDlg\Release\Server.exe                  "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy NotePad\ProjectFiles\Release\Notepad.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Performance\ProjectFiles\Release\Performance.exe         "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Picture\ProjectFiles\Release\Picture.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy PropertySheet\ProjectFiles\Release\PropertySheet.exe     "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Scribble\ProjectFiles\Release\Scribble.exe               "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Simple\ProjectFiles\Release\Simple.exe                   "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Splitter\ProjectFiles\Release\Splitter.exe               "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Subclass\ProjectFiles\Release\Subclass.exe               "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy TabDemo\ProjectFiles\Release\TabDemo.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy TabDialogDemo\ProjectFiles\Release\TabDialogDemo.exe     "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Themes\ProjectFiles\Release\Themes.exe                   "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"
copy Threads\ProjectFiles\Release\Threads.exe                 "Win32xx Tests\VS2005\Release"    >>"Win32xx Tests\VS2005.log"

REM:  Build the VS2005 static Library
devenv /build Debug StaticLibrary\ProjectFiles\Library_2005.sln /projectconfig Debug		>>"Win32xx Tests\VS2005.log"
devenv /build Release StaticLibrary\ProjectFiles\Library_2005.sln /projectconfig Release	>>"Win32xx Tests\VS2005.log"

REM:  Build the VS2005 test file
devenv /build Debug StaticLibrary\Frame\Frame_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release StaticLibrary\Frame\Frame_2005.sln /projectconfig Release			>>"Win32xx Tests\VS2005.log"


mkdir "Win32xx Tests\VS2005\StaticLib"
mkdir "Win32xx Tests\VS2005\StaticLib\Debug"
mkdir "Win32xx Tests\VS2005\StaticLib\Release"

copy StaticLibrary\Frame\Debug\Frame.exe      	"Win32xx Tests\VS2005\StaticLib\Debug"    	>>"Win32xx Tests\VS2005.log"
copy StaticLibrary\Frame\Release\Frame.exe     	"Win32xx Tests\VS2005\StaticLib\Release"    	>>"Win32xx Tests\VS2005.log"



REM: Compile Tutorials
::Compile code
devenv /build Debug Tutorial1\Tutorial1_2005.sln /projectconfig Debug 			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial1\Tutorial1_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial2\Tutorial2_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial2\Tutorial2_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial3\Tutorial3_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial3\Tutorial3_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial4\Tutorial4_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial4\Tutorial4_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial5\Tutorial5_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial5\Tutorial5_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial6\Tutorial6_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial6\Tutorial6_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial7\Tutorial7_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial7\Tutorial7_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial8\Tutorial8_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial8\Tutorial8_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"
devenv /build Debug Tutorial9\Tutorial9_2005.sln /projectconfig Debug			>>"Win32xx Tests\VS2005.log"
devenv /build Release Tutorial9\Tutorial9_2005.sln /projectconfig Release		>>"Win32xx Tests\VS2005.log"

mkdir "Win32xx Tests\VS2005\Tutorials"
mkdir "Win32xx Tests\VS2005\Tutorials\Debug"

copy Tutorial1\Debug\Tutorial1.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial2\Debug\Tutorial2.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial3\Debug\Tutorial3.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial4\Debug\Tutorial4.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial5\Debug\Tutorial5.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial6\Debug\Tutorial6.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial7\Debug\Tutorial7.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial8\Debug\Tutorial8.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"
copy Tutorial9\Debug\Tutorial9.exe         "Win32xx Tests\VS2005\Tutorials\Debug"    >>"Win32xx Tests\VS2005.log"

mkdir "Win32xx Tests\VS2005\Tutorials\Release"

copy Tutorial1\Release\Tutorial1.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial2\Release\Tutorial2.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial3\Release\Tutorial3.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial4\Release\Tutorial4.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial5\Release\Tutorial5.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial6\Release\Tutorial6.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial7\Release\Tutorial7.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial8\Release\Tutorial8.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"
copy Tutorial9\Release\Tutorial9.exe         "Win32xx Tests\VS2005\Tutorials\Release"    >>"Win32xx Tests\VS2005.log"

popd