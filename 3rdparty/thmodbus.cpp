#include "thmodbus.h"
#include "MainCostanti.h"
#include "QDebug"
#include <QTime>

//#define WDOG_OFF
#define TOGLI_ERRORE

#define OGNI_URGENTE        2
#define OGNI_ANALOGICO      2
#define SCAN_COUNT         10

#define ScanModuloUrgente   0
#define ScanModuloDigitale  1
#define ScanModuloAnalogico 2

thModbus::thModbus(CModulo *pNewModulo, int newModuloCount ,CErrore  *pNewErrore,bool newDebug,QString newCom,int newPorta,bool newDogNonAbilitato)
{

    pModulo=pNewModulo;
    pErrore=pNewErrore;
    Index=0;
    IndexUrgente=0;
    IndexAnalogico=0;
    IndexCount=0;
    IndexUrgenteCount=0;
    IndexAnalogicoCount=0;
    Debug=newDebug;
    Porta=newPorta;
    DogNonAbilitato=newDogNonAbilitato;
    Com=newCom;
    Debug=false;
    ModuloCount=newModuloCount;
    Errore=false;


    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled & (modulo->Porta== Porta)){
            if(!modulo->Urgente){
                if((modulo->TipoSlot==CTipoSlot::OutAnalogico) | (modulo->TipoSlot==CTipoSlot::InAnalogico))
                  IndexAnalogicoCount++;
                else
                  IndexCount++;
            }else
                IndexUrgenteCount++;
        }
    }

    if(IndexAnalogicoCount)
       pIndexAnalogico= new int[IndexAnalogicoCount];
    else
       pIndexAnalogico=NULL;

    if(IndexUrgenteCount)
       pIndexUrgente= new int[IndexUrgenteCount];
    else
       pIndexUrgente=NULL;

    if(IndexCount)
       pIndex= new int[IndexCount];
    else
       pIndex=NULL;

    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled & (modulo->Porta== Porta)){
            if(!modulo->Urgente ){
                if((modulo->TipoSlot==CTipoSlot::OutAnalogico) | (modulo->TipoSlot==CTipoSlot::InAnalogico)){
                    pIndexAnalogico[IndexAnalogico]=i;
                    IndexAnalogico++;
                }else{
                    pIndex[Index]=i;
                    Index++;
                }

            }else{
                pIndexUrgente[IndexUrgente]=i;
                IndexUrgente++;
            }
        }

    }

    IndexUrgente=0;
    Index=0;
    IndexAnalogico=0;
    ScanModuloCount=0;
    OgniAnalogico=qMin(OGNI_ANALOGICO,IndexAnalogicoCount);
    OgniUrgente=qMin(OGNI_URGENTE,IndexUrgenteCount);

    pScanModuloTime= new QTime();
    StartTime=new QTime();
    Count=SCAN_COUNT;


    m_modbus = modbus_new_rtu( Com.toLatin1().constData(), 38400,'N', 8,  1 );
    if( modbus_connect( m_modbus ) == -1 ){
          modbus_close( m_modbus );
          modbus_free( m_modbus );
          setSimula(true);
          return;
    }


    if(!ResetModbus()){
        modbus_close( m_modbus );
        modbus_free( m_modbus );
        setSimula(true);
        return;
    }
    setSimula(false);



}

