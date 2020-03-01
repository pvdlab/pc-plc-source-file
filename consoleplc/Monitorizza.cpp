#include "Monitorizza.h"
#include <QSqlError>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "DataFile.h"



QMonitorizza::QMonitorizza(CCiclo *newProcessa,QString newdbhost,QString newdpuser,QString newdbpassword,int newPorta,QObject *parent) : QThread(parent)
{

    Dbuser=newdpuser;
    Dbpassword=newdbpassword;
    Dbhost=newdbhost;
    DbPorta=newPorta;
    Processa=newProcessa;
    IsUpdate=true;
    TempoCount=0;
    TempoMax=0;
    DataBase=NULL;
    Sorgente = new int[Processa->SorgenteCount];

    DataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","UpdateRecenti"));
    if(DataBase==NULL){
       MostraErrore("QMonitorizza DatataBase == NULL");
       return;
    }
    DataBase->setDatabaseName(NOME_DB);
    DataBase->setHostName(Dbhost);
    DataBase->setPort(DbPorta);
    if (!DataBase->open(Dbuser, Dbpassword)) {
        std::string str;
        str.append(DataBase->lastError().text().toUtf8());
        MostraErrore(str);
        return;
    }

    addSegnale(Processa->Vuoto[Processa->IdCiclo].Penning->Segnale());
    addSegnale(Processa->Vuoto[Processa->IdCiclo].Baratron.Segnale);
    addSegnale(Processa->Vuoto[Processa->IdCiclo].Resistenza->Temp_Resistenza);
    for(int i=0; i< Processa->SorgenteCount ; i++)
        addSegnale(Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->GetI);
    for(int i=0; i< Processa->GasCount ; i++)
        addSegnale(Processa->GGas[i].GetG);
    addSegnale(Processa->Bias[Processa->IdCiclo].GetV);
    addSegnale(Processa->Bias[Processa->IdCiclo].GetI);

    QString str="CREATE TABLE IF NOT EXISTS `DATA` (`ID_DATA` int(11) NOT NULL AUTO_INCREMENT, `ID_CICLO` int(11) DEFAULT NULL,"
                "`TEMPO` int(11) DEFAULT NULL,`TIPO_STEP` tinyint(4) DEFAULT 0, ";
    for(int i=0; i< pSegnali.count(); i++){
        if((pSegnali.at(i)->TipoComp==CTipoComDef::PENNING) | (pSegnali.at(i)->TipoComp==CTipoComDef::BARATRON) )
            str+=" `" +QString::fromUtf8(pSegnali.at(i)->Descrizione()) + "` smallint(6) UNSIGNED DEFAULT NULL, ";
        else
            str+=" `" +QString::fromUtf8(pSegnali.at(i)->Descrizione()) + "` tinyint(4) UNSIGNED DEFAULT NULL, ";
    }
    str+=" PRIMARY KEY (`ID_DATA`) ) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;";
    if(DataBase!=NULL){
        QSqlQuery query(*DataBase);
        if (!query.exec(str)){
            std::string str;
            str.append(strUpdate.toUtf8());
            MostraErrore(str);
            str.clear();
            str.append(query.lastError().text().toUtf8());
            MostraErrore(str);
        }
    }

    QString connection;
    connection=DataBase->connectionName();
    DataBase->close();
    delete DataBase;
    QSqlDatabase::removeDatabase(connection);

}

void QMonitorizza::run(){

    for(;;){
        strUpdate="";
        if(Processa->pData[Processa->IdCiclo].ProcessoIsOn ){
           Monitorizza();
        }else
           msleep(500);
    }

}

#define SLEEP  100
#define SECONDI 10
#define MINUTI 600
#define MAX_TEMPO 300 //IN COATING AL MASSIMO OGNIO 30 SECONDI AGGIUNGO
#define DOPO_EVENTO 2

