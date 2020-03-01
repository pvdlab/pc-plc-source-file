#include "StdAfx.h"
#include "Simula.h"
#include <stdio.h>
#include "MainCostanti.h"

#ifdef QT_APP
#include "QDebug"
#endif

#define BARATRON_START  10000
#define BARATRON_STEP   500
#define BARATRON_STOP   500

#define PENNING_START  8500
#define PENNING_STEP   500
#define PENNING_STOP   3000

#define PIRANI_ALLARM    3000

CSimula::CSimula()
{

}


CSimula& CSimula::operator()(CCiclo *NewProcessa, int NewStepTime)
{
    Processa=NewProcessa;
    m_Penning[0]=10000;
    m_Baratron[0]=8000;
    m_Pirani[0]=GAS_PIRANI_ALLARM-100;
    m_Penning[1]=10000;
    m_Baratron[1]=8000;
    m_Pirani[1]=GAS_PIRANI_ALLARM-100;
    m_StetTime[0]=NewStepTime;
    m_StetTime[1]=NewStepTime;
 
    for(int i=0; i<Processa->CamereCount;i++){
        *Processa->Vuoto[i].Camera.Termostato=true;
        *Processa->Vuoto[i].Camera.Temp=180;
        *Processa->Vuoto[i].Resistenza->Temp_Resistenza =450;
        *Processa->Vuoto[i].Resistenza->Temp_Camera =180;
        *Processa->Vuoto[i].Resistenza->Temp_Acqua =150;
        *Processa->Vuoto[i].Turbo.Temp=250;
        *Processa->Vuoto[i].Baratron.Segnale=BARATRON_START -100;
        *Processa->Vuoto[i].Baratron.Pirani=PIRANI_ALLARM-100;
        *Processa->Vuoto[i].Penning->Penning=PENNING_START -100;
        *Processa->Vuoto[i].Camera.PortaChiusa=true;
        *Processa->Vuoto[i].Penning->Interlock=false;
    }


    TimerOnOff[0](500,500);
    TimerOnOff[1](500,500);
	m_ComandoManuale=false;
	m_OnCatodi=false;

	return *this;
}


void CSimula::eventoSet(char *Buffer){

  /*  char *pStr=Buffer;
    pStr++;
    m_Evento=*pStr++;
    int tempo=StrWORD(&pStr);
    m_EventoTimer(tempo*1000);
    m_EventoTimer.Timer();*/

}

bool CSimula::eventomainMacchina(){

  /*  if(!m_EventoTimer.IsAttivo())
        return false;

    m_EventoTimer.Timer();

    switch (m_Evento) {
      case Simula_BiasLow:
        *Processa->Bias[Processa->IdCiclo].GetV=BIAS_MIN_ETCHING_GETV-10;
        break;
      default:
        break;
    }



    return true;*/
}

bool CSimula::eventoBiaLow(){


    return false;
}

#define BIAS_LOW

