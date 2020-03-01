#include "Resistenza.h"
#include <QDebug>



CResistenza::CResistenza()
{
    m_Set=false;
    m_Reset=true;
    m_PreSet=false;
    m_ResetSegnali=false;
    m_Off=false;
    m_Errore=false;
    m_PiraniAllarm=false;
    m_Temp_Set=RES_MAX_TEMP;
    m_TimerSet(RES_TIME_SET);  //RES_TIME_SET
    m_NonScalda=false;
    m_Temp_CameraRotta=false;
    m_Temp_TurboRotta=false;
    m_Temp_Resistenza=0;
    Debug=false;

    ReleDx=NULL;
    ReleSx=NULL;
    PowOn=NULL;
    PowErr=NULL;
    PowIsOn=NULL;
    Temp_Resistenza=NULL;
    Temp_Camera=NULL;
    Temp_Acqua=NULL;
    Temp_Turbo=NULL;
    AzotoValv=NULL;
    ArgonValv=NULL;
    AcetiValv=NULL;
    Azoto=NULL;
    Argon=NULL;
    Aceti=NULL;
    ManVuoto=NULL;

    pErrore.append(E_RES_RELE_POWER,"E_RES_RELE_POWER");
    pErrore.append(E_RES_TERMICO_POWER,"E_RES_TERMICO_POWER");
    pErrore.append(E_RES_TEMP_TURBO,"E_RES_TEMP_TURBO");
    pErrore.append(E_RES_TEMP_CAMERA,"E_RES_TEMP_CAMERA");
    pErrore.append(E_RES_PIRANI_ALLARM,"E_RES_PIRANI_ALLARM");
    pErrore.append(E_RES_NEXT_SETP,"E_RES_NEXT_SETP");
    pErrore.append(E_RES_TEMP,"E_RES_TEMP");
    pErrore.append(E_RES_TERMOCOPPIA,"E_RES_TERMOCOPPIA");
    pErrore.append(E_RES_TEMP_ACQUA,"E_RES_TEMP_ACQUA");
    pErrore.append(E_RES_NON_SCALDA,"E_RES_NON_SCALDA");
    pErrore.append(E_RES_ALTA_TEMPERATURA,"E_RES_ALTA_TEMPERATURA");

}



bool CResistenza::PreSet()
{

  if(!Enabled)
      return true;

  return m_PreSet;


}

void CResistenza::ceckNextStepError(){

    pErrore.Errore(FalseTrue(m_IsGoToStep),E_RES_NEXT_SETP,0);
}

void CResistenza::ceckPowerError(){

    if(PowOn->Segnale()){
        pErrore.Errore(PowErr->Segnale(),E_RES_TERMICO_POWER,0);
        pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_RES_RELE_POWER,0);
    }else{
        pErrore.Errore(PowErr->Segnale(),E_RES_TERMICO_POWER,0);
        pErrore.Errore(PowIsOn->Segnale(),E_RES_RELE_POWER,0);
    }

}

bool CResistenza::GoToStep(WORD NewPotenza,WORD NewTemp,WORD NewPressione)
{
   double Tempo;

   if(!Enabled)
       return true;

   m_Pressione=NewPressione;

   m_IsGoToStep=true;
   m_IsGoToStep=__min(m_IsGoToStep,ReleDx->Set(false));
   m_IsGoToStep=__min(m_IsGoToStep,ReleDx->Set(false));


   if(NewPotenza>100)
      m_Potenza=100;
   m_Potenza=NewPotenza;

   if(m_Potenza>0)
       m_IsGoToStep=__min(m_IsGoToStep,PowOn->Set(true));
   else
       m_IsGoToStep=__min(m_IsGoToStep,PowOn->Set(false));

   if(NewTemp>RES_MAX_TEMP)
      m_Temp_Set=RES_MAX_TEMP;
   m_Temp_Set=NewTemp;


   Tempo=(double)m_Potenza;
   Tempo/=100;
   Tempo*=((double)RES_T_ONOFF_RES);

   m_TimerOn((WORD)Tempo,(WORD)Tempo);
   m_TimerOff(RES_T_ONOFF_RES-(WORD)Tempo,RES_T_ONOFF_RES-(WORD)Tempo);
   m_TimerOff.Reset();
   m_Off=false;

   m_Temp_Resistenza=Temp_Resistenza->Segnale();
   m_TimerSet.Reset();

   return m_IsGoToStep;

}

bool CResistenza::SetOn()
{
    return PowOn->Set(true);
}

bool   CResistenza::IsOn()
{
    return PowIsOn->Segnale();
}


bool CResistenza::ManualeRun(){

    PowOn->Set(true);
    return Set(CTipoStep::Nessuno);

}

