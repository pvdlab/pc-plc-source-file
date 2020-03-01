#ifndef BARATRON_H
#define BARATRON_H
#include "MainCostanti.h"
#include "Segnale.h"
#include "Timer.h"

class CBaratron
{
public:
    CBaratron();
    virtual ~CBaratron();
    double Pressione();

public:
    bool         Set(WORD NewPressione);
    bool         Reset(WORD NewPressione);
    bool         Errore();
    bool         SetOff();
    void         Ventila();

public:
    CIOSegnale   *Valvola;
    CIOSegnale   *Segnale;
    CIOSegnale   *Pirani;
    bool         Enabled;

private:
    bool         m_Set;
    bool         m_Reset;


};

#endif // BARATRON_H
