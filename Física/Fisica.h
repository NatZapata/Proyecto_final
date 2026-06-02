
namespace logica {
namespace Fisica {

constexpr double GRAVEDAD       = 1200.0;
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

// Fuerza electrica que 'fuente' (energizada) ejerce sobre un 'juguete'.
// F = k * q1 * q2 / r^2 , dirigida desde el juguete hacia la fuente (atraccion).
Vec2 fuerzaCoulomb(const Vec2& posFuente, const Vec2& posJuguete,
                   double cargaFuente = CARGA_JUGADOR,
                   double cargaJuguete = CARGA_JUGUETE);

// Segunda ley de Newton
// Fuerza de impacto F = m * a. El "danio" recibido es proporcional a esa fuerza.
double fuerzaImpacto(double masa, double aceleracion);

// Danio que produce una piedra de masa 'masa' que viaja a rapidez 'rapidez'.
// Modelado como F = m * a, donde a se estima a partir de la rapidez de impacto.
double danioPorImpacto(double masa, double rapidez);

}
}

#endif
