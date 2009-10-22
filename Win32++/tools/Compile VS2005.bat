REM: A batch program to rebuild the Win32++ samples using VS2005 (Express).
REM: The contents of the log file is erased
REM: The contents of the VS2005 directory will be erased

::SetPaths
@set PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files\Microsoft Visual Studio 8\VC\BIN;C:\Program Files\Microsoft Visual Studio 8\Common7\tests;C:\Program Files\Microsoft Visual Studio 8\Common7\tests\bin;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%
@set PATH=C:\Program Files (x86)\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files\Microsoft Visual Studio 8\VC\BIN;C:\Program Files\Microsoft Visual Studio 8\Common7\tests;C:\Program Files\Microsoft Visual Studio 8\Common7\tests\bin;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%


::Cleanup
call clean
call CleanTutorials
pushd "..\..\Networking"
call clean
popd


REM: Change the directory to the parent
pushd ..\..
if exist "Win32++\tests\VS2005\Debug" rmdir /s /q "Win32++\tests\VS2005\Debug"
if exist "Win32++\tests\VS2005\Release" rmdir /s /q "Win32++\tests\VS2005\Release"
if exist "Win32++\tests\VS2005\Tutorials" rmdir /s /q "Win32++\tests\VS2005\Tutorials"


::Compile code
devenv /build Debug Browser\ProjectFiles\Browser_2005.sln  			> "Win32++\tests\VS2005.log"
devenv /build Release Browser\ProjectFiles\Browser_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Dialog\ProjectFiles\Dialog_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Dialog\ProjectFiles\Dialog_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug DialogBars\ProjectFiles\DialogBars_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release DialogBars\ProjectFiles\DialogBars_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug DialogDemo\ProjectFiles\DialogDemo_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release DialogDemo\ProjectFiles\DialogDemo_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug DialogTab\ProjectFiles\DialogTab_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release DialogTab\ProjectFiles\DialogTab_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug DirectX\ProjectFiles\DirectX_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release DirectX\ProjectFiles\DirectX_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Dock\ProjectFiles\Dock_2005.sln 				>>"Win32++\tests\VS2005.log"
devenv /build Release Dock\ProjectFiles\Dock_2005.sln 				>>"Win32++\tests\VS2005.log"
devenv /build Debug DockContainer\ProjectFiles\DockContainer_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release DockContainer\ProjectFiles\DockContainer_2005.sln 	>>"Win32++\tests\VS2005.log"
devenv /build Debug DockTabbedMDI\ProjectFiles\DockTabbedMDI_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release DockTabbedMDI\ProjectFiles\DOckTabbedMDI_2005.sln 	>>"Win32++\tests\VS2005.log"
devenv /build Debug Explorer\ProjectFiles\Explorer_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Explorer\ProjectFiles\Explorer_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug FastGDI\ProjectFiles\FastGDI_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release FastGDI\ProjectFiles\FastGDI_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug FormDemo\ProjectFiles\FormDemo_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release FormDemo\ProjectFiles\FormDemo_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Frame\ProjectFiles\Frame_2005.sln 				>>"Win32++\tests\VS2005.log"
devenv /build Release Frame\ProjectFiles\Frame_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug MDIFrame\ProjectFiles\MDIFrame_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release MDIFrame\ProjectFiles\MDIFrame_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug MDIFrameDemo\ProjectFiles\MDIDemo_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release MDIFrameDemo\ProjectFiles\MDIDemo_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Networking\ClientDlg\Client_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Networking\ClientDlg\Client_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Networking\ServerDlg\Server_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Networking\ServerDlg\Server_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug NotePad\ProjectFiles\Notepad_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release NotePad\ProjectFiles\Notepad_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Performance\ProjectFiles\Performance_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Performance\ProjectFiles\Performance_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Picture\ProjectFiles\Picture_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Picture\ProjectFiles\Picture_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug PropertySheet\ProjectFiles\PropertySheet_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release PropertySheet\ProjectFiles\PropertySheet_2005.sln 	>>"Win32++\tests\VS2005.log"
devenv /build Debug Scribble\ProjectFiles\Scribble_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Scribble\ProjectFiles\Scribble_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Simple\ProjectFiles\Simple_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Simple\ProjectFiles\Simple_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Splitter\ProjectFiles\Splitter_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Splitter\ProjectFiles\Splitter_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug TabDemo\ProjectFiles\TabDemo_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release TabDemo\ProjectFiles\TabDemo_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Themes\ProjectFiles\Themes_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Themes\ProjectFiles\Themes_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Debug Threads\ProjectFiles\Threads_2005.sln 			>>"Win32++\tests\VS2005.log"
devenv /build Release Threads\ProjectFiles\Threads_2005.sln 			>>"Win32++\tests\VS2005.log"

mkdir "Win32++\tests\VS2005"
mkdir "Win32++\tests\VS2005\Debug"

