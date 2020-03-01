#ifndef TURBO_H
#define TURBO_H
#include "MainCostanti.h"
#include "Segnale.h"
#include "Timer.h"

class CTurbo
{
public:
    CTurbo();
    virtual ~CTurbo();

public:
    void         CeckStato();
    bool         Set();
    bool         Reset();
    bool         SetOff();
    double       OnTime() {return (m_OnTime.Timer(Start->Segnale()));}
    void         OnTime(double NewOldOnTime){ m_OnTime.OnTime(NewOldOnTime);}
    bool         isOn();
    void         Simula(bool newSimula);
    bool         ErroreSetNormale() {return m_ErroreSetNormale;}

public:
    bool         Debug;
    CIOSegnale   *Start;
    CIOSegnale   *Stop;
    CIOSegnale   *Acc;
    CIOSegnale   *Errore;
    CIOSegnale   *Normale;
    CIOSegnale   *PowOn;
    CIOSegnale   *PowIsOn;
    CIOSegnale   *Acqua;
    CIOSegnale   *Temp;
    CIOSegnale   *Penning;

private:
    COnTimer     m_OnTime;
    CTimer       m_TimerSet;
    CTimer       m_TimerReset;
    CTimer       m_TimerSetNormale;
    bool         m_Set;
    bool         m_Reset;
    bool         m_ErroreSetNormale;

};

#endif // TURBO_H