void QMonitorizza::Monitorizza(){

    DataBase = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","UpdateRecenti"));
    if(DataBase==NULL){
       MostraErrore("QMonitorizza DatataBase == NULL");
       return;
    }
    DataBase->setDatabaseName(NOME_DB);
    DataBase->setHostName(Dbhost);
    DataBase->setPort(-1);
    if (!DataBase->open(Dbuser, Dbpassword)) {
        std::string str;
        str.append(DataBase->lastError().text().toUtf8());
        MostraErrore(str);
        Processa->pErrore.Errore(true,E_FILE_CICLO,0);
        return;
    }

    QSqlQuery query(*DataBase);
    query.exec("SELECT MAX(CICLO) FROM CICLO WHERE ANNO = '" + QDate::currentDate().toString("yyyy") + "'");
    if(!query.next())
        Processa->pData[Processa->IdCiclo].File.Ciclo=1;
    else
        Processa->pData[Processa->IdCiclo].File.Ciclo=query.record().value("MAX(CICLO)").toInt()+1;

    QString str="INSERT INTO CICLO ( CICLO,ID_PROCESSO,ANNO,MACCHINA,START)";
    str+=" VALUES ( '" + QString::number(Processa->pData[Processa->IdCiclo].File.Ciclo)  + "', ";
    str+=" '" + QString::number(Processa->pData[Processa->IdCiclo].File.ProcessoId)  + "', ";
    str+=" '" + QDateTime::currentDateTime().toString("yyyy")   + "', ";
    str+=" '" + QString::number(Processa->IdCiclo)  + "', ";
    str+=" '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")  + "') ";
    if(!query.exec(str)){
       std::string msg;
       msg.append(str.toUtf8());
       MostraEvento(msg,Color::BG_RED);
       msg="INSERT INTO CICLO";
       str.append(query.lastError().text().toUtf8());
       MostraErrore(msg);
       Processa->pErrore.Errore(true,E_FILE_CICLO,0);
       qDebug() << str;
       return;
    }
    Processa->pData[Processa->IdCiclo].File.CicloId=query.lastInsertId().toInt();

   /* std::string processo=getRunPath();
    processo.append(PLC_PROCESSO_PATH);
    processo.append(PLC_PROCESSO);
    processo.append(intToString(Processa->pData[Processa->IdCiclo].File.ProcessoId));
    //FILE* fpin = fopen(processo.data(), "r");

    processo=getRunPath();
    processo.append(PLC_CICLO_PATH);
    processo.append(PLC_PROCESSO);
    processo.append(intToString(Processa->pData[Processa->IdCiclo].File.CicloId));
    FILE* fpuot = fopen(processo.data(), "w");
    if (fpin){
        while (!feof(fpin)) {
            char buffer[D_PROCESSO];
            char *pBuffer=buffer;
            fread(pBuffer, D_PROCESSO, 1, fpin);
            fwrite(pBuffer, D_PROCESSO, 1, fpuot);
        }
        fclose(fpuot);
        fclose(fpin);
    }

    processo=getRunPath();
    processo.append(PLC_CICLO_PATH);
    processo.append(PLC_CICLO);
    processo.append(intToString(Processa->pData[Processa->IdCiclo].File.CicloId));
    FILE* fp = fopen(processo.data(), "w");
    if (fp){
        fclose(fp);
    }*/

    if(Processa->pData[Processa->IdCiclo].File.CicloId)
       setEventoProcessoFileName(Processa->pData[Processa->IdCiclo].File.CicloId);
    else{
        MostraErrore("CICLO_ID NULLO");
        Processa->pErrore.Errore(true,E_FILE_CICLO,0);
        return;
    }
    MostraEventoProcessoReset();

    Processa->pData[Processa->IdCiclo].RecordCount=0;
    std::string msg;
    msg.append(" Start Processo ");
    msg.append(Processa->Processo_GetFileNome());
    msg.append(" Ciclo ");
    msg.append(intToString(Processa->pData[Processa->IdCiclo].File.Ciclo));
    msg.append(" Del ");
    msg.append(intToString( QDate::currentDate().toString("yyyy").toInt()));
    MostraEvento(msg,Color::FG_GREEN);

    TempoMax=0;
    m_Timer(SLEEP);
    m_Timer.Reset();
    CicloRecordCount=0;
    IdCiclo=1;
    VediPer=1;
    RecordInit();
    RecordAggiungi();
    Update();
    int oldCTipoStep;
    while (Processa->pData[Processa->IdCiclo].ProcessoIsOn) {
        if(m_Timer.Timer()){
            if(oldCTipoStep!=Processa->pData[Processa->IdCiclo].TipoStep){
               RecordAggiungi();
               Update();
               oldCTipoStep=Processa->pData[Processa->IdCiclo].TipoStep;
            }
            if(Aggiungi())
               RecordAggiungi();
            if((Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Riscaldo) | (Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Venting)){
               Update();
            }else{
               if(CicloRecordCount>5)
                   Update();
            }
            m_Timer.Reset();
        }
        msleep(5);
    }

    while(!IsUpdate){
        msleep(5);
    }

    str="SELECT * FROM  CICLO  WHERE ID_CICLO ='" + QString::number( Processa->pData[Processa->IdCiclo].File.CicloId) + "'";;
    //VERIFICA SE ESISTE IL CICLO ALTRIMENTI CI METTE UNA VITA
    if (query.exec(str)){
        str ="UPDATE CICLO SET STOP = " ;
        str+=" '" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")  + "' " ;
        str+= " WHERE ID_CICLO = '" + QString::number( Processa->pData[Processa->IdCiclo].File.CicloId) + "'";
        if (!query.exec(str)){
            MostraEventoProcesso("Impossibile Aggiornare TAB CICLO",Color::FG_RED);
        }
    }

    QString connection;

    connection=DataBase->connectionName();
    DataBase->close();
    delete DataBase;
    QSqlDatabase::removeDatabase(connection);

    return;
}

