
#include "StdAfx.h"
#include "Gas.h"
#include <QDebug>
#include "MainCostanti.h"


CGas::CGas()
{

	m_Set=false;
    m_Reset=true;
    m_PreSet=false;
    m_ResetSegnali=false;
    m_ResetSetG=false;
    m_ActStep=0;
	m_InReset=false;
    m_ResetErrore=false;
	m_Errore=false;
	m_Offset=0;
	m_ErrorPreSet=false;
    m_ErrorGasMin=false;

    SetG=NULL;
    GetG=NULL;
    Valvola=NULL;
    Baratron=NULL;

    m_TimerReset(GAS_TIME_RESET_GAS);
    m_TimerPreStep(GAS_TIME_PRESTEP_GAS);
	m_TimerErrore(500);
    m_TimerDp(GAS_TIME_MIN);

	m_TimerErrore.Reset();
	m_TimerPreStep.Reset();

    pErrore.append(E_GAS_INSTABILE,"E_GAS_INSTABILE");
    pErrore.append(E_GAS_MINIMO,"E_GAS_MINIMO");
    pErrore.append(E_GAS_PIRANI_ALLARM,"E_GAS_PIRANI_ALLARM");
    pErrore.append(E_GAS_NEXT_STEP,"E_GAS_NEXT_STEP");

}




bool CGas::IsMin(){

    if(m_PreSet)
        return false;

    if(SetG->TipoComp==CTipoComDef::ARGON){
        if(GetG->Segnale()< GAS_MIN_ARGON)
            return true;
        else
            return false;
    }else if(SetG->TipoComp==CTipoComDef::AZOTO){
        if(GetG->Segnale()< GAS_MIN_AZOTO)
            return true;
        else
            return false;
    }else{
        if(GetG->Segnale()< GAS_MIN_ACETI)
            return true;
        else
            return false;
    }
}

WORD CGas::Min(){

    if(SetG->TipoComp==CTipoComDef::ARGON){
          return GAS_MIN_ARGON;
    }else if(SetG->TipoComp==CTipoComDef::AZOTO){
        return GAS_MIN_AZOTO;
    }else{
        return GAS_MIN_ACETI;
    }
    return 0;
}


bool CGas::GoToStep(BYTE Step,DWORD Tempo)
{
    m_IsGoToStep=true;
    m_IsGoToStep=__min(m_IsGoToStep,GoToStep(Step));
    m_IsGoToStep=__min(m_IsGoToStep,SetG->GoToStep(Step,Tempo));

    return m_IsGoToStep;;
}

void CGas::ceckNextStepError(){

    pErrore.Errore(FalseTrue(m_IsGoToStep),E_GAS_NEXT_STEP,0);

}


void CGas::ceckGasErrore(){

    pErrore.Errore(FalseTrue(m_IsGoToStep),E_GAS_NEXT_STEP,0);
}

bool CGas::GoToStep(BYTE Step)
{

   if(!Enabled | (m_ActStep==Step))
       return true;


   m_IsGoToStep=true;
   if(SetG->IsZero(Step) ){
      if(!Valvola->Segnale()){
         m_IsGoToStep=__min(m_IsGoToStep,Valvola->Set(true));
         m_TimerPreStep.Reset();
      }
      pErrore.Aspetta(GAS_TIME_STABILIZZA);
      m_IsGoToStep=__min(m_IsGoToStep,SetG->GoToStep(Step));
      m_IsGoToStep=__min(m_IsGoToStep,FalseTrue(IsMin()));
      m_GasStep=SetG->ActStep();
   }
   else{
      m_IsGoToStep=__min(m_IsGoToStep,SetG->GoToStep(Step));
      m_GasStep=SetG->ActStep();
      m_IsStart=true;
      return m_IsGoToStep;
   }

   if(m_TimerPreStep.Timer()){
      m_ActStep=Step;
      m_Info=SetG->Info();
      m_TimerDp(GAS_TIME_MIN);
      m_IsStart=true;
      return m_IsGoToStep;
    }
    else{
        return false;
    }

    return false;
}


bool CGas::ManualeRun(){

   if(SetG->Segnale()){
       if(!Valvola->Set(true))
           return true;
   }

   return Errore();

}



