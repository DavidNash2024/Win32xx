REM: A batch program to rebuild the Win32++ samples using VS2008 (Express) with the static library.
REM: The contents of the log file is erased 

REM: Set the Paths
@set PATH=c:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE;c:\Program Files\Microsoft Visual Studio 9.0\VC\BIN;c:\Program Files\Microsoft Visual Studio 9.0\Common7\tests;c:\WINDOWS\Microsoft.NET\Framework\v3.5;c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;c:\Program Files\Microsoft Visual Studio 9.0\VC\VCPackages;%PATH%
@set PATH=c:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\IDE;c:\Program Files\Microsoft Visual Studio 9.0\VC\BIN;c:\Program Files\Microsoft Visual Studio 9.0\Common7\tests;c:\WINDOWS\Microsoft.NET\Framework\v3.5;c:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;c:\Program Files\Microsoft Visual Studio 9.0\VC\VCPackages;%PATH%

::Cleanup
call clean

REM: Change the directory to the parent
pushd ..\..
if exist "Win32++\tests\VS2008\static" rmdir /s /q "Win32++\tests\VS2008\static"

REM:  Build the VS2008 static Library
devenv /rebuild Debug "Win32++\Library Projects\Library_2008.sln" 		>"Win32++\tests\VS2008_static.log"
devenv /rebuild Release "Win32++\Library Projects\Library_2008.sln" 		>>"Win32++\tests\VS2008_static.log"

::Compile code
devenv /rebuild "Debug Static Library" Browser\ProjectFiles\Browser_2008.sln  			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Browser\ProjectFiles\Browser_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Dialog\ProjectFiles\Dialog_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Dialog\ProjectFiles\Dialog_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DialogBars\ProjectFiles\DialogBars_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DialogBars\ProjectFiles\DialogBars_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DialogDemo\ProjectFiles\DialogDemo_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DialogDemo\ProjectFiles\DialogDemo_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DialogTab\ProjectFiles\DialogTab_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DialogTab\ProjectFiles\DialogTab_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Dock\ProjectFiles\Dock_2008.sln 				>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DirectX\ProjectFiles\DirectX_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DirectX\ProjectFiles\DirectX_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Dock\ProjectFiles\Dock_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DockContainer\ProjectFiles\DockContainer_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DockContainer\ProjectFiles\DockContainer_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" DockTabbedMDI\ProjectFiles\DockTabbedMDI_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" DockTabbedMDI\ProjectFiles\DOckTabbedMDI_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Explorer\ProjectFiles\Explorer_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Explorer\ProjectFiles\Explorer_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" FastGDI\ProjectFiles\FastGDI_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" FastGDI\ProjectFiles\FastGDI_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" FormDemo\ProjectFiles\FormDemo_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" FormDemo\ProjectFiles\FormDemo_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Frame\ProjectFiles\Frame_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Frame\ProjectFiles\Frame_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" MDIFrame\ProjectFiles\MDIFrame_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" MDIFrame\ProjectFiles\MDIFrame_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" MDIFrameDemo\ProjectFiles\MDIDemo_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" MDIFrameDemo\ProjectFiles\MDIDemo_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Networking\ClientDlg\Client_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Networking\ClientDlg\Client_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Networking\ServerDlg\Server_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Networking\ServerDlg\Server_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" NotePad\ProjectFiles\Notepad_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" NotePad\ProjectFiles\Notepad_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Performance\ProjectFiles\Performance_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Performance\ProjectFiles\Performance_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Picture\ProjectFiles\Picture_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Picture\ProjectFiles\Picture_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" PropertySheet\ProjectFiles\PropertySheet_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" PropertySheet\ProjectFiles\PropertySheet_2008.sln 	>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Scribble\ProjectFiles\Scribble_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Scribble\ProjectFiles\Scribble_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Simple\ProjectFiles\Simple_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Simple\ProjectFiles\Simple_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Splitter\ProjectFiles\Splitter_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Splitter\ProjectFiles\Splitter_2008.sln 		>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" TabDemo\ProjectFiles\TabDemo_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" TabDemo\ProjectFiles\TabDemo_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Themes\ProjectFiles\Themes_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Themes\ProjectFiles\Themes_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Debug Static Library" Threads\ProjectFiles\Threads_2008.sln 			>>"Win32++\tests\VS2008_static.log"
devenv /rebuild "Release Static Library" Threads\ProjectFiles\Threads_2008.sln 			>>"Win32++\tests\VS2008_static.log"