bool QMonitorizza::Aggiungi(){

    int mTempoMax=0;

    if(VediPer>1){
       VediPer--;
       mTempoMax=TempoMax;
       goto ContaTempo;
    }

    VediPer=1;
    mTempoMax=MAX_TEMPO;
    if((Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Riscaldo) | (Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Venting)){
        if((Processa->pData[Processa->IdCiclo].StepTime)<15*MINUTI*SLEEP){
           mTempoMax=qMax(TempoMax,MINUTI) ;
        }else{
           mTempoMax=MINUTI*5;
        }
    }else if((Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Etching) | (Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Coating)){
        if(Processa->pData[Processa->IdCiclo].TipoStep==CTipoStep::Etching){
           mTempoMax=qMin(mTempoMax,MINUTI*2);
        }else{
           mTempoMax=qMin(mTempoMax,MINUTI*4);
        }
        if((BiasGetI<(Processa->Bias[Processa->IdCiclo].GetI->Segnale()*0.85)) | (BiasGetI>(Processa->Bias[Processa->IdCiclo].GetI->Segnale()*1.15)) ){
           mTempoMax=qMin(mTempoMax,SECONDI);
           VediPer=DOPO_EVENTO;
           BiasGetI=Processa->Bias[Processa->IdCiclo].GetI->Segnale();
#ifdef MOSTRA_EVENTI_MONITORIZZA
           double percent=BiasGetI-Processa->Bias[Processa->IdCiclo].GetI->Segnale();
           percent/=BiasGetI;
           QString str;
           str+="Bias Varia I Nuovo = ";
           qDebug() << str << QString::number(BiasGetI) << " Vecchio " << QString::number(BiasGetI-Processa->Bias[Processa->IdCiclo].GetI->Segnale()) << " Percent " << QString::number(percent) << "%";
#endif
        }
        int biasSetV=Processa->Bias[Processa->IdCiclo].SetV->Segnale();
        int biasGetV=Processa->Bias[Processa->IdCiclo].SetV->Segnale();
        if((biasGetV<(biasSetV*0.8)) | (biasGetV>(biasSetV*1.2))){
           mTempoMax=qMin(mTempoMax,SECONDI);
           VediPer=DOPO_EVENTO;
#ifdef MOSTRA_EVENTI_MONITORIZZA
           double percent=biasSetV-biasGetV;
           percent/=biasSetV;
           QString str;
           str+="Bias Varia V SetV = ";
           qDebug() << str << QString::number(biasSetV) << " GetV " << QString::number(biasGetV) << " Percent " << QString::number(percent) << "%";
#endif
        }
        for(int i=0; i<Processa->SorgenteCount;i++){
              if(Processa->Bias[Processa->IdCiclo].Sorgente[i].Enabled){
                double geti=Processa->Bias[Processa->IdCiclo].Sorgente[i].Catodo->GetI->Segnale();
                if(Processa->Bias[Processa->IdCiclo].Sorgente[i].SetI->Segnale() & Processa->Bias[Processa->IdCiclo].Sorgente[i].Enabled ){
                    if((geti<Sorgente[i]*0.9) | (geti>Sorgente[i]*1.10)){
                        mTempoMax=qMin(mTempoMax,SECONDI);
                        VediPer=DOPO_EVENTO;
#ifdef MOSTRA_EVENTI_MONITORIZZA
                        double percent=geti-Sorgente[i];
                        percent/=geti;
                        QString str=Processa->Bias[Processa->IdCiclo].Sorgente[i].pErrore.Descrizione();
                        str+=" Varia I Nuovo = ";
                        qDebug() << str << QString::number(geti) << " Vecchio " << QString::number(Sorgente[i]) << " Percent " << QString::number(percent) << "%";
#endif
                    }
                    Sorgente[i]=geti;
                }

            }
        }
        for(int i=0; i<Processa->GasCount;i++){
            if(((Processa->GGas[i].GetG->Segnale()<(Processa->GGas[i].SetG->Segnale()*0.9))) | ((Processa->GGas[i].GetG->Segnale())>(Processa->GGas[i].SetG->Segnale()*1.1))){
                mTempoMax=qMin(mTempoMax,SECONDI);
                VediPer=DOPO_EVENTO;
            }
        }
    }

ContaTempo:
    if(mTempoMax!=0){
        if(mTempoMax!=TempoMax){
           TempoMax=mTempoMax;
           TempoCount=1;
           return true;
        }else{
            if(TempoCount>=TempoMax){
               TempoCount=0;
               return true;
            }
        }
        TempoCount++;
    }

    return false;
}

