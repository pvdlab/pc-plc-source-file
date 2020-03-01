#include "StdAfx.h"
#include "Catodo.h"
#include "MainCostanti.h"
#include <QDebug>

CCatodo::CCatodo()
{

    GetI=NULL;
    Trigger=NULL;
    OnPow=NULL;
    IsOnArc=NULL;
    Temp=NULL;
    ErrPow=NULL;
    m_IgnoraTargetConsumato=false;
    TipoTraget=0;

}

CCatodo& CCatodo::operator()(CSTimer *New_pMainTimer) 
{
	m_pMainTimer=New_pMainTimer;
	m_ArcOnOffCount=0;
    m_Set=false;
    m_Reset=true;
	m_IsOnArc=false;
	m_TrigOnCount=0;
    m_UltimoAumento=0;
    m_ProcTrigCount=1;
	m_OldAh=0;
	m_Errore=false;
	m_PreSet=false;
    m_PowIsOn=false;
	m_TimerTrigger(CAT_T_TRIGGER);
	m_AspettaTimer(CAT_T_ASPETTA);
    m_BeckArcTimer(CAT_T_ASPETTA_BECKARC);
    m_RegolaTimer(CAT_T_REGOLA);
    m_TimerOn(CAT_T_ON);
	m_AspettaTimer.Reset();
    m_TimerOffArc(CAT_T_OFF_ARC);
    m_SetArcOffTimer(CAT_T_SET_ARC_OFF);
	m_TrigCount=0;
    m_ErrPow=false;
    m_Trigger=false;
    m_OnPow=false;
    m_Riparti=true;
    m_BeckArc=false;
    m_PowerErrore=false;
    m_MaxI=0;
    m_IsGoToStep=true;
    m_ceckCatodoPowErr=false;
    m_IsCorrenteAlta=false;
    m_IgnoraTargetConsumato=false;

    pErrore.append(E_CAT_POWER,"E_CAT_POWER");
    pErrore.append(E_CAT_BACKARC,"E_CAT_BACKARC");
    pErrore.append(E_CAT_TEMP,"E_CAT_TEMP");
    pErrore.append(E_CAT_TROPPI_TRIG,"E_CAT_TROPPI_TRIG");
    pErrore.append(E_CAT_POWER_SPENTA,"E_CAT_POWER_SPENTA");
    pErrore.append(E_CAT_NON_SI_SPEGNE,"E_CAT_NON_SI_SPEGNE");
    pErrore.append(E_CAT_NEXT_STEP,"E_CAT_NEXT_STEP");
    pErrore.append(E_CAT_ARCO_NO_OFF,"E_CAT_ARCO_NO_OFF");
    pErrore.append(E_CAT_ALTA_CORRENTE,"E_CAT_ALTA_CORRENTE");
    pErrore.append(E_CAT_TARGET_CONSUMATO,"E_CAT_TARGET_CONSUMATO"); //E_CAT_ACQUA_IS_OFF
    pErrore.append(E_CAT_ACQUA_IS_OFF,"E_CAT_ACQUA_IS_OFF"); //E_CAT_ACQUA_IS_OFF

	return *this;
}



bool CCatodo::SetEnabled(bool NewEnabled)
{ 

    Enabled=NewEnabled;

    m_IsGoToStep=true;
    if(!Enabled){
        m_IsGoToStep=__min(m_IsGoToStep,Trigger->Set(false));
    }
    m_IsGoToStep=__min(m_IsGoToStep,OnPow->Set(NewEnabled));

    pErrore.Errore(FalseTrue(m_IsGoToStep),E_CAT_NEXT_STEP,1000);

    m_OnTimeFattore=GetI->Fattore();
    m_OnTimeFattore/=3600;

    return m_IsGoToStep;

}


bool CCatodo::CeckStato()
{

	m_IsOnArc=IsOnArc->Segnale();

    if(!OnPow->Segnale()){
		m_Set=false;
		*Trigger=false;
		m_Reset=true;
	}
	else{
		m_Set=true;
		m_Reset=false;
	}

	return true;

}


