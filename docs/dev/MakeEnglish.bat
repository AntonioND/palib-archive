@echo off
rd /s /q ..\english
cls

echo Running Doxygen...
pause
echo.

doxygen Doxyfile_English

echo.
echo Converting LaTeX to PDF...
pause
echo.

cd ..\english\latex
make > NUL

cd..

copy latex\refman.pdf PAlib.pdf
rd /s /q latex

echo.
echo Documentation successfully generated.
pause
