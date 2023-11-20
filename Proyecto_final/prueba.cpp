#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Estadio {
    string nombre;
};

struct Equipo {
    string nombre;
    int golesMarcados;
    int golesRecibidos;
    int puntos;
};

void simularPartido(Equipo &equipo1, Equipo &equipo2, Estadio &estadio) {
    int golesEquipo1 = rand() % 5;
    int golesEquipo2 = rand() % 5;

    equipo1.golesMarcados += golesEquipo1;
    equipo1.golesRecibidos += golesEquipo2;

    equipo2.golesMarcados += golesEquipo2;
    equipo2.golesRecibidos += golesEquipo1;

    cout << equipo1.nombre << " " << golesEquipo1 << " - " << golesEquipo2 << " " << equipo2.nombre;
    cout << " / Jugado en el Estadio " << estadio.nombre << endl;

    if (golesEquipo1 > golesEquipo2) {
        equipo1.puntos += 3; // Equipo 1 gana
    } else if (golesEquipo1 < golesEquipo2) {
        equipo2.puntos += 3; // Equipo 2 gana
    } else {
        equipo1.puntos += 1; // Empate
        equipo2.puntos += 1; // Empate
    }
}

void simularGrupo(vector<Equipo> &grupo, char letraGrupo, vector<string> &estadios, int dia) {
    cout << "--------------------------------------------------------------------------------- "<< endl;
    cout << "DIA " << dia << endl;
    cout << "GRUPO " << letraGrupo << ":" << endl;

    random_shuffle(estadios.begin(), estadios.end());

    size_t index = 0;
    for (size_t i = 0; i < grupo.size() - 1; ++i) {
        for (size_t j = i + 1; j < grupo.size(); ++j) {
            Estadio estadioActual;
            if (index < estadios.size()) {
                estadioActual.nombre = estadios[index];

                // Generar un porcentaje de winrate aleatorio entre 20% y 80%
                int winrateEquipo1 = rand() % 61  + 20;
                int winrateEquipo2 = 100 - winrateEquipo1;

                cout << grupo[i].nombre << " (" << winrateEquipo1 << "%) vs "
                     << grupo[j].nombre << " (" << winrateEquipo2 << "%) " <<endl;

                simularPartido(grupo[i], grupo[j], estadioActual);
                ++index;
            } else {
                cout << "No hay estadio disponible para este partido." << endl;
            }
        }
    }
}