double CCatodo::OnTime() {

    m_OnTimeFattore=GetI->Segnale();
    m_OnTimeFattore*=GetI->Fattore();
    m_OnTimeFattore/=3600;
    double valore;
    if(Enabled){
        valore=m_OnTime.Timer(IsOnArc->Segnale(),m_OnTimeFattore);
    }else{
        valore=m_OnTime.Timer(false,m_OnTimeFattore);
    }

    m_Consumo=(int)valore;
    return valore;

}

bool CCatodo::IsTargetConsumato(CIOSegnale *pSetI){

    int NewConsumo=pSetI->OnTimeAh();
    m_NextConsumo=m_Consumo;
    m_NextConsumo+=NewConsumo;

    if(getTipoApp()==CTipoApp::Client){
        if(m_NextConsumo>CAT_TARGET_CONSUMATO)
            return true;
        else
           return false;
    }

    if(Debug & (m_NextConsumo>CAT_TARGET_CONSUMATO)){
       std::string str=pSetI->Descrizione();
       str.append(" Consumo ");
       str.append(intToString(m_NextConsumo));
       MostraEvento(str, Color::FG_RED);
    }

    return false;
    //pErrore.Errore(Confronto(m_NextConsumo,CAT_TARGET_CONSUMATO),E_CAT_TARGET_CONSUMATO,0);


}

#define ATTIVA-REGOLA