int thModbus::getNext(){


    TotScanModuloCount++;

Ripeti:

    for(int i=0;i<ModuloCount;i++){
       CModulo *modulo=&pModulo[i];
       if(modulo->DaScrivere & (modulo->TipoSlot==CTipoSlot::OutDigitale) & modulo->Urgente){
          ScanModulo=ScanModuloUrgente;
          break;
       }
    }


    if(ScanModulo==ScanModuloUrgente){
        if(IndexUrgenteCount==0){
           ScanModulo++;
           goto Ripeti;
        }else{
            if(IndexUrgente+1>IndexUrgenteCount){
                IndexUrgente=0;
                ScanModulo++;
                goto Ripeti;
            }else{
                for(int i=IndexUrgente;i< IndexUrgenteCount; i++  ){
                    int actindex=pIndexUrgente[i];
                    CModulo *modulo=&pModulo[actindex];
                    if(modulo->DaScrivere){
                       OgniUrgente--;
                       if(!OgniUrgente){
                          OgniUrgente=qMin(OGNI_URGENTE,IndexUrgenteCount);
                          ScanModulo++;
                       }
                       return pIndexUrgente[IndexUrgente++];
                    }
                }
                IndexUrgente=0;
                ScanModulo++;
                goto Ripeti;
            }
        }
    }else if(ScanModulo==ScanModuloDigitale){
        if(IndexCount==0){
           ScanModulo++;
           goto Ripeti;
        }else{
            if(Index+1>IndexCount){
                Index=0;
                ScanModulo++;
                goto Ripeti;
            }else
                return pIndex[Index++];
        }
    }else if(ScanModulo==ScanModuloAnalogico){
        if(IndexAnalogicoCount==0){
           ScanModulo++;
           goto Ripeti;
        }else{
            if(IndexAnalogico+1>IndexAnalogicoCount){
                IndexAnalogico=0;
                ScanModulo++;
                goto Ripeti;
            }else{
                for(int i=IndexAnalogico;i< IndexAnalogicoCount; i++  ){
                    int actindex=pIndexAnalogico[i];
                    CModulo *modulo=&pModulo[actindex];
                    if((modulo->TipoSlot==CTipoSlot::InAnalogico) | ((modulo->TipoSlot==CTipoSlot::OutAnalogico) & modulo->DaScrivere) ){
                        OgniAnalogico--;
                        if(!OgniAnalogico){
                          ScanModulo++;
                          OgniAnalogico=qMin(OGNI_ANALOGICO,IndexAnalogicoCount);;
                       }
                       return pIndexAnalogico[IndexAnalogico++];
                    }
                }
                IndexAnalogico=0;
                ScanModulo++;
                goto Ripeti;
            }
        }
    }else{
        Count--;
        if(!Count){
            ScanModuloTime=pScanModuloTime->elapsed();
            ScanModuloTime/=SCAN_COUNT;
            Count=SCAN_COUNT;
            pScanModuloTime->start();
        }
        ScanModulo=0;
        TotScanModuloCount=0;
        goto Ripeti;
    }

    return 0;




}

#define SCAN_ERRORE_TIME 400

