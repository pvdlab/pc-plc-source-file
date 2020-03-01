#ifndef CICLO_H
#define CICLO_H

#include "Processo.h"


class CCiclo : public CProcesso
{
public:
    CCiclo(int newtipoapp,int newTipoImpianto,int argc, char *argv[]);
    WORD   Ciclo_GetMsg(char *pStr);
    void   Ciclo_Main();

private:
    void   Ciclo_Run();
    void   Ciclo_CeckErrori();
    bool   Ciclo_Reset();
    bool   Ciclo_PreSet();
    bool   Ciclo_NextStepGoto();
    bool   Ciclo_NextStepForza();
    bool   Ciclo_SorgentiAccoppiate(int index);
    bool   Ciclo_IsTargetConsumato();
    void   Ciclo_AllungaCiclo();

private:
    CTimer m_TimerPreSet;
    bool   m_IsInNextStep;
    bool   m_PreSet;
    bool   m_InPreSet;
    bool   m_GasInPreset;
    bool   m_PowInPreset;
    bool   m_BiasInPreset;
    bool   m_Macchina_ResetGas;
    bool   m_SetGasOffSet;
    bool   m_StandByCatodi;
    bool   m_Riparti;
    bool   m_StopVuoto;
    int    m_RiscaldoCount;
    int    m_OnTime;
    int    m_NextStep;
    int    m_NextTimeStep;
    BYTE   m_CatodoOn;


};

#endif // CICLO_H