bool CCatodo::Set(CIOSegnale  *pSetI)
{

   if(!Enabled)
       return true;

   m_Reset=false;

   if( Errore() | (pSetI->Segnale()<CAT_MIN_SET_CORRENTE)){
		*Trigger=false;
		*OnPow=false;
		Aspetta=true;
        m_Trigger=false;
		return true;
   }

#ifdef TEST_BIAS_ARC_OFF
   if(m_TimerOffArc.Timer()){
       *Trigger=false;
       *OnPow=false;
       return true;
   }
#endif

    if(ErrPow->Segnale()){
       if(OnPow->Segnale()){
           std::string str;
           str.append(pErrore.Descrizione());
           str.append(" ");
           str.append("Spengo Per Errore Power");
           MostraEventoProcesso(str,Color::FG_RED);
       }
       m_PowerErrore=true;
       *Trigger=false;
       *OnPow=false;
       Aspetta=true;
       return true;
   }

   m_PowerErrore=false;
   m_ErrPow=false;

   if(!OnPow->Segnale() ){
         *OnPow=true;
         *Trigger=false;
         m_TimerOn.Reset();
         m_TimerOn.Timer();
         return true;
   }else{
        if(!m_TimerOn.Timer())
            return true;
   }

   if(!m_IsOnArc){
       if(!Trigger->Segnale()){
	      m_TimerTrigger.Reset();
          *Trigger=true;
          m_RegolaTimer(CAT_T_REGOLA);
          m_RegolaTimer.Reset();
          if(!m_Trigger){
              m_Trigger=true;
              m_ArcOnOffCount++;
              m_ProcTrigCount++;
              if(Debug){
                  std::string str;
                  str.append(pErrore.Descrizione());
                  str.append(" Trig Count ");
                  str.append(intToString(m_ProcTrigCount));
                  str.append(" ArcOnOff Count ");
                  str.append(intToString(m_ArcOnOffCount));
                  str.append(" m_Segnale ");
                  str.append(intToString(pSetI->Segnale()));
                  str.append(" m_Regola ");
                  str.append(doubleToString(pSetI->getRegola()));
                  MostraEventoProcesso(str,Color::FG_DEFAULT);
              }

          }

          if(m_CTipoStep==CTipoStep::Etching){
		    if(m_pMainTimer->Time()-m_ArcAh>=CAT_ETCHING_ARC_ONOFF_TIME)
               m_ArcOnOffCount=0;
		  }
		  else{
		    if(m_pMainTimer->Time()-m_ArcAh>=CAT_COATING_ARC_ONOFF_TIME)
               m_ArcOnOffCount=0;
		  }
          m_ArcAh=m_pMainTimer->Time();
		  m_AspettaTimer.Reset();
	   }
       else{
          if (m_TimerTrigger.Timer()){
            *Trigger=false;
            m_Trigger=false;
            m_IsOnArc=true;
            m_RegolaTimer(CAT_T_REGOLA);
            m_RegolaTimer.Reset();
            m_TimerOffArc.Reset();
		  }
	   }
   }else{
        ///DOPO AVER ACCESSO ASPETTO PER VEDERE SE SI SPEGNE SUBITO
        if(m_TimerOffArc.Timer()){
           m_ArcOnOffCount=0;
        }
        ///ASPETTA PRIMA DI LEGGERE SE È ACCESO
        if(!m_AspettaTimer.Timer()){
             if(IsOnArc->Segnale()){
                m_AspettaTimer.Reset();
                m_IsOnArc=IsOnArc->Segnale();
             }
        }else
           m_IsOnArc=IsOnArc->Segnale();
   }

#ifdef ATTIVA-REGOLA
    if(m_RegolaTimer.Timer() & (pSetI->OnTime()>20000)){
        double di;
        double mseti=pSetI->Segnale();
        mseti*=pSetI->Fattore();
        ///REGOLA SOLO SE LA CORRENTE È DIVERSA DA ZERO
        if(pSetI->Get()){
            double mgeti=GetI->Segnale();
            mgeti*=GetI->Fattore();
            di=mgeti-pSetI->Get()*pSetI->Fattore();
            if((di < -0.5) | (di > 0.5)){
                if(mgeti ){
                    double newseti=mseti;
                    newseti*=pSetI->Get()*pSetI->Fattore();
                    newseti/=mgeti;
                    //m_Regola=
                    if(pSetI->Fattore()){
                        newseti/=pSetI->Fattore();
                        if((newseti>CAT_MIN_GET_CORRENTE) & (newseti<CAT_MAX_GET_CORRENTE) ){
                            pSetI->Set(newseti);
                            m_BeckArc=false;
                            m_IsCorrenteAlta=false;
                        }else{
                            if((newseti<CAT_MIN_GET_CORRENTE) & (IsOnArc->Segnale())){
                                if(!m_BeckArc){
                                    m_BeckArc=true;
                                    std::string str;
                                    str.append(pErrore.Descrizione());
                                    str.append(" m_BeckArc ");
                                    str.append(intToString(newseti*pSetI->Fattore()));
                                    MostraEventoProcesso(str,Color::FG_RED);
                                }else{
                                    std::string str;
                                    str.append(pErrore.Descrizione());
                                    str.append(" CORRENTE BASSA ");
                                    str.append(intToString(newseti*pSetI->Fattore()));
                                    MostraEventoProcesso(str,Color::FG_RED);
                                    pSetI->SetStart();
                                }
                            }else{
                                if(!m_IsCorrenteAlta){
                                    m_IsCorrenteAlta=true;
                                    std::string str;
                                    str.append(pErrore.Descrizione());
                                    str.append(" CORRENTE ALTA ");
                                    str.append(intToString(newseti*pSetI->Fattore()));
                                    MostraEventoProcesso(str,Color::FG_RED);
                                }
                                pSetI->SetStart();
                            }

                        }
                    }

                   m_RegolaTimer(6000);
                   m_RegolaTimer.Reset();
                }
            }else{
                pSetI->SetStabile();
            }
        }else{
            pSetI->Set(0);
        }

    }
#endif
    m_Set=true;
	

    return m_Set;
}

bool CCatodo::SetArcOff()
{

   if(!Enabled )
      return true;

   *Trigger=false;
   *OnPow=false;
   m_RegolaTimer.Reset();
#ifdef TEST_BIAS_ARC_OFF
   if(!m_TimerOffArc.IsAttivo())
      m_TimerOffArc.Reset();
#endif
   if(!IsOnArc->Segnale()){
      m_IsOnArc=false;
	  return true;
   }else
      return false;

}

bool CCatodo::IsSetArcOff(){

    return m_TimerOffArc.IsAttivo();
}

void CCatodo::ResetTrigger()
{

  m_ArcOnOffCount=0;
  m_ProcTrigCount=0;
  m_Errore=false;
	 
}