void thModbus::Scan(int index){


    uint8_t  dest[1024];
    uint16_t * dest16 = (uint16_t *) dest;
    uint8_t  * Ddata = new uint8_t[2];
    uint16_t * Adata = new uint16_t[8];

    memset( dest, 0, 1024 );
    int ret=-1;
    if(!index)
       ActIndex=getNext();
    else
       ActIndex=index;

    if(pModulo[ActIndex].Enabled){
        CModulo *modulo=&pModulo[ActIndex];
        modbus_set_slave( m_modbus, modulo->Index );
        switch( modulo->TipoSlot )
          {
            case CTipoSlot::OutDigitale :
               {
                 if(modulo->DaScrivere ){
                  modulo->InScrittura=true;
                  QTime time;
                  time.start();
                  int byte_count = (modulo->Count / 8) + ((modulo->Count % 8) ? 1 : 0);
                  memset(Ddata,0,2);
                  for(int j=0;j<byte_count;j++){
                     Ddata[j]=modulo->Scrivo[j];
                  }
                  QTime ScanStartTime;
                  ScanStartTime.start();
                  ret = modbus_write_bits( m_modbus, modulo->DataIndex, modulo->Count, Ddata  );
                  if(ret>0){
                     for(int j=0;j<byte_count;j++){
                       modulo->Data[j]=Ddata[j];
                     }
                     modulo->DaScrivere=false;
                     modulo->LettiCount++;
                     modulo->ScanTime+=ScanStartTime.elapsed();
                     pErrore->Errore(false,index+1,SCAN_ERRORE_TIME);
                  }else{
#ifdef TOGLI_ERRORE
                    std::string str;
                    str.append("Modulo [");
                    str.append(intToString(modulo->Index));
                    str.append("] Errore");
                    MostraEvento(str,Color::FG_GREEN);
#else
                     pErrore->Errore(true,index+1,SCAN_ERRORE_TIME);
                     modulo->ErroriCount++;
#endif
                  }
                  modulo->InScrittura=false;
                 }

               }
               break;
            case CTipoSlot::InDigitale:
               {
                 QTime ScanStartTime;
                 ScanStartTime.start();
                 ret = modbus_read_input_bits( m_modbus, modulo->DataIndex, modulo->Count, dest );
                 int byte_count = (modulo->Count / 8) + ((modulo->Count % 8) ? 1 : 0);
                 if(ret>0){
                    for(int j=0;j<byte_count;j++){
                      modulo->Data[j]=dest[j];
                    }
                    modulo->LettiCount++;
                    modulo->ScanTime+=ScanStartTime.elapsed();
                    pErrore->Errore(false,index+1,SCAN_ERRORE_TIME);
                 }else{
#ifdef TOGLI_ERRORE
                    std::string str;
                    str.append("Modulo [");
                    str.append(intToString(modulo->Index));
                    str.append("] Errore");
                    MostraEvento(str,Color::FG_GREEN);
#else
                     pErrore->Errore(true,index+1,SCAN_ERRORE_TIME);
                     modulo->ErroriCount++;
#endif
                 }
               }
               break;
            case CTipoSlot::OutAnalogico:
              {
                QTime ScanStartTime;
                ScanStartTime.start();
                if(modulo->DaScrivere){
                  modulo->InScrittura=true;
                  memset(Adata,0,16);
                  BYTE *pBuffer;
                  pBuffer=modulo->Scrivo;
                  WORD Segnale;
                  for(int j=0;j<modulo->Count;j++){
                     Segnale=StrWORD(&pBuffer);
                     Adata[modbus_registro_in_index(modulo,j)]=Segnale;
                  }
                  ret=modbus_write_registers(m_modbus, modulo->DataIndex , modulo->Count, Adata);
                  if(ret>0){
                      for(int j=0;j<modulo->Count;j++){
                       pBuffer=modulo->Data;
                       int len=0;
                       Segnale=Adata[j];
                       pBuffer+=modbus_registro_out_index(modulo,j)*2;
                       pBuffer=WORDStr(pBuffer,Segnale,&len);
                     }
                  }
                  if(ret>0){
                      modulo->DaScrivere=false;
                      modulo->LettiCount++;
                      modulo->ScanTime+=ScanStartTime.elapsed();
                      pErrore->Errore(false,index+1,2*SCAN_ERRORE_TIME);
                  }else{
#ifdef TOGLI_ERRORE
                    std::string str;
                    str.append("Modulo [");
                    str.append(intToString(modulo->Index));
                    str.append("] Errore");
                    MostraEvento(str,Color::FG_GREEN);
#else
                     pErrore->Errore(true,index+1,2*SCAN_ERRORE_TIME);
                     modulo->ErroriCount++;
#endif
                  }
                  modbus_poll(m_modbus);
                  modulo->InScrittura=false;
                }
              }
              break;
            case CTipoSlot::InAnalogico:
              {
                QTime ScanStartTime;
                ScanStartTime.start();
                ret = modbus_read_input_registers( m_modbus, modulo->DataIndex , modulo->Count, dest16 );
                if(ret>0){
                  BYTE *pBuffer;
                  pBuffer=modulo->Data;
                  short Segnale;
                  for(int j=0;j<modulo->Count;j++){
                     Segnale=dest16[j];
                     memcpy(pBuffer,&Segnale,sizeof(short));
                     pBuffer+=2;
                  }
                  modulo->LettiCount++;
                  modulo->ScanTime+=ScanStartTime.elapsed();
                  pErrore->Errore(false,index+1,2*SCAN_ERRORE_TIME);
                }else{
#ifdef TOGLI_ERRORE
                    std::string str;
                    str.append("Modulo [");
                    str.append(intToString(modulo->Index));
                    str.append("] Errore");
                    MostraEvento(str,Color::FG_GREEN);
#else
                     pErrore->Errore(true,index+1,2*SCAN_ERRORE_TIME);
                     modulo->ErroriCount++;
#endif
                }
              }
              break;
            default:
              break;

          }

       }

    delete[] Ddata;
    delete[] Adata;

    if(pErrore->Errore()){
        Errore=true;
        modbus_close( m_modbus );
        modbus_free( m_modbus );
        setSimula(true);
        MostraEvento("ERRORE MODBUS MACCHINA IN SIMULAZIONE",Color::FG_RED);
        return;
    }

    getScanTime();

}

int thModbus::modbus_registro_in_index(CModulo *modulo,int index){

    if(modulo->Versione==3028){
        switch ( index){
          case 0:
            return 0;
            break;
          case 2:
            return 1;
            break;
          case 4:
            return 2;
            break;
          case 6:
            return 3;
            break;
          case 1:
            return 4;
            break;
          case 3:
            return 5;
            break;
          case 5:
            return 6;
            break;
          case 7:
            return 7;
            break;

        }
    }

    return index;

}

