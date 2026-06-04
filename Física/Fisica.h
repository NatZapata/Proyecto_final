#ifndef FISICA_H
#define FISICA_H

#include "Geometria.h"


namespace logica {
namespace Fisica {

// constantes pixeles, segundos
constexpr double GRAVEDAD       = 1200.0;   // px/s^2
constexpr double K_COULOMB      = 9.0e6;
constexpr double CARGA_JUGADOR  = 1.2;
constexpr double CARGA_JUGUETE  = 0.8;


inline double mru(double posicion, double velocidad, double dt) {
    return posicion + velocidad * dt;
}

double velocidadSaltoParaAltura(double altura, double gravedad = GRAVEDAD);

inline double aplicarGravedad(double vy, double dt, double gravedad = GRAVEDAD) {
    return vy + gravedad * dt;
}

Vec2 velocidadProyectil(const Vec2& origen, const Vec2& objetivo,
                        double rapidez, double gravedad = GRAVEDAD);


Vec2 fuerzaCoulomb(const Vec2& posFuente, const Vec2& posJuguete,
                   double cargaFuente = CARGA_JUGADOR,
                   double cargaJuguete = CARGA_JUGUETE);


double fuerzaImpacto(double masa, double aceleracion);


double danioPorImpacto(double masa, double rapidez);

} // namespace Fisica
} // namespace logica

#endif // FISICA_H
