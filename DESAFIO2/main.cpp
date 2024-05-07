#include <iostream>
#include <string>

using namespace std;

class Estacion {
private:
    string nombre;
    int tiempoSiguiente;
    Estacion* siguiente;
    bool esTransferencia;

public:
    Estacion(string nombre, int tiempoSiguiente) : nombre(nombre), tiempoSiguiente(tiempoSiguiente), siguiente(nullptr), esTransferencia(false) {}

    string obtenerNombre() const {
        return nombre;
    }

    int obtenerTiempoSiguiente() const {
        return tiempoSiguiente;
    }

    Estacion* obtenerSiguiente() const {
        return siguiente;
    }

    void establecerSiguiente(Estacion* siguienteEstacion) {
        siguiente = siguienteEstacion;
    }

    bool obtenerEsTransferencia() const {
        return esTransferencia;
    }

    void establecerEsTransferencia(bool valor) {
        esTransferencia = valor;
    }

    ~Estacion() {
        delete siguiente;
    }
};


class Linea {
private:
    string nombre;
    Estacion* primeraEstacion;

public:
    Linea(string nombre) : nombre(nombre), primeraEstacion(nullptr) {}

    ~Linea() {
        delete primeraEstacion;
    }

    string obtenerNombre() const {
        return nombre;
    }

    void agregarEstacion(const string& nombreEstacion, int tiempoSiguiente) {
        if (!primeraEstacion) {
            primeraEstacion = new Estacion(nombreEstacion, tiempoSiguiente);
        } else {
            Estacion* ultimaEstacion = primeraEstacion;
            while (ultimaEstacion->obtenerSiguiente()) {
                ultimaEstacion = ultimaEstacion->obtenerSiguiente();
            }
            ultimaEstacion->establecerSiguiente(new Estacion(nombreEstacion, tiempoSiguiente));
        }
    }

    Estacion* obtenerEstacion(const string& nombreEstacion) {
        Estacion* estacionActual = primeraEstacion;
        while (estacionActual) {
            if (estacionActual->obtenerNombre() == nombreEstacion) {
                return estacionActual;
            }
            estacionActual = estacionActual->obtenerSiguiente();
        }
        return nullptr;
    }

    void eliminarEstacion(const string& nombreEstacion) {
        Estacion* anterior = nullptr;
        Estacion* actual = primeraEstacion;
        while (actual) {
            if (actual->obtenerNombre() == nombreEstacion) {
                if (actual->obtenerEsTransferencia()) {
                    cout << "No se puede eliminar una estación de transferencia." << endl;
                    return;
                }

                if (anterior) {
                    anterior->establecerSiguiente(actual->obtenerSiguiente());
                } else {
                    Estacion* siguiente = primeraEstacion->obtenerSiguiente();
                    primeraEstacion->establecerSiguiente(nullptr);
                    delete primeraEstacion;
                    primeraEstacion = siguiente;
                }
                return;
            }
            anterior = actual;
            actual = actual->obtenerSiguiente();
        }
        cout << "No se encontró la estación con el nombre especificado en la línea." << endl;
    }

    void mostrarEstaciones() const {
        cout << "Estaciones de la línea " << nombre << ":" << endl;
        Estacion* estacionActual = primeraEstacion;
        while (estacionActual) {
            cout << "- " << estacionActual->obtenerNombre();
            if (estacionActual->obtenerSiguiente()) {
                cout << " (" << estacionActual->obtenerSiguiente()->obtenerTiempoSiguiente() << " minutos hasta la siguiente estación)";
            }
            cout << endl;
            estacionActual = estacionActual->obtenerSiguiente();
        }
    }
};

class RedMetro {
private:
    Linea** lineas;
    int cantidadLineas;
    int capacidad;

public:
    RedMetro() : lineas(nullptr), cantidadLineas(0), capacidad(0) {}

    ~RedMetro() {
        for (int i = 0; i < cantidadLineas; ++i) {
            delete lineas[i];
        }
        delete[] lineas;
    }

    void agregarLinea(const string& nombreLinea) {
        if (cantidadLineas >= capacidad) {
            capacidad = (capacidad == 0) ? 1 : capacidad * 2;
            Linea** nuevaLista = new Linea*[capacidad];
            for (int i = 0; i < cantidadLineas; ++i) {
                nuevaLista[i] = lineas[i];
            }
            delete[] lineas;
            lineas = nuevaLista;
        }
        lineas[cantidadLineas++] = new Linea(nombreLinea);
    }

    void agregarEstacionALinea(const string& nombreLinea, const string& nombreEstacion, int tiempoSiguiente) {
        for (int i = 0; i < cantidadLineas; ++i) {
            if (lineas[i]->obtenerNombre() == nombreLinea) {
                lineas[i]->agregarEstacion(nombreEstacion, tiempoSiguiente);
                return;
            }
        }
        cout << "No se encontró la línea con el nombre especificado." << endl;
    }

