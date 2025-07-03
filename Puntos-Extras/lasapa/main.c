#include <stdio.h>
#include <string.h>
#include "funciones.h"

int main()
{
    int opcion;
    
    printf("Bienvenido al Sistema de Control de Talento Humano\n");
    
    do
    {
        opcion = menu();
        
        switch(opcion)
        {
            case 1:
                inicializarEmpleados();
                break;
            case 2:
                mostrarEmpleados();
                break;
            case 3:
                registrarAsistencia();
                break;
            case 4:
                listarAsistencias();
                break;
            case 5:
                buscarAsistencias();
                break;
            case 6:
                gestionarVacaciones();
                break;
            case 7:
                mostrarEstadisticasPersonal();
                break;
            case 8:
                mostrarRemuneraciones();
                break;
            case 9:
                gestionarAscensos();
                break;
            case 10:
                printf("\nGracias por usar el Sistema de Control de Talento Humano\n");
                break;
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
        
    } while(opcion != 10);
    
    return 0;
}