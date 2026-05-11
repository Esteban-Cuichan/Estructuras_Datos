@echo off
title Compilador Proyecto Estructuras
echo -----------------------------------
echo Compilando todo el proyecto modular...
echo -----------------------------------
set RUTA_CEDULA="..\Librerias\ValidarCedula"
set RUTA_REGEX="..\Librerias\validarExpresion"
g++ Main.cpp Lista.cpp Nodo.cpp ^
 "%RUTA_CEDULA%\validarCedula.cpp" ^
 "%RUTA_REGEX%\validarExpresion.cpp" ^
 -I%RUTA_CEDULA% ^
 -I%RUTA_REGEX% ^
 -o Main.exe

if %errorlevel% == 0 (
    echo [EXITO] Compilacion terminada. ¡A dar de alta usuarios!
    echo -----------------------------------
    Main.exe
) else (
    echo -----------------------------------
    echo [ERROR] No se pudo compilar. 
    echo Verifica que los nombres de las carpetas coincidan exactamente.
    pause
)