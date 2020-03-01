#ifndef QMODULO_H
#define QMODULO_H

#include <QWidget>
#include <QList>
#include "qanalogico.h"
#include "Segnale.h"
#include "NetPlc.h"

class QSlot : public QWidget
{
    Q_OBJECT
public:
    explicit QSlot(QWidget *parent =0);
    ~QSlot();
    void  setSlot(int newslot,QList <CModulo *> modulo,QNetPlc *Processa);
    void  Edit();
    void  setManuale(bool newManuale);

protected:
    void paintEvent(QPaintEvent* event);

signals:
    void sendCambiaSegnale(int newslot,int newpoint, int newsegnale);

public slots:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);


private:
    int  slot;
    bool IsDigitale;
    int  TipoSlot;
    CModulo     *LastModulo;
    QVBoxLayout *Layout;
    QMLabel     *label;
    QList <QAnalogico *> Analogico;
    QList <QDigitale *> Digitale;
};



class QModulo : public QWidget
{
    Q_OBJECT

public:
    explicit QModulo(QWidget *parent =0);
    ~QModulo();
    void Init(QNetPlc *newProcessa);
    void Edit();
    void  setManuale(bool newManuale);

protected:
    void paintEvent(QPaintEvent *event);

signals:
    void sendCambiaSegnale(int newslot,int newpoint, int newsegnale);

public slots:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);

private:
    QList       < QSlot *> Slot;
    QNetPlc     *Processa;
    QWidget     *widUp;
    QWidget     *widDown;
    int         ModuloCount;
};

#endif // QMODULO_H
