REM Clean the project folders.
call "CleanSamples.bat"
call "CleanTutorials.bat"

REM Stagger these compilation scripts with timeouts to prevent a race condition.
start "Building CodeBlocks" cmd /c "Compile CodeBlocks.bat"
timeout /t 1 /nobreak >nul
start "Building C++ Builder" cmd /c "Compile C++ Builder.bat"
timeout /t 1 /nobreak >nul
start "Building Visual Studio" cmd /c "Compile Visual Studio.bat"