void QMonitorizza::Update(){

    if((CicloRecordCount==0) | !IsUpdate)
          return;

    IsUpdate=false;
    int pos=strUpdate.lastIndexOf(",");
    strUpdate.replace(pos,1,";");

    QUpdate *thread = new QUpdate(DataBase,strUpdate);
    connect(thread, SIGNAL(isUpdate(bool)),
            this, SLOT(onUpdate(bool)));

    thread->setAutoDelete(false);
    QThreadPool *threadPool = QThreadPool::globalInstance();
    threadPool->start(thread);

    std::string processo=getRunPath();
    processo=getRunPath();
    processo.append(PLC_CICLO_PATH);
    processo.append(PLC_CICLO);
    processo.append(intToString(Processa->pData[Processa->IdCiclo].File.CicloId));
   /* FILE* fp = fopen(processo.data(), "a");
    if (fp){
        ;//fwrite(pBuffer, D_PROCESSO, 1, fp);
    }
    fclose(fp);*/
    RecordInit();
    CicloRecordCount=0;
}

void QMonitorizza::onUpdate(bool newIsUpdate){

    IsUpdate=newIsUpdate;
}

void QMonitorizza::RecordAggiungi(){

    if(CicloRecordCount==0)
      strUpdate+=" VALUES (" ;
    else
      strUpdate+=" (" ;
    strUpdate+="'" + QString::number(Processa->pData[Processa->IdCiclo].File.CicloId) + "' ,";
    strUpdate+="'" + QString::number(Processa->pData[Processa->IdCiclo].Time/1000) + "' ,";
    strUpdate+="'" + QString::number(Processa->pData[Processa->IdCiclo].TipoStep) + "' ,";

    for(int i=0; i< pSegnali.count()-1; i++){
        double val;
        if((pSegnali.at(i)->TipoComp==CTipoComDef::PENNING))
            val=(double)getPenning()*ZIP_PRESSIONE;
        else if( (pSegnali.at(i)->TipoComp==CTipoComDef::BARATRON))
            val=(double)getBaratron()*ZIP_PRESSIONE;
        else
            val=pSegnali.at(i)->getCompZipDouble();
        strUpdate+=" '" + QString::number(val) + "', ";
    }
    double val=pSegnali.at(pSegnali.count()-1)->getCompZipDouble();
    strUpdate+=" '" +QString::number(val) + "'), ";
    CicloRecordCount++;
    Processa->pData[Processa->IdCiclo].RecordCount++;
}

