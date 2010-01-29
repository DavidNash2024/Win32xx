REM: A batch program to rebuild the Win32++ samples using VS2010 (Express).
REM: The contents of the log file is erased 

REM: Set the Paths
@Path=C:\Program Files\Microsoft Visual Studio 10.0\VSTSDB\Deploy;C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\;C:\Program Files\Microsoft Visual Studio 10.0\VC\BIN;C:\Program Files\Microsoft Visual Studio 10.0\Common7\Tools;C:\Windows\Microsoft.NET\Framework\v4.0.21006;C:\Windows\Microsoft.NET\Framework\v3.5;C:\Program Files\Microsoft Visual Studio 10.0\VC\VCPackages;%PATH%


@Path=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VSTSDB\Deploy;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools;C:\Windows\Microsoft.NET\Framework\v4.0.21006;C:\Windows\Microsoft.NET\Framework\v3.5;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\VCPackages;%PATH%


::Cleanup
call clean
call CleanTutorials
pushd "..\..\Networking"
call clean
popd


REM: Change the directory to the parent
pushd ..\..
if exist "Win32++\tests\VS2010\Debug" rmdir /s /q "Win32++\tests\VS2010\Debug"
if exist "Win32++\tests\VS2010\Release" rmdir /s /q "Win32++\tests\VS2010\Release"
if exist "Win32++\tests\VS2010\Tutorials" rmdir /s /q "Win32++\tests\VS2010\Tutorials"


::Compile code
devenv /build Debug Browser\ProjectFiles\Browser.sln  			> "Win32++\tests\VS2010.log"
devenv /build Release Browser\ProjectFiles\Browse.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Dialog\ProjectFiles\Dialog.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Dialog\ProjectFiles\Dialog.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug DialogBars\ProjectFiles\DialogBars.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release DialogBars\ProjectFiles\DialogBars.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug DialogDemo\ProjectFiles\DialogDemo.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release DialogDemo\ProjectFiles\DialogDemo.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug DialogTab\ProjectFiles\DialogTab.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release DialogTab\ProjectFiles\DialogTab.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug DirectX\ProjectFiles\DirectX.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release DirectX\ProjectFiles\DirectX.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Dock\ProjectFiles\Dock.sln 				>>"Win32++\tests\VS2010.log"
devenv /build Release Dock\ProjectFiles\Dock.sln 				>>"Win32++\tests\VS2010.log"
devenv /build Debug DockContainer\ProjectFiles\DockContainer.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release DockContainer\ProjectFiles\DockContainer.sln 	>>"Win32++\tests\VS2010.log"
devenv /build Debug DockTabbedMDI\ProjectFiles\DockTabbedMDI.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release DockTabbedMDI\ProjectFiles\DockTabbedMDI.sln 	>>"Win32++\tests\VS2010.log"
devenv /build Debug Explorer\ProjectFiles\Explorer.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Explorer\ProjectFiles\Explorer.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug FastGDI\ProjectFiles\FastGDI.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release FastGDI\ProjectFiles\FastGDI.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug FormDemo\ProjectFiles\FormDemo.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release FormDemo\ProjectFiles\FormDemo.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Frame\ProjectFiles\Frame.sln 				>>"Win32++\tests\VS2010.log"
devenv /build Release Frame\ProjectFiles\Frame.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug MDIFrame\ProjectFiles\MDIFrame.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release MDIFrame\ProjectFiles\MDIFrame.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug MDIFrameDemo\ProjectFiles\MDIDemo.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release MDIFrameDemo\ProjectFiles\MDIDemo.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Networking\ClientDlg\Client.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Networking\ClientDlg\Client.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Networking\ServerDlg\Server.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Networking\ServerDlg\Server.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug NotePad\ProjectFiles\Notepad.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release NotePad\ProjectFiles\Notepad.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Performance\ProjectFiles\Performance.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Performance\ProjectFiles\Performance.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Picture\ProjectFiles\Picture.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Picture\ProjectFiles\Picture.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug PropertySheet\ProjectFiles\PropertySheet.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release PropertySheet\ProjectFiles\PropertySheet.sln 	>>"Win32++\tests\VS2010.log"
devenv /build Debug Scribble\ProjectFiles\Scribble.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Scribble\ProjectFiles\Scribble.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Simple\ProjectFiles\Simple.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Simple\ProjectFiles\Simple.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Splitter\ProjectFiles\Splitter.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Splitter\ProjectFiles\Splitter.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug TabDemo\ProjectFiles\TabDemo.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release TabDemo\ProjectFiles\TabDemo.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Themes\ProjectFiles\Themes.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Themes\ProjectFiles\Themes.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Debug Threads\ProjectFiles\Threads.sln 			>>"Win32++\tests\VS2010.log"
devenv /build Release Threads\ProjectFiles\Threads.sln 			>>"Win32++\tests\VS2010.log"

mkdir "Win32++\tests\VS2010"
mkdir "Win32++\tests\VS2010\Debug"

