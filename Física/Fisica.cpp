#include "Fisica.h"

#include <QtMath>
#include <algorithm>

QPointF Fisica::pasoParabolico(const QPointF& posicion,
                               const QPointF& velocidad,
                               double tiempoDelta,
                               double gravedad)
{
    const double x = posicion.x() + velocidad.x() * tiempoDelta;
    const double y = posicion.y() + velocidad.y() * tiempoDelta + 0.5 * gravedad * tiempoDelta * tiempoDelta;
    return QPointF(x, y);
}

double Fisica::fuerzaNewton(double masa, double aceleracion)
{
    return masa * aceleracion;
}

int Fisica::danoPorFuerza(double fuerza, double factorBalance)
{
    const int dano = static_cast<int>(qRound(fuerza / factorBalance));
    return std::clamp(dano, 4, 22);
}

QPointF Fisica::atraccionCoulomb(const QPointF& origen,
                                 const QPointF& destino,
                                 double q1,
                                 double q2,
                                 double magnitudMaxima)
{
    const QPointF direccion = destino - origen;
    const double distanciaCuadrada = std::max(70.0, direccion.x() * direccion.x() + direccion.y() * direccion.y());
    const double distancia = qSqrt(distanciaCuadrada);
    const double fuerza = std::min(magnitudMaxima, CoulombK * qAbs(q1 * q2) / distanciaCuadrada);
    return QPointF((direccion.x() / distancia) * fuerza, (direccion.y() / distancia) * fuerza);
}
