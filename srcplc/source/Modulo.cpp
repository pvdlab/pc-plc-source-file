#include "StdAfx.h"
#include "Modulo.h"
#include "MainCostanti.h"

CPoint::CPoint(){

    char *pDescrizione=Descrizione;
    memset(pDescrizione,'\0,',D_DESCRIZIONE);
    Point=0;

}


CModulo::CModulo()
{

  Count=0;
  TipoSlot=0;
  ScanTime=0;
  Data=NULL;
  Scrivo=NULL;
  Enabled=false;
  Posizionato=false;
  
}

void CModulo::setDescrizione(int point, const char descrizione[]){

    if((point>=0) & (point <=Count)){
       char *pDescrizione=pPoint[point].Descrizione;
       int len=__min(D_DESCRIZIONE,strlen(descrizione));
       memcpy(pDescrizione,descrizione,len);
    }


}

void CModulo::setSlot(int newSlot, int newIndex,int NewTipoSlot, int NewCount,
                      int NewVersione, int newFondoScala, bool newEnabled ,const char newnetPlc[],
                      bool NewUrgente, BYTE newporta,BYTE   newDataIndex)
{

  DataIndex=newDataIndex;
  pPoint=new CPoint[NewCount];
  memset(pPoint,0,NewCount*sizeof(CPoint));
  Index=newIndex;
  TipoSlot = NewTipoSlot; 
  Count = NewCount;
  Versione=NewVersione;
  FondoScala=newFondoScala;
  Slot=newSlot;
  Risposta=Count;
  LettiCount=0;
  ErroriCount=0;
  Errore=false;
  DaScrivere=false;
  InScrittura=false;
  Enabled=newEnabled;
  netPlc=newnetPlc;
  Urgente=NewUrgente;
  ScanTime=0;
  systemTime=0;
  Porta=newporta;

  if(TipoSlot==CTipoSlot::InDigitale || TipoSlot==CTipoSlot::OutDigitale )
	  IsDigitale=true;
  else
	  IsDigitale=false;

  if(NewCount>0 ){
     if(IsDigitale ){
        SizeData=(NewCount / 8) + ((NewCount % 8) ? 1 : 0);
     }
     else
        SizeData=2*NewCount;
     Data=new BYTE[SizeData];
	 memset(Data,0,SizeData);
     if(TipoSlot==CTipoSlot::OutDigitale || TipoSlot==CTipoSlot::OutAnalogico ){
         Scrivo=new BYTE[SizeData];
         memset(Scrivo,0,SizeData);
     }
  }


}


void CModulo::SetOff(){

    if(IsDigitale ){
       SizeData=(Count / 8) + ((Count % 8) ? 1 : 0);
    }
    else
       SizeData=2*Count;

    if(TipoSlot==CTipoSlot::OutDigitale || TipoSlot==CTipoSlot::OutAnalogico ){
        Scrivo=new BYTE[SizeData];
        memset(Scrivo,0,SizeData);
    }

}

WORD CModulo::getSegnale(int Point)
{
  short RetVal=0;
  BYTE *pBuffer;

  pBuffer=Data;
  switch(TipoSlot){
    case CTipoSlot::OutDigitale:
    case CTipoSlot::InDigitale:
        if(Point<8){
          RetVal=GetBit(*pBuffer, (BYTE)Point);
        }
        else{
          pBuffer++;
          RetVal=GetBit(*pBuffer, (BYTE)(Point-8));
        }
        if(RetVal<0){
           RetVal=0;
        }
        break;
    case CTipoSlot::OutAnalogico:
    case CTipoSlot::InAnalogico:
        pBuffer+=Point*2;
        RetVal=StrShort(&pBuffer);
        if(RetVal<0){
           RetVal=0;
        }
        break;
  }

  return RetVal;
}


WORD  CModulo::setSegnale(int Point,WORD Segnale)
{

  BYTE *pBuffer;

  if(Point>=Count)
      return 0;


  switch(TipoSlot){
    case CTipoSlot::OutDigitale:
        if(getSimula()==false ){
            if(InScrittura){
               return getSegnale(Point);
            }
            pBuffer=Scrivo;
            SetBit(pBuffer,Point,Segnale);
            DaScrivere=true;
        }
        else{
            pBuffer=Data;
            SetBit(pBuffer,Point,Segnale);
        }
      break;
    case CTipoSlot::InDigitale:
      if(getSimula()==true){
          pBuffer=Data;
          SetBit(pBuffer,Point,Segnale);
      }
      break;
    case CTipoSlot::OutAnalogico:
      if(getSimula()==false ){
          if(InScrittura){
             return getSegnale(Point);
          }
          pBuffer=Scrivo;
          pBuffer+=Point*2;
          memcpy(pBuffer,&Segnale,sizeof(WORD));
          DaScrivere=true;
      }
      else{
          pBuffer=Data;
          pBuffer+=Point*2;
          memcpy(pBuffer,&Segnale,sizeof(WORD));
      }
      break;
    case CTipoSlot::InAnalogico:
      if(getSimula()==true){
         pBuffer=Data;
         pBuffer+=Point*2;
         memcpy(pBuffer,&Segnale,sizeof(WORD));
      }
      break;


  }


  return Segnale;
}

