
#include "StdAfx.h"
#include "Errore.h"
#include <QDebug>

CErrore::CErrore()
{
    m_pErr=NULL;
    m_pMainTimer=NULL;
    m_ErrTimer(0);
    m_ListaCount=0;
    m_Lista.clear();
}

CErrore& CErrore::operator()(BYTE *pErr,CSTimer *New_pMainTimer,int NewTime, const char *newDescrizione)
{
    m_pErr=pErr;
	m_pMainTimer=New_pMainTimer;
    m_LastAttivita=m_pMainTimer->Time();
	m_Time=NewTime;
    m_Errore=false;
	m_Aspetta=false;
	memset(m_bErrore,0,BUFF_SIZE);
    memset(m_emitErrore,0,D_COMP_ERRORE);
    m_ErrTimer(NewTime);
    m_TmpErrore=false;
    index=0;
    char *pBuffer=m_Descrizione;
    memset(pBuffer,0,D_DESCRIZIONE);
    if(newDescrizione!= NULL){
        int len=strlen(newDescrizione);
        if(len>D_DESCRIZIONE)
            len=D_DESCRIZIONE;
        memcpy(pBuffer,newDescrizione,len);
    }

	return *this;
}

void CErrore::append(int index,const char *descrizione){

    if(index > (D_COMP_ERRORE-2)*8)
        return;

    while(m_ListaCount<index){
        m_ListaCount++;
        m_Lista.push_back("NULL");
    }

    m_Lista.push_back(descrizione);
    m_ListaCount++;

}

void CErrore::Clear(){

    m_Lista.clear();

}

void CErrore::Aspetta(int NewTime)
{

    m_Aspetta=true;
    m_ErrTimer(NewTime);

}

void CErrore::Errore(bool NewErrore,BYTE Pos,DWORD NewTime)
{

    if(Pos > (D_COMP_ERRORE-2)*8)
        return ;

	if(NewErrore && (NewErrore!=GetChrBit(m_bErrore, Pos))){
        if(m_ErrTimer.IsAttivo()){
            if (m_ErrTimer.LeftTime() > NewTime){
                m_ErrTimer(NewTime);
            }
            //qDebug() << "NUOVO TEMPO" << QString::number(NewTime) ;
        }else{
            m_ErrTimer(NewTime);
            //qDebug() << "NUOVO TEMPO" << QString::number(NewTime) ;
        }
        m_ErrTimer.Timer();
        //qDebug() << "IS ATTIVO" << QString::number(m_ErrTimer.IsAttivo()) ;//<< QString::number(Pos) << QString::number(NewErrore);
    }

    SetChrBit(m_bErrore, Pos, NewErrore);

}



bool CErrore::Errore()
{

    if(!m_ErrTimer.IsAttivo())
        return m_Errore;

    if(m_Aspetta){
      if(m_ErrTimer.Timer()){
          m_Aspetta=false;
          m_ErrTimer(m_Time);
          m_ErrTimer.Reset();
      }
      else{
          return false;
      }
    }

    if(m_ErrTimer.Timer()){
      if((m_bErrore[0]!=0) | (m_bErrore[1]!=0) | (m_bErrore[2]!=0) ){
         if((m_bErrore[0]!=*(m_pErr+2)) | (m_bErrore[1]!=*(m_pErr+3)) | (m_bErrore[2]!=*(m_pErr+4)) ){
           BYTE *pErr=m_pErr;
           int len=0;
           WORD Tempo=(WORD)m_pMainTimer->Time();
           pErr=WORDStr(pErr,Tempo,&len);
           pErr=ByteStr(pErr,m_bErrore[0],&len);
           pErr=ByteStr(pErr,m_bErrore[1],&len);
           pErr=ByteStr(pErr,m_bErrore[2],&len);
         }
         Edit();
         m_Errore=true;
         setErrore(true);
      }else{
         m_ErrTimer.Reset();
         m_Errore=false;
      }
    }

    return m_Errore;

}

void CErrore::first(){
    index=0;
}

bool CErrore::next(int &newErrore){

    bool trovato=false;
    newErrore=-1;
    for(int i=index;i<16;i++){
        if(GetChrBit(m_bErrore, i)) {
            index=i;
            newErrore=i;
            trovato=true;
            break;
        }
    }

    return trovato;

}

bool CErrore::IsErrore(int newErrore){

    if(GetChrBit(m_bErrore, newErrore))
        return true;
    else
        return false;

}

int CErrore::getErrore(){

    return index;
}

