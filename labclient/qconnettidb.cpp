#include "qconnettidb.h"
#include <QTimer>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qdebug.h>

QConnettiDB::QConnettiDB(CNetDb *newpNet,QObject *parent)
    : QThread(parent)
{
    DbDati=NULL;
    DbProcesso=NULL;
    pNet=newpNet;

}


void QConnettiDB::run(){


    if(DbDati!=NULL){
        DbDati->close();
        delete DbDati;
        DbDati->removeDatabase("DATI");
    }

    if(DbProcesso!=NULL){
        DbProcesso->close();
        delete DbProcesso;
        if(pNet->Impianto=="6012"){
           DbProcesso->removeDatabase("PROCESSO-" + pNet->Impianto );
           DbProcesso->removeDatabase("PROCESSO-" + pNet->Impianto );
        }else
           DbProcesso->removeDatabase("PROCESSO-" + pNet->Impianto );
    }

    QString timeout="MYSQL_OPT_CONNECT_TIMEOUT";

    if(!pNet->IsRidotta){
        DbProcesso = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","PROCESSO"));
        if(DbProcesso!=NULL){
            DbProcesso->setConnectOptions(timeout + "=" + QString::number(20));
            QString nome;
            if(pNet->Impianto=="6012")
                nome="PROCESSO-" + pNet->Impianto ;
            else
                nome="PROCESSO-" + pNet->Impianto;
            DbProcesso->setDatabaseName(nome);
            if(pNet->IsLocale)
               DbProcesso->setHostName("localhost");
            else
               DbProcesso->setHostName(pNet->DbProcessoHost);
            DbProcesso->setPort(pNet->DbProcessoPorta.toInt());
            if (!DbProcesso->open(pNet->DbProcessoClient, pNet->DbProcessoPass)) { //DataBase->lastError().text()
               emit VersioneRidotta(true);
            }else
               emit VersioneRidotta(false);
        }else{
            QMessageBox msgbox;
            msgbox.setIcon(QMessageBox::Critical);
            msgbox.setWindowTitle("QConnettiDB");
            msgbox.setText("IMPOSSIBILE CREARE DbProcesso");
            msgbox.exec();
            emit VersioneRidotta(false);
        }

    }else
        emit VersioneRidotta(true);


    DbDati = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","DATI"));
    DbDati->setConnectOptions(timeout + "=" + QString::number(20));
    DbDati->setDatabaseName("DATI");
    DbDati->setHostName(pNet->DbDatiHost);
    DbDati->setPort(pNet->DbDatiPorta.toInt());
    if (!DbDati->open(pNet->DbDatiClient, pNet->DbDatiPass)) { //DataBase->lastError().text()
       emit mostraErrore(DbDati->lastError().text());
       return;
    }else{
       QSqlQuery *queryEvento= new QSqlQuery(*DbDati);
       queryEvento->exec("SELECT * FROM EVENTO");
       //qDebug() << queryEvento->numRowsAffected();
       QSqlQuery *queryData= new QSqlQuery(*DbDati);
       if(queryData->exec("SELECT MAX( ID_CICLO ) FROM CICLO")){
           queryData->next();
           QString idCiclo=queryData->record().value("MAX( ID_CICLO )").toString();
           if(!queryData->exec("SELECT * FROM DATA WHERE ID_CICLO = " + idCiclo + " GROUP BY TEMPO LIMIT 0, 5000 ")){
              qDebug() << "SELECT * FROM DATA WHERE ID_CICLO = " + idCiclo + " GROUP BY TEMPO LIMIT 0, 5000 ";
              qDebug() << queryData->lastError().text();
              emit mostraErrore(queryData->lastError().text());

           }else{
              qDebug() << queryData->numRowsAffected();
           }
       }
       QSqlQuery *QueryLeek= new QSqlQuery(*DbDati);
       QueryLeek->exec("SELECT * FROM LEEK");
       //qDebug() << QueryLeek->numRowsAffected();
       emit Connesso(DbDati,DbProcesso,queryEvento,queryData,QueryLeek);

    }

}


