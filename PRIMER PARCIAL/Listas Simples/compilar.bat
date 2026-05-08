@echo off
title Compilador de Listas Simples
echo -----------------------------------
echo Compilando: Main, Lista y Nodo...
echo -----------------------------------
g++ Main.cpp Lista.cpp Nodo.cpp -L. -lvalidarCedula -o Main.exe

if %errorlevel% == 0 (
    echo [EXITO] Compilacion terminada.
    echo -----------------------------------
    Main.exe
) else (
    echo -----------------------------------
    echo [ERROR] No se pudo compilar el proyecto.
    echo Revisa que no tengas el programa Main.exe abierto.
    pause
)