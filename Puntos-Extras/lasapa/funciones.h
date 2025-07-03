

#define MAX_EMPLEADOS 20
#define MAX_ASISTENCIAS 100
#define MAX_VACACIONES 50

typedef enum {
    AUSENTE = 0,
    A_TIEMPO = 1,
    ATRASO = 2,
    SALIDA_TEMPRANA = 3
} EstadoAsistencia;

typedef enum {
    PENDIENTE = 0,
    APROBADA = 1,
    RECHAZADA = 2
} EstadoVacacion;

typedef enum {
    OPERARIO = 0,
    ASISTENTE = 1,
    ANALISTA = 2,
    ESPECIALISTA = 3,
    COORDINADOR = 4,
    GERENTE = 5,
    DIRECTOR = 6
} NivelCargo;

struct Empleado {
    int id;
    char nombre[50];
    int cedula;
    char puesto[50];
    char correo[80];
    int telefono;
    float salario;
    int diasVacacionesTotales;
    int diasVacacionesUsados;
    NivelCargo nivel;
    int antiguedadAnios;
};

struct Asistencia {
    int idEmpleado;
    char nombre[50];
    char fecha[20];
    char horaEntrada[10];
    char horaSalida[10];
    EstadoAsistencia estado;
};

struct Vacacion {
    int idEmpleado;
    char nombre[50];
    char fechaSolicitud[20];
    char fechaInicio[20];
    char fechaFin[20];
    int diasSolicitados;
    EstadoVacacion estado;
};

// Funciones básicas
void leerCadena(char *cadena, int n);
int menu();

// Gestión de empleados
void inicializarEmpleados();
void mostrarEmpleados();
void mostrarRemuneraciones();
void gestionarAscensos();
int buscarEmpleadoPorId(int id, struct Empleado *empleadoEncontrado);
int actualizarEmpleado(struct Empleado empleado);

// Gestión de asistencias
void registrarAsistencia();
void listarAsistencias();
void buscarAsistencias();

// Gestión de vacaciones
void gestionarVacaciones();

// Estadísticas
void mostrarEstadisticasPersonal();

// Funciones de archivos
void guardarEmpleados(struct Empleado *empleados, int numEmpleados);
int leerEmpleados(struct Empleado *empleados, int maxEmpleados);
void guardarAsistencia(struct Asistencia *asistencia);
int leerAsistencias(struct Asistencia *asistencias, int maxRegistros);
void guardarVacacion(struct Vacacion *vacacion);
int leerVacaciones(struct Vacacion *vacaciones, int maxRegistros);

// Utilidades
void obtenerFechaActual(char *fecha);
const char* estadoAsistenciaToString(EstadoAsistencia estado);
const char* estadoVacacionToString(EstadoVacacion estado);
const char* nivelCargoToString(NivelCargo nivel);
