#ifndef QVUOTO_H
#define QVUOTO_H

#include <QWidget>
#include "NetPlc.h"
#include "qanalogico.h"
#include "DisegnoMacchina.h"

class QResistenza : public QWidget
{
    Q_OBJECT

public:
    explicit QResistenza(QWidget *parent = 0);
    ~QResistenza();
    void Init(QNetPlc *newProcessa);
    void Edit();
    void  setManuale(bool newManuale){
        AcquaCamera->setManuale(newManuale);
        AriaCompressaOn->setManuale(newManuale);
        LVentValv->setManuale(newManuale);
    }

    QDigitale   *AcquaCamera;
    QDigitale   *AriaCompressaOn;
    QDigitale   *LVentValv;

private:
    QVBoxLayout *Layout;
    QMLabel     *label;
    QAnalogico  *TempCamera;
    QAnalogico  *TempResistenza;
    QAnalogico  *TempCatodo;
    QDigitale   *Interlock;
    QDigitale   *PortaChiusa;
    QDigitale   *TavRuota;
    QNetPlc     *Processa;

};

class QGas : public QWidget
{
    Q_OBJECT
public:
    explicit QGas(QWidget *parent = 0);
    ~QGas();
    void Init(QNetPlc *newProcessa,int index);
    void Edit();
    void setManuale(bool newManuale){;
         Valv->setManuale(newManuale);
         SetGas->setManuale(newManuale);
    }
    QAnalogico  *SetGas;
    QAnalogico  *GetGas;
    QDigitale   *Valv;

private:
    QVBoxLayout *Layout;
    QMLabel     *label;
    QNetPlc   *Processa;

};

class QTurbo : public QWidget
{
    Q_OBJECT
public:
    explicit QTurbo(QWidget *parent = 0);
    ~QTurbo();
    void Init(QNetPlc *newProcessa);
    void Edit();
    void setManuale(bool newManuale){;
         Start->setManuale(newManuale);
         Stop->setManuale(newManuale);
         Acqua->setManuale(newManuale);
    }
    QDigitale   *Start;
    QDigitale   *Stop;
    QDigitale   *Acqua;

private:
    QVBoxLayout *Layout;
    QAnalogico  *Temperatura;
    QMLabel     *label;
    QMLabel     *OnTime;
    QDigitale   *Accelerazione;
    QDigitale   *Normale;
    QDigitale   *Errore;
    QNetPlc     *Processa;

};

class QMeccanica : public QWidget
{
    Q_OBJECT
public:
    explicit QMeccanica(QWidget *parent = 0);
    ~QMeccanica();
    void Init(QNetPlc *newProcessa);
    void Edit();
    void  setManuale(bool newManuale){;
          OnPwer->setManuale(newManuale);
    }
    QDigitale   *OnPwer;

private:
    QVBoxLayout *Layout;
    QMLabel     *label;
    QMLabel     *OnTime;
    QDigitale   *IsOnPwer;
    QDigitale   *Errore;
    QNetPlc     *Processa;

};


class QVuoto : public QWidget
{
    Q_OBJECT

public:
    explicit QVuoto(QWidget *parent = 0);
    ~QVuoto();
    void Init(QNetPlc *newProcessa);
    void Edit();
    void  setManuale(bool newManuale){
        Meccanica->setManuale(newManuale);
        Turbo->setManuale(newManuale);
        Resistenza->setManuale(newManuale);
        for(int i=0; i<3; i++)
          Gas[i]->setManuale(newManuale);
    }
    void setVersioneRidotta();

signals:
    void sendCambiaSegnale(int newslot,int newpoint, int newsegnale);
    void StartProcesso();

public slots:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);
    void on_StartProcesso();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QWidget     *widSx;
    CDisegnoMacchina   *Macchina;
    QWidget     *widDx;
    QWidget     *widCentro;
    QWidget     *widResistenza;
    QResistenza *Resistenza;
    QSensore    *Penning;
    QSensore    *Pirani;
    QSensore    *Baratron;
    QTurbo      *Turbo;
    QMeccanica  *Meccanica;
    QNetPlc     *Processa;
    QGas        *Gas[3];
    int hFoglio,wFoglio;
    bool VersioneRidotta;

};

#endif // QVUOTO_H
