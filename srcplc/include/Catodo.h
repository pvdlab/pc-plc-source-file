#ifndef CCATODO_H
#define CCATODO_H


#include "Segnale.h"
#include "Errore.h"

class CPosCatodo
{
public:
    CPosCatodo() {}
};

class CCatodo
{

public:
    CCatodo();
    CCatodo& operator()(CSTimer *pMainTimer);
    bool   SetEnabled(bool NewEnabled);
    bool   Riparti(){return m_Riparti;}
    void   setMaxI(WORD newMaxI){m_MaxI=newMaxI;}
    WORD   getMaxI(){return m_MaxI;}
    bool   ArcIsOn(){return m_IsOnArc;}
    bool   IsOn(){return m_ceckCatodoPowErr;}
    void   ceckCatodoPowErr();
    void   SetOff();
    bool   IsGoToStep() {return m_IsGoToStep;}
    double OnTime() ;
    void   OnTime(DWORD NewOldOnTime){ m_OnTime.OnTime(NewOldOnTime);}
    void   TrigCount(WORD NewTrigCount) { m_TrigCount=NewTrigCount;}
    WORD   TrigCount() { return m_TrigCount;}
    bool   Set(CIOSegnale *pSetI);
    bool   Reset();
    bool   Errore();
    void   ResetTrigger();
    bool   CeckStato();
    WORD   TrigOnCount(){ return m_TrigOnCount+m_ProcTrigCount;}
    void   TrigOnCount(WORD NewTrigOnCount){ m_TrigOnCount=NewTrigOnCount;}
    bool   SetArcOff();
    bool   IsSetArcOff();
    void   CTipoStep(BYTE NewCTipoStep){ m_CTipoStep=NewCTipoStep;}
    void   SetAh(WORD NewSetAh){m_TimerOn(NewSetAh);}
    void   Init(BYTE *Buffer);
    void   ResetErrore() { pErrore.Reset();m_Errore=false ;}
    void   IgnoraTargetConsumato(){m_IgnoraTargetConsumato=true;}
    bool   IsTargetConsumato(CIOSegnale *pSetI);
    int    Consumo(){return m_NextConsumo;}
    WORD   Min(){return CAT_MIN_GET_CORRENTE;}

public:
    CErrore        pErrore;
    CIOSegnale     *GetI;
	CIOSegnale     *Trigger;
	CIOSegnale     *OnPow;
	CIOSegnale     *IsOnArc;
    CIOSegnale     *Temp;
    CIOSegnale     *ErrPow;
    CIOSegnale     *AcquaIsOn;
    bool           Enabled;
    bool           Aspetta;
    bool           Debug;
    BYTE           TipoTraget;

private:
    COnTimer       m_OnTime;
	CSTimer        *m_pMainTimer;
	bool           m_Reset;
	bool           m_Set;
	bool           m_IsOnArc;
	bool           m_ceckCatodoPowErr;
	bool           m_PowIsOn;
    CTimer         m_TimerOn;
	CTimer         m_TimerTrigger;
    CTimer         m_AspettaTimer;
    CTimer         m_BeckArcTimer;
    CTimer         m_RegolaTimer;
    CTimer         m_SetArcOffTimer;
    CSTimer        m_Ah;
    CTimer         m_TimerOffArc;
    DWORD          m_ArcAh;
	WORD           m_ArcOnOffCount;
    WORD           m_UltimoAumento;
	WORD           m_TrigOnCount;
    WORD           m_ProcTrigCount;
    DWORD          m_OldAh;
	bool           m_Errore;
    bool           m_PreSet;
    BYTE           m_CTipoStep;
    WORD           m_TrigCount;
	bool		   m_ErrPow;
    bool           m_Trigger;
    bool           m_OnPow;
    bool           m_Riparti;
    bool           m_BeckArc;
    bool           m_IsCorrenteAlta;
    bool           m_PowerErrore;
    WORD           m_MaxI;
    double         m_Regola;
    bool           m_IsGoToStep;
    double         m_OnTimeFattore;
    int            m_Consumo;
    bool           m_IgnoraTargetConsumato;
    int            m_NextConsumo;

	
};



class CSorgente
{

public:
    CSorgente();
    CSorgente& operator()(CSTimer *pMainTimer);
    bool Riparti(){return m_Riparti;}
    bool SchermoApri();
    bool SchermoChiudi();
    bool IsGoToStep() {return m_IsGoToStep;}
    bool ManualeRun();


public:
	CErrore       pErrore;
    CIOSegnale    *SetI;
	CIOSegnale    *PowOn;
    CIOSegnale    *PowIsOn;
    CIOSegnale    *PowErr;
    CIOSegnale    *Schermo;
    CCatodo       *Catodo;
    bool          Enabled;

private:
	CSTimer       *m_pMainTimer;
	bool          m_Reset;
	bool          m_Set;
	bool          m_ResetSegnali;
	bool          m_StendBy;
    bool          m_InReset;
    CTimer        m_Timer;
    BYTE          m_ActStep;
	WORD          m_Corrente;
	WORD          m_CatodiOn;
	bool          m_Spedito;
	BYTE          m_CTipoStep;
	BYTE          m_CatodoCount;
	bool          m_Errore;
	BYTE          m_Index;
	bool          m_BiasIsOn;
    bool          m_CatodoOnPerBias;
    bool          m_Riparti;
    bool          m_EraSpenta;
    bool          m_IsGoToStep;
	
public:
    bool          SetArcOff();
	void          CeckStato();
    bool          Set();
	bool          Reset();
	bool          Errore();
	bool          StendBy();
	bool          ResetStendBy();
	bool          ResetSegnali();
	bool          GoToStep(BYTE Step,DWORD Tempo,BYTE);
	bool          GoToStep(BYTE Step,BYTE);
	bool          IsOn();
    void          ceckCatodoPowErr();
	bool          ArcIsOn();
	bool          SetOn();
	bool          SetOff();
    void	      BiasIsOn(bool NewBiasIsOn){ m_BiasIsOn=NewBiasIsOn;}
	void          ResetErrore() ;
    void          ceckNextStepError();
    void          ceckPowerError();


private:
    //bool          SetEnabled(int Info);
    bool          Converti(int Info,BYTE *pBuffer);

};



#endif 
