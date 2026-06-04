#include "Personaje.h"
#include "Fisica.h"
#include <algorithm>

namespace logica {

Personaje::Personaje(double x, double sueloY, double gravedad, int energiaInicial)
    : Entidad(x, sueloY - 122.0, 82.0, 122.0),
      energia_(energiaInicial), energiaMax_(energiaInicial),
      sueloY_(sueloY), gravedad_(gravedad),
      estado_(Estado::Corriendo), energizado_(false), tiempoEnergizado_(0.0),
      movIzquierda_(false), movDerecha_(false), velocidadLateral_(265.0) {
    if (energiaInicial <= 0) {
        throw EntidadInvalidaException("energia inicial debe ser positiva");
    }
    setSprite("woody_run");
}

void Personaje::saltar(double alturaDeseada) {
    if (estado_ != Estado::Corriendo) return;
    double v0 = Fisica::velocidadSaltoParaAltura(alturaDeseada, gravedad_);
    vy_ = -v0;
    estado_ = Estado::Saltando;
    setSprite("woody_jump");
}

void Personaje::moverIzquierda(bool activo) { movIzquierda_ = activo; }
void Personaje::moverDerecha(bool activo) { movDerecha_ = activo; }

void Personaje::actualizar(double dt) {
    vx_ = 0.0;
    if (movIzquierda_ && !movDerecha_) vx_ = -velocidadLateral_;
    if (movDerecha_ && !movIzquierda_) vx_ =  velocidadLateral_;
    x_ += vx_ * dt;
    x_ = std::clamp(x_, 20.0, 700.0); // área

    if (estado_ == Estado::Saltando) {
        vy_ = Fisica::aplicarGravedad(vy_, dt, gravedad_);
        y_ += vy_ * dt;
        double pisoTop = sueloY_ - alto_;
        if (y_ >= pisoTop) {
            y_ = pisoTop;
            vy_ = 0.0;
            estado_ = Estado::Corriendo;
            setSprite("woody_run");
        }
    }

    if (energizado_) {
        tiempoEnergizado_ -= dt;
        if (tiempoEnergizado_ <= 0.0) desactivarEnergizado();
    }

    energia_ = std::clamp(energia_, 0.0, energiaMax_);
}

void Personaje::recibirDanio(double cantidad) {
    if (cantidad < 0.0) cantidad = 0.0;
    energia_ -= cantidad;
    if (energia_ < 0.0) energia_ = 0.0;
}

void Personaje::recibirEnergia(double cantidad) {
    if (cantidad < 0.0) cantidad = 0.0;
    energia_ = std::min(energia_ + cantidad, energiaMax_);
}

void Personaje::activarEnergizado(double duracion) {
    energizado_ = true;
    tiempoEnergizado_ = duracion;
    setSprite(estado_ == Estado::Saltando ? "woody_jump" : "woody_run");
}

void Personaje::desactivarEnergizado() {
    energizado_ = false;
    tiempoEnergizado_ = 0.0;
}

} // namespace logica
