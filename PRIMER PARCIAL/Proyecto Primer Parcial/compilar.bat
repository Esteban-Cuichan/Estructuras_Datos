@echo off
g++ *.cpp -o main
if %errorlevel% == 0 (
    echo Compilacion exitosa. Ejecutando...
    echo -----------------------------------
    main.exe
) else (
    echo -----------------------------------
    echo Hubo errores en la compilacion.
)