#include "StdAfx.h"
#include "Segnale.h"
#include "DataFile.h"
#include "math.h"
#include <QDebug>

CIOSegnale::CIOSegnale()
{

  m_Segnale=0;
  m_SetSegnale=0;
  m_IsStabile=false;
  m_Slot=0;
  m_Point=0;
  m_Enabled=false;
  m_FondoScala=1;
  m_Step=NULL;
  m_Set=false;
  m_Reset=false;
  m_ActStep=0;
  m_ActProStep=0;	
  m_Load=false;
  m_FalseTrue=false;
  m_Modulo=NULL;
  Zip=1;
  m_Regola=1;
  m_IsStart=false;
  GraficoIndex=0;
  Spedito=0;
  TipoComp=0;
  m_Fattore=1;
  m_DSegnale=0;
  m_SecondoFattore=1;
  char *pBuffer=m_Descrizione;
  memset(pBuffer,0,D_DESCRIZIONE);

#ifdef QT_APP
  PenWidth=2;
  PenColor=Qt::white;
  lastPoint.setX(0);
  lastPoint.setY(0);
#endif


}

CIOSegnale& CIOSegnale::operator()(BYTE  Slot,BYTE  Point,const char *newDescrizione,WORD FondoScala,BYTE Tipo,BYTE OffSet,CModulo *NewModulo)
{
    if(!this)
        return *this;

    if(newDescrizione!=NULL){
        char *pBuffer=m_Descrizione;
        int len=strlen(newDescrizione);
        if(len>D_DESCRIZIONE)
            len=D_DESCRIZIONE;
        memcpy(pBuffer,newDescrizione,len);
        m_Enabled=true;
    }else{
        m_Enabled=false;
    }


	m_TipoSlot=Tipo;
	m_Point=Point;
	m_Slot=Slot;
	if(FondoScala>1){ 
	  m_FondoScala=FondoScala;
	}
	else{ 
	  m_FondoScala=0;
	  m_FalseTrue=(bool)FondoScala;
	}
	m_StendBy=false;
	m_OffSet=OffSet;
    m_Modulo=NewModulo;
	
    if(m_TipoSlot==CTipoSlot::OutAnalogico  ){
		if(m_Step!=NULL)
		   delete [] m_Step;
		m_Step = new IoStepType[D_STEP];
		memset(m_Step,'\0',sizeof(IoStepType)*D_STEP);

	}

	return *this;
}

void CIOSegnale::ForzaToStep(){

    if(m_Step!=NULL)
       delete [] m_Step;
    m_Step = new IoStepType[D_STEP];
    memset(m_Step,'\0',sizeof(IoStepType)*D_STEP);

}

CIOSegnale& CIOSegnale::operator=(WORD NewSegnale)
{

    if(!this)
        return *this;

   if(!m_Enabled)
       return *this;

   getSegnale();

   if(m_Segnale==NewSegnale)
    	return *this;

   m_Segnale=NewSegnale;
   setSegnale();

   return *this;
}

CIOSegnale& CIOSegnale::operator*=(double_t val)
{
   if(!this | !m_Step | !m_Enabled)
       return *this;

   m_Segnale*=val;
   setSegnale();

   return *this;
}

CIOSegnale& CIOSegnale::operator+=(WORD step)
{
   if(!this)
       return *this;

   if(!m_Step)
	  return *this;

   if(!m_Enabled)
       return *this;

   m_Segnale+=step;
   setSegnale();

   return *this;
}

CIOSegnale& CIOSegnale::operator-=(WORD step)
{
    if(!this)
        return *this;

    if(!m_Enabled)
        return *this;

   if(!m_Step)
	  return *this;

   if (m_Segnale-step >= 0)
        m_Segnale-=step;
   else
       m_Segnale = 0;

   setSegnale();
  
   return *this;
}

double  CIOSegnale::DSegnale(){

    Segnale();
    return m_Segnale*m_Fattore;
}

int CIOSegnale::Segnale()
{

  if(!this)
      return 0;

  if(!m_Enabled)
      return m_Segnale;

  getSegnale();

  return m_Segnale;

}

bool CIOSegnale::isLetto(WORD tempo){

    if(!this)
        return false;

    if(!m_Enabled)
       return false;

    if(getSimula())
        return true;

    DWORD acttempo=GetTickCount();
    acttempo-=tempo;

    if(m_Modulo->systemTime>acttempo)
        return true;
    else
        return false;

}



