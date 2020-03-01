#ifndef PROCESSO_H
#define PROCESSO_H

#include "Macchina.h"


class CProcesso : public CMacchina
{
public:
    CProcesso(int newtipoapp,int newTipoImpianto,int argc, char *argv[]);
    WORD         Processo_GetStep(char *pStr);
    WORD         Processo_Load(char *pStr);
    void         Processo_UnLoad();
    void         Processo_LoadStep(int Step,int CTipoStep,int Repit,int Tempo,int Pressione,int Potenza,int Temperatura);
    WORD         Processo_GetMsgSendStep(char *buffer,int IdMacchina,int idprocesso,char *processo,StepType pStep[], int pStepCount);
    WORD         Processo_GetMsgSendCompStep(char *buffer,int IdMacchina,CIOSegnale *,int pStepCount);
    WORD         Processo_GetMsgRecord(char *pStr);
    WORD         Processo_GetMsgProcessiDisponibili(char *pStr);
    std::string  Processo_GetFileNome();
    WORD         Processo_LoadDaFile(char *pStr);
    bool         Processo_LoadDaFile();
    WORD         Processo_Elimina(char *pStr);
    WORD         Processo_GetMsgElimaProcesso(char *Buffer ,int IdMacchina,int idprocesso,char *processo);

private:
    WORD         Processo_LoadStep(char *pStr);
    WORD         Processo_LoadCompStep(char *pStr);

private:
    int mProcessoId;


/*public:
    BYTE         ProcessoNome[UTENTE_SIZE];
    WORD         StepCount;
    int          StepIndex;
    StepType     Step[D_STEP];
    bool          IsLoadDaFile;*/
};

#endif // PROCESSO_H