copy Browser\ProjectFiles\Debug\Browser.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Dialog\ProjectFiles\Debug\Dialog.exe                   "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DialogBars\ProjectFiles\Debug\DialogBars.exe           "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DialogDemo\ProjectFiles\Debug\DialogDemo.exe           "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DialogTab\ProjectFiles\Debug\DialogTab.exe             "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DirectX\ProjectFiles\Debug\DirectX.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Dock\ProjectFiles\Debug\Dock.exe                       "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DockContainer\ProjectFiles\Debug\DockContainer.exe     "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy DockTabbedMDI\ProjectFiles\Debug\DockTabbedMDI.exe     "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Explorer\ProjectFiles\Debug\Explorer.exe               "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy FastGDI\ProjectFiles\Debug\FastGDI.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy FormDemo\ProjectFiles\Debug\FormDemo.exe               "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Frame\ProjectFiles\Debug\Frame.exe                     "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy MDIFrame\ProjectFiles\Debug\MDIFrame.exe               "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy MDIFrameDemo\ProjectFiles\Debug\MDIDemo.exe            "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Networking\ClientDlg\Debug\Client.exe                  "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Networking\ServerDlg\Debug\Server.exe                  "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy NotePad\ProjectFiles\Debug\Notepad.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Performance\ProjectFiles\Debug\Performance.exe         "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Picture\ProjectFiles\Debug\Picture.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy PropertySheet\ProjectFiles\Debug\PropertySheet.exe     "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Scribble\ProjectFiles\Debug\Scribble.exe               "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Simple\ProjectFiles\Debug\Simple.exe                   "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Splitter\ProjectFiles\Debug\Splitter.exe               "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy TabDemo\ProjectFiles\Debug\TabDemo.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Themes\ProjectFiles\Debug\Themes.exe                   "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"
copy Threads\ProjectFiles\Debug\Threads.exe                 "Win32++\tests\VS2010\Debug"    >>"Win32++\tests\VS2010.log"

mkdir "Win32++\tests\VS2010\Release"

copy Browser\ProjectFiles\Release\Browser.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Dialog\ProjectFiles\Release\Dialog.exe                   "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DialogBars\ProjectFiles\Release\DialogBars.exe           "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DialogDemo\ProjectFiles\Release\DialogDemo.exe           "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DialogTab\ProjectFiles\Release\DialogTab.exe             "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DirectX\ProjectFiles\Release\DirectX.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Dock\ProjectFiles\Release\Dock.exe                       "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DockContainer\ProjectFiles\Release\DockContainer.exe     "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy DockTabbedMDI\ProjectFiles\Release\DockTabbedMDI.exe     "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Explorer\ProjectFiles\Release\Explorer.exe               "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy FastGDI\ProjectFiles\Release\FastGDI.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy FormDemo\ProjectFiles\Release\FormDemo.exe               "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Frame\ProjectFiles\Release\Frame.exe                     "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy MDIFrame\ProjectFiles\Release\MDIFrame.exe               "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy MDIFrameDemo\ProjectFiles\Release\MDIDemo.exe            "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Networking\ClientDlg\Release\Client.exe                  "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Networking\ServerDlg\Release\Server.exe                  "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy NotePad\ProjectFiles\Release\Notepad.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Performance\ProjectFiles\Release\Performance.exe         "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Picture\ProjectFiles\Release\Picture.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy PropertySheet\ProjectFiles\Release\PropertySheet.exe     "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Scribble\ProjectFiles\Release\Scribble.exe               "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Simple\ProjectFiles\Release\Simple.exe                   "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Splitter\ProjectFiles\Release\Splitter.exe               "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy TabDemo\ProjectFiles\Release\TabDemo.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Themes\ProjectFiles\Release\Themes.exe                   "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"
copy Threads\ProjectFiles\Release\Threads.exe                 "Win32++\tests\VS2010\Release"    >>"Win32++\tests\VS2010.log"


REM: Compile Tutorials
ECHO "Compliling Tutorials" >>"Win32++\tests\VS2010.log"
::Compile code
devenv /build Debug Tutorials\Tutorial1\Tutorial1_2010.sln  		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial1\Tutorial1_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial2\Tutorial2_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial2\Tutorial2_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial3\Tutorial3_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial3\Tutorial3_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial4\Tutorial4_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial4\Tutorial4_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial5\Tutorial5_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial5\Tutorial5_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial6\Tutorial6_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial6\Tutorial6_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial7\Tutorial7_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial7\Tutorial7_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial8\Tutorial8_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial8\Tutorial8_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Debug Tutorials\Tutorial9\Tutorial9_2010.sln 		>>"Win32++\tests\VS2010.log"
devenv /build Release Tutorials\Tutorial9\Tutorial9_2010.sln 		>>"Win32++\tests\VS2010.log"

mkdir "Win32++\tests\VS2010\Tutorials"
mkdir "Win32++\tests\VS2010\Tutorials\Debug"

ECHO "Copying Debug Tutorials" >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial1\Debug\Tutorial1.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial2\Debug\Tutorial2.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial3\Debug\Tutorial3.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial4\Debug\Tutorial4.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial5\Debug\Tutorial5.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial6\Debug\Tutorial6.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial7\Debug\Tutorial7.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial8\Debug\Tutorial8.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial9\Debug\Tutorial9.exe         "Win32++\tests\VS2010\Tutorials\Debug"    >>"Win32++\tests\VS2010.log"

mkdir "Win32++\tests\VS2010\Tutorials\Release"

ECHO "Copying Release Tutorials" >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial1\Release\Tutorial1.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial2\Release\Tutorial2.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial3\Release\Tutorial3.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial4\Release\Tutorial4.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial5\Release\Tutorial5.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial6\Release\Tutorial6.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial7\Release\Tutorial7.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial8\Release\Tutorial8.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"
copy Tutorials\Tutorial9\Release\Tutorial9.exe         "Win32++\tests\VS2010\Tutorials\Release"    >>"Win32++\tests\VS2010.log"

REM: Pop the directory change off the stack
popd