copy Browser\ProjectFiles\Debug\Browser.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Dialog\ProjectFiles\Debug\Dialog.exe                   "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DialogBars\ProjectFiles\Debug\DialogBars.exe           "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DialogDemo\ProjectFiles\Debug\DialogDemo.exe           "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DialogTab\ProjectFiles\Debug\DialogTab.exe             "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DirectX\ProjectFiles\Debug\DirectX.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Dock\ProjectFiles\Debug\Dock.exe                       "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DockContainer\ProjectFiles\Debug\DockContainer.exe     "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy DockTabbedMDI\ProjectFiles\Debug\DockTabbedMDI.exe     "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Explorer\ProjectFiles\Debug\Explorer.exe               "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy FastGDI\ProjectFiles\Debug\FastGDI.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy FormDemo\ProjectFiles\Debug\FormDemo.exe               "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Frame\ProjectFiles\Debug\Frame.exe                     "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy MDIFrame\ProjectFiles\Debug\MDIFrame.exe               "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy MDIFrameDemo\ProjectFiles\Debug\MDIDemo.exe            "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Networking\ClientDlg\Debug\Client.exe                  "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Networking\ServerDlg\Debug\Server.exe                  "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy NotePad\ProjectFiles\Debug\Notepad.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Performance\ProjectFiles\Debug\Performance.exe         "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Picture\ProjectFiles\Debug\Picture.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy PropertySheet\ProjectFiles\Debug\PropertySheet.exe     "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Scribble\ProjectFiles\Debug\Scribble.exe               "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Simple\ProjectFiles\Debug\Simple.exe                   "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Splitter\ProjectFiles\Debug\Splitter.exe               "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy TabDemo\ProjectFiles\Debug\TabDemo.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Themes\ProjectFiles\Debug\Themes.exe                   "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"
copy Threads\ProjectFiles\Debug\Threads.exe                 "Win32++\tests\VS2005\Debug"    >>"Win32++\tests\VS2005.log"

mkdir "Win32++\tests\VS2005\Release"

copy Browser\ProjectFiles\Release\Browser.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Dialog\ProjectFiles\Release\Dialog.exe                   "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DialogBars\ProjectFiles\Release\DialogBars.exe           "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DialogDemo\ProjectFiles\Release\DialogDemo.exe           "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DialogTab\ProjectFiles\Release\DialogTab.exe             "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DirectX\ProjectFiles\Release\DirectX.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Dock\ProjectFiles\Release\Dock.exe                       "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DockContainer\ProjectFiles\Release\DockContainer.exe     "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy DockTabbedMDI\ProjectFiles\Release\DockTabbedMDI.exe     "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Explorer\ProjectFiles\Release\Explorer.exe               "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy FastGDI\ProjectFiles\Release\FastGDI.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy FormDemo\ProjectFiles\Release\FormDemo.exe               "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Frame\ProjectFiles\Release\Frame.exe                     "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy MDIFrame\ProjectFiles\Release\MDIFrame.exe               "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy MDIFrameDemo\ProjectFiles\Release\MDIDemo.exe            "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Networking\ClientDlg\Release\Client.exe                  "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Networking\ServerDlg\Release\Server.exe                  "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy NotePad\ProjectFiles\Release\Notepad.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Performance\ProjectFiles\Release\Performance.exe         "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Picture\ProjectFiles\Release\Picture.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy PropertySheet\ProjectFiles\Release\PropertySheet.exe     "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Scribble\ProjectFiles\Release\Scribble.exe               "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Simple\ProjectFiles\Release\Simple.exe                   "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Splitter\ProjectFiles\Release\Splitter.exe               "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy TabDemo\ProjectFiles\Release\TabDemo.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Themes\ProjectFiles\Release\Themes.exe                   "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"
copy Threads\ProjectFiles\Release\Threads.exe                 "Win32++\tests\VS2005\Release"    >>"Win32++\tests\VS2005.log"


REM: Compile Tutorials
pushd "Tutorials"
::Compile code
devenv /build Debug Tutorial1\Tutorial1_2005.sln  		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial1\Tutorial1_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial2\Tutorial2_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial2\Tutorial2_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial3\Tutorial3_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial3\Tutorial3_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial4\Tutorial4_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial4\Tutorial4_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial5\Tutorial5_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial5\Tutorial5_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial6\Tutorial6_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial6\Tutorial6_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial7\Tutorial7_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial7\Tutorial7_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial8\Tutorial8_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial8\Tutorial8_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Debug Tutorial9\Tutorial9_2005.sln 		>>"Win32++\tests\VS2005.log"
devenv /build Release Tutorial9\Tutorial9_2005.sln 		>>"Win32++\tests\VS2005.log"

mkdir "Win32++\tests\VS2005\Tutorials"
mkdir "Win32++\tests\VS2005\Tutorials\Debug"

copy Tutorial1\Debug\Tutorial1.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial2\Debug\Tutorial2.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial3\Debug\Tutorial3.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial4\Debug\Tutorial4.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial5\Debug\Tutorial5.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial6\Debug\Tutorial6.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial7\Debug\Tutorial7.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial8\Debug\Tutorial8.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"
copy Tutorial9\Debug\Tutorial9.exe         "Win32++\tests\VS2005\Tutorials\Debug"    >>"Win32++\tests\VS2005.log"

mkdir "Win32++\tests\VS2005\Tutorials\Release"

copy Tutorial1\Release\Tutorial1.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial2\Release\Tutorial2.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial3\Release\Tutorial3.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial4\Release\Tutorial4.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial5\Release\Tutorial5.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial6\Release\Tutorial6.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial7\Release\Tutorial7.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial8\Release\Tutorial8.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"
copy Tutorial9\Release\Tutorial9.exe         "Win32++\tests\VS2005\Tutorials\Release"    >>"Win32++\tests\VS2005.log"

REM: Pop the directory change off the stack
popd
popd

