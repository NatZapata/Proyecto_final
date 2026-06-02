#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Entidad.h"


// Juguetes que actuan como obstaculo.  Hereda de Entidad.


namespace logica {

class Obstaculo : public Entidad {
public:
    enum class Movimiento { Estatico, Lateral, Vertical };

private:
    Movimiento movimiento_;
    double costoEnergia_;
    double baseY_;          // posicion vertical
    double baseX_;          // posicion horizontal
    double fase_;
    double amplitud_;
    double frecuencia_;
    double velScroll_;      // velocidad

public:
    Obstaculo(double x, double y, double ancho, double alto,
              Movimiento mov, double costoEnergia,
              double velScroll, const std::string& sprite);

    void actualizar(double dt) override;
    std::string tipo() const override { return "Obstaculo"; }

    double costoEnergia() const { return costoEnergia_; }
    Movimiento movimiento() const { return movimiento_; }
};

} // namespace logica

#endif // OBSTACULO_H