void QMonitorizza::RecordInit(){

    strUpdate="INSERT INTO DATA (ID_CICLO, " ;
    strUpdate+= " TEMPO ,";
    strUpdate+= " TIPO_STEP ,";
    for(int i=0; i< pSegnali.count()-1; i++){
        strUpdate+=" " +QString::fromUtf8(pSegnali.at(i)->Descrizione()) + ", ";
    }
    strUpdate+=" " +QString::fromUtf8(pSegnali.at(pSegnali.count()-1)->Descrizione()) + ") ";
}

void QMonitorizza::addSegnale(CIOSegnale *pNewSegnale){

    if(!pNewSegnale)
        return;

    for(int i=0;i<pSegnali.count();i++){
        if(QString(pSegnali.at(i)->Descrizione())==QString(pNewSegnale->Descrizione())){
            return;
        }
    }
    pSegnali.append(pNewSegnale);
}

void QMonitorizza::Simula(){
    if(IdCiclo!=Processa->pData[Processa->IdCiclo].File.CicloId){
       queryData= new QSqlQuery(*DataBase);
       QString idCiclo=QString::number(Processa->pData[Processa->IdCiclo].File.CicloId);
       if(queryData->exec("SELECT * FROM DATA WHERE ID_CICLO = " + idCiclo + " GROUP BY TEMPO LIMIT 0, 5000 ")){
          queryData->next();
       }
    }

    while(queryData->record().value("TEMPO").toInt()*1000<(int)Processa->pData[Processa->IdCiclo].Time) {
        queryData->next();
    }
    *Processa->Vuoto[Processa->IdCiclo].Penning->Segnale()=queryData->record().value("Penn").toInt();
    *Processa->Vuoto[Processa->IdCiclo].Baratron.Segnale=queryData->record().value("Bara").toInt();
    *Processa->Vuoto[Processa->IdCiclo].Resistenza->Temp_Resistenza=queryData->record().value("Tres").toInt()/Processa->Vuoto[Processa->IdCiclo].Resistenza->Temp_Resistenza->Zip;
    *Processa->Bias[Processa->IdCiclo].GetI=queryData->record().value("BiasGI").toInt()/Processa->Bias[Processa->IdCiclo].GetI->Zip;
    *Processa->Bias[Processa->IdCiclo].GetV=queryData->record().value("BiasGV").toInt()/Processa->Bias[Processa->IdCiclo].GetV->Zip;
    for(int i=0; i< pSegnali.count()-1; i++){
        ;//QString value=QString(pSegnali.at(i)->Descrizione());

       /*
        for(int i=0; i< Processa->SorgenteCount ; i++)
            addSegnale(Processa->Bias[Processa->IdCiclo].Sorgente[i].GetI);
        for(int i=0; i< Processa->GasCount ; i++)
            addSegnale(Processa->GGas[i].GetG);
        addSegnale(Processa->Bias[Processa->IdCiclo].GetV);
        addSegnale(Processa->Bias[Processa->IdCiclo].GetI);*/

    }

}