int CIOSegnale::FSegnale(int New_FondoScala)
{
  if(!this)
     return 0;

  if(!m_Enabled)
     return 0;

  double DSegnale;

  getSegnale();
  if(m_Segnale){
    DSegnale=(double)m_Segnale;
    if(m_FondoScala)
      DSegnale/=m_FondoScala;
    DSegnale*=New_FondoScala;
    return (int)DSegnale;
  }

  return 0;

}


void CIOSegnale::setSegnale()
{
  if(!this)
      return ;

  if(m_Modulo==NULL)
      return;

  if(m_Enabled)
     m_Segnale=m_Modulo->setSegnale(m_Point,m_Segnale);

}

void CIOSegnale::getSegnale()
{
  if(!this)
       return ;

  if(!m_Enabled)
      return;

  if(m_Modulo==NULL)
      return;

  m_Segnale=m_Modulo->getSegnale(m_Point);
  
  if(m_FalseTrue){
	  m_Segnale=FalseTrue(m_Segnale);
  }

}


void CIOSegnale::Aggiornato(){

    if(!this)
         return ;

    for(int i=0; i<D_COMP_STEP;i++ ){
        m_Step[i].InAggiorna=false;
    }
}

void CIOSegnale::AzzeraStep(){

    for(int i=0;i<m_Step[m_ActProStep].StepCount;i++){
        m_Step[m_ActProStep].Step[i].Rampa=0;
        m_Step[m_ActProStep].Step[i].Start=0;
        m_Step[m_ActProStep].Step[i].Stop=0;
    }
}



bool CIOSegnale::AllungaZero(double incremento){

    if(m_Step[m_ActProStep].StepCount < 2 )
        return false;

    for(int i=0; i<m_Step[m_ActProStep].StepCount; i+=2){
        DWORD StepTempo=m_Step[m_ActProStep].Step[i].Rampa;
        StepTempo+=m_Step[m_ActProStep].Step[i+1].Rampa;
        if(m_Step[m_ActProStep].Step[i].Start!=0){
            double StopRampa=m_Step[m_ActProStep].Step[i].Rampa;
            StopRampa*=(incremento);
            if(0.8*StopRampa>StepTempo)//SE È È IL 20% IN PIU NONVA BENE
                return false;
            m_Step[m_ActProStep].Step[i].Rampa=(int)__min(StopRampa,StepTempo);
            m_Step[m_ActProStep].Step[i+1].Rampa=(int)__min(0,abs(StepTempo-StopRampa));
        }else if(m_Step[m_ActProStep].Step[i+1].Start!=0){
            double StopRampa=m_Step[m_ActProStep].Step[i+1].Rampa;
            StopRampa*=(incremento);
            if(0.8*StopRampa>StepTempo)
                return false;
            m_Step[m_ActProStep].Step[i].Rampa=(int)__min(StopRampa,StepTempo);
            m_Step[m_ActProStep].Step[i+1].Rampa=(int)__min(0,abs(StepTempo-StopRampa));
            m_Step[m_ActProStep].Step[i].Start=m_Step[m_ActProStep].Step[i+1].Start;
            m_Step[m_ActProStep].Step[i].Stop=m_Step[m_ActProStep].Step[i+1].Stop;
            m_Step[m_ActProStep].Step[i+1].Start=0;
            m_Step[m_ActProStep].Step[i+1].Stop=0;
        }
    }

    return true;
}


void CIOSegnale::Allunga(double incremento,DWORD DaTempo){

    if(m_Step[m_ActProStep].StepCount==0)
        return;

    for(int i=0;i<m_ActStep;i++){
        DaTempo-=m_Step[m_ActProStep].Step[i].Rampa;
    }

    double StopRampa=m_Step[m_ActProStep].Step[m_ActStep].Rampa;
    StopRampa-=DaTempo;
    StopRampa*=(incremento);
    StopRampa+=DaTempo;
    m_Step[m_ActProStep].Step[m_ActStep].Rampa=(int)StopRampa;
    m_Timer.TimeStop(StopRampa);

    for(int i=m_ActStep+1;i<m_Step[m_ActProStep].StepCount;i++){
        if(m_Step[m_ActProStep].Step[i].Rampa!=0){
           StopRampa=m_Step[m_ActProStep].Step[i].Rampa;
           StopRampa*=(incremento);
           m_Step[m_ActProStep].Step[i].Rampa=(int)StopRampa;
        }
    }

}