int thModbus::modbus_registro_out_index(CModulo *modulo,int index){

    if(modulo->Versione==3028){
        switch ( index){
          case 0:
            return 0;
            break;
          case 1:
            return 2;
            break;
          case 2:
            return 4;
            break;
          case 3:
            return 6;
            break;
          case 4:
            return 1;
            break;
          case 5:
            return 3;
            break;
          case 6:
            return 5;
            break;
          case 7:
            return 7;
            break;

        }
    }

    return index;

}

void thModbus::getScanTime(){

    bool CalcoloScan=true;
    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->TipoSlot==CTipoSlot::InDigitale){
           if((modulo->LettiCount<5) & modulo->Enabled )
               CalcoloScan=qMin(CalcoloScan,false);
        }
    }

    if(CalcoloScan){
      double TotTempo=StartTime->elapsed();
      double tempo=StartTime->elapsed();
      int letti=0;
      int errori=0;
      tempo=0;
      for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled){
              if((modulo->LettiCount!=0) & Debug){
                   double ScanTempo=modulo->ScanTime;
                   ScanTempo/=modulo->LettiCount;
                   QString TipoSlot;
                   switch( modulo->TipoSlot )
                       {
                       case CTipoSlot::OutDigitale :
                         TipoSlot="OUT DIGIT";
                       break;
                       case CTipoSlot::InDigitale :
                         TipoSlot="IN DIGITA";
                       break;
                       case CTipoSlot::OutAnalogico :
                          TipoSlot="OUT ANALOG";
                       break;
                       case CTipoSlot::InAnalogico :
                         TipoSlot="IN ANALOG";
                       break;

                   }
                   if(Debug)
                      qDebug() << TipoSlot <<  " Index "+ QString::number(modulo->Index) << " Letti " << modulo->LettiCount
                                               << " Scan Time: " << ScanTempo<< " ms" ;
                   tempo+=ScanTempo*modulo->LettiCount;
                }else{
                    if((modulo->ErroriCount>DEF_ERRORE_LETTURA_SCRITTURA) & (modulo->LettiCount==0)){
                        //if(Debug)
                        qDebug() << " Index " + QString::number(modulo->Index) << " Errore " << modulo->ErroriCount;
                        if((modulo->TipoSlot==CTipoSlot::OutDigitale) | (modulo->TipoSlot==CTipoSlot::OutAnalogico))
                          pErrore->Errore(true,E_MODBUS_SCRITTURA,0);
                        else
                          pErrore->Errore(true,E_MODBUS_LETTURA,0);
                        pErrore->Errore();
                        modulo->Errore=true;
                    }
               }

               letti+=modulo->LettiCount;
               errori+=modulo->ErroriCount;
               modulo->ScanTime=0;
               modulo->LettiCount=0;
               modulo->ErroriCount=0;
            }
    }
    double ScanTempo=tempo;
    ScanTempo/=letti;
    if(Debug)
        qDebug() << "Scan " << ScanTempo << " Tot Scan " << tempo <<  " Misuro dopo " << TotTempo << TotTempo/letti
             << "ScanModuloTime " << ScanModuloTime;
    //ScanTime=ScanModuloTime;
    StartTime->start();
 }

}



void thModbus::run(){

    StartTime->start();

    while (1) {
        Scan(0);
    }


}




#define TENTATIVI_LETTURA 2
void thModbus::SetRitardo(int ritardo){

    int ret;
    uint8_t * pDdata = new uint8_t[2];

    memset(pDdata,1,2);
    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled & (modulo->Versione!=0) ){
            modbus_set_slave( m_modbus, modulo->Index );
            for(int j=0; j<TENTATIVI_LETTURA ; j++){
                ret=modbus_write_register(m_modbus, 7 , ritardo);
                if(ret>0){
                    std::string str;
                    str.append("Modulo [");
                    str.append(intToString(modulo->Index));
                    str.append("] SET RITARDO ");
                    str.append(intToString(ritardo));
                    MostraEvento(str,Color::FG_DEFAULT);
                    break;
                }
                msleep(5);
            }


        }

    }
}

void thModbus::ResetAllarmi(){

    int ret;

    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled){
            modbus_set_slave( m_modbus, modulo->Index );
            if(modulo->TipoSlot==CTipoSlot::OutDigitale ){
                if(modulo->Versione==3188){
                    for(int j=0; j< TENTATIVI_LETTURA; j++){
                      ret = modbus_write_bit( m_modbus, 33, 0 );
                      if(ret>0){
                          std::string str;
                          str.append("Modulo [");
                          str.append(intToString(modulo->Index));
                          str.append("] RESET ALLARMI ");
                          MostraEvento(str,Color::FG_DEFAULT);
                          break;
                      }
                      msleep(5);
                    }
                }

            }else if(modulo->TipoSlot==CTipoSlot::OutAnalogico){
                for(int j=0; j< TENTATIVI_LETTURA; j++){
                    ret=modbus_write_bit(m_modbus, 10 , 0);
                    if(ret>0){
                        std::string str;
                        str.append("Modulo [");
                        str.append(intToString(modulo->Index));
                        str.append("] RESET ALLARMI ");
                        MostraEvento(str,Color::FG_DEFAULT);
                        break;
                    }
                    msleep(5);
                }

            }
        }

    }

}



