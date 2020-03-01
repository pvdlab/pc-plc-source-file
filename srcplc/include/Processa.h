#ifndef CPROCESSA_H
#define CPROCESSA_H

#include "Catodo.h"
#include "Vuoto.h"
#include "Gas.h"
#include "Bias.h"
#include "Resistenza.h"
#include "DataFile.h"
#include "Errore.h"
#include "StdAfx.h"
#ifdef NUOVA_CLASSE_PROCESSO
#include "Processoplc.h"
#endif


class CProcessa
{

public:
    CProcessa(int newtipoapp,int newTipoImpianto);

    BYTE       getComando(){return pData.Comando;}
    void       printErrori();
    void       Macchina_SalvaRegola();
    void       resetTimeOut(){m_PowerTimeOut.Reset();}

public:

    bool              Debug;
    WORD              Impianto;
    CGas              *GGas;
    CModulo           *pModulo;
    CBias             *Bias;
    CMiscela          Miscela;
    CResistenza       Resistenza;
    CVuoto            Vuoto;
#ifndef NUOVA_CLASSE
    CPlc              Plc;
#endif
	DataType          pData;
    BYTE              Init[D_BUFFER];
    bool              m_Riparti;
    WORD              SorgenteCount;
    WORD              GasCount;
    WORD              CatodoCount;
    WORD              ModuliCount;


    bool              Spedito;
    CErrore           pErrore;
    WORD              m_ErrSize;
    int               TipoApp;
    WORD              m_ModuloCount;
    char              SimulaBuffer[D_MSG];
    bool              IsPerSimula;
    BYTE              *m_pErrMacchina;
    CIOSegnale        *PowOutdig;
    CIOSegnale        *Reset;
    CSTimer           m_MainTimer;
#ifndef NUOVA_CLASSE_PROCESSO
    //////TOLGO///////////////////////
    StepType          m_Step[D_STEP];
    WORD              m_StepCount;
    ///////ENDTOLGO///////////////
#else
   CProcessoPlc *ProcessoPlc;
#endif


private:
#ifndef NUOVA_CLASSE_PROCESSO
    //////TOLGO///////////////////////
    CTimer       m_Timer;
    CTimer       m_StepTimer;
    WORD         m_StopTime;
    WORD         m_StepIndex;
    bool         m_IsLoadDaFile;
    ///////ENDTOLGO///////////////
#endif

    char              Msg[D_BUFFER];
    CIOSegnale        *pSegnali;
    int               SegnaleCount;
	CTimer            m_OnOffStepTimer;
    CTimer            m_TimerPreSet;
	CTimer            TimerEvent;
    CTimer            TimerOnArc;
    CTimer            m_PowerTimeOut;
    CTimer            m_GasTimeOut;
	bool              m_End;
    DWORD             m_Tempo;
    WORD              m_Repit;
	bool              m_PreSet;
	bool              m_InPreSet;
	bool              m_GasInPreset;
	bool              m_BiasInPreset;
	bool              m_PowInPreset;
    DWORD             m_TimeOn;
    bool              m_PowOn;
    bool              m_InReset;
	bool              m_StandByCatodi;
	bool              m_NewEvento;
    DWORD             m_NextTimeStep;
    int               m_NextStep;
	bool              m_IsInNextStep;
    bool              m_SetGasOffSet;
    WORD              m_OnTime;
    bool              m_ErrorePreset;
    WORD              m_SegnaliCount;
    bool              m_PlcErrore;
    WORD              m_LenPlcSegnali;
    int               m_RipartiCount;
    bool              m_ArcIsOn;
    bool              ProcessoCorrotto;
    bool              m_BiasIsLow;
    bool              m_Macchina_ResetGas;
    BYTE              m_RiscaldoCount;

public:
    WORD              mainComando(char *pStr) ;
	void              mainMacchina(void);
    void              mainCiclo();
    WORD              mainMessaggio(char *Buffer, int len);
    CIOSegnale        *Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala );
    CIOSegnale        *Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala, double a,double b );
    WORD              OffSet(int Slot, int Point);
#ifndef NUOVA_CLASSE_PROCESSO
    WORD              Processo_GetStep(char *pStr);
    WORD              Processo_Load(char *pStr);
    void              Processo_UnLoad();
    WORD              Processo_GetRecord(char *pStr);
    bool              Processo_LoadDaFile();
    std::string       Processo_GetFileNome();
    WORD              Processo_LoadStep(char *pStr);
    WORD              Processo_LoadCompStep(char *pStr);
    void              Processo_LoadStep(int Step,int TipoStep,int Repit,int Tempo,int Pressione,int Potenza,int Temperatura);
    WORD              Processo_GetSendStep(char *buffer,int idprocesso,char *processo);
    WORD              Processo_GetSendCompStep(char *buffer,CIOSegnale *);
#endif
    bool              Macchina_PowOn(); //BYTE *pErr
    bool              Macchina_PowOff();
    bool              Macchina_PowIsOn();
    bool              Macchina_PowIsOff();
    WORD              Macchina_GetNetData(char *Buffer);
    WORD              Macchina_InitFile(char *Buffer);
    void              Macchina_SetSegnale(BYTE *pStr);
    bool              Macchina_ResetGas();
    bool              Macchina_FlussaAcqua();
    int               Macchina_InitSegnali(int );

    WORD              Ciclo_GetMsg(char *pStr);
    void              Ciclo_CeckErrori();
    bool              Ciclo_Reset();
    void              Ciclo_Allunga();
    bool              Ciclo_PreSet();
    bool              Ciclo_NextStepGoto();
    bool              Ciclo_NextStepForza();
    void              Ciclo_ResetErrore() ;
    bool              Ciclo_SorgentiAccoppiate(int index);


};


#endif