void leerEquiposDesdeArchivo(const string &nombreArchivo, vector<Equipo> &equipos) {
    ifstream archivo(nombreArchivo);

    if (archivo.is_open()) {
        string nombreEquipo;
        while (getline(archivo, nombreEquipo)) {
            Equipo equipo;
            equipo.nombre = nombreEquipo;
            equipo.golesMarcados = 0;
            equipo.golesRecibidos = 0;
            equipo.puntos = 0;
            equipos.push_back(equipo);
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo: " << nombreArchivo << endl;
    }
}

void mostrarTablaPosiciones(vector<Equipo> &equiposGrupo) {
    // Ordenar equipos por puntos de manera descendente
    sort(equiposGrupo.begin(), equiposGrupo.end(), [](const Equipo &a, const Equipo &b) {
        return a.puntos > b.puntos;
    });

    cout << "Equipo\t\tPuntos\tGF\tGC\tDif.Goles" << endl;
    for (const auto &equipo : equiposGrupo) {
        cout << left << setw(15) << equipo.nombre << "\t"
             << setw(7) << equipo.puntos << "\t"
             << setw(7) << equipo.golesMarcados << "\t"
             << setw(7) << equipo.golesRecibidos << "\t"
             << setw(10) << equipo.golesMarcados - equipo.golesRecibidos << endl;
    }

    // Mostrar equipos clasificados
    cout << "Equipos Clasificados:" << endl;
    for (size_t i = 0; i < 2 && i < equiposGrupo.size(); ++i) {
        cout << equiposGrupo[i].nombre << endl;
    }
}


int main() {
    srand(static_cast<unsigned>(time(0)));

    const string nombreArchivoEquipos = "../Documento/Equipos.txt";
    vector<Equipo> equipos;
    leerEquiposDesdeArchivo(nombreArchivoEquipos, equipos);

    vector<string> estadios = {
        "Estadio Luzhniki (Moscu)",
        "Estadio de San Petersburgo (San Petersburgo)",
        "Estadio de la Copa del Mundo de la FIFA Ekaterimburgo (Ekaterimburgo)",
        "Estadio del Spartak (Moscu)",
        "Estadio de Nizhni Novgorod (Nizhni Novgorod)",
        "Estadio de Kazan (Kazan)"
    };

    char letraGrupo = 'A';
    int dia = 1;
    vector<vector<Equipo>> equiposClasificadosPorGrupo;

    for (size_t i = 0; i < equipos.size(); i += 4) {
        vector<Equipo> grupo(equipos.begin() + i, equipos.begin() + i + 4);
        simularGrupo(grupo, letraGrupo, estadios, dia);

        cout << "--------------------------------------------------------------------------------- "<< endl;
        cout << endl << "Tabla de Posiciones - Grupo " << letraGrupo << ":" << endl;
        mostrarTablaPosiciones(grupo);

        sort(grupo.begin(), grupo.end(), [](const Equipo &a, const Equipo &b) {
            return a.puntos > b.puntos;
        });

        // Almacenar solo los dos equipos clasificados en un grupo en el vector equiposClasificadosPorGrupo
        vector<Equipo> equiposClasificadosGrupo;
        equiposClasificadosGrupo.push_back(grupo[0]);
        equiposClasificadosGrupo.push_back(grupo[1]);

        equiposClasificadosPorGrupo.push_back(equiposClasificadosGrupo);

        letraGrupo++;
        dia++;
    }

    cout << "--------------------------------------------------------------------------------- "<< endl;
    cout << "Octavos de Final:" << endl;

    vector<Equipo> equiposGanadoresOctavos;

    for (size_t i = 0; i < equiposClasificadosPorGrupo.size(); i += 2) {
        for (size_t j = 0; j < 2; ++j) {
            cout << equiposClasificadosPorGrupo[i][j].nombre << " vs " << equiposClasificadosPorGrupo[i + 1][1 - j].nombre << endl;

            Estadio estadioOctavos;
            estadioOctavos.nombre = "Estadio Octavos";

            // Simular el partido
            int golesEquipo1 = rand() % 5;
            int golesEquipo2 = rand() % 5;

            cout << equiposClasificadosPorGrupo[i][j].nombre << " " << golesEquipo1 << " - " << golesEquipo2 << " " << equiposClasificadosPorGrupo[i + 1][1 - j].nombre;
            cout << " / Jugado en el Estadio " << estadioOctavos.nombre << endl;

            if (golesEquipo1 > golesEquipo2) {
                equiposGanadoresOctavos.push_back(equiposClasificadosPorGrupo[i][j]);
            } else if (golesEquipo1 < golesEquipo2) {
                equiposGanadoresOctavos.push_back(equiposClasificadosPorGrupo[i + 1][1 - j]);
            } else {

                bool repite = true;
                cout << "PENALES" << endl;

                while (repite == true)
                {
                    int fallosEquipo1 = 0;
                    int fallosEquipo2 = 0;

                    for (int tiro = 0; tiro < 5; ++tiro) {
                    int falloEquipo1 = rand() % 2; // Simular fallo para Equipo 1 en este tiro (0: gol, 1: fallo)
                    int falloEquipo2 = rand() % 2; // Simular fallo para Equipo 2 en este tiro (0: gol, 1: fallo)

                    fallosEquipo1 += falloEquipo1;
                    fallosEquipo2 += falloEquipo2;
                    }

                    // Resultado final de la serie de penales
                    cout << "Resultado final de la serie de penales:" << endl;
                    cout << equiposClasificadosPorGrupo[i][j].nombre << ": " << 5 - fallosEquipo1 << " goles" << endl;
                    cout << equiposClasificadosPorGrupo[i + 1][1 - j].nombre << ": " << 5 - fallosEquipo2 << " goles" << endl;

                    // Determinar el equipo ganador de la serie de penales
                    if (fallosEquipo1 != fallosEquipo2) {
                        if (fallosEquipo1 < fallosEquipo2) {
                            cout << equiposClasificadosPorGrupo[i][j].nombre << " avanza a la siguiente ronda" << endl;
                            repite = false;
                            equiposGanadoresOctavos.push_back(equiposClasificadosPorGrupo[i][j]); // Agregar equipo ganador a equiposGanadoresOctavos
                        } else {
                            cout << equiposClasificadosPorGrupo[i + 1][1 - j].nombre << " avanza a la siguiente ronda " << endl;
                            repite = false;
                            equiposGanadoresOctavos.push_back(equiposClasificadosPorGrupo[i + 1][1 - j]); // Agregar equipo ganador a equiposGanadoresOctavos
                        }
                    } else {
                        cout << "Empate en penales, se repite tanda de penales" << endl;
                        repite = true;
                    }
                }
            }
        }
    }
    cout << endl << "Equipos Ganadores de Octavos de Final:" << endl;
    for (const auto &equipo : equiposGanadoresOctavos) {
        cout << equipo.nombre << endl;
    }

    cout << "--------------------------------------------------------------------------------- "<< endl;
    cout << "Cuartos de Final:" << endl;

    vector<Equipo> equiposGanadoresCuartos;

    for (size_t i = 0; i < equiposGanadoresOctavos.size(); i += 2) {
        cout << equiposGanadoresOctavos[i].nombre << " vs " << equiposGanadoresOctavos[i + 1].nombre << endl;

        Estadio estadioCuartos;
        estadioCuartos.nombre = "Estadio Cuartos";

        // Simular el partido de cuartos de final
        int golesEquipo1 = rand() % 5;
        int golesEquipo2 = rand() % 5;

        cout << equiposGanadoresOctavos[i].nombre << " " << golesEquipo1 << " - " << golesEquipo2 << " " << equiposGanadoresOctavos[i + 1].nombre;
        cout << " / Jugado en el Estadio " << estadioCuartos.nombre << endl;

        if (golesEquipo1 > golesEquipo2) {
            equiposGanadoresCuartos.push_back(equiposGanadoresOctavos[i]);
        } else if (golesEquipo1 < golesEquipo2) {
            equiposGanadoresCuartos.push_back(equiposGanadoresOctavos[i + 1]);
        } else {

            bool repite = true;
            cout << "PENALES" << endl;

            while (repite == true)
            {
                int fallosEquipo1 = 0;
                int fallosEquipo2 = 0;

                for (int tiro = 0; tiro < 5; ++tiro) {
                    int falloEquipo1 = rand() % 2; // Simular fallo para Equipo 1 en este tiro (0: gol, 1: fallo)
                    int falloEquipo2 = rand() % 2; // Simular fallo para Equipo 2 en este tiro (0: gol, 1: fallo)

                    fallosEquipo1 += falloEquipo1;
                    fallosEquipo2 += falloEquipo2;
                }

                // Resultado final de la serie de penales
                cout << "Resultado final de la serie de penales:" << endl;
                cout << equiposGanadoresOctavos[i].nombre << ": " << 5 - fallosEquipo1 << " goles" << endl;
                cout << equiposGanadoresOctavos[i + 1].nombre << ": " << 5 - fallosEquipo2 << " goles" << endl;

                // Determinar el equipo ganador de la serie de penales
                if (fallosEquipo1 != fallosEquipo2) {
                    if (fallosEquipo1 < fallosEquipo2) {
                        cout << equiposGanadoresOctavos[i].nombre << " avanza a la siguiente ronda" << endl;
                        repite = false;
                        equiposGanadoresCuartos.push_back(equiposGanadoresOctavos[i]);
                    } else {
                        cout << equiposGanadoresOctavos[i + 1].nombre << " avanza a la siguiente ronda " << endl;
                        repite = false;
                        equiposGanadoresCuartos.push_back(equiposGanadoresOctavos[i + 1]);
                    }
                } else {
                    cout << "Empate en penales, se repite tanda de penales" << endl;
                    repite = true;
                }
            }
        }
    }
    cout << endl << "Equipos Ganadores de Cuartos de Final:" << endl;
    for (const auto &equipo : equiposGanadoresCuartos) {
        cout << equipo.nombre << endl;
    }

    cout << "--------------------------------------------------------------------------------- "<< endl;
    cout << "Semifinal:" << endl;

    vector<Equipo> equiposGanadoresSemifinal;

    for (size_t i = 0; i < equiposGanadoresCuartos.size(); i += 2) {
        cout << equiposGanadoresCuartos[i].nombre << " vs " << equiposGanadoresCuartos[i + 1].nombre << endl;

        Estadio estadioCuartos;
        estadioCuartos.nombre = "Estadio Semifinal";

        // Simular el partido de cuartos de final
        int golesEquipo1 = rand() % 5;
        int golesEquipo2 = rand() % 5;

        cout << equiposGanadoresCuartos[i].nombre << " " << golesEquipo1 << " - " << golesEquipo2 << " " << equiposGanadoresCuartos[i + 1].nombre;
        cout << " / Jugado en el Estadio " << estadioCuartos.nombre << endl;

        if (golesEquipo1 > golesEquipo2) {
            equiposGanadoresSemifinal.push_back(equiposGanadoresCuartos[i]);
        } else if (golesEquipo1 < golesEquipo2) {
            equiposGanadoresSemifinal.push_back(equiposGanadoresCuartos[i + 1]);
        } else {

            bool repite = true;
            cout << "PENALES" << endl;

            while (repite == true)
            {
                int fallosEquipo1 = 0;
                int fallosEquipo2 = 0;

                for (int tiro = 0; tiro < 5; ++tiro) {
                    int falloEquipo1 = rand() % 2; // Simular fallo para Equipo 1 en este tiro (0: gol, 1: fallo)
                    int falloEquipo2 = rand() % 2; // Simular fallo para Equipo 2 en este tiro (0: gol, 1: fallo)

                    fallosEquipo1 += falloEquipo1;
                    fallosEquipo2 += falloEquipo2;
                }

                // Resultado final de la serie de penales
                cout << "Resultado final de la serie de penales:" << endl;
                cout << equiposGanadoresCuartos[i].nombre << ": " << 5 - fallosEquipo1 << " goles" << endl;
                cout << equiposGanadoresCuartos[i + 1].nombre << ": " << 5 - fallosEquipo2 << " goles" << endl;

                // Determinar el equipo ganador de la serie de penales
                if (fallosEquipo1 != fallosEquipo2) {
                    if (fallosEquipo1 < fallosEquipo2) {
                        cout << equiposGanadoresCuartos[i].nombre << " avanza a la siguiente ronda" << endl;
                        repite = false;
                        equiposGanadoresSemifinal.push_back(equiposGanadoresCuartos[i]);
                    } else {
                        cout << equiposGanadoresCuartos[i + 1].nombre << " avanza a la siguiente ronda " << endl;
                        repite = false;
                        equiposGanadoresSemifinal.push_back(equiposGanadoresCuartos[i + 1]);
                    }
                } else {
                    cout << "Empate en penales, se repite tanda de penales" << endl;
                    repite = true;
                }
            }
        }
    }
    cout << endl << "Equipos Ganadores de Semifinal:" << endl;
    for (const auto &equipo : equiposGanadoresSemifinal) {
        cout << equipo.nombre << endl;
    }

    cout << "--------------------------------------------------------------------------------- "<< endl;
    cout << "FINAL:" << endl;

    vector<Equipo> equiposGanadoresfinal;

    for (size_t i = 0; i < equiposGanadoresSemifinal.size(); i += 2) {
        cout << equiposGanadoresSemifinal[i].nombre << " vs " << equiposGanadoresSemifinal[i + 1].nombre << endl;

        Estadio estadioCuartos;
        estadioCuartos.nombre = "Estadio Final";

        // Simular el partido de cuartos de final
        int golesEquipo1 = rand() % 5;
        int golesEquipo2 = rand() % 5;

        cout << equiposGanadoresSemifinal[i].nombre << " " << golesEquipo1 << " - " << golesEquipo2 << " " << equiposGanadoresSemifinal[i + 1].nombre;
        cout << " / Jugado en el Estadio " << estadioCuartos.nombre << endl;

        if (golesEquipo1 > golesEquipo2) {
            equiposGanadoresfinal.push_back(equiposGanadoresSemifinal[i]);
        } else if (golesEquipo1 < golesEquipo2) {
            equiposGanadoresfinal.push_back(equiposGanadoresSemifinal[i + 1]);
        } else {

            bool repite = true;
            cout << "PENALES:" << endl;

            while (repite == true)
            {
                int fallosEquipo1 = 0;
                int fallosEquipo2 = 0;

                for (int tiro = 0; tiro < 5; ++tiro) {
                    int falloEquipo1 = rand() % 2; // Simular fallo para Equipo 1 en este tiro (0: gol, 1: fallo)
                    int falloEquipo2 = rand() % 2; // Simular fallo para Equipo 2 en este tiro (0: gol, 1: fallo)

                    fallosEquipo1 += falloEquipo1;
                    fallosEquipo2 += falloEquipo2;
                }

                // Resultado final de la serie de penales
                cout << "Resultado final de la serie de penales:" << endl;
                cout << equiposGanadoresSemifinal[i].nombre << ": " << 5 - fallosEquipo1 << " goles" << endl;
                cout << equiposGanadoresSemifinal[i + 1].nombre << ": " << 5 - fallosEquipo2 << " goles" << endl;

                // Determinar el equipo ganador de la serie de penales
                if (fallosEquipo1 != fallosEquipo2) {
                    if (fallosEquipo1 < fallosEquipo2) {
                        cout << equiposGanadoresSemifinal[i].nombre << " ES EL CAMPEON" << endl;
                        repite = false;
                        equiposGanadoresfinal.push_back(equiposGanadoresSemifinal[i]);
                    } else {
                        cout << equiposGanadoresSemifinal[i + 1].nombre << " ES EL CAMPEON " << endl;
                        repite = false;
                        equiposGanadoresfinal.push_back(equiposGanadoresSemifinal[i + 1]);
                    }
                } else {
                    cout << "Empate en penales, se repite tanda de penales" << endl;
                    repite = true;
                }
            }
        }
    }
    cout << endl << "Equipo Ganador de Final del mundial es:" << endl;
    for (const auto &equipo : equiposGanadoresfinal) {
        cout << equipo.nombre << endl;
    }
}