DWORD CIOSegnale::Load(WORD Inf,WORD ProStep,WORD Step,WORD Start,WORD Stop,DWORD Rampa)
{
  if(!this)
     return 0;

  if(!m_Enabled)
     return 0;

  DWORD Old_Time;
  WORD New_Segnale;

  if((Step>D_COMP_STEP) | (ProStep>D_STEP) |  !m_Step)
     return 0;

  m_Load=true;

  m_Step[ProStep].Step[Step].Inf=Inf;
  m_Step[ProStep].Step[Step].Start=Start;
  m_Step[ProStep].Step[Step].Stop=Stop;

  if(m_Timer.IsAttivo()){
    if(m_ActProStep==ProStep && m_ActStep==Step){
      Old_Time=m_Timer.Time();
      m_Step[ProStep].Step[Step].Rampa=Rampa*1000;
	  SetStepTempo();
	  m_Timer.Time(Old_Time);
      if(Start==Stop){
         m_SetSegnale=Start;
	     *this=Start;
      }else if(Start<Stop){
         New_Segnale=(Stop-Start);
	     New_Segnale*=(WORD)((double)Old_Time/(double)m_Timer.TimeStop());
         m_SetSegnale=Start+New_Segnale;
         *this=Start+New_Segnale;
	  }
      else{
         New_Segnale=(Stop-Start);
	     New_Segnale*=(WORD)((double)Old_Time/(double)m_Timer.TimeStop());
         m_SetSegnale=Start-New_Segnale;
         *this=Start-New_Segnale;
	  }

	}
	else
     m_Step[ProStep].Step[Step].Rampa=Rampa*1000;
  }
  else{
    m_Step[ProStep].Step[Step].Rampa=Rampa*1000;
    if(m_ActProStep==ProStep && m_ActStep==Step){
        m_SetSegnale=Start;
        *this=Start;
    }
  }

  if(m_Step[ProStep].StepCount<Step)
     m_Step[ProStep].StepCount=Step;

  m_Step[ProStep].Attivo=true;
  m_Step[ProStep].StepCount=Step+1;

  return  m_Step[ProStep].Step[Step].Rampa;
}




bool CIOSegnale::reLoad(WORD Inf,WORD ProStep,WORD Step,WORD Start,WORD Stop,DWORD Rampa)
{
  if(!this)
      return false;

  bool retval=false;

  if(!m_Enabled)
     return retval;

  if((Step>D_COMP_STEP) | (ProStep>D_STEP) |  !m_Step)
     return false;

  m_Step[ProStep].Step[Step].Inf=Inf;
  if(m_Step[ProStep].Step[Step].Start!=Start)
      retval=true;
  m_Step[ProStep].Step[Step].Start=Start;
  if(m_Step[ProStep].Step[Step].Stop!=Stop)
      retval=true;
  m_Step[ProStep].Step[Step].Stop=Stop;

  if(m_Step[ProStep].Step[Step].Rampa/1000!=Rampa)
      retval=true;
  m_Step[ProStep].Step[Step].Rampa=Rampa*1000;

  return retval;
}

DWORD CIOSegnale::Rampa(WORD ProStep,WORD Step){

    if(!this)
         return 0;

    if(!m_Enabled)
       return 0;

    if((ProStep>D_STEP) || (Step>D_COMP_STEP))
        return 0;

    return m_Step[ProStep].Step[Step].Rampa/1000;
}

DWORD CIOSegnale::reLoad(WORD ProStep,WORD Step,DWORD Rampa)
{
  if(!this)
       return 0;

  if(!m_Enabled)
     return Rampa;

  if((Step>D_COMP_STEP) | (ProStep>D_STEP) |  !m_Step)
     return Rampa;

  m_Step[ProStep].Step[Step].Rampa=Rampa*1000;

  return Rampa;
}



void CIOSegnale::PrintStep()
{

    if(!this)
         return ;

    if(!m_Enabled)
       return ;


    if(m_Step){
     for(int i=0;i<D_STEP;i++){
        PrintStep(i);
      }
  }

}

