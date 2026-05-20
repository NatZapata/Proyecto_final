#ifndef FISICA_H
#define FISICA_H

#pragma once
#include <QPointF>

class Fisica
{
public:
    static constexpr double Gravedad = 980.0;
    static constexpr double CoulombK = 9000.0;

    static QPointF pasoParabolico(const QPointF& posicion,
                                  const QPointF& velocidad,
                                  double tiempoDelta,
                                  double gravedad = Gravedad);

    static double fuerzaNewton(double masa, double aceleracion);
    static int danoPorFuerza(double fuerza, double factorBalance = 85.0);

    static QPointF atraccionCoulomb(const QPointF& origen,
                                    const QPointF& destino,
                                    double q1,
                                    double q2,
                                    double magnitudMaxima = 360.0);
};
#endif // FISICA_H
