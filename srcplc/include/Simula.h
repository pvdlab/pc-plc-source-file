#ifndef CSIMULA_H
#define CSIMULA_H

#include "StdAfx.h"
#include "Ciclo.h"

class CSimula
{


public:
    CSimula();
    CSimula& operator()(CCiclo *NewProcessa,int newStepTime);
    void     main();
    void     Init();

private:
    void            eventoSet(char *Buffer);
    bool            eventomainMacchina();
    bool            eventoBiaLow();

private:
    CCiclo          *Processa;
    CTimer          TimerPenning[2];
    CTimer          TimerBaratron[2];
    CTimer          TimerPirani[2];
    CTimer          Timer[2];
    CTimer          m_EventoTimer[2];
    BYTE            m_Evento[2];
    WORD            m_Penning[2];
    WORD            m_Baratron[2];
    WORD            m_Pirani[2];
    CTimer          TimerOnOff[2];
    CTimer          m_TimerEvento[2];
    bool            m_ComandoManuale;
    bool            m_OnCatodi;
    int             m_StetTime[2];


};


#endif
