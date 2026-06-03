#include "ObjetoCayendo.h"
#include "Fisica.h"

namespace logica {

ObjetoCayendo::ObjetoCayendo(double x, double y, double valorEnergia, bool premiado,
                             double gravedad, double sueloY, const std::string& sprite)
    : Entidad(x, y, 52.0, 52.0),
      valorEnergia_(valorEnergia), premiado_(premiado),
      gravedad_(gravedad), sueloY_(sueloY) {
    setSprite(sprite);
    vy_ = 60.0; //  velocidad inicial de caida
}

void ObjetoCayendo::actualizar(double dt) {
    // Caida con gravedad
    vy_ = Fisica::aplicarGravedad(vy_, dt, gravedad_ * 0.45);
    x_ += vx_ * dt;
    y_ += vy_ * dt;

    // Si toca el suelo, se pierde
    if (y_ + alto_ >= sueloY_) {
        setActivo(false);
    }
}

} // namespace logica