void CErrore::Informa(bool NewErrore,BYTE Pos)
{
    int Tempo;
	BYTE *Tmp=m_pErr;
    
	Tmp+=2;
    SetChrBit(Tmp, Pos, NewErrore);
	Tempo=(int)m_pMainTimer->Time();
    memcpy(m_pErr,&Tempo,sizeof(int));
	 
}
	
void CErrore::Reset()
{
    memset(m_pErr,'\0',D_COMP_ERRORE);
    memset(m_emitErrore,'\0',D_COMP_ERRORE);
    memset(m_bErrore,'\0',BUFF_SIZE);
    m_Errore=false;
	m_ErrTimer.Reset();
    m_TmpErrore=false;

}	

int CErrore::Edit(std::vector<std::string> *newLista){

    BYTE *tmp=m_pErr;
    int tempo=StrWORD(&tmp);
    BYTE Primo,Secondo,Terzo;
    int count;
    bool cambiato=false;

    Primo=StrByte(&tmp);
    Secondo=StrByte(&tmp);
    Terzo=StrByte(&tmp);

    newLista->clear();
    if(tempo==0 )
        return -1;

    for(int i=0;i< D_COMP_ERRORE;i++){
       if(m_emitErrore[i]!=m_pErr[i]){
           cambiato=true;
           break;
       }
    }

    if(!cambiato)
        return -1;

    memcpy(m_emitErrore,m_pErr,D_COMP_ERRORE);
    count=__min(m_ListaCount,8);
    for(int i=0 ; i< count; i++){
          if(GetBit(Primo, i)){
              std::string str=m_Descrizione;
              str.append(" ");
              str.append(m_Lista[i]);
              newLista->push_back(str);
          }
    }
    if(m_ListaCount>8){
       count= m_ListaCount-8;
       for(int i=0 ; i< count; i++){
            if(GetBit(Secondo, i)){
                std::string str=m_Descrizione;
                str.append(" ");
                str.append(m_Lista[i+8]);
                newLista->push_back(str);
            }
       }
    }

    if(m_ListaCount>16){
       count= m_ListaCount-16;
       for(int i=0 ; i< count; i++){
            if(GetBit(Terzo, i)){
                std::string str=m_Descrizione;
                str.append(" ");
                str.append(m_Lista[i+16]);
                newLista->push_back(str);
            }
       }
    }

    if(newLista->size())
        return tempo;
    else
        return 0;

}

void CErrore::Edit(){

    BYTE *tmp=m_pErr;
    int tempo=StrWORD(&tmp);
    BYTE primo=StrByte(&tmp);
    BYTE secondo=StrByte(&tmp);
    BYTE terzo=StrByte(&tmp);
    int count;
    bool cambiato=false;

    for(int i=0;i< D_COMP_ERRORE;i++){
       if(m_emitErrore[i]!=m_pErr[i]){
           cambiato=true;
           break;
       }
    }

    if(!cambiato)
        return ;

    memcpy(m_emitErrore,m_pErr,D_COMP_ERRORE);

    if(primo){
        count=__min(m_ListaCount,8);
        for(int i=0 ; i< count; i++){
              if(GetBit(primo, i)){
                 //QString Nuova= QString::number( i ) + "DA_LISTA_ERRORE Tempo " + QString::number( tempo )
                 //         + " Primo " + QString::number(primo ) ;
                 //qDebug() << Nuova;
                 if(m_Descrizione){
                   std::string str=m_Descrizione;
                   str.append(" ");
                   str.append(m_Lista[i]);
                   MostraErrore(str);
                 }else
                   MostraErrore(m_Lista[i]);
              }
        }
    }

    if(secondo){
        if(m_ListaCount>8){
           count= m_ListaCount-8;
           for(int i=0 ; i< count; i++){
                if(GetBit(secondo, i)){
                    if(m_Descrizione){
                      std::string str=m_Descrizione;
                      str.append(" ");
                      str.append(m_Lista[i+8]);
                      MostraErrore(str);
                    }else
                      MostraErrore(m_Lista[i+8]);
                }
           }
        }
    }

    if(terzo){
        if(m_ListaCount>16){
           count= m_ListaCount-16;
           for(int i=0 ; i< count; i++){
                if(GetBit(terzo, i)){
                    if(m_Descrizione){
                      std::string str=m_Descrizione;
                      str.append(" ");
                      str.append(m_Lista[i+16]);
                      MostraErrore(str);
                    }else
                      MostraErrore(m_Lista[i+16]);
                }
           }
        }
    }

}

