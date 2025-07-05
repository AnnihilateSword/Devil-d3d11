@echo off

REM omits the number after parallel, and CMake attempts to automatically detect 
REM and use the most appropriate number of threads.
REM This is usually based on the number of CPU cores in your system.

call cmake --build . --parallel --config Debug
PAUSE

阿斯顿