void CSimula::main()
{
 double NewSegnale;
 int i;

 if(!getSimula())
     return;

 /*if(Processa->IsPerSimula){
    eventoSet(Processa->SimulaBuffer);
    Processa->IsPerSimula=false;

 }

 if(eventomainMacchina()){
     return;
 }*/

 //if(Processa->pData[IdVuoto].Comando==CTipoComando::Manuale)
 //    return;

/* NewSegnale=(double)Processa->GGas[1].SetG->Segnale();

 if(NewSegnale>0){
     NewSegnale=(double)Processa->GGas[1].SetG->Segnale();
	 if(NewSegnale<3300){
        NewSegnale*=0.60;
        NewSegnale-=160;
	 }
	 else{
		NewSegnale*=1.03;
        NewSegnale-=1611;

	 }

     *Processa->Vuoto[Processa->IdVuoto].Baratron.Segnale=(WORD)NewSegnale;


  }*/
 //*Processa->Vuoto[Processa->IdVuoto].Camera.AcquaIsOn=Processa->Vuoto[Processa->IdVuoto].Turbo.Acqua->Segnale();
 for(int i=0; i<Processa->CamereCount;i++ ){
     if(Processa->Vuoto[i].Turbo.PowOn->Segnale())
         *Processa->Vuoto[i].Turbo.PowIsOn=true;
     else
         *Processa->Vuoto[i].Turbo.PowIsOn=false;

     if(Processa->Vuoto[i].Tavola.OnOff->Segnale()){
         if(TimerOnOff[i].Timer()){
            if(Processa->Vuoto[i].Tavola.Ruota->Segnale())
                *Processa->Vuoto[i].Tavola.Ruota=false;
            else
                *Processa->Vuoto[i].Tavola.Ruota=true;
            TimerOnOff[i].Reset();
         }
      }

      if(Processa->Vuoto[i].Resistenza->PowOn->Segnale())
           *Processa->Vuoto[i].Resistenza->PowIsOn=true;
      else
           *Processa->Vuoto[i].Resistenza->PowIsOn=false;

      int comando=Processa->pData[i].Comando;

      switch(comando)
        {
        case CTipoComando::Vacuum:
        //case CTipoComando::StartProcesso:
        case CTipoComando::Preset:
        case CTipoComando::NextStep:
        case CTipoComando::Reset:
        case CTipoComando::Manuale:
         *Processa->Vuoto[i].Camera.Valv=false;
         if(Processa->Vuoto[i].Meccanica.PowOn->Segnale()){
           *Processa->Vuoto[i].Meccanica.PowIsOn=true;
             int tempo=PENNING_START -100-PENNING_STOP;
             tempo/=PENNING_STEP;
             tempo+=2;
             tempo*=m_StetTime[i];
             TimerPenning[i](tempo,m_StetTime[i]);
         }
         else
           *Processa->Vuoto[i].Meccanica.PowIsOn=false;

         if(i==0){
           /*  if(Processa->Vuoto[i].Turbo.Start->Segnale()){
               if(!Processa->Vuoto[i].Turbo.Normale->Segnale())
                  *Processa->Vuoto[i].Turbo.Acc=true;
               int tempo=BARATRON_START -100-BARATRON_STOP;
               tempo/=BARATRON_STEP;
               tempo+=2;
               tempo*=m_StetTime[i];
               TimerBaratron[i](tempo,m_StetTime[i]);
             }
             else
               *Processa->Vuoto[i].Turbo.Acc=false;*/
             if(Processa->Vuoto[i].Turbo.Start->Segnale()){
                 *Processa->Vuoto[i].Turbo.Normale=true;
                 *Processa->Vuoto[i].Turbo.Acc=false;
             }
         }else{
             if(Processa->Vuoto[i].Turbo.Start->Segnale()){
                 *Processa->Vuoto[i].Turbo.Normale=true;
                 *Processa->Vuoto[i].Turbo.Acc=false;
             }
         }


         if(TimerPenning[i].TimerStep()){
           if(m_Penning[i]+PENNING_STEP>PENNING_STOP)
            m_Penning[i]-=PENNING_STEP;
           else
            TimerPenning[i].Break();
           *Processa->Vuoto[i].Penning->Penning=m_Penning[i];
         }

        if(TimerBaratron[i].TimerStep()){
          if(m_Baratron[i]>BARATRON_STOP){
            m_Baratron[i]-=BARATRON_STEP;
          }
          else{
            TimerBaratron[i].Break();
            *Processa->Vuoto[i].Turbo.Acc=false;
            *Processa->Vuoto[i].Turbo.Normale=true;
            }
          *Processa->Vuoto[i].Baratron.Segnale=m_Baratron[i];
        }
        break;


      case CTipoComando::StopVuoto:
          if(!Processa->Vuoto[i].Turbo.Start->Segnale()){
            *Processa->Vuoto[i].Baratron.Segnale=BARATRON_START +100;
            *Processa->Vuoto[i].Penning->Penning=PENNING_START +100;
            *Processa->Vuoto[i].Turbo.Acc=false;
            *Processa->Vuoto[i].Turbo.Normale=false;
            if(Processa->Vuoto[i].Meccanica.PowOn->Segnale()){
               *Processa->Vuoto[i].Meccanica.PowIsOn=false;
            }
            TimerBaratron[i].Reset();
            TimerPenning[i].Reset();
          }
          break;

      }


 }

 if(Processa->Bias[Processa->IdCiclo].BiasNext->Segnale())
    *Processa->Bias[Processa->IdCiclo].BiasIsNext=true;
 else
    *Processa->Bias[Processa->IdCiclo].BiasIsNext=false;

 if(Processa->Bias[Processa->IdCiclo].PowOn->Segnale())
      *Processa->Bias[Processa->IdCiclo].PowIsOn=true;
 else
      *Processa->Bias[Processa->IdCiclo].PowIsOn=false;

 if(Processa->Bias[Processa->IdCiclo].HVolt->Segnale()){
    *Processa->Bias[Processa->IdCiclo].IsHVolt=true;
    *Processa->Bias[Processa->IdCiclo].IsLVolt=false;
 }else if(Processa->Bias[Processa->IdCiclo].LVolt->Segnale()){
    *Processa->Bias[Processa->IdCiclo].IsHVolt=false;
    *Processa->Bias[Processa->IdCiclo].IsLVolt=true;
 }else{
     *Processa->Bias[Processa->IdCiclo].IsHVolt=false;
     *Processa->Bias[Processa->IdCiclo].IsLVolt=false;
 }

  *Processa->Bias[Processa->IdCiclo].OnEnable=Processa->Bias[Processa->IdCiclo].On->Segnale();

  NewSegnale=(double)Processa->Bias[Processa->IdCiclo].SetV->Segnale();
  *Processa->Bias[Processa->IdCiclo].GetV=(WORD)NewSegnale;
  NewSegnale=(double)Processa->Bias[Processa->IdCiclo].SetV->Segnale()*0.05;
  *Processa->Bias[Processa->IdCiclo].GetI=(WORD)NewSegnale;

  for(i=0; i<Processa->SorgenteCount;i++){
      *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->AcquaIsOn=*Processa->Vuoto[Processa->IdCiclo].Camera.AcquaIsOn;
      if(Processa->Bias[Processa->IdCiclo].Sorgente[i].PowOn->Segnale())
        *Processa->Bias[Processa->IdCiclo].Sorgente[i].PowIsOn=true;
	  else{
       *Processa->Bias[Processa->IdCiclo].Sorgente[i].PowIsOn=false;
       *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->OnPow=false;
       *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->IsOnArc=false;
       *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->Temp=false;
    }
  }

  for(i=0; i<Processa->SorgenteCount;i++){
      if(Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->OnPow->Segnale()){
        NewSegnale=(double)Processa->Bias[Processa->IdCiclo].Sorgente[i].SetI->Segnale();
        NewSegnale*=Processa->Bias[Processa->IdCiclo].Sorgente[i].SetI->Fattore();
        NewSegnale/=Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->GetI->Fattore();
        *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->GetI=(WORD)NewSegnale;
        if(NewSegnale!=0)
            *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->IsOnArc=true;
        else
            *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->IsOnArc=false;
      }
      else{
        NewSegnale=(double)Processa->Bias[Processa->IdCiclo].Sorgente[i].SetI->Segnale();
        *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->IsOnArc=false;
        *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->GetI=0;
      }
      *Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->ErrPow=false;
  }

  for(i=0; i<Processa->GasCount;i++){
      NewSegnale=(double)Processa->GGas[i].SetG->FSegnale(Processa->GGas[i].GetG->FondoScala())*1;
      *Processa->GGas[i].GetG=(WORD)NewSegnale;
  }





}