int CIOSegnale::getNoZeroStepCount(){

    if(!this)
       return 0;

    if(!m_Enabled)
       return 0;

    int count=0;

    if(!m_Step)
        return 0;

    for(int i=0;i< D_COMP_STEP; i++){
        count+=m_Step[i].StepCount;
    }

    return count;
}

void CIOSegnale::PrintStep(int step)
{

    if(!this)
         return ;

    if(!m_Enabled)
       return ;

    if(m_Step){
        bool IsNull=true;
        for(int i=0;i< m_Step[step].StepCount; i++){
            if(m_Step[step].Step[i].Rampa!=0){ //m_Step[step].Step[i].Start!=0
                IsNull=false;
                break;
            }
        }
        if(!IsNull){
           for(int i=0;i< m_Step[step].StepCount; i++){
               std::string str;
               str.append(m_Descrizione);
               str.append(" ");
               str.append(intToString(step));
               str.append(" Da ");
               str.append(intToString(m_Step[step].Step[i].Start));
               str.append(" A ");
               str.append(intToString(m_Step[step].Step[i].Stop));
               str.append(" Rampa ");
               str.append(intToString(m_Step[step].Step[i].Rampa));
               str.append(" Info ");
               str.append(intToString(m_Step[step].Step[i].Inf));
               MostraEvento(str,Color::FG_DEFAULT);
           }
        }


  }

}

void CIOSegnale::append(WORD ProStep)
{
  if(!this)
         return ;

  if(!m_Enabled)
       return ;

  if((ProStep>D_STEP) )
      return ;

  if(m_Step[ProStep].StepCount+1>D_COMP_STEP )
       return ;

  int Step=m_Step[ProStep].StepCount;

  m_Step[ProStep].Step[Step+1].Rampa=m_Step[ProStep].Step[Step].Rampa;
  m_Step[ProStep].Step[Step+1].Start =m_Step[ProStep].Step[Step].Start;
  m_Step[ProStep].Step[Step+1].Stop =m_Step[ProStep].Step[Step].Stop;
  m_Step[ProStep].Step[Step+1].Inf =m_Step[ProStep].Step[Step].Inf;

  m_Step[ProStep].StepCount=Step+1;

}

void CIOSegnale::UnLoad(int StepIndex)
{

  if(!this)
       return ;

  if(!m_Enabled)
     return ;
  
  if(m_Step){
   for(int i=StepIndex;i<D_STEP;i++){
	  m_Step[i].Attivo=false;
      m_Step[i].StepCount=0;
      m_Step[i].InAggiorna=true;
      for(int j=0;j<D_COMP_STEP;j++){
		m_Step[i].Step[j].Inf=0;
		m_Step[i].Step[j].ProStep=0;
		m_Step[i].Step[j].Rampa=0;
		m_Step[i].Step[j].Start=0;
		m_Step[i].Step[j].Step=0;
		m_Step[i].Step[j].Stop=0;
	 }
   }
   m_ActProStep=StepIndex;
  }
  m_Load=false;

}


void CIOSegnale::StendBy()
{  
  if(!this)
     return ;

  if(!m_Enabled)
     return ;

  if(m_Step){
	if(!m_StendBy){
      m_StendBySegnale=m_Segnale;
	  *this=0;
	  m_Segnale=0;
	  m_Timer.StendBy();
	  m_StendBy=true;
	}
  }
}

void CIOSegnale::ResetStendBy()
{  
   if(!this)
         return ;

   if(!m_Enabled)
      return ;

   if(m_Step){
	if(m_StendBy){
	  *this=m_StendBySegnale;
	  m_Timer.ResetStendBy();
	  m_StendBy=false;
	}
   }

}

bool CIOSegnale::Enabled(){

    if(!this)
         return false;

    return m_Enabled;
}

bool CIOSegnale::ResetSegnali()
{
  if(!this)
       return false;

  if(!m_Enabled)
       return false;

  if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
      return true;

  m_IsStabile=false;
  m_SetSegnale=0;
  if(m_Step){
    m_Set=false;
    m_ActStep=0;	
    SetStepTempo();
     *this=m_Step[m_ActProStep].Step[m_ActStep].Start;
  }
  return true;

}


bool CIOSegnale::Reset()
{

  if(!this | !m_Enabled)
      return false;

  m_IsStabile=false;
  m_SetSegnale=0;
  if(m_Step){
    m_Reset=true;
    m_Reset=Set(0);
    m_StendBy=false;
    m_ActStep=0;
    m_ActProStep=0;
    SetStepTempo();
    m_Timer.Reset();
    m_Set=false;
    return m_Reset;
  }

  return true;

}


