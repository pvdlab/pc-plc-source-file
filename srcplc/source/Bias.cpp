#include "StdAfx.h"
#include "Bias.h"
#include "MainCostanti.h"
#include <QDebug>

//#define NO_ERRORE
#define NO_TAP 10
CBias::CBias(){

}

void CBias::Init(WORD NewSorgenteCount, BYTE newBias)
{
    SorgenteCount= NewSorgenteCount;
    m_Bias=newBias;

    OnEnable=NULL;
    IsLVolt=NULL;
    LVolt=NULL;
    HVolt=NULL;
    IsHVolt=NULL;
    Allarme=NULL;
    Arco=NULL;
    BiasNext=NULL;

    if(!this)
      delete[] Sorgente;
    Sorgente = new CSorgente[SorgenteCount];
	m_Set=false;
    m_Reset=true;
    m_PrimoEtching=false;
    m_ResetSegnali=false;
    m_ActStep=0;
	m_ArcCount=0;
    m_Timer(BIAS_TAP_TIME/3);
    m_AspettaTimer(BIAS_ASPETTA);
    m_ErroreTap=false;
    m_IsSetTap=false;
	m_ResetTap=false;
	m_ArcIsOn=false;
	m_BiasIsLow=false;
    m_BiasOffTime(120000);
	m_BiasLowCount=BIAS_LOW_PRIMO;
    m_Info=NO_TAP;
    m_Schutter=false;
    m_Riparti=true;
    m_Repit=0;
    m_ContrappostiIsErrore=false;
    m_ContrappostiVedo=false;
    m_IsGoToStep=true;

    pErrore.append(E_BIAS_RELE_POWER,"E_BIAS_RELE_POWER");
    pErrore.append(E_BIAS_TERMICO_POWER,"E_BIAS_TERMICO_POWER");
    pErrore.append(E_BIAS_IN_ETCHING,"E_BIAS_IN_ETCHING");
    pErrore.append(E_BIAS_CORTO_COATING,"E_BIAS_CORTO_COATING");
    pErrore.append(E_BIAS_NO_CORRENTE,"E_BIAS_NO_CORRENTE");
    pErrore.append(E_BIAS_TROPPI_LOW_V,"E_BIAS_TROPPI_LOW_V");
    pErrore.append(E_BIAS_ALLARM,"E_BIAS_ALLARM");
    pErrore.append(E_BIAS_CORTO_ETCHING,"E_BIAS_CORTO_ETCHING");
    pErrore.append(E_BIAS_NON_CAMBIA_TAP,"BIAS_NON CAMBIA_TAP");
    pErrore.append(E_BIAS_TAP_ERRATO,"E_BIAS_TAP_ERRAT");
    pErrore.append(E_BIAS_GETV_IS_ZERO,"E_BIAS_GETV_IS_ZERO");
    pErrore.append(E_BIAS_CATODI_SPENTI,"E_BIAS_CATODI_CONTRAPPOSTI_SPENTI");
    pErrore.append(E_BIAS_NEXT_STEP,"E_BIAS_NEXT_STEP"); //E_BIAS_CORTO_PRESET
    pErrore.append(E_BIAS_CORTO_PRESET,"E_BIAS_CORTO_PRESET");
    pErrore.append(E_BIAS_ETCHING_TOO_LOW,"E_BIAS_ETCHING_TOO_LOW");
    pErrore.append(W_BIAS_IS_LOW,"W_BIAS_IS_LOW");


}

void CBias::ceckNextStepError(){

    pErrore.Errore(FalseTrue(m_SetTap),E_BIAS_NON_CAMBIA_TAP,0);
    //pErrore.Errore(FalseTrue(m_IsGoToStep),E_BIAS_NEXT_STEP,0);

}

void CBias::ceckPowerError(){

    if(PowOn->Segnale()){
        pErrore.Errore(PowErr->Segnale(),E_BIAS_TERMICO_POWER,0);
        pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_BIAS_RELE_POWER,0);
    }else{
        pErrore.Errore(PowErr->Segnale(),E_BIAS_TERMICO_POWER,0);
        pErrore.Errore(PowIsOn->Segnale(),E_BIAS_RELE_POWER,0);
    }


}



bool CBias::IsSetTap()
{

    return m_IsSetTap;

}