bool CGas::Set()
{
    WORD   dp;
	WORD   dt;
    WORD   DBaratron;
    WORD   GetGas;


    if(!Enabled | !SetG->Attivo()){
       GetGas=GetG->FSegnale(SetG->FondoScala());
	   if(GetGas<20){
	      if(Valvola->Segnale()){
            *Valvola=false;
		    m_ErrorGasMin=false;
		  }
		  else{
			m_ErrorGasMin=true;
		  }
	   }
	   return true;
	}

    IsMin();

    if(!SetG->IsStart()){
        return m_Set;
    }

    if(m_GasStep!=SetG->ActStep()){
       m_GasStep=SetG->ActStep();
       pErrore.Aspetta(GAS_TIME_STABILIZZA);
	}
    
    if(Errore()){
	  m_Set=false;
	  m_Reset=false;
	  return m_Set;
    }

    m_ResetSegnali=false;
    m_Reset=false;

    if(m_Info!=SetG->Info()){
      m_Info=SetG->Info();
    }
	
    if(SetG->Segnale()){
	   *Valvola=true;
	   if(m_TimerRitardo.Timer()){
		   m_PreSet=true;
	   }
    }
    else{
       *Valvola=false;
	   m_TimerPreStep.Reset();
    }

    bool BaratonOff;
    if(!Baratron)
        BaratonOff=true;
    else
        BaratonOff=false;

    if(!m_Info | SetG->IsRampa() | BaratonOff)
	  m_Set=SetG->Set();
    else{
		if(m_TimerDp.Timer()){
		  DBaratron=0.005*m_Info;
          dp=abs(m_Info-Baratron->Segnale());
          if(Baratron->Segnale()<m_Info-DBaratron){
              if(SetG->Segnale()+1<GAS_MAX_GETGAS)
               *SetG=SetG->Segnale()+1;
		  }
          else if(Baratron->Segnale()>m_Info+DBaratron){
               if(SetG->Segnale()-1>GAS_MIN_GETGAS)
                 *SetG=SetG->Segnale()-1;
          }
		  if(dp){
			  dt=GAS_T_MAX/dp;
              if(dt<GAS_TIME_MIN)
                  dt=GAS_TIME_MIN;
			  else if (dt>GAS_T_MAX)
				 dt=GAS_T_MAX;
		  }
		  else
             dt=GAS_T_MAX;
		  m_TimerDp(dt);
		}
    }

 	return m_Set;
}


void CGas::CeckStato()
{
	if(!Enabled)
		return ;

	if(GetG->Segnale()!=0){ 
      m_Reset=false;
	  m_Set=true;
	}
	else{
	  m_Reset=true;
	  m_Set=false;
	}

}

void  CGas::ResetErrore() {

    pErrore.Reset();
    m_Errore=false ;

}

bool CGas::Reset()
{

   m_PreSet=false;

   if(SetG->Segnale() > 0){
     m_TimerReset.Reset();
     m_ResetSetG=true;
   }

   m_ResetErrore=true;
   m_Reset=true;
   m_Reset=__min(m_Reset,SetG->Reset());

   if(m_ResetSetG){
       if(!m_TimerReset.Timer()){
           m_Reset=false;
           return m_Reset;
       }
   }

   if(!m_Reset)
       return m_Reset;

   m_Reset=__min(m_Reset, Valvola->Set(false));
   if(m_Reset){
       m_TimerPreStep.Reset();
       m_ResetSegnali=false;
       m_ErrorPreSet=false;
       m_ResetSetG=false;
       m_Set=false;
       m_ActStep=0;
       m_InReset=false;
       m_Errore=false;
       m_Offset=0;
       m_ErrorGasMin=false;
       pErrore.Reset();
   }

   return m_Reset;
}


bool CGas::SetOff()
{
   return Reset();
}



bool CGas::StendBy()
{
   if(!Enabled || m_StendBy )
		return true;

   SetG->StendBy();   

   m_StendBy=true;

   return m_StendBy;
}

bool CGas::ResetStendBy()
{
   if(!Enabled || !m_StendBy )
		return true;

   SetG->ResetStendBy();   
   m_StendBy=false;

   return true;
}

bool CGas::ResetSegnali()
{
    if(!Enabled)
		return true;

	if(m_ResetSegnali)
      return m_ResetSegnali;

   m_ResetSegnali=SetG->ResetSegnali();
   if(m_ResetSegnali){
      m_ResetSegnali=true;
 	  m_Set=false;
	  m_Reset=false;
	  m_ActStep=0;
   }


   return m_ResetSegnali;
}


bool CGas::Errore()
{

	WORD GetGas,SetGas;

    if(!Enabled | !Valvola->Segnale())
         return false;


    if(SetG->Segnale()){
      SetGas=SetG->Segnale();
      SetGas*=0.7;
	  GetGas=GetG->FSegnale(SetG->FondoScala());
      pErrore.Errore(Confronto(SetGas,GetGas+1),E_GAS_INSTABILE,E_TIME);

    }
	else
      pErrore.Errore(false,E_GAS_INSTABILE,E_TIME);

    if(SetG->Segnale())
      pErrore.Errore(IsMin() ,E_GAS_MINIMO,E_TIME);

    m_Errore=pErrore.Errore();
	
    return m_Errore;

}




CMiscela::CMiscela()
{

  m_Info[0]=0;
  m_Info[1]=0;
  m_Info[2]=0;

}



CMiscela::~CMiscela()
{

}

bool CMiscela::GoToStep(WORD Pressione)
{
   if(!Enabled)
	   return true;

   m_Pressione=Pressione;

   return true;

   
}


bool CMiscela::Set()
{
   if(!Enabled)
	   return true;
   

   return true;

   
}

