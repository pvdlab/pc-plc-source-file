#ifndef PENNING_H
#define PENNING_H
#include "Segnale.h"
#include "Timer.h"
#include "Errore.h"

class CPenning
{
public:
    CPenning(CErrore     *pNewErrore);
    virtual ~CPenning();

public:
    bool        Debug;
    bool        Set();
    bool		Reset();
    bool		SetOff();
    bool        ProcessoIsEnabled() {return m_ProcessoIsEnabled;}
    WORD        Tipo;
    CIOSegnale  *Segnale();
    double      Pressione() ;

private:
    double      Pressione(CIOSegnale  *pSegnale);

private:
    bool Errore();

public:
    CIOSegnale  *Penning;
    CIOSegnale  *Pirani;
    CIOSegnale  *Interlock;
    CIOSegnale  *PowOn;
    CIOSegnale  *TurboNormale;
    CIOSegnale  *TurboStart;


private:
    CErrore     *pErrore;
    CIOSegnale  *pSegnale;
    CTimer      m_TimerStart;
    CTimer      m_TimerInterlock;
    CTimer      m_TimerProStart;
    bool        m_Set;
    bool        m_Reset;
    bool        m_InterlockErrore;
    bool        m_IsStartErrore;
    bool        m_ProcessoEnabledTimeOut;
    bool        m_ProcessoIsEnabled;
    double      m_Pressione;

};
#endif // PENNING_H
