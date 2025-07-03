#include <stdio.h>
#include <string.h>
#include "funciones.h"

// Funciones de utilidad
void leerCadena(char *cadena, int n)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    if (fgets(cadena, n, stdin))
    {
        int len = strlen(cadena) - 1;
        if (len >= 0 && cadena[len] == '\n')
            cadena[len] = '\0';
    }
    else
        cadena[0] = '\0';
}

// Versión simplificada sin time.h - pide al usuario ingresar la fecha
void obtenerFechaActual(char *fecha)
{
    printf("Ingrese la fecha actual (DD/MM/AAAA): ");
    leerCadena(fecha, 20);
}

const char *estadoAsistenciaToString(EstadoAsistencia estado)
{
    const char *estados[] = {"Ausente", "A tiempo", "Atraso", "Salida temprana"};
    return estados[estado];
}

const char *estadoVacacionToString(EstadoVacacion estado)
{
    const char *estados[] = {"Pendiente", "Aprobada", "Rechazada"};
    return estados[estado];
}

const char *nivelCargoToString(NivelCargo nivel)
{
    const char *niveles[] = {"Operario", "Asistente", "Analista", "Especialista", 
                            "Coordinador", "Gerente", "Director"};
    return niveles[nivel];
}

// Funciones de archivos
void guardarEmpleados(struct Empleado *empleados, int numEmpleados)
{
    FILE *f = fopen("empleados.bin", "wb");
    if (!f)
    {
        printf("Error al abrir el archivo de empleados.\n");
        return;
    }
    fwrite(empleados, sizeof(struct Empleado), numEmpleados, f);
    fclose(f);
    printf("Datos de empleados guardados correctamente.\n");
}

int leerEmpleados(struct Empleado *empleados, int maxEmpleados)
{
    FILE *f = fopen("empleados.bin", "rb");
    if (!f)
        return 0;
    int numLeidos = fread(empleados, sizeof(struct Empleado), maxEmpleados, f);
    fclose(f);
    return numLeidos;
}

void guardarAsistencia(struct Asistencia *asistencia)
{
    FILE *f = fopen("asistencias.bin", "ab");
    if (!f)
    {
        printf("Error al abrir el archivo de asistencias.\n");
        return;
    }
    fwrite(asistencia, sizeof(struct Asistencia), 1, f);
    fclose(f);
}

int leerAsistencias(struct Asistencia *asistencias, int maxRegistros)
{
    FILE *f = fopen("asistencias.bin", "rb");
    if (!f)
        return 0;
    int numLeidos = fread(asistencias, sizeof(struct Asistencia), maxRegistros, f);
    fclose(f);
    return numLeidos;
}

void guardarVacacion(struct Vacacion *vacacion)
{
    FILE *f = fopen("vacaciones.bin", "ab");
    if (!f)
    {
        printf("Error al abrir el archivo de vacaciones.\n");
        return;
    }
    fwrite(vacacion, sizeof(struct Vacacion), 1, f);
    fclose(f);
}

int leerVacaciones(struct Vacacion *vacaciones, int maxRegistros)
{
    FILE *f = fopen("vacaciones.bin", "rb");
    if (!f)
        return 0;
    int numLeidos = fread(vacaciones, sizeof(struct Vacacion), maxRegistros, f);
    fclose(f);
    return numLeidos;
}

// Menú principal
int menu()
{
    int opc;
    printf("\n===============================================\n");
    printf("      SISTEMA DE CONTROL DE TALENTO HUMANO    \n");
    printf("===============================================\n");
    printf("1. Inicializar empleados\n");
    printf("2. Mostrar empleados\n");
    printf("3. Registrar asistencia\n");
    printf("4. Listar asistencias\n");
    printf("5. Buscar asistencias\n");
    printf("6. Gestionar vacaciones\n");
    printf("7. Ver estadisticas\n");
    printf("8. Mostrar remuneraciones\n");
    printf("9. Gestionar ascensos\n");
    printf("10. Salir\n");
    printf(">>> ");
    scanf("%d", &opc);
    return opc;
}

// Gestión de empleados
int buscarEmpleadoPorId(int id, struct Empleado *empleadoEncontrado)
{
    struct Empleado empleados[MAX_EMPLEADOS];
    int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);

    for (int i = 0; i < numEmpleados; i++)
    {
        if (empleados[i].id == id)
        {
            *empleadoEncontrado = empleados[i];
            return 1;
        }
    }
    return 0;
}

