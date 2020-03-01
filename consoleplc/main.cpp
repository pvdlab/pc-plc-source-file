

#include <QtCore>
#include "Timer.h"
#include "Simula.h"
#include "MainCostanti.h"
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sys/file.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "Socket.h"
#include "LoopPlc.h"
#include "StdAfx.h"
#include "Initfile.h"
#include "Test.h"
#include "Ciclo.h"
#include <iostream>
#include "Timer.h"
#include <QDebug>

//#define TEST_6012



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);



    /*BYTE bit;
    int numero=0;
    bit=__min(numero,1);

    WORD numero=100;
    if(numero*0.05<100)
        qDebug() << QString::number(numero*0.05);

    int m_Bias=BP5_BIAS;
    setSimula(false);

    if((m_Bias==BP5_BIAS) & !getSimula()){
        qDebug() << "eccomi";
    }*/

    QString appNome;
    char st[200];
    getcwd(st,200);
    QString RunPath=st;
    if(!RunPath.isEmpty()){
        int pos =RunPath.lastIndexOf("/");
        appNome=RunPath.mid(pos+1,RunPath.length()-pos);
        if(pos<RunPath.length()){
            RunPath=RunPath.left(pos);
        }
        setRunPath(RunPath.toUtf8());
    }else{
        return a.exec();
    }

    CCInitFile *Init = new CCInitFile("ConsolePlc",RunPath + PLC_INIT);

    int  Impianto=Init->isMacchina(argc, argv);
    bool EsisteInitFile=Init->getFile();
    bool IsSudo=Init->isSudo(argc, argv);
    bool IsSimula=Init->isSimula(argc, argv);
    if(IsSimula){
        MostraEvento("Avvio In modalita Simulazione",Color::FG_GREEN);
        setSimula(IsSimula);
    }else
        setSimula(-1);

    bool IsDebug=Init->isDebug(argc, argv);
    if(IsDebug)
        MostraEvento("Avvio In modalita DEBUG",Color::FG_DEFAULT);
    setDebug(IsDebug);

    if(IsSudo){
        Init->mostra();
        Init->appaned(SET_IMPIANTO, "Impianto: [6010/6011]  \t\t");
        Init->appaned(SET_HOST, "DbHost \t\t\t\t");
        Init->appaned(SET_UTENTE_LOCALE, "DbUtente \t\t\t");
        Init->appaned(SET_PASS_LOCALE, "DbPass \t\t\t\t");
        Init->appaned(SET_PORTA, "PlcPorta \t\t\t");
        Init->appaned(SET_HOME, "home path [/home/ricvb] \t");
        Init->appaned(SET_LICENZA, "Licenza formato [M-yyyy] \t"); //formato M-yyyy
        Init->appaned(SET_COMPLETO, "Completo [vero/falso: 1/0]  \t");
        Init->appaned(SET_COM1, "[/dev/ttyS0]  \t");
        Init->appaned(SET_COM2, "[/dev/ttyS1]  \t");
        Init->appaned(SET_COM3, "[/dev/ttyS2]  \t");
        Init->appaned(SET_COM4, "[/dev/ttyS3]  \t");
        for(;;){
           QString str= Init->leggiInput();
              if(str=="exit")
                  break;
        }
        Init->setFile();
        Init->getFile();
    }



    std::string erroreFile;
    erroreFile.append(RunPath.toUtf8());
    erroreFile.append(PLC_ERRORE);
    setErrorFileName(erroreFile);
    bool errore=false;

    std::string versione;
    versione.append("VERSIONE SOFTWARE ");
    versione.append(intToString(VERSIONESOFTWARE));
    MostraEvento(versione,Color::FG_DEFAULT);

    if(!EsisteInitFile ){
       MostraErrore("Init File Inesistente ");
       MostraErrore("Contattare L'assistenza Tecnica ");
       return a.exec();
    }

    bool Completo=Init->at(SET_COMPLETO).toInt();

    if(Completo){
        MostraEvento("Versione Software Completa",Color::FG_DEFAULT);
    }else{
        MostraEvento("Versione Software Ridotta",Color::FG_DEFAULT);
    }

    QString licenza=Init->at(SET_LICENZA).trimmed();
    int pos =licenza.indexOf("-");
    int lmese=licenza.left(pos).toInt();
    int lanno=licenza.mid(pos+1,licenza.length()-pos).toInt();
    setLicenza(lanno,lmese);

    if(!isLicenza()){
        ;
    }

    creaDir();

    if(Impianto==0){
        QString str=Init->at(SET_IMPIANTO);
        if(str.length()==4){
            std::string msg="Impianto Tipo: ";
            msg.append(str.toUtf8());
            MostraEvento(msg,Color::FG_BLUE);
            Impianto=str.toInt();

        }else{
            MostraEvento("Impianto NON RICONOSCIUTO",Color::FG_RED);
            return a.exec();
        }
    }

    if((Impianto!= ARC_6010) & (Impianto!=ARC_6011) & (Impianto!=ARC_6012)){
        std::string msg="Impianto  Tipo ";
        msg.append(intToString(Impianto));
        msg.append(" Non Riconosciuto ");
        MostraErrore(msg);
        return a.exec();
    }

    QString homePath=Init->at(SET_HOME);
    int Porta=Init->at(SET_PORTA).toInt();
    setPlcListenPort(Porta);
    if(!homePath.isEmpty()){
        QDir dir(homePath);
        if(!dir.exists()){
            if(!dir.mkdir(homePath)){
                std::string msg="Home Path Errata ";
                msg.append(homePath.toUtf8());
                MostraErrore(msg);
            }
        }
        setHomePath(homePath.toUtf8());
    }

    if(homePath.isEmpty()){
       MostraErrore("Nessuna Home Path utilizzo Run Path");
       setHomePath(RunPath.toUtf8());
    }

    std::string eventoFile;
    eventoFile.append(getHomePath());
    eventoFile.append(PLC_MAIN_EVENTO);
    setEventoFileName(eventoFile);
    setSalvaSuFile(true);

    QDir dir(homePath);
    if(!dir.exists()){
        errore=true;
        std::string msg="Path Inesistente ";
        msg.append(homePath.toUtf8());
        MostraErrore(msg);
    }


    ////IL PROCESSO È UNICO/////
    std::string msg="pgrep ";
    msg.append(appNome.toUtf8());
    MostraEvento(msg,Color::FG_BLUE);
    std::vector<std::string> processi=mSystem(msg );
    if(processi.size() >1){
        MostraErrore("processo gia in esecuzione");
        errore=true;
    }

    //netstat -an | grep :6100
    ////LA PORTA È LIBERA///////
    int tentativi=10;
    msg="netstat -an | grep :";
    msg.append(intToString(getPlcListenPort()));
    MostraEvento(msg,Color::FG_BLUE);
    for(;;){
        std::vector<std::string> listen=mSystem(msg);
        if(listen.size() >0){
            std::string msg="Porta Occupata ";
            msg.append(intToString(getPlcListenPort()));
            MostraErrore(msg);
            MostraErrore(listen.at(0));
            MostraErrore("\n Riprovo Tra 60 Secondi ");
            sleep(60);
            if(tentativi==0){
               errore=true;
            }
            tentativi--;
        }else
            break;
    }

    QString user=Init->at(SET_UTENTE_LOCALE);
    QString pass=Init->at(SET_PASS_LOCALE);
    QString host=Init->at(SET_HOST);

    tentativi=20;
    for(;;){
      if(ConnettiDB(NOME_DB, user,pass,host)){
          std::string msg="MI Al DB connetto in ";
          msg.append(intToString(60*(20-tentativi)) );
          MostraEvento( msg,Color::FG_DEFAULT);
          break;
      }if(tentativi==0){
         errore=true;

      }
      tentativi--;
      sleep(60);
    }

    setTipoApp(CTipoApp::Server);
    if(errore){
       MostraErrore("Non è possibile Far Partire il Plc");
       return a.exec();
    }
    CCiclo Processo(CTipoApp::Server,Impianto,argc, argv);
    CLoopPlc *Monitorizza= new CLoopPlc(&Processo,host,user,pass,-1);
    Monitorizza->start();

    return a.exec();
}



