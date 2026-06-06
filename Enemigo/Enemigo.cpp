#include "Enemigo.h"
#include "Personaje.h"
#include "Fisica.h"
#include <cmath>
#include <algorithm>

namespace logica {

Enemigo::Enemigo(double x, double sueloY, double gravedad, double agresividadBase)
    : Entidad(x, sueloY - 78.0, 104.0, 78.0),
    jugadorX_(0.0), jugadorY_(0.0), distanciaAlJugador_(1e9),
    jugadorEnRango_(false), estado_(Estado::Esperando),
    cadenciaDisparo_(3.45), tiempoDesdeDisparo_(0.0),
    agresividad_(agresividadBase), gravedad_(gravedad), sueloY_(sueloY),
    contadorDisparos_(0) {
    setSprite("enemy");
}

void Enemigo::percibir(const Personaje& jugador) {
    jugadorX_ = jugador.centroX();
    jugadorY_ = jugador.centroY();
    double dx = jugadorX_ - centroX();
    double dy = jugadorY_ - centroY();
    distanciaAlJugador_ = std::sqrt(dx * dx + dy * dy);
    jugadorEnRango_ = true; // enemigo fijo
}

void Enemigo::razonar() {
    if (!jugadorEnRango_) {
        estado_ = Estado::Esperando;
    } else if (tiempoDesdeDisparo_ >= cadenciaDisparo_ / std::max(0.35, agresividad_)) {
        estado_ = Estado::Atacando;
    } else {
        estado_ = Estado::Preparando;
    }
}

void Enemigo::actuar(double dt) {
    tiempoDesdeDisparo_ += dt;

    y_ = sueloY_ - alto_;

    if (estado_ == Estado::Atacando) {
        // cada 3 uno grande
        bool grande = (contadorDisparos_ % 3 == 0);
        ++contadorDisparos_;

        PiedraData p;
        p.ancho  = grande ? 38.0 : 22.0;
        p.masa   = grande ? 5.0  : 2.0;
        p.x      = centroX() - 20.0;
        p.y      = centroY() - 18.0;
        p.activa = true;

        // calculo de trayectoria parabolica
        Vec2 origen(p.x, p.y);
        Vec2 objetivo(jugadorX_, jugadorY_ - 10.0);
        double distancia    = std::abs(origen.x - objetivo.x);
        double tiempoVuelo  = std::clamp(distancia / 430.0, 1.15, 1.75);
        double gravEfectiva = gravedad_ * 0.6;
        double vx = (objetivo.x - origen.x) / tiempoVuelo;
        double vy = (objetivo.y - origen.y
                     - 0.5 * gravEfectiva * tiempoVuelo * tiempoVuelo) / tiempoVuelo;

        double factor = std::clamp(agresividad_, 0.55, 1.35);
        p.vx = vx * factor;
        p.vy = vy * 0.96;

        piedras_.push_back(p);

        tiempoDesdeDisparo_ = 0.0;
        estado_ = Estado::Preparando;
    }
}

void Enemigo::actualizarPiedras(double dt) {
    for (PiedraData& p : piedras_) {
        if (!p.activa) continue;
        // mov parabolico
        p.vy += gravedad_ * 0.6 * dt;
        p.x  += p.vx * dt;
        p.y  += p.vy * dt;

        if (p.y + p.ancho >= sueloY_ || p.x + p.ancho < -40.0 || p.x > 4000.0) {
            p.activa = false;
        }
    }
    piedras_.erase(
        std::remove_if(piedras_.begin(), piedras_.end(),
                       [](const PiedraData& p){ return !p.activa; }),
        piedras_.end()
        );
}

double Enemigo::calcularDanioPiedra(const PiedraData& p) {
    double rapidez = std::sqrt(p.vx * p.vx + p.vy * p.vy);
    return Fisica::danioPorImpacto(p.masa, rapidez);
}

void Enemigo::aprender(double desempenoJugador) {
    desempenoJugador = std::clamp(desempenoJugador, 0.0, 1.0);
    double objetivo = 0.55 + desempenoJugador * 0.85;
    agresividad_ += (objetivo - agresividad_) * 0.035;
    agresividad_ = std::clamp(agresividad_, 0.45, 1.45);
}

void Enemigo::actualizar(double dt) {
    (void)dt;
}

} // namespace logica