int actualizarEmpleado(struct Empleado empleado)
{
    struct Empleado empleados[MAX_EMPLEADOS];
    int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);

    for (int i = 0; i < numEmpleados; i++)
    {
        if (empleados[i].id == empleado.id)
        {
            empleados[i] = empleado;
            guardarEmpleados(empleados, numEmpleados);
            return 1;
        }
    }
    return 0;
}

void inicializarEmpleados()
{
    struct Empleado empleados[5];

    // Verificar si ya existen empleados
    FILE *f = fopen("empleados.bin", "rb");
    if (f)
    {
        printf("Ya existe un archivo de empleados! Sobreescribir? (1=Si, 0=No): ");
        int respuesta;
        scanf("%d", &respuesta);
        fclose(f);
        if (!respuesta)
        {
            printf("Operacion cancelada.\n");
            return;
        }
    }

    // Datos pre-establecidos para 5 empleados con niveles de cargo y antiguedad
    int ids[] = {1001, 1002, 1003, 1004, 1005};
    char nombres[][50] = {"Juan Perez", "Maria Lopez", "Carlos Ruiz", "Ana Suarez", "Pedro Mora"};
    int cedulas[] = {1712345678, 1723456789, 1734567890, 1745678901, 1756789012};
    char puestos[][50] = {"Mecanico", "Administradora", "Vendedor", "Contadora", "Gerente"};
    char correos[][80] = {
        "juan.perez@autoempresa.com",
        "maria.lopez@autoempresa.com",
        "carlos.ruiz@autoempresa.com",
        "ana.suarez@autoempresa.com",
        "pedro.mora@autoempresa.com"};
    int telefonos[] = {987654321, 987654322, 987654323, 987654324, 987654325};
    float salarios[] = {700.00, 1200.00, 600.00, 1100.00, 2200.00};
    int diasTotales[] = {15, 20, 12, 18, 25};
    int diasUsados[] = {0, 5, 0, 8, 10};
    NivelCargo niveles[] = {OPERARIO, COORDINADOR, ASISTENTE, ESPECIALISTA, GERENTE};
    int antiguedad[] = {1, 5, 2, 4, 8};

    // Crear los 5 empleados
    for (int i = 0; i < 5; i++)
    {
        empleados[i].id = ids[i];
        strcpy(empleados[i].nombre, nombres[i]);
        empleados[i].cedula = cedulas[i];
        strcpy(empleados[i].puesto, puestos[i]);
        strcpy(empleados[i].correo, correos[i]);
        empleados[i].telefono = telefonos[i];
        empleados[i].salario = salarios[i];
        empleados[i].diasVacacionesTotales = diasTotales[i];
        empleados[i].diasVacacionesUsados = diasUsados[i];
        empleados[i].nivel = niveles[i];
        empleados[i].antiguedadAnios = antiguedad[i];
    }

    guardarEmpleados(empleados, 5);
    printf("\nEmpleados inicializados correctamente.\n");
}

void mostrarEmpleados()
{
    struct Empleado empleados[MAX_EMPLEADOS];
    int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);

    if (numEmpleados == 0)
    {
        printf("\nNo hay empleados registrados.\n\n");
        return;
    }

    printf("\n================================================================================\n");
    printf("                             LISTA DE EMPLEADOS                                 \n");
    printf("================================================================================\n");
    printf("| ID   | Nombre            | Cedula     | Puesto          | Salario  | Vacaciones |\n");
    printf("================================================================================\n");

    for (int i = 0; i < numEmpleados; i++)
    {
        printf("| %4d | %-17s | %10d | %-15s | %8.2f | %2d/%2d     |\n",
               empleados[i].id, empleados[i].nombre, empleados[i].cedula,
               empleados[i].puesto, empleados[i].salario,
               empleados[i].diasVacacionesUsados, empleados[i].diasVacacionesTotales);
    }

    printf("================================================================================\n\n");
}

