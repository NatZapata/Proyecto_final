#include "GestorRecursos.h"

GestorRecursos::GestorRecursos() {
    const QString base = ":/assets/sprites/";
    const QString fondos = ":/assets/backgrounds/";

    cargar("background_room",   { fondos + "background_room.png" });
    cargar("background_garden", { fondos + "background_garden.png" });

    // woody
    cargar("woody_run", {
                            base + "woody_run0.png", base + "woody_run1.png",
                            base + "woody_run2.png", base + "woody_run3.png"
                        });
    cargar("woody_jump", { base + "woody_jump.png" });

    // araña enemiga
    cargar("enemy", {
                        base + "enemy0.png", base + "enemy1.png",
                        base + "enemy2.png", base + "enemy3.png"
                    });

    // señor cara de papa. dinamico
    cargar("potato", {
                         base + "potato0.png", base + "potato1.png",
                         base + "potato2.png", base + "potato3.png"
                     });

    // traveller. dinamico
    cargar("traveller", {
                            base + "traveller0.png", base + "traveller1.png",
                            base + "traveller2.png", base + "traveller3.png"
                        });

    // juguetes estaticos y objetos que caen.
    const QStringList juguetes = {
        "toy_ball", "toy_car", "toy_dino", "toy_duck", "toy_pyramid",
        "toy_rocket", "toy_teddy", "toy_xylophone", "toy_xylo2",
        "rex", "alien"
    };
    for (const QString& j : juguetes) {
        cargar(j, { base + j + ".png" });
    }
}

void GestorRecursos::cargar(const QString& clave, const QStringList& rutas) {
    QVector<QPixmap> frames;
    for (const QString& ruta : rutas) {
        QPixmap p(ruta);
        if (p.isNull()) {
            throw RecursoNoDisponibleException(ruta.toStdString());
        }
        frames.push_back(p);
    }
    animaciones_.insert(clave, frames);
}

QPixmap GestorRecursos::frame(const QString& clave, int indice) const {
    auto it = animaciones_.constFind(clave);
    if (it == animaciones_.constEnd() || it->isEmpty()) {
        return QPixmap();
    }
    const QVector<QPixmap>& frames = *it;
    int i = ((indice % frames.size()) + frames.size()) % frames.size();
    return frames.at(i);
}

int GestorRecursos::numFrames(const QString& clave) const {
    auto it = animaciones_.constFind(clave);
    return (it == animaciones_.constEnd()) ? 0 : it->size();
}
