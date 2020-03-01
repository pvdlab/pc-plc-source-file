#ifndef CVUOTO_H
#define CVUOTO_H

#include "Segnale.h"
#include "Errore.h"
#include "Camera.h"
#include "Pirani.h"
#include "Penning.h"
#include "Baratron.h"
#include "Meccanica.h"
#include "Tavola.h"
#include "Turbo.h"
#include "Resistenza.h"



class CVuoto
{


public:
    CVuoto();
    bool Riparti(){return FalseTrue(m_Errore);}

public:
    CErrore        pErrore;
    void           ResetErrore();
    void           CeckStato();
    bool           Set();
    bool           Reset();
    bool           Errore();
    bool           GoToStep(WORD );//,WORD,WORD
    bool           EnabledVenting();
    void           Ventila();
    void           StopVentila() {*Camera.Valv=false;}
    int            Stato();
    DWORD          VuotoTime() ;
    DWORD          StopVuotoTime() ;
    DWORD          Timer() {return m_TimerEvent.Timer();}
    void           StendBy();
    void           BaratronOffSet() {m_BaratronOffSet=Baratron.Segnale->Segnale();}
    void           Init(char *Buffer);
    WORD           getVentigTime(){return m_TimerStopVuoto.LeftTime();}

public:
    bool           Debug;
    CPenning       *Penning;
    CMeccanica     Meccanica;
    CBaratron      Baratron;
    CTurbo         Turbo;
    CTavola        Tavola;
    CCamera        Camera;
    CIOSegnale     *ManVuoto;
    CResistenza    *Resistenza;
    int            IdVuoto;


private:
    CTimer         m_TimerEvent;
    CTimer         m_TimerStopVuoto;
    bool           m_InSet;
    bool           m_InReset;
    bool           m_InStopVuoto;
    bool           m_StopVuoto;
    WORD           m_Pressione;
    bool           m_Errore;
    bool           m_Set;
    bool           m_Reset;
    bool           m_PreSet;
    DWORD          m_VuotoTime;
    DWORD          m_StopVuotoTime;
    bool           m_FlussaAcqua;
    WORD           m_BaratronOffSet;
    bool           m_EnabledVenting;

};


#endif // CVUOTO_H
