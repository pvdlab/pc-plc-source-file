#ifndef CBIAS_H
#define CBIAS_H

#include "Segnale.h"
#include "Errore.h"
#include "Catodo.h"


class CBias
{

public:
    CBias();
    void Init(WORD NewSorgenteCount,BYTE newBias);
    bool              SetArcOff();
    bool Riparti(){return m_Riparti;}
    void Repit(BYTE newRepit){m_Repit=newRepit;}
    CSorgente    *Sorgente;
    bool IsGoToStep() {return m_IsGoToStep;}
    double DSegnale(CIOSegnale  *pSegnale);
    int    Segnale(CIOSegnale  *pSegnale);
    int    Segnale(double dSegnale);

public:
	CErrore         pErrore;
    CIOSegnale      *SetV;
	CIOSegnale      *GetV;
	CIOSegnale      *GetI;
	CIOSegnale      *PowOn;
	CIOSegnale      *PowIsOn;
	CIOSegnale      *PowErr;
	CIOSegnale      *BiasIsOn;
	CIOSegnale      *On;
    CIOSegnale      *OnEnable;
    CIOSegnale      *IsLVolt;
    CIOSegnale      *LVolt;
	CIOSegnale      *HVolt;
	CIOSegnale      *IsHVolt;
	CIOSegnale      *Allarme;
    CIOSegnale      *Arco;
    CIOSegnale      *Interlock;
    CIOSegnale      *BiasNext;
    CIOSegnale      *BiasIsNext;
    bool            Enabled;
    WORD            SorgenteCount;
    bool            Debug;

private:
    BYTE            m_Bias;
    bool            m_PrimoEtching;
	bool            m_Errore;
    BYTE            m_CTipoStep;
	CTimer          m_Timer;
    CTimer          m_AspettaTimer;
    CTimer          m_RisalitaTimer;
    CBTimer         m_BiasOffTime;
	bool            m_Reset;
	bool            m_Set;
	bool            m_ResetSegnali;
	bool            m_StendBy;
    WORD            m_ArcCount;
    WORD            m_ActStep;
    bool            m_ArcIsOn;
    WORD            m_Info;
    bool            m_SetTap;
    bool            m_IsSetTap;
	bool            m_ErroreTap;
	bool            m_ResetTap;
	bool            m_BiasIsLow;
    BYTE            m_BiasLowCount;
    BYTE            m_Repit;
    BYTE            m_PerOffTime;
    bool            m_Schutter;
    bool            m_Riparti;
    int             m_BiasAllarmCount;
    bool            m_ContrappostiIsErrore;
    bool            m_ContrappostiVedo;
    bool            m_IsGoToStep;
    int             m_PercentOffTime;
    WORD            m_SetVBias;
    WORD            m_GetVBias;

public: 
    BYTE            BiasLowCount();
    WORD            OnTime();
    bool            IsSetTap();
	void            CeckStato();
    bool            Set();
    bool            ManualeRun();
    bool            IsOn() ;
	bool            SetOn();
	bool            Reset();
	bool            SetOff();
	bool            StendBy();
	bool            ResetStendBy();
	bool            ResetSegnali();
	bool            Errore();
	void            ArcIsOn(bool NewArcIsOn); 
    bool            GoToStep(BYTE,DWORD ,BYTE,BYTE);
    bool            GoToStep(BYTE,BYTE ,BYTE);
    bool            SetTap(WORD);
    bool            Set_MDXII_Tap(WORD);
    bool            Set_BP5_Tap(WORD);
    void            ResetErrore() { pErrore.Reset(); m_Errore=false ;}
    void            Shutter(bool newSchutter) {m_Schutter=newSchutter;}
    bool            ContrappostiIsErrore();
    void            ceckNextStepError();
    void            ceckPowerError();
    int             Min(){return 0;}
    bool            IsSchermiChiusi();

};

#endif
