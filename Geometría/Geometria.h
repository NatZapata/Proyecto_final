#ifndef GEOMETRIA_H
#define GEOMETRIA_H


namespace logica {


struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    Vec2() = default;
    Vec2(double x_, double y_) : x(x_), y(y_) {}

    Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }
    Vec2 operator*(double s)      const { return {x * s, y * s}; }
};

// detectar colision
struct RectF {
    double x = 0.0, y = 0.0, w = 0.0, h = 0.0;

    RectF() = default;
    RectF(double x_, double y_, double w_, double h_) : x(x_), y(y_), w(w_), h(h_) {}

    double derecha()  const { return x + w; }
    double abajo()    const { return y + h; }
    double centroX()  const { return x + w / 2.0; }
    double centroY()  const { return y + h / 2.0; }

    // Interseccion AABB clasica.
    bool intersecta(const RectF& o) const {
        return x < o.x + o.w && x + w > o.x &&
               y < o.y + o.h && y + h > o.y;
    }
};

} // namespace logica

#endif // GEOMETRIA_H