int  CIOSegnale::InfoGotoStep(int Step)
{
    if(!this)
        return -1;

    if(!m_Enabled)
       return -1;

    if(Step>D_STEP)
        return -1;

    if(m_Step!=NULL)
        return m_Step[Step].Step[0].Inf;
    else
        return -1;
}

int  CIOSegnale::Info()
{ 
    if(!this)
        return -1;

    if(!m_Enabled)
       return -1;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return -1;

	if(m_Step!=NULL) 
		return m_Step[m_ActProStep].Step[m_ActStep].Inf; 
    else
        return -1;
}



bool  CIOSegnale::GoToStep(WORD Step)
{
    if(!this)
        return false;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP) | !m_Step | !m_Enabled)
        return false;

    m_Set=false;
    m_ActStep=0;
    if((m_ActProStep==Step) & m_IsStabile ){
        *this=m_SetSegnale;

    }else{
        *this=m_Step[Step].Step[m_ActStep].Start;
        m_SetSegnale= m_Step[Step].Step[m_ActStep].Start;
    }
    m_ActProStep=Step;
    m_IsStart=false;
    SetStepTempo();

    return Set(m_SetSegnale);
}


bool  CIOSegnale::GoToStep(WORD Step,DWORD Tempo)
{ 
    if(!this)
        return false;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP) | !m_Step | !m_Enabled)
        return false;

	DWORD MTempo=0;

    m_Set=false;
    if((m_ActProStep==Step) & m_IsStabile & (m_SetSegnale!=0)){
        *this=m_SetSegnale;
    }else{
        *this=m_Step[Step].Step[m_ActStep].Start;
        m_SetSegnale= m_Step[Step].Step[m_ActStep].Start;
    }
    m_ActProStep=Step;
    for(int i=0;i<m_Step[m_ActProStep].StepCount;i++){
      if(MTempo+m_Step[m_ActProStep].Step[i].Rampa>=Tempo){
          m_ActStep=i;
          break;
      }
      MTempo+=m_Step[m_ActProStep].Step[i].Rampa;
    }
    SetStepTempo();
    int Dt=(int)Tempo-(int)MTempo;
    if(Dt>=0)
      m_Timer.Time(Dt);
    m_IsStart=false;

    return Set(m_SetSegnale);
}



bool  CIOSegnale::IsRampa() 
{ 
    if(!this)
        return false;

    if(!m_Enabled)
       return false;

    if(!m_Step)
	  return true;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return false;

    if(m_Step[m_ActProStep].Step[m_ActStep].Stop != m_Step[m_ActProStep].Step[m_ActStep].Start)
	    return true;
	else
		return false;
}

bool  CIOSegnale::IsZero(WORD Step) 
{ 
    if(!this)
        return true;

    if(!m_Step)
	  return true;

    if((Step>D_STEP) )
        return false;

	if(!m_Step[Step].Step[0].Stop && !m_Step[Step].Step[0].Start)
	    return false;
	else
		return true;
}


IoStepType   CIOSegnale::Step(){

    if(this){
        if((m_ActProStep<D_STEP) )
            return m_Step[m_ActProStep];
    }


}

WORD  CIOSegnale::StepCount(){

    if(!this)
        return 0;

    if(!m_Step)
        return 0;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return 0;

    return m_Step[m_ActProStep].StepCount;

}

WORD  CIOSegnale::StepCount(int Step){

    if(!this)
        return 0;

    if(!m_Step)
        return 0;

    if((Step>D_STEP) | (Step >D_COMP_STEP))
        return 0;

    return m_Step[Step].StepCount;

}

bool CIOSegnale::isStepNull(WORD index){

    if(!this)
        return true;

    bool isnull=true;
    for(int i=0; i< D_COMP_STEP;i++){
        if(m_Step[index].Step[i].Start | m_Step[index].Step[i].Stop){
            isnull=false;
            break;
        }
    }

    return isnull;
}

std::string oldstr;
#define PRED_SLOT 4
#define PRED_POINT 2
#define MOSTRA_SEGNALE_MSG






