#include "Comunica.h"

CComunica::CComunica(int newtipoapp,int newTipoImpianto): CCiclo( newtipoapp, newTipoImpianto)
{

}

void CComunica::Comunica_Main(){

    Ciclo_Main();
    //Macchina_Main();

}


int CComunica::Comunica(char *Buffer, int len){

    BYTE PrimoMsg;
    char *pMsg=Buffer;
    PrimoMsg=*pMsg;
    pData.MessaggioRicevuto=PrimoMsg;
    switch (PrimoMsg){
        case TipoMessaggio_Test:
           return len;
           break;
        case TipoMessaggio_SetComando :
           pMsg++;
           pData.Comando=*pMsg;
           NewEvento=true;
           //Ciclo_Comando();
           //Macchina_Comando();
           pMsg--;
           return len;
           break;
        case TipoMessaggio_CaricaProcesso :
           return Processo_Load(++pMsg);
           break;
        case TipoMessaggio_GetData:
           return Macchina_GetNetData(pMsg);
           break;
        case TipoMessaggio_GetStep:
           return Processo_GetStep(pMsg);
           break;
        case TipoMessaggio_GetProcMsg:
           return Ciclo_GetMsg(pMsg);
           break;
        case TipoMessaggio_GetProcRecord:
          return Processo_GetRecord(pMsg);
          break;

    }

    MostraEvento("TipoMessaggio Non Valido",Color::FG_RED);
    return Macchina_GetNetData(pMsg);

}


