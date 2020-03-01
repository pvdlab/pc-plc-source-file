
#include "StdAfx.h"
#include "Plc.h"
#include "MainCostanti.h"


CPlc::CPlc()
{

    ScanTime=0;

}


void CPlc::OpenPort(){

#ifdef CON_MODBUS
    if( getSimula()==true ){
        MostraEvento("Plc in simulazione",Color::FG_RED);
        return;
    }

    QString path=getRunPath();
    path+=PLC_INIT;
    CCInitFile *Init = new CCInitFile("ConsolePlc",path);
    Init->getFile();
    QString porta=Init->at(SET_COM + QString::number(1));
    if(porta.length()>9 ){
        pScanModulo= new thModbus(pModulo,ModuloCount,&pErroreModulo,true,porta,1,DogNonAbilitato);
    }
    for(int i=2; i< MAX_COM_PORT;i++){
        QString porta=Init->at(SET_COM + QString::number(i));
        if(porta.length()>9 ){
            thModbus *thmod= new thModbus(pModulo,ModuloCount,&pErroreModulo,true,porta,1,DogNonAbilitato);
            thmod->start();
        }
    }

    delete Init;
#endif


}


void CPlc::ScanModulo(){

#ifdef CON_MODBUS

    if(getSimula())
        return;
    if(!pScanModulo->Errore){
       pScanModulo->Scan(0);
       ScanTime=pScanModulo->ScanModuloTime;
    }else{
       ScanTime=255;
    }

#endif

}

void CPlc::InitScanModulo(){

#ifdef CON_MODBUS

    if(getSimula())
        return;

    for(int i=0; i<ModuloCount;i++)
        pScanModulo->Scan(0);


#endif

}




int CPlc::OffSet(int Slot, int Point)
{ 
    int i;
    BYTE RetVal=0;

    for(i=0;i<Slot;i++){
      if(pModulo[Slot].IsDigitale )
        RetVal+=pModulo[Slot].Count/8;
      else
        RetVal+=2*pModulo[Slot].Count;
    }
    if(pModulo[Slot].IsDigitale)
       return  RetVal;
    else
       return  RetVal+2*(Point);
}


int CPlc::SizeData()
{
    int Size=0;
	int i;

    for(i=0 ;i< ModuloCount ;i++){
      if(pModulo[i].IsDigitale )
         Size+=(pModulo[i].Count/8);
      else
         Size+=(pModulo[i].Count*2);

	}

	return Size;
	
}



int CPlc::GetPlcSegnali(BYTE *Buffer)
{
  int i;
  int len=0;
  BYTE *pBuffer=Buffer;

  for(i=0 ;i<ModuloCount;i++){
     memcpy(pBuffer,pModulo[i].Data,pModulo[i].SizeData);
     pBuffer+=pModulo[i].SizeData;
     len+=pModulo[i].SizeData;
  }
  
    
  return len;
}


int CPlc::SetPlcSegnali(BYTE **Buffer)
{
  int i;
  int len=0;
  BYTE *pBuffer=*Buffer;

  for(i=0 ;i<ModuloCount;i++){
     memcpy(pModulo[i].Data,pBuffer,pModulo[i].SizeData);
     pBuffer+=pModulo[i].SizeData;
     len+=pModulo[i].SizeData;
  }

  *Buffer=pBuffer;
  return len;

}

int CPlc::GetLenPlcSegnali()
{
  int len=0;

  for(int i=0 ;i<ModuloCount;i++){
     len+=pModulo[i].SizeData;
  }


  return len;
}


void CPlc::SetSegnale(BYTE *pStr)
{
  int slot,point;
  WORD Valore;

  pStr++;
  if(pStr)
    slot=*pStr++;
  if(pStr)
    point=*pStr++;
  Valore=StrWORD(&pStr);
  pModulo[slot].setSegnale(point,Valore);

}

CIOSegnale *CPlc::Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala )
{
    CIOSegnale *pTmpSegnale;

   if(pModulo[Slot].Count>=Point){
      pTmpSegnale=new CIOSegnale;
      pTmpSegnale->operator () (Slot,Point,Descrizione,FondoScala,pModulo[Slot].TipoSlot,OffSet(Slot,Point) ,&pModulo[Slot]);
      pModulo[Slot].setDescrizione(Point,Descrizione);
      return pTmpSegnale;
    }
   else
    return NULL;

}


CIOSegnale *CPlc::Segnale(const char Descrizione[],int Slot,int Point,WORD FondoScala, double a,double b )
{
    CIOSegnale *pTmpSegnale;

    if(pModulo[Slot].Count>=Point){
      pTmpSegnale=new CIOSegnale;
      pTmpSegnale->operator () (Slot,Point,Descrizione,FondoScala,pModulo[Slot].TipoSlot,OffSet(Slot,Point) ,&pModulo[Slot]);
      pTmpSegnale->setFattore(a);
      pTmpSegnale->setSecondoFattore(b);
      pModulo[Slot].setDescrizione(Point,Descrizione);
      return pTmpSegnale;
    }
   else
    return NULL;

}

