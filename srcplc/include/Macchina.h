#ifndef MACCHINA_H
#define MACCHINA_H

#include "Plc.h"
#include "Bias.h"
#include "Gas.h"
#include "Vuoto.h"
#include "Target.h"



class CImpianto
{
public:
    CGas          *GGas;
    CBias         *Bias;
    CResistenza   *Resistenza;
    CVuoto        *Vuoto;
    DataType      *pData;
    WORD          ErrSize;
    CErrore       pErrore;
    int           Index;
};


class CMacchina : public CPlc
{
public:
    CMacchina(int newtipoapp,int newTipoImpianto,int argc, char *argv[]);
    void          Macchina_Main();
    void          Macchina_Run();
    WORD          Macchina_Set(char *Buffer);
    WORD          Macchina_SetComando(char *Buffer);
    WORD          Macchina_GetNetData(char *Buffer);
    void          Macchina_PrintErrori();
    WORD          Macchina_InitSegnali();
    WORD          Macchina_SetSegnale(BYTE *pStr);
    bool          Macchina_PowOn(int Index);
    bool          Macchina_PowOff(int Index);
    bool          Macchina_PowIsOn(int Index);
    bool          Macchina_PowIsOff(int Index);
    bool          Macchina_OnePowIsOn();
    bool          Macchina_OneGasIsOn();
    WORD          Macchina_InitFile(char *Buffer);
    bool          Macchina_ResetGas();
    bool          Macchina_Reset();
    void          Macchina_PowOutdig();
    void          Macchina_ResetErrore();
    WORD          Macchina_GetOnTime(char *Buffer);
    bool          Macchina_FlussaAcqua();
    void          Macchina_SalvaRegola();
    int           Macchina_TragetCambia(char *Buffer);  

private:
    WORD          Macchina_GetNetData(char *Buffer,DataType  *pData);
    std::string   Macchina_GetInitFileNome();
    std::string   Macchina_GetRegolaFileNome();

public:
    BYTE          *pBufferErrore;
    int           IdVuoto;
    int           IdCiclo;
    int           IdPro;
    int           IdRun;
    CImpianto     *pImpianto;
    int           Impianto;
    int           TipoApp;
    CGas          *GGas;
    CBias         *Bias;
    CIOSegnale    *PowOutdig;
    CVuoto        *Vuoto;
    CSTimer       MainTimer;
    CTimer        *TimerEvent;
    CTimer        *StepTimer;
    CTimer        *Timer;
    int           SorgenteCount;
    int           GasCount;
    CScorta       *pScorta;
    CTarget       *pTarget;
    DataType      *pData;
    CErrore       pErrore;
    CErrore       pErroreDue;
    bool          Debug;
    bool          MacchinaInManuale;
    bool          PorteAperte;
    WORD          ErrSize;
    char          Msg[D_BUFFER];
    bool          NewEvento;
    int           TimeStop;
    BYTE          ProcessoNome[UTENTE_SIZE];
    WORD          StepCount;
    int           StepIndex;
    StepType      Step[2][D_STEP];
    bool          IsLoadDaFile;
    int           CamereCount;
    int           BufferLen;

private:
    CTimer        m_PowerTimeOut;
    CTimer        m_GasTimeOut;
    CTimer        m_OnOffStepTimer;
    CTimer        m_ResetTimerOut;
    CTimer        m_ManualeTimeOut;
    bool          m_PowOn;
    bool          m_ResetGas;
    bool          m_Macchina_ResetGas;
    BYTE          m_Comado;
    std::string   m_oldstr;
    CompType      m_pComp;
    bool          m_ManualeSet;
    bool          m_Termostato;
    bool          m_IsStartProcesso;


};

#endif // MACCHINA_H
