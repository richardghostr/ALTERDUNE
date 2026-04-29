echo Deployment finished. You can copy the generated folder to a target machine without Qt installed.
@echo off
rem deploy_windows.bat - Bundle Qt runtime and related dependencies for a GUI executable
rem Usage: deploy_windows.bat path\to\your\alterdune_qt.exe [path\to\qml_dir] [output_dir]
+
:: Resolve args
if "%~1"=="" (
  echo Usage: %~nx0 path\to\your\alterdune_qt.exe [path\to\qml_dir] [output_dir]
  exit /b 1
)
set EXE=%~1
set QMLDIR=%~2
set OUTDIR=%~3

if "%QMLDIR%"=="" set QMLDIR=%~dp0\..\gui\qml
+if "%OUTDIR%"=="" (
  set EXE_DIR=%~dp1
+  set EXE_NAME=%~n1
+  set OUTDIR=%EXE_DIR%\%EXE_NAME%_deploy
+)
+
echo Creating deploy folder: %OUTDIR%
if not exist "%OUTDIR%" mkdir "%OUTDIR%"
+
echo Copying executable to deploy folder...
copy /Y "%EXE%" "%OUTDIR%" >nul
+
:: Prefer windeployqt to collect Qt DLLs, plugins and QML imports
where windeployqt >nul 2>&1
+if errorlevel 0 (
  echo Running windeployqt --qmldir "%QMLDIR%" --dir "%OUTDIR%" "%EXE%" --release
  windeployqt --qmldir "%QMLDIR%" --dir "%OUTDIR%" "%EXE%" --release
  if errorlevel 1 (
    echo windeployqt failed. Continuing with manual copy attempts.
  )
) else (
  echo windeployqt not found in PATH, will attempt manual copies of common runtimes and plugins.
)
+
:: Copy project data and QML (if present near repository or next to exe)
+if exist "%~dp0\..\data\" (
  echo Copying project data folder to %OUTDIR%\data
  xcopy "%~dp0\..\data\*" "%OUTDIR%\data\" /E /I /Y >nul
) else if exist "%~dp1data\" (
  echo Copying data folder found near exe
  xcopy "%~dp1data\*" "%OUTDIR%\data\" /E /I /Y >nul
) else (
  echo No data folder found to copy (skipping)
)
+
+if exist "%~dp0\..\gui\qml\" (
  echo Copying qml imports to %OUTDIR%\qml
  xcopy "%~dp0\..\gui\qml\*" "%OUTDIR%\qml\" /E /I /Y >nul
) else if exist "%~dp1qml\" (
  echo Copying qml folder found near exe
  xcopy "%~dp1qml\*" "%OUTDIR%\qml\" /E /I /Y >nul
) else (
  echo No qml folder found to copy (skipping)
)
+
+:: Attempt to include common MinGW runtime DLLs if present next to the exe
+echo Looking for MinGW runtime DLLs next to the executable...
+set DLLS=libstdc++-6.dll libgcc_s_seh-1.dll libgcc_s_dw2-1.dll libwinpthread-1.dll
+for %%D in (%DLLS%) do (
  if exist "%~dp1%%D" (
    echo Copying %%D to %OUTDIR%
    copy /Y "%~dp1%%D" "%OUTDIR%" >nul
  )
+)
+
+:: If platforms plugin missing, try to copy from Qt install location if windeployqt was not used
+if not exist "%OUTDIR%\platforms\qwindows.dll" (
  echo Checking for platforms\qwindows.dll in common locations...
  rem Try to find in PATH (Qt bin may contain plugins folder near it)
+  for %%P in (%PATH:;= %) do (
    if exist "%%P\..\plugins\platforms\qwindows.dll" (
      echo Found qwindows.dll near %%P, copying plugins...
      xcopy "%%P\..\plugins\platforms\*" "%OUTDIR%\platforms\" /E /I /Y >nul
+      goto :PLUGINS_DONE
    )
+  )
+  echo platforms\qwindows.dll not found automatically. If GUI fails on target machine, run this script from a Qt command prompt or install Qt on build machine and rerun.
+:PLUGINS_DONE
+)
+
+echo Deployment helper finished. Check %OUTDIR% for the bundled exe, Qt DLLs, plugins, qml imports and project data.
+echo You can zip %OUTDIR% and distribute to a target machine without Qt installed.
+exit /b 0
+
*** End Patch
