#include "Baratron.h"

CBaratron::CBaratron()
{
  m_Set=false;
  m_Reset=true;
  Valvola=NULL;
  Segnale=NULL;
  Pirani=NULL;

}



CBaratron::~CBaratron()
{

}


double CBaratron::Pressione(){

    double m_Pressione=Segnale->Segnale();
    m_Pressione*=Segnale->Fattore();
    return m_Pressione;

}



bool CBaratron::Set(WORD NewPressione)
{

    if(NewPressione<=VAC_BARATRON_SET){
       if(Enabled)
         *Valvola=true;
       m_Set=true;
       m_Reset=false;
   }

   return m_Set;
}

void CBaratron::Ventila()
{
    if(Enabled)
        *Valvola=false;
}


bool CBaratron::Reset(WORD NewPressione)
{

   if(NewPressione>VAC_BARATRON_SET){
       if(Enabled)
         *Valvola=false;
       m_Set=true;
       m_Reset=false;
   }

   return m_Reset;
}

bool CBaratron::Errore()
{
  /* if(Enabled){
     if(m_Set && Segnale->Segnale()>(WORD)VAC_PRESSIONE_ERRORE)
       return true;
     else
       return false;
   }
   else{
     if(m_Set && Pirani->Segnale()>(WORD)VAC_PRESSIONE_ERRORE)
       return true;
     else
       return false;
   }*/


   return false;

}

bool CBaratron::SetOff()
{
    if(Enabled)
      *Valvola=false;
    m_Reset=true;
    m_Set=false;

    return true;
}
