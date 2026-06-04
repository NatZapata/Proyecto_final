#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"

namespace logica {

class Personaje : public Entidad {
public:
    enum class Estado { Corriendo, Saltando };

private:
    double energia_;
    double energiaMax_;
    double sueloY_;
    double gravedad_;
    Estado estado_;
    bool   energizado_;
    double tiempoEnergizado_;

    // movimiento horizontal controlado por teclado.
    bool   movIzquierda_;
    bool   movDerecha_;
    double velocidadLateral_;

public:
    Personaje(double x, double sueloY, double gravedad, int energiaInicial);

    void actualizar(double dt) override;
    std::string tipo() const override { return "Personaje"; }

    void saltar(double alturaDeseada);
    void moverIzquierda(bool activo);
    void moverDerecha(bool activo);
    void recibirDanio(double cantidad);
    void recibirEnergia(double cantidad);
    void activarEnergizado(double duracion);
    void desactivarEnergizado();

    double energia() const { return energia_; }
    double energiaMax() const { return energiaMax_; }
    bool   sinEnergia() const { return energia_ <= 0.0; }
    bool   enElSuelo() const { return estado_ == Estado::Corriendo; }
    bool   energizado() const { return energizado_; }
    Estado estado() const { return estado_; }
    double sueloY() const { return sueloY_; }
};

} // namespace logica

#endif // PERSONAJE_H
