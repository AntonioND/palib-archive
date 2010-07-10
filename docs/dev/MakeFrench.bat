@echo off
rd /s /q ..\french
cls

echo Running Doxygen...
pause
echo.

doxygen Doxyfile_French

echo.
echo Converting LaTeX to PDF...
pause
echo.

cd ..\french\latex
make > NUL

cd..

copy latex\refman.pdf PAlib.pdf
rd /s /q latex

echo.
echo Documentation successfully generated.
pause