bool CBias::GoToStep(BYTE Step,BYTE CTipoStep,BYTE PerOffTime)
{

   if(!Enabled)
	   return true;

   if(m_Info!=SetV->InfoGotoStep(Step)){
     if(!SetTap(SetV->InfoGotoStep(Step))){
        m_IsGoToStep=false;
        return false;
     }
   }

   m_IsGoToStep=true;
   m_IsGoToStep=__min(m_IsGoToStep,SetV->GoToStep(Step));

   for(int i=0;i<SorgenteCount;i++){
      Sorgente[i].ResetErrore();
      Sorgente[i].Catodo->OnTime();
   }

   DWORD ArcOnTime;
   if(CTipoStep==CTipoStep::Etching){
       ArcOnTime=Sorgente[0].SetI->OnTime(Step);
       for(int i=0;i<SorgenteCount;i++){
         if(Sorgente[i].Enabled)
           ArcOnTime+=Sorgente[i].SetI->OnTime(Step);
       }

   }else{
       ArcOnTime=Sorgente[0].SetI->OnTime(Step);
       for(int i=0;i<SorgenteCount;i++){
         if(Sorgente[i].Enabled)
            ArcOnTime=__max(ArcOnTime,Sorgente[i].SetI->OnTime(Step));
       }
   }

   m_BiasOffTime(ArcOnTime);

   m_ContrappostiVedo=false;
   m_ContrappostiIsErrore=false;
   m_PrimoEtching=SetV->IsPrimoStep();
   m_BiasIsLow=false;
   m_BiasOffTime.Reset();
   m_PerOffTime=PerOffTime;
   m_CTipoStep=CTipoStep;
   m_ActStep=Step;
   m_BiasAllarmCount=0;
   m_Riparti=false;
   ceckNextStepError();

   return m_IsGoToStep;

}


bool CBias::GoToStep(BYTE Step,DWORD Tempo,BYTE CTipoStep,BYTE PerOffTime)
{
   
   if(!Enabled)
	   return true;

   m_IsGoToStep=true;
   m_IsGoToStep=__min(m_IsGoToStep,GoToStep(Step,CTipoStep,PerOffTime));
   m_IsGoToStep=__min(m_IsGoToStep,SetV->GoToStep(Step,Tempo));

   return m_IsGoToStep;
}

bool CBias::SetOn()
{

    return PowOn->Set(true);
}

bool CBias::SetOff()
{
	return Reset();
}

bool CBias::IsOn()
{
    return PowIsOn->Segnale();

}


bool  CBias::ManualeRun(){

    Errore();
    return true;

}


bool CBias::Set()
{

    setBias_V(DSegnale(GetV));
    setBias_I(DSegnale(GetI));
    for(int i=0; i< SorgenteCount;i++){
       Sorgente[i].Catodo->OnTime();
    }

    if(!Enabled | !SetV->Attivo() | m_IsSetTap)
        return true;

    if(Errore() | ContrappostiIsErrore() | getAcquaErrore()){
        SetArcOff();
        return false;
    }


    for(int i=0;i<SorgenteCount;i++){
       m_ArcIsOn=__max(m_ArcIsOn,Sorgente[i].ArcIsOn());

    }

    m_BiasIsLow=false;
    m_ResetSegnali=false;
	m_Reset=false;


    m_Set=SetV->Set();

    if(!m_Timer.Timer()){
        if((m_Bias==BP5_BIAS) & !getSimula()){
           *HVolt=false;
           *LVolt=false;
        }
        return m_Set;
    }

    if(!GetV->Segnale())
        return m_Set;

    if(SetV->Segnale()>0){
#ifdef TEST_BIAS_ARC_OFF
      if(m_ArcIsOn){
            if(Confronto(m_SetVBias*0.5, m_GetVBias)){
                for(int i=0;i<SorgenteCount;i++){
                    Sorgente[i].SetArcOff();
                }
            }
        }
#endif
        for(int i=0;i<SorgenteCount;i++){
            Sorgente[i].BiasIsOn(true);
            Sorgente[i].Set();
        }
    }else{
        for(int i=0;i<SorgenteCount;i++){
            Sorgente[i].SetArcOff();
        }
    }


 	return m_Set;
}