// Nueva función para mostrar remuneraciones detalladas
void mostrarRemuneraciones()
{
    struct Empleado empleados[MAX_EMPLEADOS];
    int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);

    if (numEmpleados == 0)
    {
        printf("\nNo hay empleados registrados.\n\n");
        return;
    }

    printf("\n============================================================================================\n");
    printf("                           REMUNERACIONES DE EMPLEADOS                                      \n");
    printf("============================================================================================\n");
    printf("| ID   | Nombre            | Puesto          | Nivel          | Antiguedad | Salario Base  |\n");
    printf("============================================================================================\n");

    for (int i = 0; i < numEmpleados; i++)
    {
        printf("| %4d | %-17s | %-15s | %-14s | %2d anios   | $%10.2f |\n",
               empleados[i].id, 
               empleados[i].nombre, 
               empleados[i].puesto,
               nivelCargoToString(empleados[i].nivel),
               empleados[i].antiguedadAnios,
               empleados[i].salario);
    }

    printf("============================================================================================\n\n");
}

// Nueva función para gestionar ascensos y cambios de remuneración
void gestionarAscensos()
{
    struct Empleado empleado;
    int idEmpleado;

    printf("\n=== GESTION DE ASCENSOS Y REMUNERACIONES ===\n");
    mostrarRemuneraciones();

    printf("Ingrese el ID del empleado para ascenso/ajuste: ");
    scanf("%d", &idEmpleado);

    if (!buscarEmpleadoPorId(idEmpleado, &empleado))
    {
        printf("Error: No se encontro el empleado con ID %d.\n", idEmpleado);
        return;
    }

    printf("\nEmpleado: %s (ID: %d)\n", empleado.nombre, empleado.id);
    printf("Cargo actual: %s\n", empleado.puesto);
    printf("Nivel actual: %s\n", nivelCargoToString(empleado.nivel));
    printf("Salario actual: $%.2f\n", empleado.salario);
    printf("Antiguedad: %d anios\n\n", empleado.antiguedadAnios);

    printf("Opciones:\n");
    printf("1. Realizar ascenso de nivel\n");
    printf("2. Ajustar salario manualmente\n");
    printf("3. Aplicar aumento por antiguedad (5%% por anio)\n");
    printf("4. Cancelar\n");
    printf(">>> ");

    int opcion;
    scanf("%d", &opcion);

    switch (opcion)
    {
    case 1: // Ascenso de nivel
        if (empleado.nivel < DIRECTOR)
        {
            empleado.nivel++;
            
            // Aumento de salario basado en el nuevo nivel (20% por nivel)
            float aumento = empleado.salario * 0.20;
            empleado.salario += aumento;

            printf("\nAscenso realizado:\n");
            printf("Nuevo nivel: %s\n", nivelCargoToString(empleado.nivel));
            printf("Nuevo salario: $%.2f (+$%.2f)\n", empleado.salario, aumento);
            
            printf("\nDesea actualizar el nombre del puesto? (1=Si, 0=No): ");
            int actualizarPuesto;
            scanf("%d", &actualizarPuesto);

            if (actualizarPuesto)
            {
                printf("Ingrese el nuevo nombre del puesto: ");
                leerCadena(empleado.puesto, 50);
            }

            actualizarEmpleado(empleado);
            printf("\nAscenso registrado exitosamente.\n");
        }
        else
        {
            printf("El empleado ya tiene el nivel maximo (Director).\n");
        }
        break;

    case 2: // Ajuste manual
        printf("Ingrese el nuevo salario: $");
        float nuevoSalario;
        scanf("%f", &nuevoSalario);

        if (nuevoSalario < empleado.salario)
        {
            printf("Advertencia: El nuevo salario es menor que el actual.\n");
            printf("Confirmar reduccion de salario? (1=Si, 0=No): ");
            int confirmar;
            scanf("%d", &confirmar);
            
            if (!confirmar)
            {
                printf("Operacion cancelada.\n");
                break;
            }
        }

        float diferencia = nuevoSalario - empleado.salario;
        empleado.salario = nuevoSalario;
        
        actualizarEmpleado(empleado);
        printf("\nSalario actualizado: $%.2f (%s$%.2f)\n", 
               empleado.salario, 
               diferencia >= 0 ? "+" : "", 
               diferencia);
        break;

    case 3: // Aumento por antigüedad
        {
            float porcentaje = empleado.antiguedadAnios * 0.05; // 5% por año
            float aumento = empleado.salario * porcentaje;
            empleado.salario += aumento;

            actualizarEmpleado(empleado);
            printf("\nAumento por antiguedad aplicado:\n");
            printf("Porcentaje: %.1f%% (%d anios)\n", porcentaje * 100, empleado.antiguedadAnios);
            printf("Nuevo salario: $%.2f (+$%.2f)\n", empleado.salario, aumento);
        }
        break;

    case 4:
        printf("Operacion cancelada.\n");
        break;

    default:
        printf("Opcion invalida.\n");
    }
}