void CIOSegnale::SetStabile(){


    if(this)
      m_IsStabile=true;

    if(m_Step[m_ActProStep].Step[m_ActStep].Start!=0){
        double newRegola=m_SetSegnale;
        newRegola/=m_Step[m_ActProStep].Step[m_ActStep].Start;
        if(m_Regola!=newRegola){
            QString str=m_Descrizione;
            qDebug() << str + " SetStabile  Segnale = " <<  QString::number(m_SetSegnale)
                     << " m_Regola " << QString::number(m_Regola,'f',4);
            m_Regola=newRegola;
        }
    }



}



bool CIOSegnale::IsPrimoStep(){

    if(!this)
        return false;

    bool primo=false;

    if(!m_Step)
        return false;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return false;

    if(m_ActProStep>0){
       if((m_Step[m_ActProStep].StepCount!=0) & (m_Step[m_ActProStep-1].StepCount==0))
           primo=true;
    }


    return primo;


}


int CIOSegnale::Get(){

    if(!this)
        return 0;

    if(!m_Step)
        return 0 ;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return 0;

    return m_Step[m_ActProStep].Step[m_ActStep].Start;
}

bool CIOSegnale::SetStart( ){

    if(!this)
        return true;

    m_SetSegnale=m_Step[m_ActProStep].Step[m_ActStep].Start;
    *this=m_Step[m_ActProStep].Step[m_ActStep].Start;

    if(m_Modulo->getSegnale(m_Point)!=m_Step[m_ActProStep].Step[m_ActStep].Start)
         return false;


    return true;

}

bool CIOSegnale::Set( int  newSegnale){

    if(!this)
        return true;

    m_SetSegnale=newSegnale;
    *this=newSegnale;

    if(m_Modulo->getSegnale(m_Point)!=newSegnale)
         return false;


    return true;

}

bool CIOSegnale::IsStart(){

    if(!this | !m_Step | m_IsStart)
        return true;

    m_IsStart=Set(m_Step[m_ActProStep].Step[m_ActStep].Start);

    return m_IsStart;

}


bool CIOSegnale::Set()
{
    WORD Step;

    if(!this)
        return true;

    if(!m_Step)
      return true;

    m_Reset=false;

    if(m_Set )
      return m_Set;

    if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
        return m_Set;

    if(m_Timer.IsStepTimer()){
      if((Step=(WORD)m_Timer.TimerStep())) {
        if (m_Step[m_ActProStep].Step[m_ActStep].Stop>m_Step[m_ActProStep].Step[m_ActStep].Start)
            m_SetSegnale+=Step;
        else if (m_Step[m_ActProStep].Step[m_ActStep].Stop<m_Step[m_ActProStep].Step[m_ActStep].Start)
            m_SetSegnale-=Step;
        *this=m_SetSegnale;
      }
    }
    else{
       if(m_SetSegnale & m_Step[m_ActProStep].Step[m_ActStep].Start){
          *this=m_SetSegnale;
       }else{
          *this=m_Step[m_ActProStep].Step[m_ActStep].Start;
#ifdef MOSTRA_SEGNALE_MSG
          if((m_Slot==PRED_SLOT) & (m_Point==PRED_POINT)){
               std::string str;
               str.append(m_Descrizione);
               str.append(" Segnale Uso m_ActStep  ");
               str.append(intToString(m_Step[m_ActProStep].Step[m_ActStep].Start));
               if(oldstr!=str){
                  MostraEventoProcesso(str,Color::FG_DEFAULT);
                  oldstr=str;
               }
           }
#endif
       }
    }

    if(m_Timer.Timer() && (m_ActStep+1)<D_STEP){
       m_ActStep++;
       *this=m_Step[m_ActProStep].Step[m_ActStep].Start;
       m_SetSegnale= m_Step[m_ActProStep].Step[m_ActStep].Start;
       if(m_ActStep>m_Step[m_ActProStep].StepCount)
          m_Set=true;
       else
          SetStepTempo();
    }

    return m_Set;

}