void thModbus::SetWotchDog(){

    if(DogNonAbilitato)
        return;

    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        if(modulo->Enabled){
            modbus_set_slave( m_modbus, modulo->Index );
            if(modulo->TipoSlot==CTipoSlot::OutDigitale ){
                if(modulo->Versione!=0){
                    for(int j=0; j< TENTATIVI_LETTURA; j++){
                      int ret = modbus_write_bit( m_modbus, 32, 1 );
                      if(ret>0){
                          std::string str;
                          str.append("Modulo [");
                          str.append(intToString(modulo->Index));
                          str.append("] ABILITO WotchDog ");
                          MostraEvento(str,Color::FG_DEFAULT);
                          break;
                      }else{
                          if(j== TENTATIVI_LETTURA-1){
                              std::string str;
                              str.append("Modulo [");
                              str.append(intToString(modulo->Index));
                              str.append("] WotchDog NON ABILITATO");
                              MostraEvento(str,Color::FG_RED);
                              break;
                          }

                      }
                      msleep(5);
                    }
                }
            }
        }

    }


}



bool thModbus::ResetModbus(){


    if( !m_modbus )
       return false;

    uint8_t * pDdata = new uint8_t[2];
    memset(pDdata,0,2);
    uint16_t * Adata = new uint16_t[8];
    for(int i=0; i<8;i++)
       Adata[i]=0;
    int ret;

    bool mSimola=false;
    for(int i=0;i<ModuloCount;i++){
        CModulo *modulo=&pModulo[i];
        modbus_set_slave( m_modbus, modulo->Index );
        if(modulo->TipoSlot==CTipoSlot::OutDigitale  ){
           if(modulo->Enabled){
             for(int j=0; j< TENTATIVI_LETTURA; j++){
                    if(modulo->Versione!=0)
                      ret = modbus_write_bits( m_modbus, 16, modulo->Count, pDdata  );
                    else
                      ret = modbus_write_bits( m_modbus, 0, modulo->Count, pDdata  );
                    if(ret>0){
                        std::string str;
                        str.append("Modulo [");
                        str.append(intToString(modulo->Index));
                        str.append("] RISPONDE ");
                        MostraEvento(str,Color::FG_DEFAULT);
                        break;
                    }
                    if((ret==-1) & (j==TENTATIVI_LETTURA-1)){
                       std::string str;
                       str.append("Modulo [");
                       str.append(intToString(modulo->Index));
                       str.append("] NON RISPONDE ");
                       MostraEvento(str,Color::FG_RED);
                       mSimola=true;
                    }
                }

                //QThread::msleep(5);
           }

        }else if(modulo->TipoSlot==CTipoSlot::OutAnalogico ){
            if(modulo->Enabled){
                for(int j=0; j< TENTATIVI_LETTURA; j++){
                    ret=modbus_write_registers(m_modbus, 14 , modulo->Count, Adata);
                    if(ret>0){
                        std::string str;
                        str.append("Modulo [");
                        str.append(intToString(modulo->Index));
                        str.append("] RISPONDE ");
                        MostraEvento(str,Color::FG_DEFAULT);
                        break;
                    }
                    if((ret==-1) & (j==TENTATIVI_LETTURA-1)){
                        std::string str;
                        str.append("Modulo [");
                        str.append(intToString(modulo->Index));
                        str.append("] NON RISPONDE ");
                        MostraEvento(str,Color::FG_RED);
                        mSimola=true;
                    }
                    msleep(1);

              }
            }
        }
        modbus_poll(m_modbus);
    }

    if( mSimola==true ){
        MostraEvento("Plc in simulazione",Color::FG_DEFAULT);
        delete[] pDdata;
        delete[] Adata;
        return false;
    }

#ifndef WDOG_OFF
    SetWotchDog();
#endif
    ResetAllarmi();
    setSimula(mSimola);

    delete[] pDdata;
    delete[] Adata;

    return true;


}