// Gestión de asistencias
void registrarAsistencia()
{
    struct Asistencia nuevaAsistencia;
    struct Empleado empleado;
    int idEmpleado;

    printf("\n=== REGISTRAR ASISTENCIA ===\n");
    mostrarEmpleados();

    printf("Ingrese el ID del empleado: ");
    scanf("%d", &idEmpleado);

    if (!buscarEmpleadoPorId(idEmpleado, &empleado))
    {
        printf("Error: No se encontro un empleado con el ID %d.\n", idEmpleado);
        return;
    }

    // Completar datos básicos
    nuevaAsistencia.idEmpleado = empleado.id;
    strcpy(nuevaAsistencia.nombre, empleado.nombre);

    // Fecha
    printf("Usar fecha actual? (1=Si, 0=No): ");
    int usarFechaActual;
    scanf("%d", &usarFechaActual);

    if (usarFechaActual)
    {
        obtenerFechaActual(nuevaAsistencia.fecha);
    }
    else
    {
        printf("Ingrese la fecha (DD/MM/AAAA): ");
        leerCadena(nuevaAsistencia.fecha, 20);
    }

    printf("Hora de entrada (HH:MM): ");
    leerCadena(nuevaAsistencia.horaEntrada, 10);

    printf("Hora de salida (HH:MM o vacio): ");
    leerCadena(nuevaAsistencia.horaSalida, 10);

    printf("Estado (0=Ausente, 1=A tiempo, 2=Atraso, 3=Salida temprana): ");
    int estado;
    scanf("%d", &estado);
    nuevaAsistencia.estado = (EstadoAsistencia)estado;

    guardarAsistencia(&nuevaAsistencia);

    printf("\n=== RESUMEN DE REGISTRO ===\n");
    printf("Empleado: %s (ID: %d)\n", nuevaAsistencia.nombre, nuevaAsistencia.idEmpleado);
    printf("Fecha: %s\n", nuevaAsistencia.fecha);
    printf("Entrada: %s\n", nuevaAsistencia.horaEntrada);
    printf("Salida: %s\n", strlen(nuevaAsistencia.horaSalida) > 0 ? nuevaAsistencia.horaSalida : "(No registrada)");
    printf("Estado: %s\n", estadoAsistenciaToString(nuevaAsistencia.estado));
    printf("Registro guardado con exito!\n\n");
}

void listarAsistencias()
{
    struct Asistencia asistencias[MAX_ASISTENCIAS];
    int numAsistencias = leerAsistencias(asistencias, MAX_ASISTENCIAS);

    if (numAsistencias == 0)
    {
        printf("\nNo hay registros de asistencia.\n\n");
        return;
    }

    printf("\n====================================================================================\n");
    printf("                            REGISTROS DE ASISTENCIA                                \n");
    printf("====================================================================================\n");
    printf("| ID   | Nombre            | Fecha       | Entrada | Salida  | Estado             |\n");
    printf("====================================================================================\n");

    for (int i = 0; i < numAsistencias; i++)
    {
        printf("| %4d | %-17s | %-11s | %-7s | %-7s | %-18s |\n",
               asistencias[i].idEmpleado, asistencias[i].nombre,
               asistencias[i].fecha, asistencias[i].horaEntrada,
               asistencias[i].horaSalida[0] ? asistencias[i].horaSalida : "N/A",
               estadoAsistenciaToString(asistencias[i].estado));
    }

    printf("====================================================================================\n\n");
}

void mostrarAsistenciasPorFiltro(const char *titulo,
                                 struct Asistencia *asistencias,
                                 int numAsistencias,
                                 int (*filtro)(struct Asistencia *, void *),
                                 void *parametro)
{

    printf("\n====================================================================================\n");
    printf("                         %s                                \n", titulo);
    printf("====================================================================================\n");
    printf("| ID   | Nombre            | Fecha       | Entrada | Salida  | Estado             |\n");
    printf("====================================================================================\n");

    int encontrado = 0;

    for (int i = 0; i < numAsistencias; i++)
    {
        if (filtro(&asistencias[i], parametro))
        {
            printf("| %4d | %-17s | %-11s | %-7s | %-7s | %-18s |\n",
                   asistencias[i].idEmpleado, asistencias[i].nombre,
                   asistencias[i].fecha, asistencias[i].horaEntrada,
                   asistencias[i].horaSalida[0] ? asistencias[i].horaSalida : "N/A",
                   estadoAsistenciaToString(asistencias[i].estado));
            encontrado = 1;
        }
    }

    if (!encontrado)
    {
        printf("No se encontraron registros que cumplan el criterio.\n");
    }

    printf("====================================================================================\n");
}