bool CBias::ContrappostiIsErrore(){


    bool errore=false;
 /*   if(Sorgente[0].Enabled & Sorgente[2].Enabled){
      if(Sorgente[0].Catodo->pErrore.Errore() & Sorgente[2].Catodo->pErrore.Errore())
         errore=true;
    }

    if(Sorgente[1].Enabled & Sorgente[3].Enabled){
     if(Sorgente[1].Catodo->pErrore.Errore() & Sorgente[3].Catodo->pErrore.Errore())
        errore=true;
    }
    if(Sorgente[4].Enabled & Sorgente[5].Enabled){
      if(Sorgente[4].Catodo->pErrore.Errore() & Sorgente[5].Catodo->pErrore.Errore())
        errore=true;
    }

    if(errore & (m_ContrappostiIsErrore!=errore)){
        if(!m_ContrappostiVedo)
          MostraEventoProcesso("VEDO E_CATODO_COTRA_SPENTI",Color::FG_RED);
        m_ContrappostiVedo=true;
    }*/
    m_ContrappostiIsErrore=errore;

    return m_ContrappostiIsErrore;

}


bool CBias::SetArcOff()
{
    bool RetVal=true;

      for(int i=0;i<SorgenteCount;i++){
       if(Sorgente[i].PowIsOn->Segnale())
            RetVal=__min(RetVal,Sorgente[i].Catodo->SetArcOff());
      }

      return RetVal;
}


bool CBias::Reset()
{

   m_Reset=true;
   m_Reset=__min(m_Reset,SetV->Reset());
   m_Reset=__min(m_Reset, LVolt->Set(false));
   m_Reset=__min(m_Reset, HVolt->Set(false));
   m_Reset=__min(m_Reset, On->Set(false));
   m_Reset=__min(m_Reset, PowOn->Set(false));
   //qDebug() << "RESET pImpianto->Bias";

   if(m_Reset){
       m_RisalitaTimer.Reset();
       m_AspettaTimer.Reset();
       m_Timer.Reset();
       m_ResetTap=false;
       m_IsSetTap=false;
       m_Info=NO_TAP;
       m_ResetSegnali=false;
       m_Set=false;
       m_ActStep=0;
       m_ArcCount=0;
       m_ArcIsOn=false;
       m_BiasIsLow=false;
       m_BiasOffTime.Reset();
       m_BiasLowCount=BIAS_LOW_PRIMO;
       m_PrimoEtching=false;
       m_Riparti=true;
       m_Repit=0;
       m_IsGoToStep=true;
       m_ContrappostiIsErrore=false;
   }


   return m_Reset;
}


bool CBias::StendBy()
{
   if(!Enabled | m_StendBy )
		return true;

   SetV->StendBy();   

   m_StendBy=true;

   return m_StendBy;
}

bool CBias::ResetStendBy()
{
   if(!Enabled || !m_StendBy )
		return true;

   SetV->ResetStendBy();   
   m_StendBy=false;

   return true;
}


bool CBias::ResetSegnali()
{
    if(!Enabled)
		return false;

	if(m_ResetSegnali)
      return m_ResetSegnali;

   m_ResetSegnali=SetV->ResetSegnali();
   if(m_ResetSegnali){
      m_ResetSegnali=true;
 	  m_Set=false;
	  m_Reset=false;
	  m_ActStep=0;
	  m_ArcIsOn=false;
   }
   
   return m_ResetSegnali;
}

bool CBias::IsSchermiChiusi(){

    bool mIsSchermiChiusi=false;
    for(int i=0;i<SorgenteCount;i++){
        mIsSchermiChiusi=__max(mIsSchermiChiusi,FalseTrue(Sorgente[i].Schermo->Segnale()));
    }
    return mIsSchermiChiusi;

}

void  CBias::ArcIsOn(bool NewArcIsOn) 
{ 
	m_ArcIsOn=NewArcIsOn;
}


BYTE CBias::BiasLowCount(){

    return m_BiasOffTime.PercentOffTime();
}