void CCatodo::ceckCatodoPowErr()
{
  
  if(!m_ceckCatodoPowErr){
      if(ErrPow->Segnale() | getSimula()){
         if(Debug & !m_ceckCatodoPowErr){
             std::string str;
             str.append(pErrore.Descrizione());
             str.append(" ceckCatodoPowErr");
             MostraEventoProcesso(str,Color::FG_DEFAULT);
         }
         m_ceckCatodoPowErr=true;
	  }
  }

  pErrore.Errore(FalseTrue(m_ceckCatodoPowErr),E_CAT_POWER_SPENTA,E_TIME);

  m_PowIsOn=true;
	 
}

void CCatodo::SetOff()
{
  m_PowIsOn=false;
  m_ceckCatodoPowErr=false;
	 
}

bool CCatodo::Reset()
{

    m_Reset=true;
    m_Reset=__min(m_Reset,Trigger->Set(false));
    m_Reset=__min(m_Reset,OnPow->Set(false));

    if(m_Reset){
       m_RegolaTimer.Reset();
       m_TimerTrigger.Reset();
       m_AspettaTimer.Reset();
       m_ArcOnOffCount=0;
       m_UltimoAumento=0;
       m_TrigOnCount+=m_ProcTrigCount;
       m_ProcTrigCount=1;
       m_OldAh+=(WORD)m_Ah.Time();
       m_Set=false;
       m_Reset=true;
       m_PreSet=false;
       m_Errore=false;
       m_IsOnArc=false;
       m_OnPow=false;
       m_Trigger=false;
       m_Riparti=true;
       m_BeckArc=false;
       m_PowerErrore=false;
       m_IsGoToStep=true;
       m_PowIsOn=false;
       m_ceckCatodoPowErr=false;
       m_IsCorrenteAlta=false;
       m_IgnoraTargetConsumato=false;
    }

    return m_Reset;
}

bool CCatodo::Errore()
{

    if(!Enabled )
        return false;

    if(m_Errore){
        return m_Errore;
    }


    pErrore.Errore(m_BeckArc,E_CAT_BACKARC,5*E_TIME);
    pErrore.Errore(ErrPow->Segnale(),E_CAT_POWER,E_TIME);
    pErrore.Errore(Confronto(m_ArcOnOffCount,CAT_ARC_ONOFF_COUNT),E_CAT_ARCO_NO_OFF,E_TIME);

    if(OnPow->Segnale() ){
        if(getSimula())
           pErrore.Errore(false,E_CAT_ACQUA_IS_OFF,E_TIME);
        else
           pErrore.Errore(FalseTrue(AcquaIsOn->Segnale()),E_CAT_ACQUA_IS_OFF,E_TIME);
        pErrore.Errore(false,E_CAT_NON_SI_SPEGNE,E_TIME);
    }else{
        pErrore.Errore(false,E_CAT_ACQUA_IS_OFF,E_TIME);
    }

    if(Temp!=NULL)
       pErrore.Errore(Confronto(Temp->Segnale(),CAT_MAX_TEMP_ACQUA),E_CAT_TEMP,E_TIME);
    else
       pErrore.Errore(false,E_CAT_TEMP,E_TIME);

    //pErrore.Errore(FalseTrue(m_ceckCatodoPowErr),E_CAT_POWER_SPENTA,E_TIME);
    pErrore.Errore(Confronto(m_ProcTrigCount,CAT_MAX_TRIGGER_COUNT),E_CAT_TROPPI_TRIG,E_TIME);
    if(m_IgnoraTargetConsumato)
       pErrore.Errore(Confronto(m_Consumo,CAT_TARGET_CONSUMATO),E_CAT_TARGET_CONSUMATO,E_TIME);

    m_Errore=pErrore.Errore();

    if(m_Errore){
       if(pErrore.IsErrore(E_CAT_POWER) | pErrore.IsErrore(CAT_MAX_TRIGGER_COUNT)
               | pErrore.IsErrore(CAT_ARC_ONOFF_COUNT) | pErrore.IsErrore(E_CAT_TARGET_CONSUMATO)){
           std::string str;
           str.append(pErrore.Descrizione());
           if(pErrore.IsErrore(CAT_MAX_TRIGGER_COUNT))
               str.append(" Fermo per CAT_MAX_TRIGGER_COUNT");
           if(pErrore.IsErrore(CAT_ARC_ONOFF_COUNT))
               str.append(" Fermo per CAT_ARC_ONOFF_COUNT");
           if(pErrore.IsErrore(E_CAT_TARGET_CONSUMATO))
               str.append(" Fermo per E_CAT_TARGET_CONSUMATO");
           if(pErrore.IsErrore(E_CAT_POWER))
               str.append(" Fermo per E_CAT_POWER");
           MostraEventoProcesso(str,Color::FG_RED);
           Enabled=false;
       }else
           m_Riparti=false;
    }

    return m_Errore;
    
}