int filtroPorId(struct Asistencia *asistencia, void *param)
{
    int id = *(int *)param;
    return asistencia->idEmpleado == id;
}

int filtroPorFecha(struct Asistencia *asistencia, void *param)
{
    char *fecha = (char *)param;
    return strcmp(asistencia->fecha, fecha) == 0;
}

int filtroPorAtraso(struct Asistencia *asistencia, void *param)
{
    return asistencia->estado == ATRASO;
}

int filtroPorHora(struct Asistencia *asistencia, void *param)
{
    char *hora = (char *)param;
    return strcmp(asistencia->horaEntrada, hora) == 0;
}

void buscarAsistencias()
{
    struct Asistencia asistencias[MAX_ASISTENCIAS];
    int numAsistencias = leerAsistencias(asistencias, MAX_ASISTENCIAS);

    if (numAsistencias == 0)
    {
        printf("\nNo hay registros de asistencia para buscar.\n\n");
        return;
    }

    printf("\n=== BUSCAR REGISTROS DE ASISTENCIA ===\n");
    printf("1. Por ID de empleado\n");
    printf("2. Por fecha\n");
    printf("3. Mostrar atrasos\n");
    printf("4. Mostrar empleados sin registro\n");
    printf("5. Por hora de entrada\n");
    printf("Opcion: ");
    int opcion;
    scanf("%d", &opcion);

    switch (opcion)
    {
    case 1:
    {
        int idBuscar;
        printf("ID del empleado: ");
        scanf("%d", &idBuscar);

        char titulo[80];
        sprintf(titulo, "REGISTROS PARA EMPLEADO ID: %-4d", idBuscar);
        mostrarAsistenciasPorFiltro(titulo, asistencias, numAsistencias, filtroPorId, &idBuscar);
        break;
    }
    case 2:
    {
        char fechaBuscar[20];
        printf("Fecha a buscar (DD/MM/AAAA): ");
        leerCadena(fechaBuscar, 20);

        char titulo[80];
        sprintf(titulo, "REGISTROS PARA FECHA: %-11s", fechaBuscar);
        mostrarAsistenciasPorFiltro(titulo, asistencias, numAsistencias, filtroPorFecha, fechaBuscar);
        break;
    }
    case 3:
    {
        mostrarAsistenciasPorFiltro("REGISTROS DE ATRASOS", asistencias, numAsistencias, filtroPorAtraso, NULL);
        break;
    }
    case 4:
    {
        struct Empleado empleados[MAX_EMPLEADOS];
        int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);

        char fechaVerificar[20];
        printf("Fecha para verificar ausencias (DD/MM/AAAA): ");
        leerCadena(fechaVerificar, 20);

        printf("\n====================================================================================\n");
        printf("          EMPLEADOS SIN REGISTRO DE ASISTENCIA PARA: %-11s               \n", fechaVerificar);
        printf("====================================================================================\n");
        printf("| ID   | Nombre            | Puesto           | Telefono   |\n");
        printf("====================================================================================\n");

        int encontrado = 0;

        for (int i = 0; i < numEmpleados; i++)
        {
            int tieneRegistro = 0;

            for (int j = 0; j < numAsistencias; j++)
            {
                if (empleados[i].id == asistencias[j].idEmpleado &&
                    strcmp(asistencias[j].fecha, fechaVerificar) == 0)
                {
                    tieneRegistro = 1;
                    break;
                }
            }

            if (!tieneRegistro)
            {
                printf("| %4d | %-17s | %-16s | %10d |\n",
                       empleados[i].id, empleados[i].nombre,
                       empleados[i].puesto, empleados[i].telefono);
                encontrado = 1;
            }
        }

        if (!encontrado)
            printf("Todos los empleados tienen registro para la fecha indicada.\n");

        printf("====================================================================================\n");
        break;
    }
    case 5:
    {
        char horaBuscar[10];
        printf("Hora especifica a buscar (HH:MM): ");
        leerCadena(horaBuscar, 10);

        char titulo[80];
        sprintf(titulo, "REGISTROS CON HORA DE ENTRADA: %-7s", horaBuscar);
        mostrarAsistenciasPorFiltro(titulo, asistencias, numAsistencias, filtroPorHora, horaBuscar);
        break;
    }
    default:
        printf("Opcion invalida.\n");
    }
}

