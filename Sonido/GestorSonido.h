#ifndef GESTORSONIDO_H
#define GESTORSONIDO_H

#include <QObject>
#include <QString>
#include <QHash>


#ifdef TIENE_AUDIO
class QSoundEffect;
#endif

class GestorSonido : public QObject {
    Q_OBJECT
public:
    explicit GestorSonido(QObject* parent = nullptr);
    ~GestorSonido() override;

    void reproducirEvento(const QString& clave);
    void reproducirFondo(const QString& clave);
    void detenerFondo();
    void setSilenciado(bool s);
    bool silenciado() const { return silenciado_; }

private:
    bool silenciado_;
#ifdef TIENE_AUDIO
    QHash<QString, QSoundEffect*> efectos_;
    QSoundEffect* fondo_;
    QSoundEffect* cargar(const QString& archivo);
#endif
};

#endif // GESTORSONIDO_H