mkdir "Win32++\tests\VS2008"
mkdir "Win32++\tests\VS2008\static"
mkdir "Win32++\tests\VS2008\static\Debug"

copy Browser\ProjectFiles\Debug\Browser.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Dialog\ProjectFiles\Debug\Dialog.exe                   "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DialogBars\ProjectFiles\Debug\DialogBars.exe           "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DialogDemo\ProjectFiles\Debug\DialogDemo.exe           "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DialogTab\ProjectFiles\Debug\DialogTab.exe             "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DirectX\ProjectFiles\Debug\DirectX.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Dock\ProjectFiles\Debug\Dock.exe                       "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DockContainer\ProjectFiles\Debug\DockContainer.exe     "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy DockTabbedMDI\ProjectFiles\Debug\DockTabbedMDI.exe     "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Explorer\ProjectFiles\Debug\Explorer.exe               "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy FastGDI\ProjectFiles\Debug\FastGDI.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy FormDemo\ProjectFiles\Debug\FormDemo.exe               "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Frame\ProjectFiles\Debug\Frame.exe                     "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy MDIFrame\ProjectFiles\Debug\MDIFrame.exe               "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy MDIFrameDemo\ProjectFiles\Debug\MDIDemo.exe            "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Networking\ClientDlg\Debug\Client.exe                  "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Networking\ServerDlg\Debug\Server.exe                  "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy NotePad\ProjectFiles\Debug\Notepad.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Performance\ProjectFiles\Debug\Performance.exe         "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Picture\ProjectFiles\Debug\Picture.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy PropertySheet\ProjectFiles\Debug\PropertySheet.exe     "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Scribble\ProjectFiles\Debug\Scribble.exe               "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Simple\ProjectFiles\Debug\Simple.exe                   "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Splitter\ProjectFiles\Debug\Splitter.exe               "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy TabDemo\ProjectFiles\Debug\TabDemo.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Themes\ProjectFiles\Debug\Themes.exe                   "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"
copy Threads\ProjectFiles\Debug\Threads.exe                 "Win32++\tests\VS2008\static\Debug"    >>"Win32++\tests\VS2008_static.log"




mkdir "Win32++\tests\VS2008\static\Release"

copy Browser\ProjectFiles\Release\Browser.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Dialog\ProjectFiles\Release\Dialog.exe                   "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DialogBars\ProjectFiles\Release\DialogBars.exe           "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DialogDemo\ProjectFiles\Release\DialogDemo.exe           "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DialogTab\ProjectFiles\Release\DialogTab.exe             "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DirectX\ProjectFiles\Release\DirectX.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Dock\ProjectFiles\Release\Dock.exe                       "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DockContainer\ProjectFiles\Release\DockContainer.exe     "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy DockTabbedMDI\ProjectFiles\Release\DockTabbedMDI.exe     "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Explorer\ProjectFiles\Release\Explorer.exe               "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy FastGDI\ProjectFiles\Release\FastGDI.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy FormDemo\ProjectFiles\Release\FormDemo.exe               "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Frame\ProjectFiles\Release\Frame.exe                     "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy MDIFrame\ProjectFiles\Release\MDIFrame.exe               "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy MDIFrameDemo\ProjectFiles\Release\MDIDemo.exe            "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Networking\ClientDlg\Release\Client.exe                  "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Networking\ServerDlg\Release\Server.exe                  "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy NotePad\ProjectFiles\Release\Notepad.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Performance\ProjectFiles\Release\Performance.exe         "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Picture\ProjectFiles\Release\Picture.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy PropertySheet\ProjectFiles\Release\PropertySheet.exe     "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Scribble\ProjectFiles\Release\Scribble.exe               "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Simple\ProjectFiles\Release\Simple.exe                   "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Splitter\ProjectFiles\Release\Splitter.exe               "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy TabDemo\ProjectFiles\Release\TabDemo.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Themes\ProjectFiles\Release\Themes.exe                   "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"
copy Threads\ProjectFiles\Release\Threads.exe                 "Win32++\tests\VS2008\static\Release"    >>"Win32++\tests\VS2008_static.log"

REM: Pop the directory change off the stack
popd

