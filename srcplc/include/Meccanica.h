#ifndef MECCANICA_H
#define MECCANICA_H
#include "MainCostanti.h"
#include "Segnale.h"
#include "Timer.h"

class CMeccanica
{
public:
    CMeccanica();
    virtual ~CMeccanica();

public:
    void          CeckStato();
    bool          Set();
    bool          SetOff();
    bool          Reset();
    double        OnTime() {return (m_OnTime.Timer(PowIsOn->Segnale()));}
    void          OnTime(double NewOldOnTime){  m_OnTime.OnTime(NewOldOnTime);   }
    bool          IsOn();

public:
    bool           Debug;
    CIOSegnale    *PowOn;
    CIOSegnale    *PowIsOn;
    CIOSegnale    *PowErr;

private:
    bool          m_Set;
    bool          m_Reset;
    COnTimer      m_OnTime;
    CTimer        m_TimerSet;


};

#endif // MECCANICA_H