bool CResistenza::Set(BYTE   TipoStep)
{
    if(!Enabled )
        return true;


    if(Errore() | !Termostato->Segnale() | getAcquaErrore()){
      *ReleDx=false;
      *ReleSx=false;
      *PowOn=false;
      return false;
    }

    if(TipoStep==CTipoStep::Riscaldo){
       *Gas=true;
       *ArgonValv=true;
       *AzotoValv=true;
       *Argon=0;
       *Azoto=0;
    }

    if(getPenning()>VAC_RESISCALDO_ALLARM){
       std::string str="Pirani ";
       str.append(doubleToString(getPenning()));
       MostraEvento( str,Color::FG_RED);
       m_PiraniAllarm=true;
    }
    else
       m_PiraniAllarm=false;

    if(m_Potenza){
        if(m_TimerSet.Timer()){
           if(Temp_Resistenza->Segnale()<(m_Temp_Set*0.8)){
               if(Temp_Resistenza->Segnale()*0.90<m_Temp_Resistenza){
                   m_NonScalda=true;
                   qDebug() << "m_NonScalda vero";
                   qDebug() <<  QString::number(Temp_Resistenza->Segnale())
                            <<  QString::number(Temp_Resistenza->Segnale()*0.90)
                            <<  QString::number(m_Temp_Resistenza) ;
               }else{
                   m_NonScalda=false;
               }
           }else{
               m_NonScalda=false;
           }
           m_Temp_Resistenza=Temp_Resistenza->Segnale();
           qDebug() << "m_Temp_Resistenza" <<   QString::number(m_Temp_Resistenza) ;
           if(Temp_Resistenza->Segnale()>(m_Temp_Set*0.6)){
               if(Temp_Camera->Segnale()<Temp_Acqua->Segnale()){
                   m_Temp_CameraRotta=true;
                   qDebug() << "m_Temp_CameraRotta vero";
                   qDebug() << QString::number(Temp_Camera->Segnale())
                            << QString::number(Temp_Acqua->Segnale()) ;
               }else{
                   m_Temp_CameraRotta=false;
               }
           }else{
               m_Temp_CameraRotta=false;
           }
           m_TimerSet.Reset();
        }
    }

    if((Temp_Resistenza->Segnale()>m_Temp_Set) | !m_Potenza){
       *ReleDx=false;
       *ReleSx=false;
       m_TimerOn.Reset();
       m_Off=false;
       return true;
    }

    if(m_Potenza>97){
        PowOn->Set(true);
        *ReleDx=true;
        *ReleSx=true;
        m_TimerOn.Reset();
        m_Off=false;
    }else{
        if(m_TimerOn.Timer() && !m_Off){
           *ReleDx=false;
           *ReleSx=false;
           m_TimerOff.Reset();
           m_Off=true;

        }
        else if(m_TimerOff.Timer() && m_Off){
           *ReleDx=true;
           *ReleSx=true;
           m_TimerOn.Reset();
           m_Off=false;
        }
    }



    m_ResetSegnali=false;
    m_Reset=false;
    m_Set=true;

    return m_Set;
}


void CResistenza::ResetErrore() {

    pErrore.Reset();
    m_Errore=false ;
    m_PiraniAllarm=false;
    m_NonScalda=false;
    m_Temp_CameraRotta=false;
    m_Temp_TurboRotta=false;

}

bool CResistenza::Reset()
{

   m_Reset=true;
   m_Reset=__min(m_Reset, PowOn->Set(false));
   m_Reset=__min(m_Reset, ReleDx->Set(false));
   m_Reset=__min(m_Reset, ReleSx->Set(false));
   m_Reset=__min(m_Reset, ArgonValv->Set(false));
   m_Reset=__min(m_Reset, AzotoValv->Set(false));
   m_Reset=__min(m_Reset, AcetiValv->Set(false));

   if(m_Reset){
       m_PiraniAllarm=false;
       m_ResetSegnali=false;
       m_NonScalda=false;
       m_Temp_CameraRotta=false;
       m_Temp_TurboRotta=false;
       m_Temp_Resistenza=0;
       m_Errore=false;
       m_Potenza=0;
       m_Temp_Set=0;
       m_Set=false;
       m_PreSet=false;
   }

   return m_Reset;
}

bool CResistenza::SetOff()
{
   return Reset();
}

bool CResistenza::StendBy()
{
   *ReleDx=false;
   *ReleSx=false;
   return true;
}

bool CResistenza::ResetStendBy()
{
   return true;
}

bool CResistenza::ResetSegnali()
{
    if(!Enabled)
        return true;

    if(m_ResetSegnali)
      return m_ResetSegnali;

   *ReleDx=false;
   *ReleSx=false;
   if(m_ResetSegnali){
      m_ResetSegnali=true;
      m_Set=false;
      m_Reset=false;
   }

   return m_ResetSegnali;
}



bool CResistenza::Errore()
{

    if(!Enabled)
        return false;

    if(m_Potenza!=0 ){
        if(getSimula())
           pErrore.Errore(false,E_RES_NON_SCALDA,E_TIME);
        else
           pErrore.Errore(m_NonScalda,E_RES_NON_SCALDA,E_TIME);

       if(Temp_Resistenza->Segnale()>RES_MAX_TEMP)
            pErrore.Errore(true,E_RES_ALTA_TEMPERATURA,E_TIME);
        else
            pErrore.Errore(false,E_RES_ALTA_TEMPERATURA,E_TIME);
    }else{
        pErrore.Errore(false,E_RES_NON_SCALDA,E_TIME);
        pErrore.Errore(false,E_RES_ALTA_TEMPERATURA,E_TIME);
    }

    if(Temp_Acqua->Segnale()>RES_TEMP_ACQUA)
        pErrore.Errore(true,E_RES_TEMP_ACQUA,E_TIME);
    else
        pErrore.Errore(false,E_RES_TEMP_ACQUA,E_TIME);

    pErrore.Errore(m_PiraniAllarm,E_RES_PIRANI_ALLARM,E_TIME);

    if( PowOn->Segnale()){
       pErrore.Errore(PowErr->Segnale(),E_RES_TERMICO_POWER,E_TIME);
       pErrore.Errore(FalseTrue(PowIsOn->Segnale()),E_RES_RELE_POWER,E_TIME);
    }else{
       pErrore.Errore(false,E_RES_TERMICO_POWER,E_TIME);
       pErrore.Errore(false,E_RES_RELE_POWER,E_TIME);
    }

    if(m_NonScalda)
        m_Errore=pErrore.Errore();
    m_Errore=pErrore.Errore();


    return m_Errore;

}



