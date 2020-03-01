#include "Arc6010.h"
#include "Arc6012.h"
#include "Arc6012_Slot.h"
#include "MainCostanti.h"

#define MDSX_SU_BSXC

bool Init_Arc6012(CMacchina *pProcessa, int TipoBias){


    int ModuliCount=21;
    int SorgenteCount=6;
    int GasCount=3;

    setResistenzaCount(4);
    setSorgenteCount(6);
    setGasCount(3);

    if(!pProcessa){
       delete[] pProcessa->GGas;
       delete[] pProcessa->pModulo;
    }

    pProcessa->TimerEvent = new CTimer[2];
    pProcessa->StepTimer= new CTimer[2];
    pProcessa->Timer= new CTimer[2];


    pProcessa->pData= new struct DataType[2];
    memset(pProcessa->Msg,0,D_MSG);
    struct DataType *p=pProcessa->pData;
    memset(p,0,sizeof(struct DataType)*2);

    pProcessa->CamereCount=2;
    pProcessa->GasCount=GasCount;
    pProcessa->GGas = new CGas[pProcessa->GasCount];

    pProcessa->SorgenteCount=SorgenteCount;

    pProcessa->ModuloCount=ModuliCount;
    pProcessa->pModulo = new CModulo[pProcessa->ModuloCount];

    pProcessa->pModulo[0].setSlot(0,1,CTipoSlot::OutDigitale,16,0,0,true,"",false,1,0);
    pProcessa->pModulo[1].setSlot(1,2,CTipoSlot::OutDigitale,16,0,0,true,"",true,1,0);  ////SET URGENTE

    pProcessa->pModulo[2].setSlot(2,3,CTipoSlot::InDigitale,16,0,0,true,"",false,1,0);
    pProcessa->pModulo[3].setSlot(3,4,CTipoSlot::InDigitale,16,3148,0,true,"",false,1,16);
    pProcessa->pModulo[4].setSlot(4,5,CTipoSlot::OutAnalogico,8,3028,10000,true,"",false,1,14);
    pProcessa->pModulo[5].setSlot(5,6,CTipoSlot::OutDigitale,8,3188,0,true,"",false,1,16);
    pProcessa->pModulo[6].setSlot(6,7,CTipoSlot::InDigitale,16,3148,0,true,"",false,1,16);

    pProcessa->pModulo[7].setSlot(7,8,CTipoSlot::InAnalogico,8,3016,10000,true,"",false,1,14);
    pProcessa->pModulo[8].setSlot(8,9,CTipoSlot::InAnalogico,4,3017,10000,true,"",false,1,14);
    pProcessa->pModulo[9].setSlot(9,10,CTipoSlot::InAnalogico,8,3024,10000,true,"",false,1,14);
    pProcessa->pModulo[10].setSlot(10,11,CTipoSlot::OutAnalogico,4,3024,10000,true,"",false,1,14);
    pProcessa->pModulo[11].setSlot(11,12,CTipoSlot::OutDigitale,8,3188,0,true,"",false,1,16);
    pProcessa->pModulo[12].setSlot(12,6,CTipoSlot::InDigitale,4,3188,0,true,"",false,1,0);
    pProcessa->pModulo[13].setSlot(13,12,CTipoSlot::InDigitale,4,3148,0,true,"",false,1,0);
    //OUT DIGITALI
    pProcessa->pModulo[14].setSlot(14,13,CTipoSlot::OutDigitale,8,3188,0,true,"",false,1,16);
    pProcessa->pModulo[15].setSlot(15,14,CTipoSlot::OutDigitale,8,3188,0,true,"",false,1,16);
    //IN DIGITALI
    pProcessa->pModulo[16].setSlot(16,13,CTipoSlot::InDigitale,8,3188,0,true,"",false,1,0);
    pProcessa->pModulo[17].setSlot(17,14,CTipoSlot::InDigitale,8,3188,0,true,"",false,1,0);
    //TERMOCOPPIE
    pProcessa->pModulo[18].setSlot(18,15,CTipoSlot::InAnalogico,4,3017,10000,true,"",false,1,14);
    //OUT DIGITALI
    pProcessa->pModulo[19].setSlot(19,16,CTipoSlot::OutDigitale,8,3188,0,true,"",false,1,16);
    pProcessa->pModulo[20].setSlot(20,16,CTipoSlot::InDigitale,8,3188,0,true,"",false,1,0);

    pProcessa->ErrSize=D_COMP_ERRORE*2;//macchina
    pProcessa->ErrSize+=D_COMP_ERRORE;//moduliPlc
    pProcessa->ErrSize+=2*D_COMP_ERRORE;//vuoto
    pProcessa->ErrSize+=D_COMP_ERRORE;//Bias
    pProcessa->ErrSize+=2*D_COMP_ERRORE;//Resistenza
    pProcessa->ErrSize+=2*2*D_COMP_ERRORE*pProcessa->SorgenteCount;
    pProcessa->ErrSize+=D_COMP_ERRORE*pProcessa->GasCount;
    pProcessa->pBufferErrore=new char[pProcessa->ErrSize];
    memset(pProcessa->pBufferErrore,'\0',pProcessa->ErrSize);

    int OffSetErrore=-D_COMP_ERRORE;
    pProcessa->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Macchina");
    pProcessa->pErroreDue(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"MacchinaDue");
    pProcessa->pErroreModulo(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Modulo");
    pProcessa->PowOutdig=pProcessa->Segnale(S_PowOutDig);

    pProcessa->Vuoto= new CVuoto[2];
    pProcessa->Vuoto[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Vuoto");
    pProcessa->Vuoto[0].Camera.PortaChiusa=pProcessa->Segnale(S_PortaChiusa);

    pProcessa->Vuoto[0].Camera.AriaComp=pProcessa->Segnale(S_AriaCompOn);
    //pProcessa->Vuoto[0].Camera.Valv=pProcessa->Segnale(S_VentValv);
    pProcessa->Vuoto[0].Camera.Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Vuoto[0].Camera.AcquaIsOn=pProcessa->Segnale(S_AcquaIsOn);
    pProcessa->Vuoto[0].Camera.Termostato=pProcessa->Segnale(S_Termostato);
    pProcessa->Vuoto[0].Camera.Gas=pProcessa->Segnale(S_ValGasCamera);

    pProcessa->Vuoto[0].Penning->Tipo=PENNING_PTR90;
    pProcessa->Vuoto[0].Penning->Penning=pProcessa->Segnale(S_Penning);
    pProcessa->Vuoto[0].Penning->Interlock=pProcessa->Segnale(S_Interlock);
    //pProcessa->Vuoto[0].Penning->PowOn=pProcessa->Segnale(S_PennOnOff);
    pProcessa->Vuoto[0].Penning->TurboNormale=pProcessa->Segnale(S_TurboNorm);
    pProcessa->Vuoto[0].Penning->TurboStart=pProcessa->Segnale(S_TurboStart);

    pProcessa->Vuoto[0].Baratron.Segnale=pProcessa->Segnale(S_Baratron);
    pProcessa->Vuoto[0].Baratron.Valvola=pProcessa->Segnale(S_BaratronValv);
    pProcessa->Vuoto[0].Baratron.Pirani=pProcessa->Segnale(S_Pirani);
    pProcessa->Vuoto[0].Baratron.Enabled=true;
    //Init Meccanica
    pProcessa->Vuoto[0].Meccanica.PowOn=pProcessa->Segnale(S_MecPowOn);
    pProcessa->Vuoto[0].Meccanica.PowIsOn=pProcessa->Segnale(S_MecPowIsOn);
    pProcessa->Vuoto[0].Meccanica.PowErr=pProcessa->Segnale(S_MecPowErr);
    //Init Turbo
    pProcessa->Vuoto[0].Turbo.Start=pProcessa->Segnale(S_TurboStart);
    pProcessa->Vuoto[0].Turbo.Stop=pProcessa->Segnale(S_TurboStop);
    pProcessa->Vuoto[0].Turbo.Acc=pProcessa->Segnale(S_TurboAcc);
    pProcessa->Vuoto[0].Turbo.Errore=pProcessa->Segnale(S_TurboErr);
    pProcessa->Vuoto[0].Turbo.Normale=pProcessa->Segnale(S_TurboNorm);
    pProcessa->Vuoto[0].Turbo.Acqua=pProcessa->Segnale(S_AcquaTurbo);
    pProcessa->Vuoto[0].Turbo.Penning=pProcessa->Segnale(S_Penning);
    pProcessa->Vuoto[0].Turbo.Temp=pProcessa->Segnale(S_TempTurbo);
    //InitTavola
    pProcessa->Vuoto[0].Tavola.OnOff=pProcessa->Segnale(S_TavolaOn);
    pProcessa->Vuoto[0].Tavola.Ruota=pProcessa->Segnale(S_TavolaRuota);
    //Init Resistenza
    pProcessa->Vuoto[0].Resistenza= new CResistenza;
    pProcessa->Vuoto[0].Resistenza->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                  &pProcessa->MainTimer,E_TIME,"Resistenza");
    pProcessa->Vuoto[0].Resistenza->PowOn=pProcessa->Segnale(S_ResPowOn);
    pProcessa->Vuoto[0].Resistenza->PowIsOn=pProcessa->Segnale(S_ResPowIsOn);
    pProcessa->Vuoto[0].Resistenza->PowErr=pProcessa->Segnale(S_ResPowErr);
    pProcessa->Vuoto[0].Resistenza->ReleDx=pProcessa->Segnale(S_ReleDx);
    pProcessa->Vuoto[0].Resistenza->ReleSx=pProcessa->Segnale(S_ReleSx);
    pProcessa->Vuoto[0].Resistenza->Temp_Resistenza=pProcessa->Segnale(S_TempResistenza);
    pProcessa->Vuoto[0].Resistenza->Temp_Camera=pProcessa->Segnale(S_TempCamera);
    pProcessa->Vuoto[0].Resistenza->Temp_Acqua=pProcessa->Segnale(S_TempAmbiente);
    pProcessa->Vuoto[0].Resistenza->Temp_Turbo=pProcessa->Segnale(S_TempTurbo);
    pProcessa->Vuoto[0].Resistenza->ArgonValv=pProcessa->Segnale(S_ArgonValv);
    pProcessa->Vuoto[0].Resistenza->AzotoValv=pProcessa->Segnale(S_AzotoValv);
    pProcessa->Vuoto[0].Resistenza->AcetiValv=pProcessa->Segnale(S_AcetiValv);
    pProcessa->Vuoto[0].Resistenza->Termostato=pProcessa->Segnale(S_Termostato);
    pProcessa->Vuoto[0].Resistenza->Argon=pProcessa->Segnale(S_ArgonSetG);
    pProcessa->Vuoto[0].Resistenza->Azoto=pProcessa->Segnale(S_AzotoSetG);
    pProcessa->Vuoto[0].Resistenza->Aceti=pProcessa->Segnale(S_AcetiSetG);
    pProcessa->Vuoto[0].Resistenza->Gas=pProcessa->Segnale(S_ValGasCamera);

    pProcessa->Vuoto[0].Camera.Temp=pProcessa->Segnale(S_TempCamera);
    pProcessa->Vuoto[0].Camera.Acqua=pProcessa->Segnale(S_AcquaValv); //Vuoto[0].Resistenza->AcquaValv=pProcessa->Segnale(S_AcquaValv);
    pProcessa->Vuoto[0].Resistenza->Enabled=true;


    pProcessa->Vuoto[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Vuoto");
    pProcessa->Vuoto[1].Camera.PortaChiusa=pProcessa->Segnale(S_PortaChiusaDue);
    pProcessa->Vuoto[1].Camera.AriaComp=pProcessa->Segnale(S_AriaCompOnDue);
    pProcessa->Vuoto[1].Camera.Valv=pProcessa->Segnale(S_VentValvDue);
    pProcessa->Vuoto[1].Camera.Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Vuoto[1].Camera.AcquaIsOn=pProcessa->Segnale(S_AcquaIsOn);
    pProcessa->Vuoto[1].Camera.Termostato=pProcessa->Segnale(S_TermostatoDue);
    pProcessa->Vuoto[1].Camera.Gas=pProcessa->Segnale(S_ValGasCameraDue);
    pProcessa->Vuoto[1].Camera.SchermoTurbo=pProcessa->Segnale(S_SchermoTurboDue);
    pProcessa->Vuoto[1].Camera.SchermoTurbo->ForzaToStep();

    pProcessa->Vuoto[1].Penning->Tipo=PENNING_PTR225;
    pProcessa->Vuoto[1].Penning->Penning=pProcessa->Segnale(S_PenningDue);
    pProcessa->Vuoto[1].Penning->Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Vuoto[1].Penning->PowOn=pProcessa->Segnale(S_PennOnOffDue);
    pProcessa->Vuoto[1].Penning->TurboNormale=pProcessa->Segnale(S_TurboNormDue);
    pProcessa->Vuoto[1].Penning->TurboStart=pProcessa->Segnale(S_TurboStartDue);
    pProcessa->Vuoto[1].Penning->Pirani=pProcessa->Segnale(S_PiraniDue);

    //pProcessa->Vuoto[1].Baratron.Segnale=pProcessa->Segnale(S_Baratron);
    //pProcessa->Vuoto[1].Baratron.Valvola=pProcessa->Segnale(S_BaratronValv);
    pProcessa->Vuoto[1].Baratron.Pirani=pProcessa->Segnale(S_PiraniDue);
    pProcessa->Vuoto[1].Baratron.Enabled=false;
    //Init Meccanica
    pProcessa->Vuoto[1].Meccanica.PowOn=pProcessa->Segnale(S_MecPowOnDue);
    pProcessa->Vuoto[1].Meccanica.PowIsOn=pProcessa->Segnale(S_MecPowIsOnDue);
    pProcessa->Vuoto[1].Meccanica.PowErr=pProcessa->Segnale(S_MecPowErrDue);
    //Init Turbo
    pProcessa->Vuoto[1].Turbo.Start=pProcessa->Segnale(S_TurboStartDue);
    pProcessa->Vuoto[1].Turbo.Stop=pProcessa->Segnale(S_TurboStopDue);
    pProcessa->Vuoto[1].Turbo.Acc=pProcessa->Segnale(S_TurboAccDue);
    pProcessa->Vuoto[1].Turbo.Errore=pProcessa->Segnale(S_TurboErrDue);
    pProcessa->Vuoto[1].Turbo.Normale=pProcessa->Segnale(S_TurboNormDue);
    pProcessa->Vuoto[1].Turbo.Acqua=pProcessa->Segnale(S_AcquaTurbo);
    pProcessa->Vuoto[1].Turbo.Penning=pProcessa->Segnale(S_PenningDue);
    pProcessa->Vuoto[1].Turbo.Temp=pProcessa->Segnale(S_TempTurboDue);
    //InitTavola
    pProcessa->Vuoto[1].Tavola.OnOff=pProcessa->Segnale(S_TavolaOnDue);
    pProcessa->Vuoto[1].Tavola.Ruota=pProcessa->Segnale(S_TavolaRuotaDue);
    //Init Resistenza
    pProcessa->Vuoto[1].Resistenza= new CResistenza;
    pProcessa->Vuoto[1].Resistenza->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                  &pProcessa->MainTimer,E_TIME,"Resistenza");
    pProcessa->Vuoto[1].Resistenza->PowOn=pProcessa->Segnale(S_ResPowOnDue);
    pProcessa->Vuoto[1].Resistenza->PowIsOn=pProcessa->Segnale(S_ResPowIsOnDue);
    pProcessa->Vuoto[1].Resistenza->PowErr=pProcessa->Segnale(S_ResPowErrDue);
    pProcessa->Vuoto[1].Resistenza->ReleDx=pProcessa->Segnale(S_ReleDxDue);
    pProcessa->Vuoto[1].Resistenza->ReleSx=pProcessa->Segnale(S_ReleSxDue);
    pProcessa->Vuoto[1].Resistenza->Temp_Resistenza=pProcessa->Segnale(S_TempResistenzaDue);
    pProcessa->Vuoto[1].Resistenza->Temp_Camera=pProcessa->Segnale(S_TempCameraDue);
    pProcessa->Vuoto[1].Resistenza->Temp_Acqua=pProcessa->Segnale(S_TempAmbiente);
    pProcessa->Vuoto[1].Resistenza->Temp_Turbo=pProcessa->Segnale(S_TempTurboDue);
    pProcessa->Vuoto[1].Resistenza->ArgonValv=pProcessa->Segnale(S_ArgonValv);
    pProcessa->Vuoto[1].Resistenza->AzotoValv=pProcessa->Segnale(S_AzotoValv);
    pProcessa->Vuoto[1].Resistenza->AcetiValv=pProcessa->Segnale(S_AcetiValv);
    pProcessa->Vuoto[1].Resistenza->Gas=pProcessa->Segnale(S_ValGasCameraDue);
    pProcessa->Vuoto[1].Resistenza->Termostato=pProcessa->Segnale(S_TermostatoDue);

    pProcessa->Vuoto[1].Resistenza->Argon=pProcessa->Segnale(S_ArgonSetG);
    pProcessa->Vuoto[1].Resistenza->Azoto=pProcessa->Segnale(S_AzotoSetG);
    pProcessa->Vuoto[1].Resistenza->Aceti=pProcessa->Segnale(S_AcetiSetG);

    pProcessa->Vuoto[1].Camera.Temp=pProcessa->Segnale(S_TempCameraDue);
    //pProcessa->Vuoto[1].Camera.TempCatodo=pProcessa->Segnale(S_TempCatodo);
    pProcessa->Vuoto[1].Camera.Acqua=pProcessa->Segnale(S_AcquaValv); //Vuoto[0].Resistenza->AcquaValv=pProcessa->Segnale(S_AcquaValv);
    pProcessa->Vuoto[1].Resistenza->Enabled=true;

    //Init  Bias
    pProcessa->Bias =new CBias[2];
    pProcessa->Bias[0].Init(SorgenteCount,TipoBias);
    pProcessa->Bias[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                             &pProcessa->MainTimer,E_TIME,"Bias");
    pProcessa->Bias[0].SetV=pProcessa->Segnale(S_BiasSetV);
    pProcessa->Bias[0].GetV=pProcessa->Segnale(S_BiasGetV);
    pProcessa->Bias[0].GetI=pProcessa->Segnale(S_BiasGetI);
    pProcessa->Bias[0].PowOn=pProcessa->Segnale(S_BiasPowOn);
    pProcessa->Bias[0].PowIsOn=pProcessa->Segnale(S_BiasPowIsOn);
    pProcessa->Bias[0].PowErr=pProcessa->Segnale(S_BiasPowErr);
    pProcessa->Bias[0].On=pProcessa->Segnale(S_BiasOn);
    pProcessa->Bias[0].Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Bias[0].HVolt=pProcessa->Segnale(S_BiasHVolt);
    pProcessa->Bias[0].LVolt=pProcessa->Segnale(S_BiasLVolt);
    pProcessa->Bias[0].IsHVolt=pProcessa->Segnale(S_BiasIsHVolt);
    pProcessa->Bias[0].IsLVolt=pProcessa->Segnale(S_BiasIsLVolt);
    pProcessa->Bias[0].BiasNext=pProcessa->Segnale(S_BiasNext);
    pProcessa->Bias[0].BiasIsNext=pProcessa->Segnale(S_BiasIsNext);
    pProcessa->Bias[0].Enabled=true;


    //0)Init Sorgente SxPorta
    pProcessa->Bias[0].Sorgente[0].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SxP");
    pProcessa->Bias[0].Sorgente[0].PowOn=pProcessa->Segnale(S_PaPowOn);
    pProcessa->Bias[0].Sorgente[0].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
    pProcessa->Bias[0].Sorgente[0].PowErr=pProcessa->Segnale(S_PaPowErr);
    pProcessa->Bias[0].Sorgente[0].SetI=pProcessa->Segnale(S_BSxPSetI);
    pProcessa->Bias[0].Sorgente[0].Enabled=true;

    pProcessa->Bias[0].Sorgente[0].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[0].Catodo->Trigger=pProcessa->Segnale(S_BSxPTrigger);
    pProcessa->Bias[0].Sorgente[0].Catodo->IsOnArc=pProcessa->Segnale(S_BSxPIsOnArc);
    pProcessa->Bias[0].Sorgente[0].Catodo->OnPow=pProcessa->Segnale(S_BSxPOnPow);
    pProcessa->Bias[0].Sorgente[0].Catodo->ErrPow=pProcessa->Segnale(S_BSxPErrPow);
    pProcessa->Bias[0].Sorgente[0].Catodo->GetI=pProcessa->Segnale(S_BSxPGetI);
    pProcessa->Bias[0].Sorgente[0].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BSxP");
    pProcessa->Bias[0].Sorgente[0].Catodo->Enabled=true;

    //1)Init Sorgente DxPorta
    pProcessa->Bias[0].Sorgente[1].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"DxP");
    //pProcessa->Bias[0].Sorgente[1].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[1].PowOn=pProcessa->Segnale(S_PaPowOn);
    pProcessa->Bias[0].Sorgente[1].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
    pProcessa->Bias[0].Sorgente[1].PowErr=pProcessa->Segnale(S_PaPowErr);
    pProcessa->Bias[0].Sorgente[1].SetI=pProcessa->Segnale(S_BDxPSetI);
    pProcessa->Bias[0].Sorgente[1].Enabled=true;

    pProcessa->Bias[0].Sorgente[1].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[1].Catodo->Trigger=pProcessa->Segnale(S_BDxPTrigger);
    pProcessa->Bias[0].Sorgente[1].Catodo->IsOnArc=pProcessa->Segnale(S_BDxPIsOnArc);
    pProcessa->Bias[0].Sorgente[1].Catodo->OnPow=pProcessa->Segnale(S_BDxPOnPow);
    pProcessa->Bias[0].Sorgente[1].Catodo->ErrPow=pProcessa->Segnale(S_BDxPErrPow);
    pProcessa->Bias[0].Sorgente[1].Catodo->GetI=pProcessa->Segnale(S_BDxPGetI);
    pProcessa->Bias[0].Sorgente[1].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BDxP");
    pProcessa->Bias[0].Sorgente[1].Catodo->Enabled=true;


    //2)Init Sorgente DxCamera
    pProcessa->Bias[0].Sorgente[2].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[2].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                              &pProcessa->MainTimer,E_TIME,"DxC");
    //Bias[0].Sorgente[2].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[2].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[2].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[2].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[2].SetI=pProcessa->Segnale(S_BDxCSetI);
    pProcessa->Bias[0].Sorgente[2].Enabled=true;

    pProcessa->Bias[0].Sorgente[2].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[2].Catodo->Trigger=pProcessa->Segnale(S_BDxCTrigger);
    pProcessa->Bias[0].Sorgente[2].Catodo->IsOnArc=pProcessa->Segnale(S_BDxCIsOnArc);
    pProcessa->Bias[0].Sorgente[2].Catodo->OnPow=pProcessa->Segnale(S_BDxCOnPow);
    pProcessa->Bias[0].Sorgente[2].Catodo->ErrPow=pProcessa->Segnale(S_BDxCErrPow);
    pProcessa->Bias[0].Sorgente[2].Catodo->GetI=pProcessa->Segnale(S_BDxCGetI);
    pProcessa->Bias[0].Sorgente[2].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"BDxC");
    pProcessa->Bias[0].Sorgente[2].Catodo->Enabled=true;


    //3)Init Sorgente SxCamera
    pProcessa->Bias[0].Sorgente[3].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[3].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SxC");
    //Bias[0].Sorgente[3].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[3].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[3].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[3].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[3].SetI=pProcessa->Segnale(S_BSxCSetI);
    pProcessa->Bias[0].Sorgente[3].Enabled=true;

    pProcessa->Bias[0].Sorgente[3].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[3].Catodo->Trigger=pProcessa->Segnale(S_BSxCTrigger);
    pProcessa->Bias[0].Sorgente[3].Catodo->IsOnArc=pProcessa->Segnale(S_BSxCIsOnArc);
    pProcessa->Bias[0].Sorgente[3].Catodo->OnPow=pProcessa->Segnale(S_BSxCOnPow);
    pProcessa->Bias[0].Sorgente[3].Catodo->ErrPow=pProcessa->Segnale(S_BSxCErrPow);
    pProcessa->Bias[0].Sorgente[3].Catodo->GetI=pProcessa->Segnale(S_BSxCGetI);
    pProcessa->Bias[0].Sorgente[3].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BSxC");
    pProcessa->Bias[0].Sorgente[3].Catodo->Enabled=true;


    //4)Init Sorgente MedCamera
    pProcessa->Bias[0].Sorgente[4].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[4].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                              &pProcessa->MainTimer,E_TIME,"MDxC");
    //Bias[0].Sorgente[4].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[4].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[4].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[4].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[4].SetI=pProcessa->Segnale(S_MDxCSetI);
    pProcessa->Bias[0].Sorgente[4].Schermo=pProcessa->Segnale(S_MDxCSchermo);
    pProcessa->Bias[0].Sorgente[4].Enabled=true;

    pProcessa->Bias[0].Sorgente[4].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[4].Catodo->Trigger=pProcessa->Segnale(S_MDxCTrigger);
    pProcessa->Bias[0].Sorgente[4].Catodo->IsOnArc=pProcessa->Segnale(S_MDxCIsOnArc);
    pProcessa->Bias[0].Sorgente[4].Catodo->OnPow=pProcessa->Segnale(S_MDxCOnPow);
    pProcessa->Bias[0].Sorgente[4].Catodo->ErrPow=pProcessa->Segnale(S_MDxCErrPow);
    pProcessa->Bias[0].Sorgente[4].Catodo->GetI=pProcessa->Segnale(S_MDxCGetI);
    pProcessa->Bias[0].Sorgente[4].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"MDxC");
    pProcessa->Bias[0].Sorgente[4].Catodo->Enabled=true;


    //4)Init Sorgente MedCamera
