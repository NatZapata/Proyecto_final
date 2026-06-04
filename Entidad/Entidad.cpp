#include "Entidad.h"

namespace logica {

Entidad::Entidad(double x, double y, double ancho, double alto)
    : x_(x), y_(y), vx_(0.0), vy_(0.0),
    ancho_(ancho), alto_(alto), activo_(true), sprite_() {
    if (ancho <= 0.0 || alto <= 0.0) {
        throw EntidadInvalidaException("dimensiones no positivas");
    }
}

Entidad::~Entidad() = default;

} // namespace logica
