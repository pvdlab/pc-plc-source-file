#ifndef PIRANI_H
#define PIRANI_H
#include "Segnale.h"
#include "Timer.h"

class CPirani
{
public:
    CPirani();

public:
    bool        Set();
    bool		Reset();
    bool		Errore(){ return m_Errore; }

public:
    CIOSegnale  *Segnale;

private:
    CTimer      m_Timer;
    bool        m_Set;
    bool        m_Reset;
    bool        m_Errore;

};
#endif // PIRANI_H