// Gestión de vacaciones
void gestionarVacaciones()
{
    printf("\n===============================================\n");
    printf("            GESTION DE VACACIONES             \n");
    printf("===============================================\n");

    int opcion;
    do
    {
        printf("\n1. Solicitar vacaciones\n");
        printf("2. Ver solicitudes pendientes\n");
        printf("3. Aprobar/Rechazar solicitudes\n");
        printf("4. Ver historial de vacaciones\n");
        printf("5. Volver al menu principal\n");
        printf(">>> ");
        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        { // Solicitar vacaciones
            struct Vacacion nuevaSolicitud;
            struct Empleado empleado;
            int idEmpleado;

            printf("\n=== SOLICITAR VACACIONES ===\n");
            mostrarEmpleados();

            printf("ID del empleado: ");
            scanf("%d", &idEmpleado);

            if (!buscarEmpleadoPorId(idEmpleado, &empleado))
            {
                printf("Error: No se encontro el empleado con ID %d.\n", idEmpleado);
                break;
            }

            // Verificar días disponibles
            int diasDisponibles = empleado.diasVacacionesTotales - empleado.diasVacacionesUsados;
            if (diasDisponibles <= 0)
            {
                printf("Error: El empleado no tiene dias disponibles.\n");
                break;
            }

            printf("Empleado: %s (Dias disponibles: %d)\n",
                   empleado.nombre, diasDisponibles);

            nuevaSolicitud.idEmpleado = empleado.id;
            strcpy(nuevaSolicitud.nombre, empleado.nombre);
            obtenerFechaActual(nuevaSolicitud.fechaSolicitud);

            printf("Fecha inicio (DD/MM/AAAA): ");
            leerCadena(nuevaSolicitud.fechaInicio, 20);

            printf("Fecha fin (DD/MM/AAAA): ");
            leerCadena(nuevaSolicitud.fechaFin, 20);

            printf("Dias solicitados: ");
            scanf("%d", &nuevaSolicitud.diasSolicitados);

            if (nuevaSolicitud.diasSolicitados > diasDisponibles)
            {
                printf("Error: Solicito mas dias de los disponibles.\n");
                break;
            }

            nuevaSolicitud.estado = PENDIENTE;
            guardarVacacion(&nuevaSolicitud);

            printf("\nSolicitud registrada (estado: pendiente).\n");
            break;
        }
        case 2:
        { // Ver solicitudes pendientes
            struct Vacacion vacaciones[MAX_VACACIONES];
            int numVacaciones = leerVacaciones(vacaciones, MAX_VACACIONES);

            if (numVacaciones == 0)
            {
                printf("\nNo hay solicitudes registradas.\n");
                break;
            }

            printf("\n====================================================================================\n");
            printf("                        SOLICITUDES DE VACACIONES PENDIENTES                        \n");
            printf("====================================================================================\n");
            printf("| ID   | Nombre            | Fecha Solicitud | Inicio     | Fin        | Dias  |\n");
            printf("====================================================================================\n");

            int encontrado = 0;
            for (int i = 0; i < numVacaciones; i++)
            {
                if (vacaciones[i].estado == PENDIENTE)
                {
                    printf("| %4d | %-17s | %-15s | %-10s | %-10s | %5d |\n",
                           vacaciones[i].idEmpleado, vacaciones[i].nombre,
                           vacaciones[i].fechaSolicitud, vacaciones[i].fechaInicio,
                           vacaciones[i].fechaFin, vacaciones[i].diasSolicitados);
                    encontrado = 1;
                }
            }

            if (!encontrado)
                printf("No hay solicitudes pendientes.\n");

            printf("====================================================================================\n");
            break;
        }
        case 3:
        { // Aprobar/rechazar solicitudes
            struct Vacacion vacaciones[MAX_VACACIONES];
            int numVacaciones = leerVacaciones(vacaciones, MAX_VACACIONES);

            if (numVacaciones == 0)
            {
                printf("\nNo hay solicitudes registradas.\n");
                break;
            }

            // Listar solicitudes pendientes
            printf("\n====================================================================================\n");
            printf("                        SOLICITUDES DE VACACIONES PENDIENTES                        \n");
            printf("====================================================================================\n");
            printf("| # | ID   | Nombre            | Inicio     | Fin        | Dias  |\n");
            printf("====================================================================================\n");

            int solicitudesPendientes[MAX_VACACIONES];
            int numPendientes = 0;

            for (int i = 0; i < numVacaciones; i++)
            {
                if (vacaciones[i].estado == PENDIENTE)
                {
                    solicitudesPendientes[numPendientes] = i;
                    printf("| %d | %4d | %-17s | %-10s | %-10s | %5d |\n",
                           numPendientes, vacaciones[i].idEmpleado, vacaciones[i].nombre,
                           vacaciones[i].fechaInicio, vacaciones[i].fechaFin,
                           vacaciones[i].diasSolicitados);
                    numPendientes++;
                }
            }

            if (numPendientes == 0)
            {
                printf("No hay solicitudes pendientes.\n");
                printf("====================================================================================\n");
                break;
            }

            printf("====================================================================================\n");

            // Procesar una solicitud
            int numSolicitud;
            printf("\nNumero de solicitud a procesar: ");
            scanf("%d", &numSolicitud);

            if (numSolicitud < 0 || numSolicitud >= numPendientes)
            {
                printf("Numero de solicitud invalido.\n");
                break;
            }

            int indice = solicitudesPendientes[numSolicitud];

            printf("\nDetalles de solicitud #%d:\n", numSolicitud);
            printf("Empleado: %s (ID: %d)\n", vacaciones[indice].nombre, vacaciones[indice].idEmpleado);
            printf("Fecha solicitud: %s\n", vacaciones[indice].fechaSolicitud);
            printf("Periodo: %s al %s (%d dias)\n",
                   vacaciones[indice].fechaInicio, vacaciones[indice].fechaFin,
                   vacaciones[indice].diasSolicitados);

            printf("\n1. Aprobar\n2. Rechazar\n3. Cancelar\n>>> ");
            int decision;
            scanf("%d", &decision);

            if (decision == 3)
            {
                printf("Operacion cancelada.\n");
                break;
            }

            if (decision != 1 && decision != 2)
            {
                printf("Opcion invalida.\n");
                break;
            }

            // Actualizar estado
            vacaciones[indice].estado = (decision == 1) ? APROBADA : RECHAZADA;

            // Si se aprueba, actualizar días usados
            if (decision == 1)
            {
                struct Empleado empleado;
                if (buscarEmpleadoPorId(vacaciones[indice].idEmpleado, &empleado))
                {
                    empleado.diasVacacionesUsados += vacaciones[indice].diasSolicitados;
                    actualizarEmpleado(empleado);
                }
            }

            // Guardar cambios
            FILE *f = fopen("vacaciones.bin", "wb");
            if (f)
            {
                fwrite(vacaciones, sizeof(struct Vacacion), numVacaciones, f);
                fclose(f);
                printf("\nSolicitud %s exitosamente.\n",
                       (decision == 1) ? "aprobada" : "rechazada");
            }
            else
            {
                printf("Error al guardar cambios.\n");
            }
            break;
        }
        case 4:
        { // Ver historial
            struct Vacacion vacaciones[MAX_VACACIONES];
            int numVacaciones = leerVacaciones(vacaciones, MAX_VACACIONES);

            if (numVacaciones == 0)
            {
                printf("\nNo hay solicitudes registradas.\n");
                break;
            }

            printf("\n====================================================================================\n");
            printf("                          HISTORIAL DE VACACIONES                                  \n");
            printf("====================================================================================\n");
            printf("| ID   | Nombre            | Inicio     | Fin        | Dias  | Estado      |\n");
            printf("====================================================================================\n");

            for (int i = 0; i < numVacaciones; i++)
            {
                printf("| %4d | %-17s | %-10s | %-10s | %5d | %-11s |\n",
                       vacaciones[i].idEmpleado, vacaciones[i].nombre,
                       vacaciones[i].fechaInicio, vacaciones[i].fechaFin,
                       vacaciones[i].diasSolicitados,
                       estadoVacacionToString(vacaciones[i].estado));
            }

            printf("====================================================================================\n");
            break;
        }
        case 5:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion invalida.\n");
        }
    } while (opcion != 5);
}

