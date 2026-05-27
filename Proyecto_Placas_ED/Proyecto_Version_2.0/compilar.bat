@echo off
chcp 65001 > nul
cls
echo =======================================================
echo          COMPILANDO PROYECTO DE ESTRUCTURAS
echo =======================================================
echo Procesando archivos de codigo fuente...
g++ main.cpp ListaDCE.cpp Nodo.cpp Turno.cpp Persona.cpp Vehiculo.cpp Fecha.cpp validarExpresion.cpp validarCedula.cpp -o main
if %errorlevel% equ 0 (
    echo [EXITO] Compilacion completada sin errores.
    echo Lanzando la aplicacion interactiva...
    echo -------------------------------------------------------
    timeout /t 1 > nul
    main.exe
) else (
    echo.
    echo [ERROR] La compilacion ha fallado. Revisa los errores de arriba.
    pause
)