bool CBias::Errore()
{
  //#ifdef  NO_ERRORE
//    return false;
//#endif

    if(!Enabled )
		return false;

    pErrore.Errore(m_ContrappostiIsErrore,E_BIAS_CATODI_SPENTI,E_TIME);

    if( PowOn->Segnale()){
       pErrore.Errore(PowErr->Segnale(),E_BIAS_TERMICO_POWER,E_TIME);
       pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_BIAS_RELE_POWER,E_TIME);
    }else{
       pErrore.Errore(false,E_BIAS_TERMICO_POWER,E_TIME);
       pErrore.Errore(false,E_BIAS_RELE_POWER,E_TIME);
    }

    m_GetVBias=GetV->Segnale();
    m_SetVBias=SetV->Segnale();

    if(m_SetVBias & (m_GetVBias<=BIAS_GETV_IS_ZERO))
       pErrore.Errore(true,E_BIAS_GETV_IS_ZERO,E_TIME);
    else
       pErrore.Errore(false,E_BIAS_GETV_IS_ZERO,E_TIME);

    if(m_CTipoStep==CTipoStep::Etching){
       if(!m_ArcIsOn){
          if(m_SetVBias & !IsSchermiChiusi())
             pErrore.Errore(Confronto(m_SetVBias*0.1, m_GetVBias),E_BIAS_CORTO_PRESET,E_TIME);
          else
             pErrore.Errore(false,E_BIAS_CORTO_PRESET,E_TIME);
       }else{
           if(m_GetVBias<(WORD)m_SetVBias*0.10){
              if(m_SetVBias)
                pErrore.Errore(true,E_BIAS_CORTO_ETCHING,E_TIME);
              else
                pErrore.Errore(false,E_BIAS_CORTO_ETCHING,E_TIME);
           }else{
              pErrore.Errore(false,E_BIAS_CORTO_ETCHING,E_TIME);
              if(m_GetVBias<(WORD)m_SetVBias*0.7){
                 m_BiasOffTime.Timer();
                 if(m_PercentOffTime!=m_BiasOffTime.PercentOffTime() & Debug)
                    MostraEvento( " m_BiasOffTime " + intToString(m_BiasOffTime.PercentOffTime() ),Color::FG_DEFAULT);
                 m_PercentOffTime=m_BiasOffTime.PercentOffTime();
              }
              else{
                 m_BiasOffTime.StendBy();
                 //pErrore.Errore(Confronto(0.2,GetI->Segnale() ),E_BIAS_NO_CORRENTE,E_TIME);
              }
           }
           pErrore.Errore(Confronto(m_BiasOffTime.PercentOffTime(),m_PerOffTime ),E_BIAS_ETCHING_TOO_LOW,E_TIME);
           pErrore.Errore(Confronto(0,m_BiasLowCount),E_BIAS_TROPPI_LOW_V,E_TIME);
       }
    }else if(m_CTipoStep==CTipoStep::Coating){
     /*  if(!m_ArcIsOn){
		 pErrore.Reset();
       }*/
	   if(m_ArcIsOn){
         pErrore.Errore(Confronto(m_SetVBias*0.5,m_GetVBias ),E_BIAS_CORTO_COATING,E_TIME);
         pErrore.Errore(Confronto(0,GetI->Segnale() ),E_BIAS_NO_CORRENTE,E_TIME);
        }
	}else{
       if(m_SetVBias>(BIAS_TEST*0.8)/(SetV->Fattore()))
           pErrore.Errore(Confronto(m_SetVBias*0.8,m_GetVBias ),E_BIAS_CORTO_PRESET,E_TIME);
       else
           pErrore.Errore(false,E_BIAS_CORTO_PRESET,E_TIME);
    }


    m_Errore=pErrore.Errore();

   /* if(m_Errore){
       if(pErrore.IsErrore(E_BIAS_TERMICO_POWER) | pErrore.IsErrore(E_BIAS_RELE_POWER) | pErrore.IsErrore(E_BIAS_RELE_POWER) | pErrore.IsErrore(E_BIAS_CORTO_ETCHING)){
           m_Riparti=false;
       }else
           m_Riparti=true;
    }
	*/

    return m_Errore;
}