CSorgente::CSorgente()
{
    SetI=NULL;
    PowOn=NULL;
    PowIsOn=NULL;
    PowErr=NULL;
    Schermo=NULL;
    Catodo=NULL;

}


CSorgente& CSorgente::operator()(CSTimer *New_pMainTimer) 
{
    Schermo=NULL;
    m_pMainTimer=New_pMainTimer;
    Catodo = new CCatodo;
    Catodo->Enabled=true;
    Enabled=true;
    m_Set=false;
    m_Reset=true;
    m_ResetSegnali=false;
    m_ActStep=0;
    m_InReset=false;
	m_StendBy=false;
	m_Corrente=0;
    m_CatodiOn=0;
    m_Spedito=true;
    m_Timer(10000,10000);
	m_Index=0;
    m_BiasIsOn=false;
    m_CatodoOnPerBias=false;
    m_Riparti=true;
    m_EraSpenta=false;
    m_IsGoToStep=true;

    pErrore.append(E_SOR_TERMICO_POWER,"E_SOR_TERMICO_POWER");
    pErrore.append(E_SOR_RELE_POWER,"E_SOR_RELE_POWER");
    pErrore.append(E_SOR_CATODI_OFF,"E_SOR_CATODI_OFF");
    pErrore.append(E_SOR_PRESET,"E_SOR_PRESET");
    pErrore.append(E_SOR_NEXT_STEP,"E_SOR_NEXT_STEP");

	return *this;
	
}

#ifdef TEST_BIAS_ARC_OFF
bool CSorgente::SetArcOff(){

    Catodo->SetArcOff();

    return true;
}

#else
bool CSorgente::SetArcOff(){

    Catodo->SetArcOff();

    if(Schermo)
       *Schermo=false;

    return true;
}
#endif


bool CSorgente::ManualeRun(){

    Errore();
    return Catodo->Set(SetI);

}

bool CSorgente::Set()
{

    if(!Enabled)
        return true;

    if(!PowIsOn->Segnale()){
        if(!m_EraSpenta){
            std::string str;
            str.append(pErrore.Descrizione());
            str.append(" La Power Era Spenta ");
            MostraEvento(str,Color::FG_RED);
            m_EraSpenta=true;
        }
        SetOn();
        return false;
    }

    if(!SetI->Attivo() | Errore() ){
        Catodo->Reset();
        if(Schermo)
           *Schermo=false;
		return false;
    }

    if(!m_BiasIsOn){
        m_CatodoOnPerBias=true;
        Catodo->Reset();
		return false;
    }

    m_CatodoOnPerBias=false;
    m_Reset=false;
	m_ResetSegnali=false;
	m_StendBy=false;
	
    m_Set=SetI->Set();
     if(Schermo){
       if(SetI->Info()==1 ){
           *Schermo=true;
        }else if(SetI->Info()==2 ){
           *Schermo=false;
        }
    }


    Catodo->Set(SetI);


 	return m_Set;
}


bool CSorgente::SchermoApri(){

    if(Schermo)
       Schermo->Set(true);

    return true;
}

bool CSorgente::SchermoChiudi(){

    if(!Schermo)
       Schermo->Set(false);

    return true;
}





bool CSorgente::StendBy()
{
   if(!Enabled )
      return true;
   
   if(PowIsOn->Segnale()){
      *Catodo->OnPow=false;
      Catodo->ResetTrigger();
   }

   SetI->StendBy();
   m_StendBy=true;

   return m_StendBy;
}

bool CSorgente::ResetStendBy()
{

   if(!Enabled | !m_StendBy)
		return true; 

   SetI->ResetStendBy();

   m_StendBy=false;

   return true;
}