#ifdef MDSX_SU_BSXC
    pProcessa->Bias[0].Sorgente[5].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"MSxC");
    //pProcessa->Bias[0].Sorgente[5].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[5].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[5].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[5].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[5].SetI=pProcessa->Segnale(S_BSxCSetI);
    pProcessa->Bias[0].Sorgente[5].Schermo=pProcessa->Segnale(S_MSxCSchermo);
    pProcessa->Bias[0].Sorgente[5].Enabled=true;

    pProcessa->Bias[0].Sorgente[5].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].Catodo->Trigger=pProcessa->Segnale(S_MSxCTrigger);
    pProcessa->Bias[0].Sorgente[5].Catodo->IsOnArc=pProcessa->Segnale(S_BSxCIsOnArc);
    pProcessa->Bias[0].Sorgente[5].Catodo->OnPow=pProcessa->Segnale(S_BSxCOnPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->ErrPow=pProcessa->Segnale(S_BSxCErrPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->GetI=pProcessa->Segnale(S_BSxCGetI);
    pProcessa->Bias[0].Sorgente[5].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"MSxC");
    pProcessa->Bias[0].Sorgente[5].Catodo->Enabled=true;
#else
    pProcessa->Bias[0].Sorgente[5].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"MSxC");
    //Bias[0].Sorgente[5].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[5].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[5].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[5].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[5].SetI=pProcessa->Segnale(S_MSxCSetI);
    pProcessa->Bias[0].Sorgente[5].GetI=pProcessa->Segnale(S_MSxCGetI);
    pProcessa->Bias[0].Sorgente[5].Schermo=pProcessa->Segnale(S_MSxCSchermo);
    pProcessa->Bias[0].Sorgente[5].Enabled=true;

    pProcessa->Bias[0].Sorgente[5].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].Catodo->Trigger=pProcessa->Segnale(S_MSxCTrigger);
    pProcessa->Bias[0].Sorgente[5].Catodo->IsOnArc=pProcessa->Segnale(S_MSxCIsOnArc);
    pProcessa->Bias[0].Sorgente[5].Catodo->OnPow=pProcessa->Segnale(S_MSxCOnPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->ErrPow=pProcessa->Segnale(S_MSxCErrPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->GetI=pProcessa->Segnale(S_MSxCGetI);
    pProcessa->Bias[0].Sorgente[5].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"MSxC");
    pProcessa->Bias[0].Sorgente[5].Catodo->Enabled=true;
#endif

        pProcessa->Bias[1].Init(SorgenteCount,TipoBias);
        pProcessa->Bias[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                 &pProcessa->MainTimer,E_TIME,"Bias");
        pProcessa->Bias[1].SetV=pProcessa->Segnale(S_BiasSetV);
        pProcessa->Bias[1].GetV=pProcessa->Segnale(S_BiasGetV);
        pProcessa->Bias[1].GetI=pProcessa->Segnale(S_BiasGetI);
        pProcessa->Bias[1].PowOn=pProcessa->Segnale(S_BiasPowOn);
        pProcessa->Bias[1].PowIsOn=pProcessa->Segnale(S_BiasPowIsOn);
        pProcessa->Bias[1].PowErr=pProcessa->Segnale(S_BiasPowErr);
        pProcessa->Bias[1].On=pProcessa->Segnale(S_BiasOn);
        pProcessa->Bias[1].Interlock=pProcessa->Segnale(S_Interlock);
        pProcessa->Bias[1].HVolt=pProcessa->Segnale(S_BiasHVolt);
        pProcessa->Bias[1].LVolt=pProcessa->Segnale(S_BiasLVolt);
        pProcessa->Bias[1].IsHVolt=pProcessa->Segnale(S_BiasIsHVolt);
        pProcessa->Bias[1].IsLVolt=pProcessa->Segnale(S_BiasIsLVolt);
        pProcessa->Bias[1].BiasNext=pProcessa->Segnale(S_BiasNext);
        pProcessa->Bias[1].BiasIsNext=pProcessa->Segnale(S_BiasIsNext);
        pProcessa->Bias[1].Enabled=true;


        //0)Init Sorgente SxPorta
        pProcessa->Bias[1].Sorgente[0].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"SxP");
        pProcessa->Bias[1].Sorgente[0].PowOn=pProcessa->Segnale(S_PaPowOn);
        pProcessa->Bias[1].Sorgente[0].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
        pProcessa->Bias[1].Sorgente[0].PowErr=pProcessa->Segnale(S_PaPowErr);
        pProcessa->Bias[1].Sorgente[0].SetI=pProcessa->Segnale(S_BSxPSetI);
        pProcessa->Bias[1].Sorgente[0].Enabled=true;

        pProcessa->Bias[1].Sorgente[0].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[0].Catodo->Trigger=pProcessa->Segnale(S_BSxPTriggerDue);
        pProcessa->Bias[1].Sorgente[0].Catodo->IsOnArc=pProcessa->Segnale(S_BSxPIsOnArc);
        pProcessa->Bias[1].Sorgente[0].Catodo->OnPow=pProcessa->Segnale(S_BSxPOnPow);
        pProcessa->Bias[1].Sorgente[0].Catodo->ErrPow=pProcessa->Segnale(S_BSxPErrPow);
        pProcessa->Bias[1].Sorgente[0].Catodo->GetI=pProcessa->Segnale(S_BSxPGetI);
        pProcessa->Bias[1].Sorgente[0].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                        &pProcessa->MainTimer,E_TIME,"BSxP");
        pProcessa->Bias[1].Sorgente[0].Catodo->Enabled=true;

        //1)Init Sorgente DxPorta
        pProcessa->Bias[1].Sorgente[1].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"DxP");
        //pProcessa->Bias[1].Sorgente[1].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[1].PowOn=pProcessa->Segnale(S_PaPowOn);
        pProcessa->Bias[1].Sorgente[1].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
        pProcessa->Bias[1].Sorgente[1].PowErr=pProcessa->Segnale(S_PaPowErr);
        pProcessa->Bias[1].Sorgente[1].SetI=pProcessa->Segnale(S_BDxPSetI);
        pProcessa->Bias[1].Sorgente[1].Schermo=pProcessa->Segnale(S_DxPSchermoDue);
        pProcessa->Bias[1].Sorgente[1].Enabled=true;

        pProcessa->Bias[1].Sorgente[1].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[1].Catodo->Trigger=pProcessa->Segnale(S_BDxPTriggerDue);
        pProcessa->Bias[1].Sorgente[1].Catodo->IsOnArc=pProcessa->Segnale(S_BDxPIsOnArc);
        pProcessa->Bias[1].Sorgente[1].Catodo->OnPow=pProcessa->Segnale(S_BDxPOnPow);
        pProcessa->Bias[1].Sorgente[1].Catodo->ErrPow=pProcessa->Segnale(S_BDxPErrPow);
        pProcessa->Bias[1].Sorgente[1].Catodo->GetI=pProcessa->Segnale(S_BDxPGetI);
        pProcessa->Bias[1].Sorgente[1].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                        &pProcessa->MainTimer,E_TIME,"BDxP");
        pProcessa->Bias[1].Sorgente[1].Catodo->Enabled=true;


        //2)Init Sorgente DxCamera
        pProcessa->Bias[1].Sorgente[2].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[2].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                  &pProcessa->MainTimer,E_TIME,"DxC");
        //Bias[1].Sorgente[2].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[2].PowOn=pProcessa->Segnale(S_PbPowOn);
        pProcessa->Bias[1].Sorgente[2].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
        pProcessa->Bias[1].Sorgente[2].PowErr=pProcessa->Segnale(S_PbPowErr);
        pProcessa->Bias[1].Sorgente[2].SetI=pProcessa->Segnale(S_BDxCSetI);
        pProcessa->Bias[1].Sorgente[2].Schermo=pProcessa->Segnale(S_DxCSchermoDue);
        pProcessa->Bias[1].Sorgente[2].Enabled=true;

        pProcessa->Bias[1].Sorgente[2].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[2].Catodo->Trigger=pProcessa->Segnale(S_BDxCTriggerDue);
        pProcessa->Bias[1].Sorgente[2].Catodo->IsOnArc=pProcessa->Segnale(S_BDxCIsOnArc);
        pProcessa->Bias[1].Sorgente[2].Catodo->OnPow=pProcessa->Segnale(S_BDxCOnPow);
        pProcessa->Bias[1].Sorgente[2].Catodo->ErrPow=pProcessa->Segnale(S_BDxCErrPow);
        pProcessa->Bias[1].Sorgente[2].Catodo->GetI=pProcessa->Segnale(S_BDxCGetI);
        pProcessa->Bias[1].Sorgente[2].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"BDxC");
        pProcessa->Bias[1].Sorgente[2].Catodo->Enabled=true;


        //3)Init Sorgente SxCamera
        pProcessa->Bias[1].Sorgente[3].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[3].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"SxC");
        //Bias[1].Sorgente[3].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[3].PowOn=pProcessa->Segnale(S_PbPowOn);
        pProcessa->Bias[1].Sorgente[3].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
        pProcessa->Bias[1].Sorgente[3].PowErr=pProcessa->Segnale(S_PbPowErr);
        pProcessa->Bias[1].Sorgente[3].SetI=pProcessa->Segnale(S_BSxCSetI);
        pProcessa->Bias[1].Sorgente[3].Enabled=true;

        pProcessa->Bias[1].Sorgente[3].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[3].Catodo->Trigger=pProcessa->Segnale(S_BSxCTriggerDue);
        pProcessa->Bias[1].Sorgente[3].Catodo->IsOnArc=pProcessa->Segnale(S_BSxCIsOnArc);
        pProcessa->Bias[1].Sorgente[3].Catodo->OnPow=pProcessa->Segnale(S_BSxCOnPow);
        pProcessa->Bias[1].Sorgente[3].Catodo->ErrPow=pProcessa->Segnale(S_BSxCErrPow);
        pProcessa->Bias[1].Sorgente[3].Catodo->GetI=pProcessa->Segnale(S_BSxCGetI);
        pProcessa->Bias[1].Sorgente[3].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                        &pProcessa->MainTimer,E_TIME,"BSxC");
        pProcessa->Bias[1].Sorgente[3].Catodo->Enabled=true;


        //4)Init Sorgente MedCamera
        pProcessa->Bias[1].Sorgente[4].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[4].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                  &pProcessa->MainTimer,E_TIME,"MDxC");
        //Bias[1].Sorgente[4].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[4].PowOn=pProcessa->Segnale(S_PbPowOn);
        pProcessa->Bias[1].Sorgente[4].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
        pProcessa->Bias[1].Sorgente[4].PowErr=pProcessa->Segnale(S_PbPowErr);
        pProcessa->Bias[1].Sorgente[4].SetI=pProcessa->Segnale(S_MDxCSetI);
        pProcessa->Bias[1].Sorgente[4].Enabled=true;

        pProcessa->Bias[1].Sorgente[4].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[4].Catodo->Trigger=pProcessa->Segnale(S_MDxCTriggerDue);
        pProcessa->Bias[1].Sorgente[4].Catodo->IsOnArc=pProcessa->Segnale(S_MDxCIsOnArc);
        pProcessa->Bias[1].Sorgente[4].Catodo->OnPow=pProcessa->Segnale(S_MDxCOnPow);
        pProcessa->Bias[1].Sorgente[4].Catodo->ErrPow=pProcessa->Segnale(S_MDxCErrPow);
        pProcessa->Bias[1].Sorgente[4].Catodo->GetI=pProcessa->Segnale(S_MDxCGetI);
        pProcessa->Bias[1].Sorgente[4].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                        &pProcessa->MainTimer,E_TIME,"MDxC");
        pProcessa->Bias[1].Sorgente[4].Catodo->Enabled=true;


        //4)Init Sorgente MedCamera
    #ifdef MDSX_SU_BSXC
        pProcessa->Bias[1].Sorgente[5].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[5].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"MSxC");
        //pProcessa->Bias[1].Sorgente[5].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[5].PowOn=pProcessa->Segnale(S_PbPowOn);
        pProcessa->Bias[1].Sorgente[5].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
        pProcessa->Bias[1].Sorgente[5].PowErr=pProcessa->Segnale(S_PbPowErr);
        pProcessa->Bias[1].Sorgente[5].SetI=pProcessa->Segnale(S_BSxCSetI);
        pProcessa->Bias[1].Sorgente[5].Schermo=pProcessa->Segnale(S_MSxSchermoDue);
        pProcessa->Bias[1].Sorgente[5].Enabled=true;

        pProcessa->Bias[1].Sorgente[5].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[5].Catodo->Trigger=pProcessa->Segnale(S_MSxCTriggerDue);
        pProcessa->Bias[1].Sorgente[5].Catodo->IsOnArc=pProcessa->Segnale(S_BSxCIsOnArc);
        pProcessa->Bias[1].Sorgente[5].Catodo->OnPow=pProcessa->Segnale(S_BSxCOnPow);
        pProcessa->Bias[1].Sorgente[5].Catodo->ErrPow=pProcessa->Segnale(S_BSxCErrPow);
        pProcessa->Bias[1].Sorgente[5].Catodo->GetI=pProcessa->Segnale(S_BSxCGetI);
        pProcessa->Bias[1].Sorgente[5].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                        &pProcessa->MainTimer,E_TIME,"MSxC");
        pProcessa->Bias[1].Sorgente[5].Catodo->Enabled=true;
    #else
        pProcessa->Bias[1].Sorgente[5].operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[5].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"MSxC");
        //Bias[1].Sorgente[5].SetRitardo(2000);
        pProcessa->Bias[1].Sorgente[5].PowOn=pProcessa->Segnale(S_PbPowOn);
        pProcessa->Bias[1].Sorgente[5].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
        pProcessa->Bias[1].Sorgente[5].PowErr=pProcessa->Segnale(S_PbPowErr);
        pProcessa->Bias[1].Sorgente[5].SetI=pProcessa->Segnale(S_MSxCSetI);
        pProcessa->Bias[1].Sorgente[5].GetI=pProcessa->Segnale(S_MSxCGetI);
        pProcessa->Bias[1].Sorgente[5].Schermo=pProcessa->Segnale(S_MSxSchermoDue);
        pProcessa->Bias[1].Sorgente[5].Enabled=true;

        pProcessa->Bias[1].Sorgente[5].Catodo->operator()(&pProcessa->MainTimer);
        pProcessa->Bias[1].Sorgente[5].Catodo->Trigger=pProcessa->Segnale(S_MSxCTriggerDue);
        pProcessa->Bias[1].Sorgente[5].Catodo->IsOnArc=pProcessa->Segnale(S_MSxCIsOnArc);
        pProcessa->Bias[1].Sorgente[5].Catodo->OnPow=pProcessa->Segnale(S_MSxCOnPow);
        pProcessa->Bias[1].Sorgente[5].Catodo->ErrPow=pProcessa->Segnale(S_MSxCErrPow);
        pProcessa->Bias[1].Sorgente[5].Catodo->GetI=pProcessa->Segnale(S_MSxCGetI);
        pProcessa->Bias[1].Sorgente[5].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                             &pProcessa->MainTimer,E_TIME,"MSxC");
        pProcessa->Bias[1].Sorgente[5].Catodo->Enabled=true;
    #endif

    //CIOSegnale  *a=pProcessa->Bias[0].Sorgente[0].Catodo->Trigger;
    //CIOSegnale  *b=pProcessa->Bias[1].Sorgente[0].Catodo->Trigger;

    //Init Gas Argon
    pProcessa->GGas[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                               &pProcessa->MainTimer,E_TIME,"Argon");
    pProcessa->GGas[0].SetRitardo(2000);
    pProcessa->GGas[0].SetG=pProcessa->Segnale(S_ArgonSetG);
    pProcessa->GGas[0].GetG=pProcessa->Segnale(S_ArgonGetG);
    pProcessa->GGas[0].Valvola=pProcessa->Segnale(S_ArgonValv);
    pProcessa->GGas[0].Enabled=true;
    //Init Gas Azoto
    pProcessa->GGas[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                    &pProcessa->MainTimer,E_TIME,"Azoto");
    pProcessa->GGas[1].SetRitardo(2000);
    pProcessa->GGas[1].SetG=pProcessa->Segnale(S_AzotoSetG);
    pProcessa->GGas[1].GetG=pProcessa->Segnale(S_AzotoGetG);
    pProcessa->GGas[1].Valvola=pProcessa->Segnale(S_AzotoValv);
    pProcessa->GGas[1].Baratron=pProcessa->Segnale(S_Baratron);
    pProcessa->GGas[1].Enabled=true;
    //Init Gas Aceti
    pProcessa->GGas[2].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                    &pProcessa->MainTimer,E_TIME,"Aceti");
    pProcessa->GGas[2].SetRitardo(2000);
    pProcessa->GGas[2].SetG=pProcessa->Segnale(S_AcetiSetG);
    pProcessa->GGas[2].GetG=pProcessa->Segnale(S_AcetiGetG);
    pProcessa->GGas[2].Valvola=pProcessa->Segnale(S_AcetiValv);
    pProcessa->GGas[2].Enabled=true;

    int colorindex=0;
    for(int i=0;  i< pProcessa->GasCount;i++){
        pProcessa->GGas[i].SetG->setColor(colorindex);
        pProcessa->GGas[i].GetG->setColor(colorindex);
        pProcessa->GGas[i].SetG->TipoComp=CTipoComDef::GAS+i;
        pProcessa->GGas[i].GetG->TipoComp=CTipoComDef::GAS+i;
        pProcessa->GGas[i].GetG->Zip=ZIP_GAS;
        colorindex++;
    }

    for(int j=0; j<pProcessa->CamereCount;j++){
        for(int i=0;  i< pProcessa->SorgenteCount;i++){
            //qDebug()  << Bias[0].Sorgente[i].GetI->Descrizione << 20+i;
            pProcessa->Bias[j].Sorgente[i].SetI->TipoComp=CTipoComDef::SORGENTI+i;
            pProcessa->Bias[j].Sorgente[i].Catodo->GetI->TipoComp=CTipoComDef::SORGENTI+i;
            pProcessa->Bias[j].Sorgente[i].SetI->setColor(colorindex);
            pProcessa->Bias[j].Sorgente[i].Catodo->GetI->setColor(colorindex);
            pProcessa->Bias[j].Sorgente[i].Catodo->GetI->Zip=ZIP_SORGENTI;
            colorindex++;
        }
        pProcessa->Bias[j].SetV->TipoComp=CTipoComDef::BIAS_V;
        pProcessa->Bias[j].SetV->setColor(colorindex);
        pProcessa->Bias[j].GetV->TipoComp=CTipoComDef::BIAS_V;
        pProcessa->Bias[j].GetV->setColor(colorindex);
        pProcessa->Bias[j].GetV->Zip=ZIP_V_BIAS;
        pProcessa->Bias[j].GetI->TipoComp=CTipoComDef::BIAS_I;
        pProcessa->Bias[j].GetI->setColor(colorindex);
        pProcessa->Bias[j].GetI->Zip=ZIP_I_BIAS;


        pProcessa->Vuoto[j].Penning->Penning->setColor(1);
        pProcessa->Vuoto[j].Penning->Penning->TipoComp=CTipoComDef::PENNING;
        pProcessa->Vuoto[j].Penning->Penning->Zip=ZIP_VACUUM;
        if(pProcessa->Vuoto[j].Baratron.Segnale){
            pProcessa->Vuoto[j].Baratron.Segnale->setColor(2);
            pProcessa->Vuoto[j].Baratron.Segnale->TipoComp=CTipoComDef::BARATRON;
            pProcessa->Vuoto[j].Baratron.Segnale->Zip=ZIP_VACUUM;
        }
        if(pProcessa->Vuoto[j].Baratron.Pirani){
            pProcessa->Vuoto[j].Baratron.Pirani->setColor(3);
            pProcessa->Vuoto[j].Baratron.Pirani->TipoComp=CTipoComDef::PIRANI;
            pProcessa->Vuoto[j].Baratron.Pirani->Zip=ZIP_VACUUM;
        }

        pProcessa->Vuoto[j].Resistenza->Temp_Resistenza->setColor(1);
        pProcessa->Vuoto[j].Resistenza->Temp_Resistenza->TipoComp=CTipoComDef::TEMP_RESISTENZA;
        pProcessa->Vuoto[j].Resistenza->Temp_Resistenza->Zip=ZIP_TEMP;
        pProcessa->Vuoto[j].Camera.Temp->setColor(2);
        pProcessa->Vuoto[j].Camera.Temp->TipoComp=CTipoComDef::TEMP_CAMERA;
        pProcessa->Vuoto[j].Turbo.Temp->setColor(3);
        pProcessa->Vuoto[j].Turbo.Temp->TipoComp=CTipoComDef::TEMP_TURBO;

        if(pProcessa->Vuoto[j].Camera.SchermoTurbo!=NULL){
            pProcessa->Vuoto[j].Camera.SchermoTurbo->TipoComp=CTipoComp::SchermoTurbo;
            pProcessa->Vuoto[j].Camera.SchermoTurbo->TipoComp*=10;
            //pProcessa->Vuoto[j].Camera.SchermoTurbo->TipoComp+=j;
        }
        //pProcessa->Vuoto[0].Camera.TempCatodo->setColor(4);
        //pProcessa->Vuoto[0].Camera.TempCatodo->TipoComp=CTipoComDef::TEMP_CATODO;

        pProcessa->Vuoto[j].Tavola.GiriSecondo(700); // con 1000 di speed 0,121 g/s

        //pProcessa->m_pErrMacchina=(char *)pProcessa->pBufferErrore;

        pProcessa->pData[j].SorgentiOn=0;
        for(int i=0;i<pProcessa->SorgenteCount;i++){
           pProcessa->pData[j].SorgentiOn=SetBit(pProcessa->pData[j].SorgentiOn, i, pProcessa->Bias[j].Sorgente[i].Enabled);
        }
        pProcessa->pData[j].File.SorgentiOn=pProcessa->pData[j].SorgentiOn;
        pProcessa->pData[j].LastComando =NULL;
        pProcessa->pData[j].LastComandoIndex=0;


        //BSXP REGOLA
        double regola=7459;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[0].SetI->setRegola(regola);

        //BDXP REGOLA
        regola=7650;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[1].SetI->setRegola(regola);

        //BDXC REGOLA
        regola=7260;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[2].SetI->setRegola(regola);

        //BSXC REGOLA
        regola=7800;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[3].SetI->setRegola(regola);

        //MDXC REGOLA
        regola=7600;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[4].SetI->setRegola(regola);

        //MSXC COME BSXC REGOLA
        regola=7706;
        regola/=7381;
        pProcessa->Bias[j].Sorgente[3].SetI->setRegola(regola);
    }



    return true;
}


