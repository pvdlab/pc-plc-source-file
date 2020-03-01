#include "Arc6011.h"
#include "Arc6011_Slot.h"


bool Init_Arc6011(CMacchina *pProcessa, int TipoBias){

    int ModuliCount=20;
    int SorgenteCount=6;
    int GasCount=3;
    setResistenzaCount(3);
    setSorgenteCount(6);
    setGasCount(3);

    if(!pProcessa){
       delete[] pProcessa->GGas;
       delete[] pProcessa->pModulo;
       //delete[] pProcessa->pBufferErrore;
    }

    pProcessa->TimerEvent = new CTimer;
    pProcessa->StepTimer= new CTimer;
    pProcessa->Timer= new CTimer;

    pProcessa->pData= new struct DataType;
    memset(pProcessa->Msg,0,D_MSG);
    struct DataType *p=pProcessa->pData;
    memset(p,0,sizeof(struct DataType));

    pProcessa->CamereCount=1;
    pProcessa->GasCount=GasCount;
    pProcessa->GGas = new CGas[pProcessa->GasCount];

    pProcessa->SorgenteCount=SorgenteCount;

    pProcessa->ModuloCount=ModuliCount;
    pProcessa->pModulo = new CModulo[pProcessa->ModuloCount];


    //OUT DIGITALI
    pProcessa->pModulo[0].setSlot(0,1,CTipoSlot::OutDigitale,8,3188,0,true,"A0",false,1,16);
    pProcessa->pModulo[1].setSlot(1,2,CTipoSlot::OutDigitale,8,3188,0,true,"A1",false,1,16);
    pProcessa->pModulo[2].setSlot(2,3,CTipoSlot::OutDigitale,8,3188,0,true,"A2",false,1,16);
    pProcessa->pModulo[3].setSlot(3,4,CTipoSlot::OutDigitale,8,3188,0,true,"A3",false,1,16);
    pProcessa->pModulo[4].setSlot(4,5,CTipoSlot::OutDigitale,8,3188,0,true,"A4",false,1,16);
    pProcessa->pModulo[5].setSlot(5,6,CTipoSlot::OutDigitale,8,3188,0,true,"A5",false,1,16);

    //IN DIGITALI
    pProcessa->pModulo[6].setSlot(6,1,CTipoSlot::InDigitale,8,3188,0,true,"E0",false,1,0);
    pProcessa->pModulo[7].setSlot(7,2,CTipoSlot::InDigitale,8,3188,0,true,"E1",false,1,0);
    pProcessa->pModulo[8].setSlot(8,3,CTipoSlot::InDigitale,8,3188,0,true,"E2",false,1,0);
    pProcessa->pModulo[9].setSlot(9,4,CTipoSlot::InDigitale,8,3188,0,true,"E3",false,1,0);
    pProcessa->pModulo[10].setSlot(10,5,CTipoSlot::InDigitale,8,3188,0,true,"E4",false,1,0);
    pProcessa->pModulo[11].setSlot(11,6,CTipoSlot::InDigitale,8,3188,0,true,"E5",false,1,0);

    //OUT ANALOGICI
    pProcessa->pModulo[12].setSlot(12,7,CTipoSlot::OutAnalogico,8,3028,10000,true,"AW",false,1,14);
    pProcessa->pModulo[13].setSlot(13,8,CTipoSlot::OutAnalogico,8,3028,10000,true,"AW",false,1,14);

    //IN ANALOGICI
    pProcessa->pModulo[14].setSlot(14,9,CTipoSlot::InAnalogico,8,3017,10000,true,"AI",false,1,14);
    pProcessa->pModulo[15].setSlot(15,10,CTipoSlot::InAnalogico,8,3017,10000,true,"AI",false,1,14);
    
    pProcessa->pModulo[16].setSlot(16,11,CTipoSlot::InDigitale,4,3130,0,true,"46",false,1,0);
    pProcessa->pModulo[17].setSlot(17,11,CTipoSlot::OutDigitale,4,3130,0,true,"46",false,1,16);

    pProcessa->pModulo[18].setSlot(18,12,CTipoSlot::InAnalogico,4,3016,10000,true,"48",false,1,14);
    pProcessa->pModulo[19].setSlot(19,13,CTipoSlot::InAnalogico,8,3018,10000,true,"TC",false,1,14);

    pProcessa->ErrSize=D_COMP_ERRORE*2;//macchina
    pProcessa->ErrSize+=D_COMP_ERRORE;//moduliPlc
    pProcessa->ErrSize+=D_COMP_ERRORE;//vuoto
    pProcessa->ErrSize+=D_COMP_ERRORE;//Bias
    pProcessa->ErrSize+=D_COMP_ERRORE;//Resistenza
    pProcessa->ErrSize+=2*D_COMP_ERRORE*pProcessa->SorgenteCount;
    pProcessa->ErrSize+=D_COMP_ERRORE*pProcessa->GasCount;
    pProcessa->pBufferErrore=new char[pProcessa->ErrSize];
    memset(pProcessa->pBufferErrore,'\0',pProcessa->ErrSize);

    int OffSetErrore=-D_COMP_ERRORE;
    pProcessa->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Macchina");
    pProcessa->pErroreDue(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"MacchinaDue");
    pProcessa->pErroreModulo(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Modulo");

    pProcessa->Vuoto= new CVuoto;
    pProcessa->Vuoto[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),&pProcessa->MainTimer,E_TIME,"Vuoto");
    pProcessa->Vuoto[0].Camera.PortaChiusa=pProcessa->Segnale(S_PortaChiusa);
    pProcessa->Vuoto[0].Camera.Valv=pProcessa->Segnale(S_LVentValv);
    pProcessa->Vuoto[0].Camera.LuceRossa=pProcessa->Segnale(S_LuceRossa);
    pProcessa->Vuoto[0].Camera.LuceVerde=pProcessa->Segnale(S_LuceVerde);
    pProcessa->Vuoto[0].Camera.LuceGialla=pProcessa->Segnale(S_LuceGialla);
    pProcessa->Vuoto[0].Camera.Reset=pProcessa->Segnale(S_Reset);
    pProcessa->Vuoto[0].Camera.AcquaIsOn=pProcessa->Segnale(S_AcquaIsOn);
    pProcessa->Vuoto[0].Camera.Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Vuoto[0].Camera.Termostato=pProcessa->Segnale(S_Termostato);


    pProcessa->Vuoto[0].Penning->Tipo=PENNING_PTR90;
    pProcessa->Vuoto[0].Penning->Penning=pProcessa->Segnale(S_Penning);
    pProcessa->Vuoto[0].Penning->Interlock=pProcessa->Segnale(S_Interlock);
    pProcessa->Vuoto[0].Penning->PowOn=pProcessa->Segnale(S_PenningOnOff);
    pProcessa->Vuoto[0].Penning->TurboNormale=pProcessa->Segnale(S_TurboNorm);
    pProcessa->Vuoto[0].Penning->TurboStart=pProcessa->Segnale(S_TurboStart);
    //pProcessa->Vuoto[0].Baratron.Segnale=pProcessa->Segnale(S_Baratron);
    //pProcessa->Vuoto[0].Baratron.Valvola=pProcessa->Segnale(S_BaratronValv);
    pProcessa->Vuoto[0].Baratron.Enabled=false;
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
    //InitTavola
    pProcessa->Vuoto[0].Tavola.OnOff=pProcessa->Segnale(S_TavolaOn);
    pProcessa->Vuoto[0].Tavola.Ruota=pProcessa->Segnale(S_TavolaRuota);
    pProcessa->Vuoto[0].Tavola.Speed=pProcessa->Segnale(S_TavolaSpeed);
    //

    //Init  Bias
    pProcessa->Bias =new CBias();
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
    pProcessa->Bias[0].OnEnable=pProcessa->Segnale(S_BiasOnEnabled);
    pProcessa->Bias[0].Arco=pProcessa->Segnale(S_BiasArc);
    pProcessa->Bias[0].Enabled=true;

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

    pProcessa->Vuoto[0].Turbo.Temp=pProcessa->Segnale(S_TempTurbo);
    pProcessa->Vuoto[0].Camera.Temp=pProcessa->Segnale(S_TempCamera);
    pProcessa->Vuoto[0].Camera.Acqua=pProcessa->Segnale(S_AcquaValv); //Vuoto[0].Resistenza->AcquaValv=pProcessa->Segnale(S_AcquaValv);
    pProcessa->Vuoto[0].Resistenza->Enabled=true;

    //0)Init Sorgente SxPorta
    pProcessa->Bias[0].Sorgente[0].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[0].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SorSxP");
    pProcessa->Bias[0].Sorgente[0].PowOn=pProcessa->Segnale(S_PaPowOn);
    pProcessa->Bias[0].Sorgente[0].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
    pProcessa->Bias[0].Sorgente[0].PowErr=pProcessa->Segnale(S_PaPowErr);
    pProcessa->Bias[0].Sorgente[0].SetI=pProcessa->Segnale(S_SxPSetI);
    pProcessa->Bias[0].Sorgente[0].Enabled=true;

    pProcessa->Bias[0].Sorgente[0].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[0].Catodo->Temp=pProcessa->Segnale(S_BSxPTemp);
    pProcessa->Bias[0].Sorgente[0].Catodo->Trigger=pProcessa->Segnale(S_SxPTrigger);
    pProcessa->Bias[0].Sorgente[0].Catodo->IsOnArc=pProcessa->Segnale(S_SxPIsOnArc);
    pProcessa->Bias[0].Sorgente[0].Catodo->Temp=pProcessa->Segnale(S_BSxPTemp);
    pProcessa->Bias[0].Sorgente[0].Catodo->OnPow=pProcessa->Segnale(S_SxPOnPow);
    pProcessa->Bias[0].Sorgente[0].Catodo->ErrPow=pProcessa->Segnale(S_SxPErrPow);
    pProcessa->Bias[0].Sorgente[0].Catodo->GetI=pProcessa->Segnale(S_SxPGetI);
    pProcessa->Bias[0].Sorgente[0].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BSxP");
    pProcessa->Bias[0].Sorgente[0].Catodo->Enabled=true;

    //1)Init Sorgente DxPorta
    pProcessa->Bias[0].Sorgente[1].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[1].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SorDxP");
    //Bias[0].Sorgente[1].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[1].PowOn=pProcessa->Segnale(S_PaPowOn);
    pProcessa->Bias[0].Sorgente[1].PowIsOn=pProcessa->Segnale(S_PaPowIsOn);
    pProcessa->Bias[0].Sorgente[1].PowErr=pProcessa->Segnale(S_PaPowErr);
    pProcessa->Bias[0].Sorgente[1].SetI=pProcessa->Segnale(S_DxPSetI);
    pProcessa->Bias[0].Sorgente[1].Enabled=true;

    pProcessa->Bias[0].Sorgente[1].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[1].Catodo->Temp=pProcessa->Segnale(S_BDxPTemp);
    pProcessa->Bias[0].Sorgente[1].Catodo->Trigger=pProcessa->Segnale(S_DxPTrigger);
    pProcessa->Bias[0].Sorgente[1].Catodo->IsOnArc=pProcessa->Segnale(S_DxPIsOnArc);
    pProcessa->Bias[0].Sorgente[1].Catodo->OnPow=pProcessa->Segnale(S_DxPOnPow);
    pProcessa->Bias[0].Sorgente[1].Catodo->ErrPow=pProcessa->Segnale(S_DxPErrPow);
    pProcessa->Bias[0].Sorgente[1].Catodo->GetI=pProcessa->Segnale(S_DxPGetI);
    pProcessa->Bias[0].Sorgente[1].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BDxP");
    pProcessa->Bias[0].Sorgente[1].Catodo->Enabled=true;


    //2)Init Sorgente DxCamera
    pProcessa->Bias[0].Sorgente[2].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[2].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                              &pProcessa->MainTimer,E_TIME,"SorDxC");
    //Bias[0].Sorgente[2].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[2].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[2].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[2].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[2].SetI=pProcessa->Segnale(S_DxCSetI);
    pProcessa->Bias[0].Sorgente[2].Enabled=true;

    pProcessa->Bias[0].Sorgente[2].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[2].Catodo->Temp=pProcessa->Segnale(S_BDxCTemp);
    pProcessa->Bias[0].Sorgente[2].Catodo->Trigger=pProcessa->Segnale(S_DxCTrigger);
    pProcessa->Bias[0].Sorgente[2].Catodo->IsOnArc=pProcessa->Segnale(S_DxCIsOnArc);
    pProcessa->Bias[0].Sorgente[2].Catodo->OnPow=pProcessa->Segnale(S_DxCOnPow);
    pProcessa->Bias[0].Sorgente[2].Catodo->ErrPow=pProcessa->Segnale(S_DxCErrPow);
    pProcessa->Bias[0].Sorgente[2].Catodo->GetI=pProcessa->Segnale(S_DxCGetI);
    pProcessa->Bias[0].Sorgente[2].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"BDxC");
    pProcessa->Bias[0].Sorgente[2].Catodo->Enabled=true;


    //3)Init Sorgente SxCamera
    pProcessa->Bias[0].Sorgente[3].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[3].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SorSxC");
    //Bias[0].Sorgente[3].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[3].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[3].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[3].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[3].SetI=pProcessa->Segnale(S_SxCSetI);
    pProcessa->Bias[0].Sorgente[3].Enabled=true;

    pProcessa->Bias[0].Sorgente[3].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[3].Catodo->Temp=pProcessa->Segnale(S_BSxCTemp);
    pProcessa->Bias[0].Sorgente[3].Catodo->Trigger=pProcessa->Segnale(S_SxCTrigger);
    pProcessa->Bias[0].Sorgente[3].Catodo->IsOnArc=pProcessa->Segnale(S_SxCIsOnArc);
    pProcessa->Bias[0].Sorgente[3].Catodo->OnPow=pProcessa->Segnale(S_SxCOnPow);
    pProcessa->Bias[0].Sorgente[3].Catodo->ErrPow=pProcessa->Segnale(S_SxCErrPow);
    pProcessa->Bias[0].Sorgente[3].Catodo->GetI=pProcessa->Segnale(S_SxCGetI);
    pProcessa->Bias[0].Sorgente[3].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"BSxC");
    pProcessa->Bias[0].Sorgente[3].Catodo->Enabled=true;


    //4)Init Sorgente MedCamera
    pProcessa->Bias[0].Sorgente[4].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[4].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                              &pProcessa->MainTimer,E_TIME,"SorMDxC");
    //Bias[0].Sorgente[4].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[4].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[4].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[4].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[4].SetI=pProcessa->Segnale(S_MidBSetI);
    pProcessa->Bias[0].Sorgente[4].Enabled=true;

    pProcessa->Bias[0].Sorgente[4].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[4].Catodo->Temp=pProcessa->Segnale(S_MidBTemp);
    pProcessa->Bias[0].Sorgente[4].Catodo->Trigger=pProcessa->Segnale(S_MidBTrigger);
    pProcessa->Bias[0].Sorgente[4].Catodo->IsOnArc=pProcessa->Segnale(S_MidBIsOnArc);
    pProcessa->Bias[0].Sorgente[4].Catodo->OnPow=pProcessa->Segnale(S_MidBOnPow);
    pProcessa->Bias[0].Sorgente[4].Catodo->ErrPow=pProcessa->Segnale(S_MidBErrPow);
    pProcessa->Bias[0].Sorgente[4].Catodo->GetI=pProcessa->Segnale(S_MidBGetI);
    pProcessa->Bias[0].Sorgente[4].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                                    &pProcessa->MainTimer,E_TIME,"MDxC");
    pProcessa->Bias[0].Sorgente[4].Catodo->Enabled=true;



    pProcessa->Bias[0].Sorgente[5].operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"SorMSxC");
    //Bias[0].Sorgente[5].SetRitardo(2000);
    pProcessa->Bias[0].Sorgente[5].PowOn=pProcessa->Segnale(S_PbPowOn);
    pProcessa->Bias[0].Sorgente[5].PowIsOn=pProcessa->Segnale(S_PbPowIsOn);
    pProcessa->Bias[0].Sorgente[5].PowErr=pProcessa->Segnale(S_PbPowErr);
    pProcessa->Bias[0].Sorgente[5].SetI=pProcessa->Segnale(S_MidASetI);
    pProcessa->Bias[0].Sorgente[5].Enabled=true;

    pProcessa->Bias[0].Sorgente[5].Catodo->operator()(&pProcessa->MainTimer);
    pProcessa->Bias[0].Sorgente[5].Catodo->Temp=pProcessa->Segnale(S_MidATemp);
    pProcessa->Bias[0].Sorgente[5].Catodo->Trigger=pProcessa->Segnale(S_MidATrigger);
    pProcessa->Bias[0].Sorgente[5].Catodo->IsOnArc=pProcessa->Segnale(S_MidAIsOnArc);
    pProcessa->Bias[0].Sorgente[5].Catodo->OnPow=pProcessa->Segnale(S_MidAOnPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->ErrPow=pProcessa->Segnale(S_MidAErrPow);
    pProcessa->Bias[0].Sorgente[5].Catodo->GetI=pProcessa->Segnale(S_MidAGetI);
    pProcessa->Bias[0].Sorgente[5].Catodo->pErrore(pProcessa->pBufferErrore+(OffSetErrore+=D_COMP_ERRORE),
                                         &pProcessa->MainTimer,E_TIME,"MSxC");
    pProcessa->Bias[0].Sorgente[5].Catodo->Enabled=true;


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

    for(int i=0;  i< pProcessa->SorgenteCount;i++){
        pProcessa->Bias[0].Sorgente[i].SetI->TipoComp=CTipoComDef::SORGENTI+i;
        pProcessa->Bias[0].Sorgente[i].Catodo->GetI->TipoComp=CTipoComDef::SORGENTI+i;
        pProcessa->Bias[0].Sorgente[i].SetI->setColor(colorindex);
        pProcessa->Bias[0].Sorgente[i].Catodo->GetI->setColor(colorindex);
        pProcessa->Bias[0].Sorgente[i].Catodo->GetI->Zip=ZIP_SORGENTI;
        colorindex++;
    }
    pProcessa->Bias[0].SetV->TipoComp=CTipoComDef::BIAS_V;
    pProcessa->Bias[0].SetV->setColor(colorindex);
    pProcessa->Bias[0].GetV->TipoComp=CTipoComDef::BIAS_V;
    pProcessa->Bias[0].GetV->setColor(colorindex);
    pProcessa->Bias[0].GetV->Zip=ZIP_V_BIAS;
    pProcessa->Bias[0].GetI->TipoComp=CTipoComDef::BIAS_I;
    pProcessa->Bias[0].GetI->setColor(colorindex);
    pProcessa->Bias[0].GetI->Zip=ZIP_I_BIAS;


    pProcessa->Vuoto[0].Penning->Penning->setColor(1);
    pProcessa->Vuoto[0].Penning->Penning->TipoComp=CTipoComDef::PENNING;
    pProcessa->Vuoto[0].Penning->Penning->Zip=ZIP_VACUUM;
    if(pProcessa->Vuoto[0].Baratron.Segnale){
        pProcessa->Vuoto[0].Baratron.Segnale->setColor(2);
        pProcessa->Vuoto[0].Baratron.Segnale->TipoComp=CTipoComDef::BARATRON;
        pProcessa->Vuoto[0].Baratron.Segnale->Zip=ZIP_VACUUM;
    }
    if(pProcessa->Vuoto[0].Baratron.Pirani){
        pProcessa->Vuoto[0].Baratron.Pirani->setColor(3);
        pProcessa->Vuoto[0].Baratron.Pirani->TipoComp=CTipoComDef::PIRANI;
        pProcessa->Vuoto[0].Baratron.Pirani->Zip=ZIP_VACUUM;
    }


    pProcessa->Vuoto[0].Resistenza->Temp_Resistenza->setColor(1);
    pProcessa->Vuoto[0].Resistenza->Temp_Resistenza->TipoComp=CTipoComDef::TEMP_RESISTENZA;
    pProcessa->Vuoto[0].Resistenza->Temp_Resistenza->Zip=ZIP_TEMP;
    pProcessa->Vuoto[0].Camera.Temp->setColor(2);
    pProcessa->Vuoto[0].Camera.Temp->TipoComp=CTipoComDef::TEMP_CAMERA;
    pProcessa->Vuoto[0].Turbo.Temp->setColor(3);
    pProcessa->Vuoto[0].Turbo.Temp->TipoComp=CTipoComDef::TEMP_TURBO;

    if(pProcessa->Vuoto[0].Camera.SchermoTurbo!=NULL){
        pProcessa->Vuoto[0].Camera.SchermoTurbo->TipoComp=CTipoComp::SchermoTurbo;
        pProcessa->Vuoto[0].Camera.SchermoTurbo->TipoComp*=10;
    }

    pProcessa->Vuoto[0].Tavola.GiriSecondo(DEF_TAVOLA_GIRI_SECONDO); // con 1000 di speed 0,121 g/s

    //pProcessa->m_pErrMacchina=(char *)pProcessa->pBufferErrore;

    pProcessa->pData[0].SorgentiOn=0;
    for(int i=0;i<pProcessa->SorgenteCount;i++){
       pProcessa->pData[0].SorgentiOn=SetBit(pProcessa->pData[0].SorgentiOn, i, pProcessa->Bias[0].Sorgente[i].Enabled);
    }
    pProcessa->pData[0].File.SorgentiOn=pProcessa->pData[0].SorgentiOn;
    pProcessa->pData[0].LastComando =NULL;
    pProcessa->pData[0].LastComandoIndex=0;


    return true;
}
