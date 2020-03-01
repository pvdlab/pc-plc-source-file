#include "Penning.h"
#include "math.h"


CPenning::CPenning(CErrore  *pNewErrore)
{

   pErrore=pNewErrore;
   m_Set=false;
   m_Reset=true;
   m_IsStartErrore=false;
   m_InterlockErrore=false;
   m_ProcessoIsEnabled=false;
   m_ProcessoEnabledTimeOut=false;
   m_TimerInterlock(VAC_T_PRESSIONE_SET);
   m_TimerStart(VAC_T_PRESSIONE_START);
   m_TimerProStart(VAC_T_PRESSIONE_PRO_START);
   Penning=NULL;
   Pirani=NULL;
   Tipo=0;
   Interlock=NULL;
   PowOn=NULL;
   m_Pressione=VAC_PRESSIONE_MAX;

}


CPenning::~CPenning()
{

}


double  CPenning::Pressione(){

    if(Tipo==PENNING_PTR90){
       //*PowOn=true;
       m_Pressione=Pressione(Penning);
    }else{
        double mPirani=Pressione(Pirani);
        double mPennig=Pressione(Penning);
        if(getTipoApp()==CTipoApp::Server){
            if(mPirani<VAC_PENNING_SET_ON)
                *PowOn=true;
            else if(mPirani>VAC_PENNING_SET_OFF)
                *PowOn=false;
            if(mPirani<VAC_PENNING_SET){
                if(mPennig*0.02<mPirani)
                    m_Pressione=mPennig;
                else if(mPirani*0.02>mPennig)
                    m_Pressione=mPirani;
            }else
                m_Pressione=mPirani;

        }else{
            if(PowOn->Segnale()){
                m_Pressione=mPennig;
            }else{
                m_Pressione=mPirani;
            }
        }


    }

    setPenning(m_Pressione);

    return m_Pressione;

}

bool CPenning::Set()
{

   if(Errore())
       return m_Set;

   Pressione();

   if(!TurboStart)
       return m_Set;

   if(m_TimerStart.Timer()){
        if (m_Pressione>VAC_PRESSIONE_IS_START){
            m_IsStartErrore=true;
         }else{
            m_IsStartErrore=false;
         }
   }

   if(m_TimerInterlock.Timer() & !Interlock->Segnale()){
       m_InterlockErrore=true;
   }

   if (m_Pressione<VAC_PRESSIONE_SET){
       m_Set=Interlock->Set(true);
       m_InterlockErrore=false;
       m_Reset=false;
       m_Set=true;
       }
   else{
       if(!getMacchinaInVuoto())
           Interlock->Set(false);
       m_Set=false;
   }

   if(!TurboNormale->Segnale()){
       m_TimerProStart.Reset();
       return m_Set;
   }else{
       if(m_TimerProStart.Timer()){
          if(!m_ProcessoIsEnabled)
             m_ProcessoEnabledTimeOut=true;
       }else
          m_ProcessoEnabledTimeOut=false;
   }



   if(!getProcessoIsOn()){
       if(Pressione()<VAC_PRESSIONE_PRO_SET){
          if(!m_ProcessoIsEnabled )
              MostraEvento("Raggiunta pressione di Processo",Color::FG_RED);
          m_ProcessoIsEnabled=true;
       }else
          m_ProcessoIsEnabled=false;
   }

   return m_Set;
}

CIOSegnale  *CPenning::Segnale(){

    return Penning;
 /*   if(Tipo==PENNING_PTR90)

    else if(PENNING_PTR225){
       return Pirani;
    }else{
       return Penning;
    }*/
}

double CPenning::Pressione(CIOSegnale  *pSegnale){

    double volt=pSegnale->Segnale();
    if(pSegnale->FondoScala())
      volt/=pSegnale->FondoScala();
    volt*=10;
    double exp=pSegnale->Fattore();
    exp*=volt;
    exp-=pSegnale->SecondoFattore();
    double DSegnale=pow(10,exp);
    //m_Pressione=DSegnale;
    return DSegnale;

}


bool CPenning::Reset()
{

   if (Segnale()->Segnale()>(WORD)VAC_PRESSIONE_SET){
       if(!getMacchinaInVuoto())
          m_Reset= Interlock->Set(false);
   }else
       m_Reset= false;

   m_Set=false;
   m_IsStartErrore=false;
   m_InterlockErrore=false;
   m_ProcessoIsEnabled=false;
   m_ProcessoEnabledTimeOut=false;
   m_TimerInterlock.Reset();
   m_TimerStart.Reset();
   m_TimerProStart.Reset();

   return m_Reset;
}


bool CPenning::SetOff()
{
    return Reset();
}


bool CPenning::Errore(){

    pErrore->Errore(m_IsStartErrore,E_PENNING_START,E_TIME);
    pErrore->Errore(m_InterlockErrore,E_INTERLOCK,E_TIME);
    pErrore->Errore(m_ProcessoEnabledTimeOut,E_PROCESS_START_ENABLED,E_TIME);


    if(TurboNormale->Segnale())
       pErrore->Errore(Confronto(m_Pressione,(VAC_PRESSIONE_ALLARME)),E_PRESSIONE_ALTA,E_TIME);
    else
       pErrore->Errore(false,E_PRESSIONE_ALTA,E_TIME);

    if(m_Pressione==0)
       pErrore->Errore(true,E_NESSUN_SENSORE,E_TIME);
    else
       pErrore->Errore(false,E_NESSUN_SENSORE,E_TIME);

    return pErrore->Errore();
}
