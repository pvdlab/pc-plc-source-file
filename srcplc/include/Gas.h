#ifndef CGAS_H
#define CGAS_H


#include "Segnale.h"
#include "Errore.h"


class CGas
{

public:
    CGas();
    bool Riparti(){return false;}
    bool IsPiraniAllarm(WORD Pirani);
    bool IsGoToStep() {return m_IsGoToStep;}
    bool ManualeRun();

public:
    CErrore         pErrore;
    CIOSegnale      *SetG;
	CIOSegnale      *GetG;
    CIOSegnale      *Valvola;
    CIOSegnale      *Baratron;
    bool            Enabled;

private:
	CTimer          m_TimerPreStep;
	CTimer          m_TimerReset;
	CTimer          m_TimerRitardo;
	CTimer          m_TimerDp;
	CTimer          m_TimerErrore;
	bool            m_Reset;
    bool            m_ResetSetG;
	bool            m_PreSet;
	bool            m_Set;
	bool            m_ResetSegnali;
	bool            m_StendBy;
	bool            m_InReset;
	bool            m_ResetErrore;
	int             m_ActStep;
	WORD            m_Info;
	bool            m_Errore;
	WORD            m_Offset;
    WORD            m_GasStep;
	bool            m_ErrorPreSet; 
    bool            m_ErrorGasMin;
	WORD            m_MidBaratron;
    bool            m_IsGoToStep;
    bool            m_IsStart;

    
public:
	WORD            OnTime(); 
	void            Offset(WORD NewOffset){m_Offset=NewOffset;}
	void            CeckStato();
    bool            Set();
    void            PreSet(bool newPreset){m_PreSet=newPreset;}
	bool            InPreSet();
	bool            Reset();
	bool            SetOff();
	bool            StendBy();
	bool            ResetStendBy();
	bool            ResetSegnali();
	bool            Errore();
	bool            ErrorPreSet(){ return m_ErrorPreSet;}
	bool            GoToStep(BYTE Step,DWORD Tempo);
    bool            GoToStep(BYTE Step);
	void            SetRitardo( int Ritardo){m_TimerRitardo( Ritardo,Ritardo);m_TimerRitardo.Reset();}
    void            ResetErrore() ;
    bool            IsMin();
    WORD            Min();
    void            ceckNextStepError();
    void            ceckGasErrore();


};

#define D_GAS 3


class CMiscela
{
public:
    CMiscela();
    virtual ~CMiscela();

public:
	CGas         *Gas[D_GAS];
    CIOSegnale   *Pressione;
    bool         Enabled;

private:
    WORD         m_Info[D_GAS];       
	WORD         m_Pressione;
    
public:
    bool         Set();
	bool         GoToStep(WORD Pressione);
            

};
 
 


#endif
