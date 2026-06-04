#ifndef NIVEL_H
#define NIVEL_H

#include <vector>
#include <string>
#include <random>
#include "Dificultad.h"


namespace logica {

class Personaje;
class Obstaculo;
class ObjetoCayendo;
class Enemigo;

enum class EstadoNivel { EnCurso, Ganado, Perdido };

class Nivel {
protected:
    ConfiguracionDificultad config_;
    double anchoMundo_, altoMundo_, sueloY_;

    Personaje*                   jugador_;
    std::vector<Obstaculo*>      obstaculos_;
    std::vector<ObjetoCayendo*>  objetos_;

    double tiempoTranscurrido_;
    double tiempoLimite_;
    int    puntaje_;
    EstadoNivel estado_;

    std::mt19937 rng_;
    double acumSpawn_;

    // nivel 1
    double velScroll_;

    // nivel 2
    Enemigo* enemigo_;
    double   acumObjeto_;
    double   tiempoCoulomb_;

    int numeroNivel_;

    void limpiarInactivos();
    void liberarTodo();
    double aleatorio(double a, double b);
    std::string spriteJugueteAleatorio();

    // logica por nivel
    void iniciarNivel1();
    void iniciarNivel2();
    void actualizarNivel1(double dt);
    void actualizarNivel2(double dt);
    void generarObstaculoNivel1();
    void generarObstaculoNivel2();
    void generarObjetoCayendo();
    void aplicarFuerzaCoulomb(double dt);

public:
    Nivel(int numeroNivel,
          const ConfiguracionDificultad& config,
          double anchoMundo, double altoMundo);
    ~Nivel();

    void iniciar();
    void actualizar(double dt);

    void saltarJugador();
    void moverJugadorIzquierda(bool activo);
    void moverJugadorDerecha(bool activo);

    const Personaje*     jugador()    const { return jugador_; }
    const std::vector<Obstaculo*>&     obstaculos() const { return obstaculos_; }
    const std::vector<ObjetoCayendo*>& objetos()    const { return objetos_; }
    const Enemigo*      enemigo()    const { return enemigo_; }

    int   numero()    const { return numeroNivel_; }
    std::string nombre()   const { return numeroNivel_ == 1 ? "Habitacion de Andy" : "Jardin de Andy"; }
    double  anchoMundo()    const { return anchoMundo_; }
    double  altoMundo()     const { return altoMundo_; }
    double  sueloY()     const { return sueloY_; }
    double  tiempoTranscurrido() const { return tiempoTranscurrido_; }
    double  tiempoLimite()  const { return tiempoLimite_; }
    double  tiempoRestante() const { return tiempoLimite_ - tiempoTranscurrido_; }
    double  tiempoCoulomb() const { return tiempoCoulomb_; }
    int  puntaje()     const { return puntaje_; }
    EstadoNivel estado()    const { return estado_; }
    const ConfiguracionDificultad& config() const { return config_; }
};

} // namespace logica

#endif // NIVEL_H