// Estadísticas
void mostrarEstadisticasPersonal()
{
    struct Asistencia asistencias[MAX_ASISTENCIAS];
    struct Empleado empleados[MAX_EMPLEADOS];
    struct Vacacion vacaciones[MAX_VACACIONES];

    int numAsistencias = leerAsistencias(asistencias, MAX_ASISTENCIAS);
    int numEmpleados = leerEmpleados(empleados, MAX_EMPLEADOS);
    int numVacaciones = leerVacaciones(vacaciones, MAX_VACACIONES);

    if (numEmpleados == 0)
    {
        printf("\nNo hay empleados para mostrar estadisticas.\n\n");
        return;
    }

    printf("\n===============================================\n");
    printf("           ESTADISTICAS DE PERSONAL           \n");
    printf("===============================================\n");

    // Resumen general
    printf("\n=== RESUMEN GENERAL ===\n");
    printf("Total empleados: %d\n", numEmpleados);
    printf("Total registros de asistencia: %d\n", numAsistencias);
    printf("Total solicitudes de vacaciones: %d\n", numVacaciones);

    // Estadísticas de asistencia
    int totalAtrasos = 0, totalSalidasTempranas = 0, totalAusencias = 0;

    for (int i = 0; i < numAsistencias; i++)
    {
        switch (asistencias[i].estado)
        {
        case AUSENTE:
            totalAusencias++;
            break;
        case ATRASO:
            totalAtrasos++;
            break;
        case SALIDA_TEMPRANA:
            totalSalidasTempranas++;
            break;
        }
    }

    printf("\n=== ESTADISTICAS DE ASISTENCIA ===\n");
    printf("Atrasos: %d (%.1f%%)\n", totalAtrasos,
           numAsistencias > 0 ? (float)totalAtrasos * 100 / numAsistencias : 0);
    printf("Salidas tempranas: %d\n", totalSalidasTempranas);
    printf("Ausencias: %d (%.1f%%)\n", totalAusencias,
           numAsistencias > 0 ? (float)totalAusencias * 100 / numAsistencias : 0);

    // Estadísticas de vacaciones
    int vacPendientes = 0, vacAprobadas = 0, vacRechazadas = 0, totalDias = 0;

    for (int i = 0; i < numVacaciones; i++)
    {
        switch (vacaciones[i].estado)
        {
        case PENDIENTE:
            vacPendientes++;
            break;
        case APROBADA:
            vacAprobadas++;
            totalDias += vacaciones[i].diasSolicitados;
            break;
        case RECHAZADA:
            vacRechazadas++;
            break;
        }
    }

    printf("\n=== ESTADISTICAS DE VACACIONES ===\n");
    printf("Solicitudes pendientes: %d\n", vacPendientes);
    printf("Solicitudes aprobadas: %d\n", vacAprobadas);
    printf("Solicitudes rechazadas: %d\n", vacRechazadas);

    if (vacAprobadas > 0)
    {
        printf("Promedio dias por vacacion: %.1f dias\n", (float)totalDias / vacAprobadas);
    }

    // Top empleados con atrasos
    int atrasosPorEmpleado[MAX_EMPLEADOS] = {0};

    for (int i = 0; i < numAsistencias; i++)
    {
        if (asistencias[i].estado == ATRASO)
        {
            for (int j = 0; j < numEmpleados; j++)
            {
                if (asistencias[i].idEmpleado == empleados[j].id)
                {
                    atrasosPorEmpleado[j]++;
                    break;
                }
            }
        }
    }

    // Ordenar por número de atrasos (descendente)
    int indices[MAX_EMPLEADOS];
    for (int i = 0; i < numEmpleados; i++)
        indices[i] = i;

    for (int i = 0; i < numEmpleados - 1; i++)
    {
        for (int j = 0; j < numEmpleados - i - 1; j++)
        {
            if (atrasosPorEmpleado[indices[j]] < atrasosPorEmpleado[indices[j + 1]])
            {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    printf("\n=== TOP 3 EMPLEADOS CON MAS ATRASOS ===\n");
    printf("--------------------------------------------------\n");
    printf("| Nombre            | Puesto           | Atrasos |\n");
    printf("--------------------------------------------------\n");

    int mostrados = 0;
    for (int i = 0; i < numEmpleados && mostrados < 3; i++)
    {
        int idx = indices[i];
        if (atrasosPorEmpleado[idx] > 0)
        {
            printf("| %-17s | %-16s | %7d |\n",
                   empleados[idx].nombre, empleados[idx].puesto, atrasosPorEmpleado[idx]);
            mostrados++;
        }
    }

    if (mostrados == 0)
        printf("No hay registros de atrasos.\n");

    printf("--------------------------------------------------\n\n");
}