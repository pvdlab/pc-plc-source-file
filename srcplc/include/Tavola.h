#ifndef TAVOLA_H
#define TAVOLA_H
#include "MainCostanti.h"
#include "Segnale.h"
#include "Timer.h"

class CTavola
{
public:
    CTavola();
    virtual ~CTavola();

public:
    bool          Debug;
    CIOSegnale   *OnOff;
    CIOSegnale   *Ruota;
    CIOSegnale   *Speed;
    CIOSegnale   *Manuale;

public:
    bool         Reset();
    bool         Set();
    bool         Errore(){return m_Errore;}
    void         GiriSecondo(WORD NewGiriSecondo){m_GiriSecondo=NewGiriSecondo;}
    WORD         GiriSecondo(){return m_GiriSecondo;}
    bool         Impara();
    void         OnTime(WORD NewOnTime){m_OnTime=NewOnTime;}
    void         OffTime(WORD NewOffTime){m_OffTime=NewOffTime;}
    WORD         OnTime(){return m_OnTime;}
    WORD         OffTime(){return m_OffTime;}
    bool         SetOff(){return  OnOff->Set(false);}


private:
    CTimer       m_Timer;
    CTimer       m_TimerStart;
    WORD         m_OnTime;
    WORD         m_OffTime;
    WORD         m_MinOnTime;
    WORD         m_MinOffTime;
    bool         m_IsImpara;
    BYTE         m_Count;
    bool         m_Ruota;
    bool         m_Errore;
    WORD         m_GiriSecondo;
    bool         m_TmpErrore;
    bool         m_Reset;


};
#endif // TAVOLA_H
