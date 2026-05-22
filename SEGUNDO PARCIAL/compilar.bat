@echo off
title Compilador Proyecto Estructuras
echo -----------------------------------
echo Compilando todo el proyecto modular...
echo -----------------------------------

:: Definimos las rutas (aquí las dejamos normales)
set RUTA_CEDULA=..\PRIMER PARCIAL\Librerias\ValidarCedula
set RUTA_REGEX=..\PRIMER PARCIAL\Librerias\validarExpresion

:: IMPORTANTE: Envolvemos cada ruta entre comillas "%variable%" para proteger el espacio en blanco
g++ Main.cpp Lista.cpp Nodo.cpp ^
 "%RUTA_CEDULA%\validarCedula.cpp" ^
 "%RUTA_REGEX%\validarExpresion.cpp" ^
 -I"%RUTA_CEDULA%" ^
 -I"%RUTA_REGEX%" ^
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