void CIOSegnale::SetStepTempo( )
{

  if(!this)
        return ;

  double Time;
  double StepTime;
  
  if(!m_Step)
    return ;

  if((m_ActProStep>D_STEP) | (m_ActStep >D_COMP_STEP))
      return;

  Time=(double)m_Step[m_ActProStep].Step[m_ActStep].Rampa;

  if(Time<0)
     return;

  StepTime=Time;
  if(m_Step[m_ActProStep].Step[m_ActStep].Stop>m_Step[m_ActProStep].Step[m_ActStep].Start)
     StepTime/=(double)(m_Step[m_ActProStep].Step[m_ActStep].Stop-m_Step[m_ActProStep].Step[m_ActStep].Start);
  else  if(m_Step[m_ActProStep].Step[m_ActStep].Stop<m_Step[m_ActProStep].Step[m_ActStep].Start)
     StepTime/=(double)(m_Step[m_ActProStep].Step[m_ActStep].Start-m_Step[m_ActProStep].Step[m_ActStep].Stop);

  m_Timer.Reset();
  m_Timer((DWORD)Time,(DWORD)StepTime);   
  
 
}

DWORD CIOSegnale::OnTime(WORD Step)
{

  if(!this)
        return 0;

  DWORD  m_OnTime=0;
  int i;

  if(!m_Step)
     return 0;

  if( Step<D_COMP_STEP){
    for(i=0;i<m_Step[Step].StepCount;i++){
      if(m_Step[Step].Step[i].Start | m_Step[Step].Step[i].Stop)
         m_OnTime+=m_Step[Step].Step[i].Rampa;
	}
    return m_OnTime;
  }

  
  return 0;
 
}

int CIOSegnale::OnTimeAh(){

    double tempo=0;

    for(int j=0;j<D_COMP_STEP;j++){
        for(int i=0;i<m_Step[j].StepCount;i++){
          if(m_Step[j].Step[i].Start | m_Step[j].Step[i].Stop){
             double numero=m_Step[j].Step[i].Rampa;
             numero/=1000;
             numero/=3600;
             numero*=m_Step[j].Step[i].Start;
             numero*=Fattore();
             tempo+=numero;
          }
        }
    }

    return (int)tempo;
}

DWORD CIOSegnale::OnTime()
{

  if(!this | !m_Step)
      return 0;

  if(m_Step[m_ActProStep].Step[m_ActStep].Start | m_Step[m_ActProStep].Step[m_ActStep].Stop)
      return m_Step[m_ActProStep].Step[m_ActStep].Rampa;

  return 0;

}

#ifdef QT_APP
void CIOSegnale::setColor(int index){

    if(index >14)
        return;

    PenWidth=2;

    switch(index){
      case 0:
       PenColor=Qt::red;
       break;
      case 1:
       PenColor=Qt::darkRed;
       break;
      case 2:
       PenColor=Qt::green;
       break;
      case 3:
       PenColor=Qt::darkGreen;
       break;
      case 4:
       PenColor=Qt::blue;
       break;
      case 5:
       PenColor=Qt::darkBlue;
       break;
      case 6:
       PenColor=Qt::cyan;
       break;
      case 7:
       PenColor=Qt::darkCyan;
       break;
      case 8:
       PenColor=Qt::magenta;
       break;
      case 9:
       PenColor=Qt::darkMagenta;
       break;
      case 10:
       PenColor=Qt::yellow;
       break;
      case 11:
       PenColor=Qt::darkYellow	;
       break;
      case 12:
       PenColor=Qt::gray;
       break;
      case 13:
       PenColor=Qt::darkGray;
       break;
      case 14:
       PenColor=Qt::lightGray;
       break;
    }


}



double CIOSegnale::getCompDouble(){

    if(!this)
        return 0;

    double DSegnale;
    if(this->TipoComp==CTipoComDef::PENNING){
         DSegnale=getPenning();
    }else if(this->TipoComp==CTipoComDef::BARATRON){
         DSegnale=getBaratron();
    }else if((this->TipoComp==CTipoComDef::BIAS_V)){
         DSegnale=getBias_V();
    }else if((this->TipoComp==CTipoComDef::BIAS_I)){
         DSegnale=getBias_I();
    }else{
         DSegnale=m_Segnale;
         DSegnale*=this->Fattore();
    }
    if(DSegnale<0)
        DSegnale=0;

    return DSegnale;

}


WORD CIOSegnale::getCompZipDouble(){

    if(!this)
        return 0;
    double val=getCompDouble();
    val*=Zip;
    if(val<0)
       val=0;
    if(val>255)
       val=255;
    return (int)val;
}

double CIOSegnale::Fattore(){
    if(!this)
      return 1;
   return m_Fattore;
}

double CIOSegnale::SecondoFattore(){
    if(!this)
       return 1;
   return m_SecondoFattore;
}


#endif


