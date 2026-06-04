#include "Nivel.h"
#include "Personaje.h"
#include "Obstaculo.h"
#include "ObjetoCayendo.h"
#include "Enemigo.h"
#include "Fisica.h"
#include <array>
#include <algorithm>
#include <cmath>

namespace logica {

Nivel::Nivel(int numeroNivel,
             const ConfiguracionDificultad& config,
             double anchoMundo, double altoMundo)
    : config_(config), anchoMundo_(anchoMundo), altoMundo_(altoMundo),
    sueloY_(altoMundo - 70.0), jugador_(nullptr),
    tiempoTranscurrido_(0.0), tiempoLimite_(config.duracionNivel),
    puntaje_(0), estado_(EstadoNivel::EnCurso),
    rng_(std::random_device{}()), acumSpawn_(0.0),
    velScroll_(config.velocidadJuego),
    enemigo_(nullptr), acumObjeto_(0.0), tiempoCoulomb_(0.0),
    numeroNivel_(numeroNivel) {
}

Nivel::~Nivel() {
    liberarTodo();
    delete enemigo_;
    enemigo_ = nullptr;
}


void Nivel::liberarTodo() { // liberar memoria
    delete jugador_;
    jugador_ = nullptr;
    for (Obstaculo* o : obstaculos_)  delete o;
    for (ObjetoCayendo* o : objetos_) delete o;
    obstaculos_.clear();
    objetos_.clear();
}

void Nivel::limpiarInactivos() {
    for (auto it = obstaculos_.begin(); it != obstaculos_.end(); ) {
        if (!(*it)->activo()) { delete *it; it = obstaculos_.erase(it); }
        else ++it;
    }
    for (auto it = objetos_.begin(); it != objetos_.end(); ) {
        if (!(*it)->activo()) { delete *it; it = objetos_.erase(it); }
        else ++it;
    }
}


double Nivel::aleatorio(double a, double b) {
    std::uniform_real_distribution<double> dist(a, b);
    return dist(rng_);
}

std::string Nivel::spriteJugueteAleatorio() {
    static const std::array<std::string, 9> juguetes = {{
        "toy_ball", "toy_car", "toy_dino", "toy_duck", "toy_pyramid",
        "toy_rocket", "toy_teddy", "toy_xylophone", "toy_xylo2"
    }};
    std::uniform_int_distribution<std::size_t> dist(0, juguetes.size() - 1);
    return juguetes[dist(rng_)];
}



void Nivel::iniciar() {
    if (numeroNivel_ == 1) iniciarNivel1();
    else                   iniciarNivel2();
}

void Nivel::iniciarNivel1() {
    liberarTodo();
    velScroll_ = config_.velocidadJuego;
    estado_ = EstadoNivel::EnCurso;
    tiempoTranscurrido_ = 0.0;
    puntaje_ = 0;
    acumSpawn_ = 0.0;
    jugador_ = new Personaje(20.0, sueloY_, config_.gravedad, config_.energiaInicial);
}

void Nivel::iniciarNivel2() {
    liberarTodo();
    delete enemigo_;
    enemigo_ = nullptr;

    velScroll_ = config_.velocidadJuego * 0.82;
    estado_ = EstadoNivel::EnCurso;
    tiempoTranscurrido_ = 0.0;
    puntaje_ = 0;
    acumSpawn_ = 0.0;
    acumObjeto_ = 0.0;
    tiempoCoulomb_ = 0.0;

    jugador_ = new Personaje(20.0, sueloY_, config_.gravedad, config_.energiaInicial);
    enemigo_ = new Enemigo(830.0, sueloY_,
                           config_.gravedad, config_.agresividadEnemigo);
}



void Nivel::actualizar(double dt) {
    if (numeroNivel_ == 1) actualizarNivel1(dt);
    else                   actualizarNivel2(dt);
}

void Nivel::actualizarNivel1(double dt) {
    if (estado_ != EstadoNivel::EnCurso) return;

    tiempoTranscurrido_ += dt;
    jugador_->actualizar(dt);

    acumSpawn_ += dt;
    double intervalo = 1.0 / config_.frecuenciaObstaculos;
    if (acumSpawn_ >= intervalo) {
        acumSpawn_ = 0.0;
        generarObstaculoNivel1();
    }

    for (Obstaculo* o : obstaculos_) {
        o->actualizar(dt);
        // colision con coordenadas de pantalla
        if (o->activo() && o->colisionaCon(*jugador_)) {
            jugador_->recibirDanio(o->costoEnergia());
            o->setActivo(false);
        } else if (!o->activo() && o->x() < jugador_->x()) {
            puntaje_ += 10;
        }
    }

    limpiarInactivos();
    puntaje_ += static_cast<int>(dt * 5.0);

    if (jugador_->sinEnergia())                    estado_ = EstadoNivel::Perdido;
    else if (tiempoTranscurrido_ >= tiempoLimite_) estado_ = EstadoNivel::Ganado;
}

void Nivel::actualizarNivel2(double dt) {
    if (estado_ != EstadoNivel::EnCurso) return;

    tiempoTranscurrido_ += dt;
    jugador_->actualizar(dt);

    double factorTiempo = std::min(1.55, 1.0 + tiempoTranscurrido_ / 90.0);
    acumSpawn_ += dt;
    double intervalo = 1.0 / (config_.frecuenciaObstaculos * factorTiempo);
    if (intervalo < 2.20) intervalo = 2.20;
    if (acumSpawn_ >= intervalo) {
        acumSpawn_ = 0.0;
        generarObstaculoNivel2();
    }

    acumObjeto_ += dt;
    if (acumObjeto_ >= aleatorio(1.05, 2.00)) {
        acumObjeto_ = 0.0;
        generarObjetoCayendo();
    }

    for (Obstaculo* o : obstaculos_) {
        o->actualizar(dt);
        if (o->activo() && o->colisionaCon(*jugador_)) {
            jugador_->recibirDanio(o->costoEnergia());
            o->setActivo(false);
        }
    }

    if (tiempoCoulomb_ > 0.0) tiempoCoulomb_ -= dt;
    aplicarFuerzaCoulomb(dt);

    for (ObjetoCayendo* obj : objetos_) {
        obj->actualizar(dt);
        if (obj->activo() && obj->colisionaCon(*jugador_)) {
            if (obj->premiado()) {
                tiempoCoulomb_ = 5.0;
                jugador_->activarEnergizado(5.0);
                puntaje_ += 25;
            } else {
                jugador_->recibirEnergia(obj->valorEnergia());
                puntaje_ += 5;
            }
            obj->setActivo(false);
        }
    }

    enemigo_->percibir(*jugador_);
    enemigo_->razonar();
    enemigo_->actuar(dt);
    double desempeno = jugador_->energia() / jugador_->energiaMax();
    enemigo_->aprender(desempeno);

    enemigo_->actualizarPiedras(dt);
    for (const PiedraData& p : enemigo_->piedras()) {
        RectF cajaPiedra(p.x, p.y, p.ancho, p.ancho);
        if (cajaPiedra.intersecta(jugador_->caja())) {
            jugador_->recibirDanio(Enemigo::calcularDanioPiedra(p));
        }
    }

    limpiarInactivos();
    puntaje_ += static_cast<int>(dt * 6.0);

    if (jugador_->sinEnergia())                    estado_ = EstadoNivel::Perdido;
    else if (tiempoTranscurrido_ >= tiempoLimite_) estado_ = EstadoNivel::Ganado;
}
// nivel 1

void Nivel::generarObstaculoNivel1() {
    const double distanciaMinima = (config_.nivel == NivelDificultad::Facil) ? 520.0
                                   : (config_.nivel == NivelDificultad::Normal) ? 460.0 : 400.0;
    for (const Obstaculo* existente : obstaculos_) {
        if (existente->activo() && existente->x() > anchoMundo_ - distanciaMinima) return;
    }

    const bool usarPapa = aleatorio(0.0, 1.0) > 0.65;
    Obstaculo::Movimiento mov = Obstaculo::Movimiento::Estatico;
    std::string sprite;
    double ancho = 74.0, alto = 74.0;
    double costo = aleatorio(6.0, 12.0);

    if (usarPapa) {
        sprite = "potato";
        mov = (aleatorio(0.0, 1.0) < 0.55) ? Obstaculo::Movimiento::Lateral
                                           : Obstaculo::Movimiento::Vertical;
        ancho = 64.0; alto = 82.0; costo += 3.0;
    } else {
        sprite = (aleatorio(0.0, 1.0) < 0.50) ? "rex" : "alien";
        ancho = sprite == "rex" ? 92.0 : 76.0;
        alto  = sprite == "rex" ? 82.0 : 76.0;
    }

    Obstaculo* o = new Obstaculo(anchoMundo_ + 90.0, sueloY_ - alto,
                                 ancho, alto, mov, costo, velScroll_, sprite);
    obstaculos_.push_back(o);
}
// nivel 2

void Nivel::generarObstaculoNivel2() {
    const double distanciaMinima = (config_.nivel == NivelDificultad::Facil) ? 540.0
                                   : (config_.nivel == NivelDificultad::Normal) ? 480.0 : 420.0;
    for (const Obstaculo* existente : obstaculos_) {
        if (existente->activo() && existente->x() > anchoMundo_ - distanciaMinima) return;
    }

    const bool dinamico = aleatorio(0.0, 1.0) > 0.55;
    std::string sprite;
    Obstaculo::Movimiento mov = Obstaculo::Movimiento::Estatico;
    double ancho = 76.0, alto = 76.0;
    double costo = aleatorio(6.0, 12.0);

    if (dinamico) {
        if (aleatorio(0.0, 1.0) < 0.55) {
            sprite = "potato";
            ancho = 64.0; alto = 82.0;
        } else {
            sprite = "traveller";
            ancho = 92.0; alto = 70.0;
        }
        mov = (aleatorio(0.0, 1.0) < 0.65) ? Obstaculo::Movimiento::Lateral
                                           : Obstaculo::Movimiento::Vertical;
        costo += 2.0;
    } else {
        sprite = (aleatorio(0.0, 1.0) < 0.5) ? "rex" : "alien";
        ancho = sprite == "rex" ? 88.0 : 74.0;
        alto  = sprite == "rex" ? 78.0 : 74.0;
    }

    Obstaculo* o = new Obstaculo(anchoMundo_ + 90.0, sueloY_ - alto,
                                 ancho, alto, mov, costo, velScroll_, sprite);
    obstaculos_.push_back(o);
}

void Nivel::generarObjetoCayendo() {
    double x = aleatorio(20.0, 700.0);
    bool premiado = aleatorio(0.0, 1.0) < 0.22;
    double energia = premiado ? 5.0 : aleatorio(8.0, 16.0);
    std::string sprite = premiado ? "alien" : spriteJugueteAleatorio();
    ObjetoCayendo* obj = new ObjetoCayendo(x, -45.0, energia, premiado,
                                           config_.gravedad, sueloY_, sprite);
    objetos_.push_back(obj);
}

void Nivel::aplicarFuerzaCoulomb(double dt) {
    if (tiempoCoulomb_ <= 0.0) return;
    Vec2 posJugador(jugador_->centroX(), jugador_->centroY());
    for (ObjetoCayendo* obj : objetos_) {
        if (!obj->activo()) continue;
        Vec2 posObj(obj->centroX(), obj->centroY());
        Vec2 F = Fisica::fuerzaCoulomb(posJugador, posObj);
        F.x = std::clamp(F.x, -850.0, 850.0);
        F.y = std::clamp(F.y, -850.0, 850.0);
        obj->aplicarFuerza(F.x, F.y, dt);
    }
}


void Nivel::saltarJugador() {
    if (jugador_ && estado_ == EstadoNivel::EnCurso)
        jugador_->saltar(180.0);
}

void Nivel::moverJugadorIzquierda(bool activo) {
    if (jugador_ && estado_ == EstadoNivel::EnCurso)
        jugador_->moverIzquierda(activo);
}

void Nivel::moverJugadorDerecha(bool activo) {
    if (jugador_ && estado_ == EstadoNivel::EnCurso)
        jugador_->moverDerecha(activo);
}

} // namespace logica
