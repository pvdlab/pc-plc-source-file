#ifndef QPOTENZA_H
#define QPOTENZA_H

#include <QWidget>
#include "NetPlc.h"
#include "qanalogico.h"
#include "DisegnoMacchina.h"



class QBias : public QWidget
{
    Q_OBJECT
public:
    explicit QBias(QWidget *parent = 0);
    ~QBias();
    void Init(QNetPlc *newProcessa, int newIdMacchina);
    void Edit();
    void  setManuale(bool newManuale){
        SetV->setManuale(newManuale);
        PowOn->setManuale(newManuale);
        HVolt->setManuale(newManuale);
        LVolt->setManuale(newManuale);
        AcquaCamera->setManuale(newManuale);
        AcquaTurbo->setManuale(newManuale);}
    QAnalogico   *SetV;
    QDigitale    *PowOn;
    QDigitale    *HVolt;
    QDigitale    *LVolt;

private:
    QVBoxLayout     *widDx;
    QVBoxLayout     *widCentro;
    QVBoxLayout     *widSx;
    QMLabel         *label;
    QAnalogico  *GetV;
    QAnalogico  *GetI;
    QDigitale   *Allarme;
    QDigitale   *Arco;
    QDigitale   *PowIsOn;
    QDigitale   *PowErr;
    QDigitale   *IsHVolt;
    QDigitale   *OnEnable;
    QDigitale   *IsLVolt;
    QDigitale   *AcquaCamera;
    QDigitale   *AcquaTurbo;
    QDigitale   *AcquaIson;
    QDigitale   *Interlock;
    QDigitale   *PortaChiusa;
    QDigitale   *TavRuota;
    QDigitale   *Termostato;
    QNetPlc     *Processa;
    int         IdMacchina;

};

class QCatodi : public QWidget
{
    Q_OBJECT
public:
    explicit QCatodi(QWidget *parent = 0);
    ~QCatodi();
    void Init(QNetPlc *newProcessa,int index,int newIdMacchina);
    void Edit();
    void  setManuale(bool newManuale){
        SetI->setManuale(newManuale);
        OnPow->setManuale(newManuale);
        OnMainPow->setManuale(newManuale);}
    QAnalogico  *SetI;
    QDigitale   *OnPow;
    QDigitale   *OnMainPow;

private:
    int         SorIndex;
    QVBoxLayout *Layout;
    QMLabel     *label;
    QMLabel     *TrigCount;
    QMLabel     *OnTime;
    QAnalogico  *GetI;
    QAnalogico  *Temp;
    QDigitale   *Trigger;
    QDigitale   *IsOnArc;
    QDigitale   *ErrPow;
    QDigitale   *Schermo;
    QNetPlc     *Processa;
    int         IdMacchina;
};

class QPotenza : public QWidget
{
    Q_OBJECT

public:
    explicit QPotenza(QWidget *parent = 0);
    ~QPotenza();
    void Init(QNetPlc *newProcessa,int newIdMacchina);
    void Edit();
    void  setManuale(bool newManuale){
        Bias->setManuale(newManuale);
        if(Processa->CamereCount==1){
            for(int i=0; i<6;i++)
                Catodo[i]->setManuale(newManuale);
        }

    }
    void setVersioneRidotta();
    CDisegnoMacchina   *Macchina;
    CDisegnoMacchina   *MacchinaDue;

signals:
    void sendCambiaSegnale(int newslot,int newpoint, int newsegnale);

public slots:
    void CambiaSegnale(int newslot,int newpoint, int newsegnale);


protected:
    void paintEvent(QPaintEvent *event);

private:

    QWidget     *widCentro;
    QWidget     *widBias;
    QWidget     *widDx;
    QWidget     *widSx;
    QBias       *Bias;
    QCatodi     *Catodo[6];
    QNetPlc     *Processa;
    int         IdMacchina;

    int hFoglio,wFoglio;
    bool VersioneRidotta;
};

#endif // QPOTENZA_H