bool CBias::Set_MDXII_Tap(WORD New_Info){

    if(m_Info==New_Info) {
      m_IsSetTap=false;
      return true;
    }

    m_SetTap=false;
    if(!PowIsOn->Segnale()){
       m_IsSetTap=false;
       *PowOn=true;
       return m_SetTap;
    }

    m_IsSetTap=true;
    if(!m_ResetTap){
       bool IsOff=false;
       IsOff=__max(IsOff,FalseTrue( SetV->Set(0)));
       *LVolt=false;
       IsOff=__max(IsOff,LVolt->Segnale());
       *HVolt=false;
       IsOff=__max(IsOff,HVolt->Segnale());
       *On=false;
       IsOff=__max(IsOff,On->Segnale());
       m_Info=10;
       if(!IsOff){
          m_ResetTap=true;
          m_Timer.Reset();
       }
       return m_SetTap;
    }


    //qDebug() << "SET TAP";
    if(!m_Timer.Timer())
        return m_SetTap;

    if(New_Info==1){
      *HVolt=true;
    }else if(New_Info==0){
      *LVolt=true;
    }

    if(!On->Set(true))
       return false;

    if(OnEnable->Segnale() ){
       m_Timer.Reset();
       m_Info=New_Info;
       m_SetTap=true;
       m_IsSetTap=false;
       m_ResetTap=false;
       if(m_Info==1)
          MostraEvento("SET pImpianto->Bias TAP ALTO",Color::FG_DEFAULT);
       else
          MostraEvento("SET pImpianto->Bias TAP BASSO",Color::FG_DEFAULT);
    }

    return m_SetTap;
}

bool CBias::Set_BP5_Tap(WORD New_Info){

    if(m_Info==New_Info) {
      m_IsSetTap=false;
      return true;
    }

    m_SetTap=false;
    if(!PowIsOn->Segnale()){
       m_IsSetTap=false;
       *PowOn=true;
       return m_SetTap;
    }

    m_IsSetTap=true;
    if(!m_ResetTap){
       bool IsOff=false;
       IsOff=__max(IsOff,FalseTrue( SetV->Set(0)));
       *LVolt=false;
       IsOff=__max(IsOff,LVolt->Segnale());
       *HVolt=false;
       IsOff=__max(IsOff,HVolt->Segnale());
       *On=false;
       IsOff=__max(IsOff,On->Segnale());
       m_Info=10;
       if(!IsOff){
          m_ResetTap=true;
          m_Timer.Reset();
       }
       return m_SetTap;
    }

    if(!m_Timer.Timer())
        return m_SetTap;

    if(New_Info==1){
      if(!HVolt->Set(true))
         return false;
    }else if(New_Info==0){
      if(!LVolt->Set(true))
         return false;
    }

    if(!On->Set(true))
       return false;

    if(IsHVolt->Segnale() | IsLVolt->Segnale()){
       m_IsSetTap=false;
       m_ResetTap=false;
       m_Timer.Reset();
       m_Info=New_Info;
       m_SetTap=true;
       if(m_Info==1)
          MostraEvento("SET pImpianto->Bias TAP ALTO",Color::FG_DEFAULT);
       else
          MostraEvento("SET pImpianto->Bias TAP BASSO",Color::FG_DEFAULT);
    }

    return m_SetTap;
}

double CBias::DSegnale(CIOSegnale  *pSegnale){

    double dSegnale=pSegnale->Segnale();
    dSegnale*=pSegnale->Fattore();
    if(m_Bias==BP5_BIAS){
        if( !IsHVolt->Segnale())
           dSegnale*=pSegnale->SecondoFattore();
    }


    return dSegnale;
}

int CBias::Segnale(CIOSegnale  *pSegnale){

    bool AltoVoltaggio;
    double dSegnale=pSegnale->DSegnale();
    if(dSegnale>400 )
        AltoVoltaggio=true;
    else
        AltoVoltaggio=false;
    dSegnale/=pSegnale->Fattore();
    if(m_Bias==BP5_BIAS){
        if(!AltoVoltaggio)
           dSegnale/=pSegnale->SecondoFattore();
    }

    return (int)dSegnale;
}

int CBias::Segnale(double dSegnale){

    bool AltoVoltaggio;
    if(dSegnale>400 )
        AltoVoltaggio=true;
    else
        AltoVoltaggio=false;
    dSegnale/=SetV->Fattore();
    if(m_Bias==BP5_BIAS){
        if(!AltoVoltaggio)
           dSegnale/=SetV->SecondoFattore();
    }

    return (int)dSegnale;
}

bool CBias::SetTap(WORD New_Info)
{

    if(m_Bias==BP5_BIAS){
        Set_BP5_Tap(New_Info);
    }else if(m_Bias==MDXII_BIAS){
        Set_MDXII_Tap(New_Info);
    }

    return m_SetTap;
}


WORD CBias::OnTime()
{
   return 5000;
}