    void agregarEstacionDeTransferencia(const string& nombreEstacion, const string& nombreLinea, const string& nombreEstacionTransferencia, int tiempoSiguiente) {
        bool encontrada = false;
        Linea* lineaOrigen = nullptr;

        // Buscar la línea de origen
        for (int i = 0; i < cantidadLineas; ++i) {
            if (lineas[i]->obtenerNombre() == nombreLinea) {
                lineaOrigen = lineas[i];
                encontrada = true;
                break;
            }
        }

        // Si la línea de origen no se encuentra, mostrar un mensaje y salir
        if (!encontrada) {
            cout << "No se encontró la línea con el nombre especificado." << endl;
            return;
        }

        // Reiniciar la variable encontrada para la búsqueda de la línea de transferencia
        encontrada = false;

        // Solicitar el nombre de la línea de transferencia al usuario
        string nombreLineaTransferencia;
        cout << "Ingrese el nombre de la línea en la que desea agregar la estación de transferencia: ";
        cin >> nombreLineaTransferencia;

        // Buscar la línea de transferencia
        Linea* lineaTransferencia = nullptr;
        for (int i = 0; i < cantidadLineas; ++i) {
            if (lineas[i]->obtenerNombre() == nombreLineaTransferencia) {
                lineaTransferencia = lineas[i];
                encontrada = true;
                break;
            }
        }

        // Si la línea de transferencia no se encuentra, crearla
        if (!encontrada) {
            // Agregar la nueva línea a la red del metro
            agregarLinea(nombreLineaTransferencia);

            // Obtener la referencia a la línea recién creada
            for (int i = 0; i < cantidadLineas; ++i) {
                if (lineas[i]->obtenerNombre() == nombreLineaTransferencia) {
                    lineaTransferencia = lineas[i];
                    break;
                }
            }
        }

        // Agregar la estación de transferencia a la línea de origen y a la línea de transferencia
        lineaOrigen->agregarEstacion(nombreEstacion, tiempoSiguiente);
        lineaOrigen->obtenerEstacion(nombreEstacion)->establecerEsTransferencia(true); // Marcar como estación de transferencia
        lineaTransferencia->agregarEstacion(nombreEstacionTransferencia, tiempoSiguiente);
        lineaTransferencia->obtenerEstacion(nombreEstacionTransferencia)->establecerEsTransferencia(true); // Marcar como estación de transferencia

        // Actualizar la cantidad de líneas en la red del metro
        cantidadLineas++;
    }

    void mostrarEstacionesDeLinea(const string& nombreLinea) const {
        for (int i = 0; i < cantidadLineas; ++i) {
            if (lineas[i]->obtenerNombre() == nombreLinea) {
                lineas[i]->mostrarEstaciones();
                return;
            }
        }
        cout << "No se encontró la línea con el nombre especificado." << endl;
    }

    void eliminarEstacionesDeLinea(const string& nombreLinea, const string& nombreEstacion) {
        for (int i = 0; i < cantidadLineas; ++i) {
            if (lineas[i]->obtenerNombre() == nombreLinea) {
                // Buscar la estación en la línea y eliminarla
                lineas[i]->eliminarEstacion(nombreEstacion);
                return;
            }
        }
        cout << "No se encontró la línea con el nombre especificado." << endl;
    }
};

int main() {
    string nombreLinea;
    RedMetro red;

    int opcion;
    do {
        cout << "Bienvenido al sistema de gestión de líneas de metro" << endl;
        cout << "1. Agregar línea" << endl;
        cout << "2. Agregar estación a línea existente" << endl;
        cout << "3. Eliminar estación de línea existente" << endl;
        cout << "4. Agregar estación de transferencia" << endl;
        cout << "5. Mostrar estaciones de una línea" << endl;
        cout << "6. Salir" << endl;
        cout << "Ingrese el número de la opción que desea: ";
        cin >> opcion;

        switch (opcion) {
        case 1: {
            cout << "Ingrese el nombre de la línea que desea agregar: ";
            cin >> nombreLinea;
            red.agregarLinea(nombreLinea);
            break;
        }
        case 2: {
            string nombreEstacion;
            int tiempoSiguiente;
            cout << "Ingrese el nombre de la línea a la que desea agregar la estación: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación que desea agregar: ";
            cin >> nombreEstacion;
            cout << "Ingrese el tiempo hasta la siguiente estación (en minutos): ";
            cin >> tiempoSiguiente;
            red.agregarEstacionALinea(nombreLinea, nombreEstacion, tiempoSiguiente);
            break;
        }
        case 3: {
            string nombreEstacion;
            cout << "Ingrese el nombre de la línea de la que desea eliminar la estación: ";
            cin >> nombreLinea;
            cout << "Ingrese el nombre de la estación que desea eliminar: ";
            cin >> nombreEstacion;
            red.eliminarEstacionesDeLinea(nombreLinea, nombreEstacion);
            break;
        }
        case 4: {
            string nombreEstacion, nombreEstacionTransferencia, nombreLinea;
            int tiempoSiguiente;

            cout << "Ingrese el nombre de la estación: ";
            cin >> nombreEstacion;
            cout << "Ingrese el nombre de la línea de la estación: ";
            cin >> nombreLinea;

            cout << "Ingrese el tiempo hasta la siguiente estación (en minutos): ";
            cin >> tiempoSiguiente;
            nombreEstacionTransferencia = nombreEstacion;
            red.agregarEstacionDeTransferencia(nombreEstacion, nombreLinea, nombreEstacionTransferencia, tiempoSiguiente);
            break;
        }
        case 5: {
            cout << "Ingrese el nombre de la línea cuyas estaciones desea mostrar: ";
            cin >> nombreLinea;
            red.mostrarEstacionesDeLinea(nombreLinea);
            break;
        }
        case 6: {
            cout << "Saliendo del programa..." << endl;
            break;
        }
        default: {
            cout << "Opción no válida. Por favor, ingrese un número entre 1 y 6." << endl;
            break;
        }
        }

    } while (opcion != 6);

    return 0;
}

