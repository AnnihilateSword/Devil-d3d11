@echo off

REM 递归删除当前目录及子目录的文件夹
for /d /r %%i in (x64) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (CMakeFiles) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (.vs) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (Debug) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (Release) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (*.dir) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (bin) do (
    if exist "%%i\" (rd /s /q "%%i")
)
for /d /r %%i in (lib) do (
    if exist "%%i\" (rd /s /q "%%i")
)

REM 递归删除当前目录及子目录的文件
for /r %%i in (*.vcxproj) do ( del "%%i" > nul 2>&1 )
for /r %%i in (*.filters) do ( del "%%i" > nul 2>&1 )
for /r %%i in (*.user) do ( del "%%i" > nul 2>&1 )
for /r %%i in (*.sln) do ( del "%%i" > nul 2>&1 )
for /r %%i in (CMakeCache.txt) do ( del "%%i" > nul 2>&1 )
for /r %%i in (cmake_install.cmake) do ( del "%%i" > nul 2>&1 )
for /r %%i in (*.cso) do ( del "%%i" > nul 2>&1 )
for /r %%i in (imgui.ini) do ( del "%%i" > nul 2>&1 )


REM 递归删除除了 Engine 目录下其他目录下的 shaders/
REM 遍历当前目录下的所有文件夹
for /d %%D in (*) do (
    REM 检查是否是 Engine 文件夹
    if /i not "%%D"=="Engine" (
        REM 检查 shaderrs 文件夹是否存在
        if exist "%%D\shaders" (
            rmdir /s /q "%%D\shaders"
        )
    )
)

REM 递归删除除了 Engine 目录下其他目录下的 content/
REM 遍历当前目录下的所有文件夹
for /d %%D in (*) do (
    REM 检查是否是 Engine 文件夹
    if /i not "%%D"=="Engine" (
        REM 检查 shaderrs 文件夹是否存在
        if exist "%%D\content" (
            rmdir /s /q "%%D\content"
        )
    )
)

echo Clean up successfully!!!
echo good luck!!!
echo.
PAUSE