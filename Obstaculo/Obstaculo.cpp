#include "Obstaculo.h"
#include <cmath>

namespace logica {

Obstaculo::Obstaculo(double x, double y, double ancho, double alto,
                     Movimiento mov, double costoEnergia,
                     double velScroll, const std::string& sprite)
    : Entidad(x, y, ancho, alto),
      movimiento_(mov), costoEnergia_(costoEnergia),
      baseY_(y), baseX_(x), fase_(0.0),
      amplitud_(mov == Movimiento::Vertical ? 70.0 : 55.0),
      frecuencia_(2.0), velScroll_(velScroll) {
    setSprite(sprite);
}

void Obstaculo::actualizar(double dt) {
    fase_ += dt;

    baseX_ -= velScroll_ * dt;
    x_ = baseX_;

    // Movimiento adicional segun el tipo
    switch (movimiento_) {
        case Movimiento::Estatico:
            y_ = baseY_;
            break;
        case Movimiento::Lateral:
            x_ = baseX_ + amplitud_ * std::sin(frecuencia_ * fase_);
            y_ = baseY_;
            break;
        case Movimiento::Vertical:
            // Oscilacion vertical tipo "saltos".
            y_ = baseY_ - std::abs(amplitud_ * std::sin(frecuencia_ * fase_));
            break;
    }

    // Cuando sale por la izquierda se desactiva
    if (x_ + ancho_ < -20.0) {
        setActivo(false);
    }
}

} // namespace logica