bool CSorgente::ResetSegnali()
{
    if(!Enabled)
        return true;

	if(m_ResetSegnali)
      return m_ResetSegnali;

   m_ResetSegnali=SetI->ResetSegnali();
   if(m_ResetSegnali){
      m_ResetSegnali=true;
 	  m_Set=false;
	  m_Reset=false;
	  m_ActStep=0;
   }
   
   return m_ResetSegnali;
}


bool CSorgente::Reset()
{

    m_Reset=true;

    m_Reset=__min(m_Reset,SetI->Reset());
    m_Reset=__min(m_Reset,Catodo->Reset());

    if(!m_Reset)
        return m_Reset;

    m_Reset=__min(m_Reset, PowOn->Set(false));

    if(m_Reset){
      m_Riparti=true;
      m_EraSpenta=false;
      m_ResetSegnali=false;
      m_Set=false;
      m_Reset=true;
      m_IsGoToStep=true;
    }

    return m_Reset;
}

bool CSorgente::SetOff()
{  
    return Reset();
}



void CSorgente::ResetErrore() 
{ 
    pErrore.Reset();
    Catodo->ResetErrore();
    m_Errore=false ;
    
}

void CSorgente::ceckNextStepError(){

    if(Enabled)
       pErrore.Errore(FalseTrue(m_IsGoToStep),E_SOR_NEXT_STEP,0);
}

void CSorgente::ceckPowerError(){

    if(!Enabled)
        return;

    if(PowOn->Segnale()){
        pErrore.Errore(PowErr->Segnale(),E_SOR_TERMICO_POWER,0);
        pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_SOR_RELE_POWER,0);
    }else{
        pErrore.Errore(PowErr->Segnale(),E_SOR_TERMICO_POWER,0);
        pErrore.Errore(PowIsOn->Segnale(),E_SOR_RELE_POWER,0);
    }


}

bool CSorgente::GoToStep(BYTE Step,BYTE CTipoStep)
{
   if(!Enabled)
		return true;

   if(m_ActStep==Step)
     return true;

   m_EraSpenta=false;
   m_CTipoStep=CTipoStep;
   m_ActStep=Step;

   m_IsGoToStep=true;
   m_IsGoToStep=__min(m_IsGoToStep,SetI->GoToStep(Step));

   Catodo->ResetTrigger();
   Catodo->CTipoStep(m_CTipoStep);
   Catodo->TrigCount(0);
   m_IsGoToStep=__min(m_IsGoToStep,Catodo->SetEnabled(true));

   return m_IsGoToStep;

}

bool CSorgente::GoToStep(BYTE Step,DWORD Tempo,BYTE CTipoStep)
{

   m_IsGoToStep=true;
   m_IsGoToStep=__min(m_IsGoToStep,GoToStep(Step,CTipoStep));
   m_IsGoToStep=__min(m_IsGoToStep,SetI->GoToStep(Step,Tempo));

   return m_IsGoToStep;

}


void CSorgente::ceckCatodoPowErr(){

    if(IsOn()){
          Catodo->ceckCatodoPowErr();
    }

}

bool CSorgente::SetOn()
{

    return PowOn->Set(true);
}



bool   CSorgente::IsOn() 
{

   return PowIsOn->Segnale();

}


bool   CSorgente::ArcIsOn() 
{

	if(!Enabled)
       return true;

    return Catodo->IsOnArc->Segnale();
		
}


bool CSorgente::Errore()
{
       
	if(!Enabled)
        return false;

    if( PowOn->Segnale()){
       pErrore.Errore(PowErr->Segnale(),E_SOR_TERMICO_POWER,E_TIME);
       pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_SOR_RELE_POWER,E_TIME);
    }else{
       pErrore.Errore(false,E_SOR_TERMICO_POWER,E_TIME);
       pErrore.Errore(false,E_SOR_RELE_POWER,E_TIME);
    }

    m_Errore=pErrore.Errore();

    if(m_Errore){
       m_Riparti=false;
    }
	
    return m_Errore;

}



void CSorgente::CeckStato()
{

    Catodo->CeckStato();

    if(SetI->Segnale()!=0){
      m_Set=true;
	  m_Reset=false;
    }
	else{
      m_Set=false;
	  m_Reset=true;
    }

}



