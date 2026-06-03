#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>
#include "Geometria.h"

// ---------------------------------------------------------------------------
//  Entidad.h  -  Clase BASE ABSTRACTA de todas las entidades del juego.
//  Define posicion, velocidad, tamaño, estado y la clave de sprite que la
//  capa GUI usara para dibujarla (la logica NO conoce Qt, solo una cadena).

namespace logica {

class Entidad {
protected:
    double x_, y_;          // posicion (esquina superior izquierda)
    double vx_, vy_;        // velocidad
    double ancho_, alto_;   // tamaño
    bool   activo_;         // si false, el nivel la elimina
    std::string sprite_;    // clave del sprite para la GUI

public:
    Entidad(double x, double y, double ancho, double alto);
    virtual ~Entidad();                       // virtual -> destruccion polimorfica correcta

    // --- Interfaz polimorfica (metodos virtuales puros => clase abstracta) ---
    virtual void actualizar(double dt) = 0;   // avanza la fisica/estado
    virtual std::string tipo() const = 0;     // identificador legible del tipo

    // --- Colisiones (AABB) ---
    RectF caja() const { return RectF(x_, y_, ancho_, alto_); }
    bool  colisionaCon(const Entidad& otra) const { return caja().intersecta(otra.caja()); }
    double x() const { return x_; }
    double y() const { return y_; }
    double vx() const { return vx_; }
    double vy() const { return vy_; }
    double ancho() const { return ancho_; }
    double alto() const  { return alto_; }
    double centroX() const { return x_ + ancho_ / 2.0; }
    double centroY() const { return y_ + alto_ / 2.0; }
    bool   activo() const { return activo_; }
    const std::string& sprite() const { return sprite_; }

    void setPos(double x, double y) { x_ = x; y_ = y; }
    void setVel(double vx, double vy) { vx_ = vx; vy_ = vy; }
    void setActivo(bool a) { activo_ = a; }
    void setSprite(const std::string& s) { sprite_ = s; }
};

}

#endif
