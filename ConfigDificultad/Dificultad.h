#ifndef DIFICULTAD_H
#define DIFICULTAD_H

#include <string>
#include <array>
#include <stdexcept>

// dificultad del juego. (velocidad, frecuencia de obstaculos, energia y enemigo)

namespace logica {

class ConfiguracionInvalidaException : public std::invalid_argument {
public:
    explicit ConfiguracionInvalidaException(const std::string& detalle)
        : std::invalid_argument("Configuracion invalida: " + detalle) {}
};

enum class NivelDificultad { Facil = 0, Normal = 1, Dificil = 2 };

struct ConfiguracionDificultad {
    NivelDificultad nivel = NivelDificultad::Normal;
    double velocidadJuego        = 240.0;  // MRU
    double frecuenciaObstaculos  = 0.6;
    double gravedad              = 1200.0; // px/s^2 para saltos
    int    energiaInicial        = 100;
    double agresividadEnemigo    = 1.0;    // multiplicador
    double duracionNivel         = 90.0;

    std::string nombre() const {
        switch (nivel) {
        case NivelDificultad::Facil:   return "Facil";
        case NivelDificultad::Normal:  return "Normal";
        case NivelDificultad::Dificil: return "Dificil";
        }
        return "Desconocida";
    }

    static ConfiguracionDificultad crear(NivelDificultad n) {
        static const std::array<ConfiguracionDificultad, 3> presets = {{
            //                          vel,  frec, grav,  energia, agres, dur
            { NivelDificultad::Facil,   210.0, 0.45, 1050.0, 120, 0.55, 60.0 },
            { NivelDificultad::Normal,  245.0, 0.60, 1120.0, 100, 0.75, 75.0 },
            { NivelDificultad::Dificil, 295.0, 0.78, 1220.0,  80, 1.05, 90.0 }
        }};

        int idx = static_cast<int>(n);
        if (idx < 0 || idx >= static_cast<int>(presets.size())) {
            throw ConfiguracionInvalidaException("nivel de dificultad fuera de rango");
        }
        return presets[static_cast<std::size_t>(idx)];
    }
};

} // namespace logica

#endif // DIFICULTAD_H
