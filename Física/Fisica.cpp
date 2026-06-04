#include "Fisica.h"
#include <cmath>

namespace logica {
namespace Fisica {

double velocidadSaltoParaAltura(double altura, double gravedad) {
    if (altura < 0.0) altura = -altura;
    return std::sqrt(2.0 * gravedad * altura);
}

Vec2 velocidadProyectil(const Vec2& origen, const Vec2& objetivo,
                        double rapidez, double gravedad) {
    // direccion horizontal hacia el objetivo.
    double dx = objetivo.x - origen.x;
    double dy = objetivo.y - origen.y;
    double dist = std::sqrt(dx * dx + dy * dy);
    if (dist < 1e-6) dist = 1e-6;

    // Componente horizontal
    double vx = rapidez * (dx / dist);
    // Componente vertical
    double vy = -std::abs(rapidez) * 0.55 + (dy / dist) * rapidez * 0.2;
    (void)gravedad;
    return {vx, vy};
}

Vec2 fuerzaCoulomb(const Vec2& posFuente, const Vec2& posJuguete,
                   double cargaFuente, double cargaJuguete) {
    double dx = posFuente.x - posJuguete.x;
    double dy = posFuente.y - posJuguete.y;
    double r2 = dx * dx + dy * dy;
    if (r2 < 100.0) r2 = 100.0;
    double r = std::sqrt(r2);

    double magnitud = K_COULOMB * cargaFuente * cargaJuguete / r2;
    return {magnitud * (dx / r), magnitud * (dy / r)};
}

double fuerzaImpacto(double masa, double aceleracion) {
    return masa * aceleracion;            // F = m * a
}

double danioPorImpacto(double masa, double rapidez) {
    double aceleracion = rapidez * 0.5;
    double fuerza = fuerzaImpacto(masa, aceleracion);
    return fuerza * 0.009;
}

} // namespace Fisica
} // namespace logica

