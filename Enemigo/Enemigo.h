#ifndef ENEMIGO_H
#define ENEMIGO_H

#include <vector>
#include "Entidad.h"

namespace logica {

class Personaje;

class IAgenteInteligente {
public:
    virtual ~IAgenteInteligente() = default;
    virtual void percibir(const Personaje& jugador) = 0;
    virtual void razonar() = 0;
    virtual void actuar(double dt) = 0;
    virtual void aprender(double desempenoJugador) = 0;
};

// datos de cada piedra lanzada
struct PiedraData {
    double x, y;
    double vx, vy;
    double masa;
    double ancho;   // ancho == alto (cuadrado)
    bool   activa;

    double centroX() const { return x + ancho / 2.0; }
    double centroY() const { return y + ancho / 2.0; }
};

class Enemigo : public Entidad, public IAgenteInteligente {
public:
    enum class Estado { Esperando, Preparando, Atacando };

private:
    // percepcion
    double jugadorX_, jugadorY_;
    double distanciaAlJugador_;
    bool   jugadorEnRango_;

    // razonamiento
    Estado estado_;

    // accion
    double cadenciaDisparo_;
    double tiempoDesdeDisparo_;
    double agresividad_;
    double gravedad_;
    double sueloY_;
    int    contadorDisparos_;

    std::vector<PiedraData> piedras_;

public:
    Enemigo(double x, double sueloY, double gravedad, double agresividadBase);
    ~Enemigo() override = default;

    void actualizar(double dt) override;
    std::string tipo() const override { return "Enemigo"; }

    // IAgenteInteligente
    void percibir(const Personaje& jugador) override;
    void razonar() override;
    void actuar(double dt) override;
    void aprender(double desempenoJugador) override;

    void actualizarPiedras(double dt); // actualizar fisica de piedras

    static double calcularDanioPiedra(const PiedraData& p); // calcular daño de piedra con f = ma

    const std::vector<PiedraData>& piedras() const { return piedras_; }

    Estado estado()      const { return estado_; }
    double cadencia()    const { return cadenciaDisparo_; }
    double agresividad() const { return agresividad_; }
};

} // namespace logica

#endif // ENEMIGO_H
