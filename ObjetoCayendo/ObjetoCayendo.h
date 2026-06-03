#ifndef OBJETOCAYENDO_H
#define OBJETOCAYENDO_H

#include "Entidad.h"

// Juguetes que caen.  Hereda de Entidad.


namespace logica {

class ObjetoCayendo : public Entidad {
    double valorEnergia_;
    bool   premiado_;
    double gravedad_;
    double sueloY_;

public:
    ObjetoCayendo(double x, double y, double valorEnergia, bool premiado,
                  double gravedad, double sueloY, const std::string& sprite);

    void actualizar(double dt) override;
    std::string tipo() const override { return "ObjetoCayendo"; }

    double valorEnergia() const { return valorEnergia_; }
    bool   premiado() const { return premiado_; }

    // Aplica una fuerza externa (Coulomb).
    void aplicarFuerza(double fx, double fy, double dt) {
        vx_ += fx * dt;
        vy_ += fy * dt;
    }
};

} // namespace logica

#endif // OBJETOCAYENDO_H
