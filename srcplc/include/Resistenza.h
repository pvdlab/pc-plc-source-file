#ifndef CRESISTENZA_H
#define CRESISTENZA_H

#include "Segnale.h"
#include "Errore.h"


class CResistenza
{

public:
    CResistenza();
    bool Riparti(){return FalseTrue(m_Errore);}
    bool IsGoToStep() {return m_IsGoToStep;}
    bool IsTempCameraRotta(){return m_Temp_CameraRotta;}

public:
    CErrore        pErrore;
    CIOSegnale     *ReleDx;
    CIOSegnale     *ReleSx;
    CIOSegnale     *PowOn;
    CIOSegnale     *PowErr;
    CIOSegnale     *PowIsOn;
    CIOSegnale     *Temp_Resistenza;
    CIOSegnale     *Temp_Camera;
    CIOSegnale     *Temp_Acqua;
    CIOSegnale     *Temp_Turbo;
    CIOSegnale     *Termostato;
    CIOSegnale     *AzotoValv;
    CIOSegnale     *ArgonValv;
    CIOSegnale     *AcetiValv;
    CIOSegnale     *Azoto;
    CIOSegnale     *Argon;
    CIOSegnale     *Aceti;
    CIOSegnale     *Gas;
    CIOSegnale     *ManVuoto;
    bool           Enabled;
    bool           m_IsGoToStep;
    bool           Debug;

private:
    bool            m_Reset;
    bool            m_PreSet;
    bool            m_Set;
    bool            m_ResetSegnali;
    bool            m_StendBy;
    WORD            m_Potenza;
    WORD            m_Temp_Set;
    CTimer          m_TimerOn;
    CTimer          m_TimerOff;
    CTimer          m_TimerSet;
    WORD            m_Pressione;
    bool            m_Off;
    bool            m_Errore;
    bool            m_PiraniAllarm;
    int             m_Temp_Resistenza;
    bool            m_NonScalda;
    bool            m_Temp_CameraRotta;
    bool            m_Temp_TurboRotta;

public:
    WORD            Potenza(){ return m_Potenza;}
    void            Potenza(WORD newPotenza){m_Potenza=newPotenza;}
    WORD            TempSet(){ return m_Temp_Set;}
    void            TempSet(WORD newTemp){m_Temp_Set=newTemp;}
    void            CeckStato();
    bool            PreSet();
    bool            Set(BYTE   CTipoStep);
    bool            ManualeRun();
    bool            IsOn() ;
    bool            SetOn();
    bool            Reset();
    bool            SetOff();
    bool            StendBy();
    bool            ResetStendBy();
    bool            ResetSegnali();
    bool            Errore();
    bool            GoToStep(WORD NewPotenza,WORD NewTemp,WORD NewPressione);
    void            Init(BYTE *Buffer);
    void            ResetErrore();
    void            ceckNextStepError();
    void            ceckPowerError();


};

#endif // CRESISTENZA_H
