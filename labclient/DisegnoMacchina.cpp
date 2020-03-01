#include "DisegnoMacchina.h"
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "qmiomenu.h"
#include "MainCostanti.h"
#include "qsetcompoente.h"
#include "qsetting.h"

///MONTOR RONZONI 640X480

#define PI 3.14159265
//È RIFERITO AL FOGLIO DI SOLIDWORKS

#define W_FOGLIO            820
#define INTERCAPEDINE       12
#define PENNA               2
#define RESISTENZA_COUNT    4
#define FONT_SIZE           18
#define PUNTI_CAVI          3
#define TAVOLA_LABEL        4
#define TARGET_LABEL        2
#define SENSORI_COUNT       2
#define VENTING_COUNT       1
#define TURBO_COUNT         1
#define VALVOLE_COUNT       2

#define PROVA_PULSANTI


CDisegnoMacchina::CDisegnoMacchina(QWidget *parent) :
    QWidget(parent)
{

    IdMacchina=-1;
    ProcessoIsEnabled=true;
    IdProcesso=0;

}

CDisegnoMacchina::~CDisegnoMacchina()
{
    delete [] pTarget;
    delete [] pGas;
    delete [] pValvole;
    delete [] pRes;
    delete [] pSensori;
    delete pTurbo;
    delete pBias;
    delete dCaviGas;
    dSorgente.clear();
    dRiscaldo.clear();

}

void CDisegnoMacchina::Init(QNetPlc *newProcessa,int newIndex){

    getFoglio(&wFoglioDisegno,&hFoglioDisegno);
    Fattore=wFoglioDisegno;
    Fattore/=W_FOGLIO;
    QList <QPunto *> listafile;
    QString NoneFile;
    Processa=newProcessa;
    bool Errore=false;
    Manuale=true;
    IdMacchina=newIndex;
    QString mImpianto=QString::number(getImpianto());
    if(mImpianto=="6012")
       mImpianto="6010";


    ResistenzeCount=getResistenzaCount();
    SorgenteCount=Processa->SorgenteCount;


    SensoreCount=SENSORI_COUNT-1;
    SensoreIndex=SensoreCount;
    ValvolaCount=Processa->GasCount+VALVOLE_COUNT-1;
    ValvolaIndex=ValvolaCount;
    ValvolaVenting=ValvolaCount;
    ValvolaBaratron=ValvolaCount-1;
    Baratron=1;
    Penning=0;
    pTarget = new QComponente[Processa->SorgenteCount];
    pGas = new QComponente[Processa->GasCount];
    pValvole = new QComponente[Processa->GasCount+VALVOLE_COUNT];
    pRes = new QComponente[ResistenzeCount];
    pSensori = new QComponente[SENSORI_COUNT];
    pTurbo= new QComponente();
    pBias= new QComponente();

    for(int i=0; i<Processa->GasCount;i++){
       pGas[i].pSegnale=Processa->GGas[i].GetG;
       pValvole[i].pSegnale=Processa->GGas[i].Valvola;
    }

    pSensori[0].pSegnale=Processa->Vuoto[IdMacchina].Penning->Segnale();
    pSensori[1].pSegnale=Processa->Vuoto[IdMacchina].Baratron.Segnale;
    pValvole[Processa->GasCount].pSegnale=Processa->Vuoto[IdMacchina].Baratron.Valvola;
    pValvole[Processa->GasCount+1].pSegnale=Processa->Vuoto[IdMacchina].Camera.Valv;
    pTurbo->pSegnale=Processa->Vuoto[IdMacchina].Turbo.Temp;


    NoneFile=":/Componenti/Target.txt";
    listafile=PuntiDaFile(NoneFile,",");
    dTarget= new QDisegnoComponente();
    dTarget->Centro.append(listafile.at(0));
    for(int i=0;i<listafile.count(); i++)
        dTarget->dData.append(listafile.at(i));

    NoneFile=":/Componenti/Resistenza.txt";
    listafile=PuntiDaFile(NoneFile,",");
    dResistenza= new QDisegnoComponente();
    dResistenza->Centro.append(listafile.at(0));
    for(int i=0;i<listafile.count(); i++)
        dResistenza->dData.append(listafile.at(i));

    NoneFile=":/Componenti/Camera-" + mImpianto + ".txt";
    dMacchiana=PuntiDaFile(NoneFile,",");

    NoneFile=":/Componenti/Label.txt";
    dLabel=PuntiDaFile(NoneFile,",");

    Errore=false;
    listafile=PuntiDaFile(":/Componenti/Tavola.txt",",");
    if(listafile.count()<2*TAVOLA_LABEL+1){
        dTavola=NULL;
        QMessageBox *msgBox  =new QMessageBox;
        msgBox->setWindowTitle("CDisegnoMacchina");
        msgBox->setText("(Tavola.txt) Tavola Punti Insufficenti" );
        msgBox->exec();
        Errore=true;
    }else{
        dTavola = new QDisegnoTavola();
        QPunto *centro=new QPunto(0,0);
        dTavola->Centro=centro;
        dTavola->Raggio=listafile.at(0)->x;
        int Index=1;
        for(int i=0;i<2*TAVOLA_LABEL;i++){
            dTavola->Label.append(listafile.at(Index++));
        }
    }


    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Target-" + mImpianto + ".txt";
    listafile=PuntiDaFile(NoneFile,",");
    int punticount=listafile.size();
    punticount/=SorgenteCount;
    int Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=new QDisegnoSorgente;
        sorgente->Descrizione="Sorgente [" + QString::number(i) + "]";
        sorgente->Colore=getColore(QTipoStatoComponente::Spento);
        for( int y = 0; y< punticount ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            sorgente->Target.append(listafile.at(Index++));
        }
        dSorgente.append(sorgente);
    }


    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Cavi-Target-" + mImpianto + ".txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=dSorgente.at(i);
        for( int y = 0; y< PUNTI_CAVI ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            sorgente->Cavi.append(listafile.at(Index++));
        }
    }

    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Label-Cavi-Target-" + mImpianto + ".txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=dSorgente.at(i);
        for( int y = 0; y< 2*TARGET_LABEL ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            sorgente->Label.append(listafile.at(Index++));
        }
    }

    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Resistenza-" + mImpianto + ".txt";
    listafile=PuntiDaFile(NoneFile,",");
    punticount=listafile.size();
    punticount/=ResistenzeCount;
    Index=0;
    for(int i=0; i< ResistenzeCount; i++){
        QDisegnoResistenza *resistenza=new QDisegnoResistenza;
        resistenza->Colore=getColore(QTipoStatoComponente::Spento);
        for(int j=0 ; j< punticount;j++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            resistenza->dData.append(listafile.at(Index++));
        }
        if(!Errore)
           dRiscaldo.append(resistenza);
    }



    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Cavi-Gas-" + mImpianto + ".txt";
    listafile=PuntiDaFile(NoneFile,",");
    dCaviGas=new QDisegnoCaviGas();
    Index=0;
    for(int i=0; i< Processa->GasCount; i++){
        QCavo *pCavo= new QCavo(QTipoCavoGas::Gas);
        pCavo->Colore=getColore(QTipoStatoComponente::Spento);
        for(int j=0 ; j< PUNTI_CAVI;j++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            if(!Errore)
              pCavo->dData.append(listafile.at(Index++));
        }
        dCaviGas->Cavo.append(pCavo);
    }


    Errore=false;
    QCavo *pCavo= new QCavo(QTipoCavoGas::Sensore);
    pCavo->Colore=getColore(QTipoStatoComponente::Spento);
    for(int j=0 ; j< PUNTI_CAVI;j++){
        if(Index>=listafile.count()){
            QMessageBox *msgBox  =new QMessageBox;
            msgBox->setWindowTitle("CDisegnoMacchina");
            msgBox->setText(NoneFile + " Fuori Index" );
            msgBox->exec();
            Errore=true;
            break;
        }
        if(!Errore)
          pCavo->dData.append(listafile.at(Index++));
    }


    dCaviGas->Cavo.append(pCavo);

    Errore=false;
    pCavo= new QCavo(QTipoCavoGas::SensoreValvola);
    pCavo->Colore=getColore(QTipoStatoComponente::Spento);
    for(int j=0 ; j< PUNTI_CAVI;j++){
         if(Index>=listafile.count()){
             QMessageBox *msgBox  =new QMessageBox;
             msgBox->setWindowTitle("CDisegnoMacchina");
             msgBox->setText(NoneFile + " Fuori Index" );
             msgBox->exec();
             Errore=true;
             break;
        }
        if(!Errore)
          pCavo->dData.append(listafile.at(Index++));
    }
    if(!Errore)
       dCaviGas->Cavo.append(pCavo);

    Errore=false;
    pCavo= new QCavo(QTipoCavoGas::Valvola);
    pCavo->Colore=getColore(QTipoStatoComponente::Spento);
    for(int j=0 ; j< PUNTI_CAVI;j++){
        if(Index>=listafile.count()){
            QMessageBox *msgBox  =new QMessageBox;
            msgBox->setWindowTitle("CDisegnoMacchina");
            msgBox->setText(NoneFile + " Fuori Index" );
            msgBox->exec();
            Errore=true;
            break;
        }
        if(!Errore)
          pCavo->dData.append(listafile.at(Index++));
    }
    if(!Errore)
      dCaviGas->Cavo.append(pCavo);
    dCaviGas->Turbo=listafile.at(listafile.size()-1);


    listafile.clear();
    NoneFile=":/Componenti/Turbo.txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    dTurbo= new QDisegnoComponente();
    dTurbo->Centro.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Colore=getColore(QTipoStatoComponente::Spento);
    dTurbo->dData.append(dTurbo->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dTurbo->dData.append(listafile.at(Index++));

    NoneFile=":/Componenti/Icone.txt";
    dBottoni=PuntiDaFile(NoneFile,",");
    QPunto *pnto=dTurbo->Label.at(2);
    pnto->y+=dCaviGas->Turbo->y;
    dBottoni.append(pnto);
    pnto=dTurbo->Label.at(3);
    pnto->y+=dCaviGas->Turbo->y;
    dBottoni.append(pnto);


    listafile.clear();
    NoneFile=":/Componenti/ValvolaGas.txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    dValvola= new QDisegnoComponente();
    dValvola->Centro.append(listafile.at(Index++));
    dValvola->Centro.append(listafile.at(Index++));
    dValvola->Label.append(listafile.at(Index++));
    dValvola->Label.append(listafile.at(Index++));
    dValvola->Colore=getColore(QTipoStatoComponente::Spento);
    dValvola->dData.append(dValvola->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dValvola->dData.append(listafile.at(Index++));


    listafile.clear();
    NoneFile=":/Componenti/Flussimetro.txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    dFlussimetro= new QDisegnoComponente();
    dFlussimetro->Centro.append(listafile.at(Index++));
    dFlussimetro->Centro.append(listafile.at(Index++));
    dFlussimetro->Label.append(listafile.at(Index++));
    dFlussimetro->Label.append(listafile.at(Index++));
    dFlussimetro->Label.append(listafile.at(Index++));
    dFlussimetro->Label.append(listafile.at(Index++));
    dFlussimetro->Colore=getColore(QTipoStatoComponente::Spento);
    dFlussimetro->dData.append(dFlussimetro->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dFlussimetro->dData.append(listafile.at(Index++));

    listafile.clear();
    NoneFile=":/Componenti/Sensore.txt";
    listafile=PuntiDaFile(NoneFile,",");
    Index=0;
    dSensore= new QDisegnoComponente();
    dSensore->Centro.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Label.append(listafile.at(Index++));
    dSensore->Colore=getColore(QTipoStatoComponente::Spento);
    dSensore->dData.append(dSensore->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dSensore->dData.append(listafile.at(Index++));


    TimerTavola = new QTimer(this);
    QObject::connect(TimerTavola, SIGNAL(timeout()),
                     this, SLOT(TavolaTimeout()));
    TimerTavola->start(500);
    TavolaGradi=0;
    RuotaGradi=0;
}

QList <QPunto *> CDisegnoMacchina::PuntiDaFile(const QString &filename, const QString &codec)
{
    QString string;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        if (!codec.isEmpty()) {
          in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
        }
        string = in.readAll();
        file.close();
    }else{
        QMessageBox *msgBox  =new QMessageBox;
        msgBox->setWindowTitle("CDisegnoMacchina");
        msgBox->setText(filename + " File Inesistente" );
        msgBox->exec();
    }

    if(string.length()!=0){
        string.replace("\r\n", "\n");
        if (string.at(string.size()-1) != '\n') {
            string += '\n';
        }
    }

    enum State {Normal, Quote} state = Normal;
    QList <QPunto *> data;
    QStringList line;
    QString value;
    QChar mcpdec=codec.at(0);

    for (int i = 0; i < string.size(); i++) {
        QChar current = string.at(i);

        // Normal
        if (state == Normal) {
            // newline
            if (current == '\n') {
                // add value
                line.append(value);
                value.clear();
                // add line
                QPunto *punto=new QPunto;
                //value.toDouble();
                //punto->TipoComponente =line.at(0).toInt() ;
                punto->x=line.at(1).toDouble() ;
                punto->y=line.at(2).toDouble() ;
                data.append(punto);
                line.clear();
            }
            // comma
            else if (current == mcpdec) { //else if (current == ',') {
                // add line
                line.append(value);
                value.clear();
            }
            // double quote
            else if (current == '"') {
                state = Quote;
            }
            // character
            else {
                value += current;
            }
        }
        // Quote
        else if (state == Quote) {
            // double quote
            if (current == '"') {
                int index = (i+1 < string.size()) ? i+1 : string.size();
                QChar next = string.at(index);
                if (next == '"') {
                    value += '"';
                    i++;
                } else {
                    state = Normal;
                }
            }
            // other
            else {
                value += current;
            }
        }
    }

    return data;
    //return parse(initString(string),codec);
}

QList <QMacchinaCavi *> CDisegnoMacchina::MacchinaCaviDaFile(const QString &filename, const QString &codec)
{
    QString string;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        if (!codec.isEmpty()) {
          in.setCodec(QTextCodec::codecForName(codec.toLatin1()));
        }
        string = in.readAll();
        file.close();
    }else{
        QMessageBox *msgBox  =new QMessageBox;
        msgBox->setWindowTitle("CDisegnoMacchina");
        msgBox->setText(filename + " File Inesistente" );
        msgBox->exec();
    }

    if(string.length()!=0){
        string.replace("\r\n", "\n");
        if (string.at(string.size()-1) != '\n') {
            string += '\n';
        }
    }

    enum State {Normal, Quote} state = Normal;
    QList <QMacchinaCavi *> data;
    QStringList line;
    QString value;
    QChar mcpdec=codec.at(0);

    for (int i = 0; i < string.size(); i++) {
        QChar current = string.at(i);

        // Normal
        if (state == Normal) {
            // newline
            if (current == '\n') {
                // add value
                line.append(value);
                value.clear();
                // add line
                QMacchinaCavi *cavi=new QMacchinaCavi;
                cavi->Descrizione=line.at(0) ;
                cavi->File=line.at(1) ;
                cavi->LabelIndex=line.at(2).toInt() ;
                cavi->TargetIndex=line.at(3).toInt() ;
                cavi->GasIndex=line.at(4).toInt() ;
                cavi->ValvolaIndex=line.at(5).toInt() ;
                cavi->SensoreIndex=line.at(6).toInt() ;
                data.append(cavi);
                line.clear();
            }
            // comma
            else if (current == mcpdec) { //else if (current == ',') {
                // add line
                line.append(value);
                value.clear();
            }
            // double quote
            else if (current == '"') {
                state = Quote;
            }
            // character
            else {
                value += current;
            }
        }
        // Quote
        else if (state == Quote) {
            // double quote
            if (current == '"') {
                int index = (i+1 < string.size()) ? i+1 : string.size();
                QChar next = string.at(index);
                if (next == '"') {
                    value += '"';
                    i++;
                } else {
                    state = Normal;
                }
            }
            // other
            else {
                value += current;
            }
        }
    }

    return data;
    //return parse(initString(string),codec);
}

void CDisegnoMacchina::Edit(){

    if(!Processa->IsVisibile)
        return;

    if(Processa->pData[IdMacchina].PowerIsEnabled){
        for(int i=0; i< Processa->GasCount;i++){
            Processa->GGas[i].SetG->DSegnale(Processa->GGas[i].SetG->Segnale()*Processa->GGas[i].SetG->Fattore());
            Processa->GGas[i].GetG->DSegnale(Processa->GGas[i].GetG->Segnale()*Processa->GGas[i].GetG->Fattore());
        }
    }

    Processa->Vuoto[IdMacchina].Turbo.Temp->DSegnale(Processa->Vuoto[IdMacchina].Turbo.Temp->Segnale()*
                                                     Processa->Vuoto[IdMacchina].Turbo.Temp->Fattore());
    Processa->Vuoto[IdMacchina].Camera.Temp->DSegnale(Processa->Vuoto[IdMacchina].Camera.Temp->Segnale()*
                                                     Processa->Vuoto[IdMacchina].Camera.Temp->Fattore());
    Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->DSegnale(Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->Segnale()*
                                                     Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->Fattore());

    if(Processa->CamereCount>1){
        if(IdMacchina==0){
           if(Processa->pData[1].ProcessoIsOn)
               ProcessoIsEnabled=false;
           else
               ProcessoIsEnabled=true;
        }else{
            if(Processa->pData[0].ProcessoIsOn)
                ProcessoIsEnabled=false;
            else
                ProcessoIsEnabled=true;
        }
    }

    QColor  Colore;
    if(Processa->Vuoto[IdMacchina].Resistenza->PowErr->Segnale()){
         Colore=getColore(QTipoStatoComponente::Errore);
         for(int i=0;i< ResistenzeCount;i++)
             pRes[i].Colore=Colore;
    }else{
        if(Processa->Vuoto[IdMacchina].Resistenza->ReleDx->Segnale()){
              Colore=Qt::red;;
              Processa->pData[IdMacchina].TipoStep=CTipoStep::Riscaldo;
        }else
              Colore=Qt::green;
        for(int i=0;i< ResistenzeCount;i++)
            pRes[i].Colore=Colore;
    }

    if(Processa->Vuoto[IdMacchina].Turbo.Errore->Segnale()){
        pTurbo->Colore=getColore(QTipoStatoComponente::Errore);
    }else{
        if(IdMacchina==0)
            IdMacchina=0;
        int Stato=Processa->pData[IdMacchina].StatoVuoto;
        bool interlock=Processa->Vuoto[IdMacchina].Camera.Interlock->Segnale();
        if(((Stato == CStatoMacchina::Vacuum) || (Stato == CStatoMacchina::InVacuum))){
            QColor ColoreGas=getColoreVuoto();
            if(Processa->Vuoto[IdMacchina].Turbo.Acc->Segnale()){
                ColoreCamera=getColoreVuoto();
                pTurbo->Colore=getColore(QTipoStatoComponente::Init);
            }else{
                pTurbo->Colore=ColoreGas;
            }
            if(Processa->pData[IdMacchina].PowerIsEnabled){
                for(int i=0; i< Processa->GasCount;i++)
                    pGas[i].Colore=getColore(QTipoStatoComponente::Init);
                for(int i=0; i< Processa->GasCount;i++){
                    if(Processa->GGas[i].SetG->Segnale()){
                        if(ProcessoIsEnabled){
                            ColoreGas=getColoreGas(i);
                            pGas[i].Colore=ColoreGas;
                        }
                        break;
                    }
                }
            }

            pTurbo->Colore=ColoreGas;
            for(int i=0;i< dCaviGas->Cavo.count();i++){
                dCaviGas->Cavo[i]->Colore=ColoreGas;
            }
            if(Processa->Vuoto[IdMacchina].Baratron.Enabled){
               if(Processa->Vuoto[IdMacchina].Baratron.Valvola->Segnale()){
                   pValvole[ValvolaBaratron].Colore=ColoreGas;
                   pSensori[Baratron].Colore=ColoreGas;
               }
            }
            pSensori[Penning].Colore=ColoreGas;
            for(int i=0; i< Processa->GasCount;i++){
                if(Processa->GGas[i].Valvola->Segnale())
                    pValvole[i].Colore=ColoreGas;
                else
                    pValvole[i].Colore=getColore(QTipoStatoComponente::Init);
            }
            ColoreCamera=ColoreGas;
        }else{
           pTurbo->Colore=getColore(QTipoStatoComponente::Spento);
           for(int i=0; i< Processa->GasCount;i++)
               pGas[i].Colore=getColore(QTipoStatoComponente::Init);
           if(Processa->Vuoto[IdMacchina].Baratron.Enabled)
              pSensori[Baratron].Colore=getColore(QTipoStatoComponente::Init);
           pSensori[Penning].Colore=getColore(QTipoStatoComponente::Init);
           pValvole[ValvolaVenting].Colore=getColore(QTipoStatoComponente::Init);
           ColoreCamera=Qt::white;
           for(int i=0;i< dCaviGas->Cavo.count();i++){
               dCaviGas->Cavo[i]->Colore=ColoreCamera;
           }
           for(int i=0; i< Processa->GasCount;i++)
               pGas[i].Colore=getColore(QTipoStatoComponente::Init);
        }
    }

    if(Processa->pData[IdMacchina].PowerIsEnabled){
        for(int i=0; i<SorgenteCount;i++){
            QColor Colore;
            bool errore=Processa->Bias[IdMacchina].Sorgente[i].Catodo->ErrPow->Segnale();
            if(errore){
               QDisegnoSorgente *pSorgente =dSorgente.at(i);
               Colore=getColore(QTipoStatoComponente::Errore);
               pSorgente->Colore=Colore;
               pTarget[i].Colore=Colore;
            }else{
                bool IsOnArc=Processa->Bias[IdMacchina].Sorgente[i].Catodo->IsOnArc->Segnale();
                //if(Processa->Bias[IdMacchina].Sorgente[i].SetI==0)
                //    IsOnArc=false;
                bool OnPow=Processa->Bias[IdMacchina].Sorgente[i].Catodo->OnPow->Segnale();
                QDisegnoSorgente *pSorgente =dSorgente.at(i);
                if(Processa->Bias[IdMacchina].Sorgente[i].Enabled & ProcessoIsEnabled){
                    if(OnPow & IsOnArc)
                        Colore=getColore(QTipoStatoComponente::Acceso);
                    else if(OnPow & !IsOnArc)
                        Colore=getColore(QTipoStatoComponente::Init);
                    else
                        Colore=getColore(QTipoStatoComponente::Spento);
                }else
                    pSorgente->Colore=getColore(QTipoStatoComponente::Spento);
                pSorgente->Colore=Colore;
                pTarget[i].Colore=Colore;
            }

        }
    }


    update();

}



void CDisegnoMacchina::MouseEvento_Manuale(QMouseEvent *event){

    if(Processa->pData[IdMacchina].StatoMacchina!= CStatoMacchina::Manuale)
        return;

    //RESISTENZE
    if(BottoneRect(dTavola->Label.at(4),dTavola->Label.at(5)).contains(event->pos().x(),event->pos().y())){
        QSetCompoente Dialog(Processa,IdMacchina,Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza,Processa->Vuoto[IdMacchina].Resistenza->TempSet(),500,"RESISTENZA");
        Dialog.exec();
    }


    //BIAS
    if(BottoneRect(dTavola->Label.at(0),dTavola->Label.at(1)).contains(event->pos().x(),event->pos().y())){
        if(Processa->Bias[IdMacchina].SetV->Segnale()==0){
          for(int i=0; i< Processa->SorgenteCount; i++)
             Processa->Bias[IdMacchina].Sorgente[i].Enabled=false;
        }
        CIOSegnale *pSegnale=Processa->Bias[IdMacchina].SetV;
        QSetCompoente Dialog(Processa,IdMacchina,pSegnale);
        Dialog.exec();
    }


    //GAS
    for(int i=0; i< Processa->GasCount;i++){
        if(pGas[i].Rect.contains(event->pos().x(),event->pos().y())){
            CIOSegnale *pSegnale=Processa->GGas[i].SetG;
            QSetCompoente Dialog(Processa,IdMacchina,pSegnale);
            Dialog.exec();
        }
    }

    //SORGENTI
    for(int i=0; i< Processa->SorgenteCount ;i++){
        bool IsGasZero=true;
        for(int j=0; j< Processa->GasCount;j++){
            if(Processa->GGas[j].SetG->Segnale()!=0)
                IsGasZero=false;
        }
        if(pTarget[i].Rect.contains(event->pos().x(),event->pos().y())){
            if(IsGasZero){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText("Operazione non ammessa Gas = 0 " );
                msgBox->exec();
                return;
            }
           for(int j=0; j< Processa->SorgenteCount ;j++){
                //CSorgente    Sorgente_i=Processa->Bias[IdMacchina].Sorgente[i];
                if((Processa->Bias[IdMacchina].Sorgente[j].SetI->Slot()==Processa->Bias[IdMacchina].Sorgente[i].SetI->Slot())
                        & (Processa->Bias[IdMacchina].Sorgente[j].SetI->Point()==Processa->Bias[IdMacchina].Sorgente[i].SetI->Point())){
                    //CSorgente    Sorgente_j=Processa->Bias[IdMacchina].Sorgente[j];
                    if((Processa->Bias[IdMacchina].Sorgente[i].SetI->Segnale()!=0) & (i!=j)){
                        QMessageBox *msgBox  =new QMessageBox;
                        msgBox->setWindowTitle("CDisegnoMacchina");
                        msgBox->setText("Operazione non ammessa Sorgente Impeganta" );
                        msgBox->exec();
                        return;
                    }
                    
                }
            }
            if(Processa->Bias[IdMacchina].SetV->Segnale()==0){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText("Operazione non ammessa Bias is 0 " );
                msgBox->exec();
             }else{
                CIOSegnale *pSegnale=Processa->Bias[IdMacchina].Sorgente[i].SetI;
                QSetCompoente Dialog(Processa,IdMacchina,pSegnale);
                Dialog.exec();
             }

        }
    }


}


void CDisegnoMacchina::mousePressEvent(QMouseEvent *event){

    MouseEvento_Bottoni(event);


    if(Processa->pData[IdMacchina].ProcessoIsOn)
        return;

    MouseEvento_Manuale(event);

/*    if(event->button()==Qt::RightButton){
       qDebug() << "ecco" ;
       for(int i=0; i<Processa->SorgenteCount; i++){
           QDisegnoSorgente *pCatodo;
           pCatodo=dSorgente.at(i);
           if(pTarget[i].Rect.contains(event->pos().x(),event->pos().y())){
               QMessageBox *msgBox  =new QMessageBox;
               msgBox->setWindowTitle("CDisegnoMacchina");
               msgBox->setText( QString(Processa->Bias[IdMacchina].Sorgente[i].SetI->Descrizione()) +  " Cambio Target?");
               msgBox->setIcon(QMessageBox::Critical);
               msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
               msgBox->setDefaultButton(QMessageBox::Yes);
               if(msgBox->exec()==QMessageBox::Yes){
                   QByteArray msg(Processa->SorgenteCount,0);
                   char *Buffer = msg.data();
                   for(int j=0; j<Processa->SorgenteCount; j++){
                       if(Processa->Bias[IdMacchina].Sorgente[j].Catodo->IsTargetConsumato(Processa->Bias[IdMacchina].Sorgente[j].SetI)){
                           Buffer[j]=1;
                       }else
                           Buffer[j]=2;
                   } //AddMsg(int Primo,int Macchina, int Secondo, QByteArray msg)
                   Processa->AddMsg(CTipoMessaggio::CambiaTarget,IdMacchina,Buffer);
               }
           }
       }
    }*/

    for(int i=0; i<Processa->SorgenteCount; i++){
        QDisegnoSorgente *pCatodo;
        pCatodo=dSorgente.at(i);
        if(Processa->Bias[IdMacchina].Sorgente[i].Catodo->IsTargetConsumato(Processa->Bias[IdMacchina].Sorgente[i].SetI) | (event->button()==Qt::RightButton)){
            if(BottoneRect(pCatodo->Label.at(2),pCatodo->Label.at(3)).contains(event->pos().x(),event->pos().y())){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText( QString(Processa->Bias[IdMacchina].Sorgente[i].SetI->Descrizione()) +  " Cambio Target?");
                msgBox->setIcon(QMessageBox::Critical);
                msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox->setDefaultButton(QMessageBox::Yes);
                if(msgBox->exec()==QMessageBox::Yes){
                    QByteArray msg(Processa->SorgenteCount,0);
                    char *Buffer = msg.data();
                    if((event->button()==Qt::RightButton)){
                        for(int j=0; j<Processa->SorgenteCount; j++)
                            Buffer[j]=2;;
                        Buffer[i]=1;
                    }else{
                        for(int j=0; j<Processa->SorgenteCount; j++){
                            if(Processa->Bias[IdMacchina].Sorgente[j].Catodo->IsTargetConsumato(Processa->Bias[IdMacchina].Sorgente[j].SetI) ){
                                Buffer[j]=1;
                            }else
                                Buffer[j]=2;
                        } //AddMsg(int Primo,int Macchina, int Secondo, QByteArray msg)
                    }

                    Processa->AddMsg(CTipoMessaggio::CambiaTarget,IdMacchina,Buffer);
                }
            }
       }
    }

    if(BottoneRect(dLabel.at(MacchinaLabel::Processo),
                   dLabel.at(MacchinaLabel::Processo+1)).contains(event->pos().x(),event->pos().y())){
        qMioMenu dialog(Processa,IdMacchina);
        dialog.exec();
        ProcessoNome=dialog.ProcessoNome;
        int ProcessoId=dialog.ProcessoId;
        QByteArray NomeFile=ProcessoNome.toUtf8().data();
        NomeFile.resize(UTENTE_SIZE);
        Processa->AddMsg(CTipoMessaggio::LoadDaFileProcesso,IdMacchina,ProcessoId,NomeFile);
        Processa->queryData.clear();
        return;
    }




}

void CDisegnoMacchina::BottoniDisegna(QRect rect ,QString str){

    QPainter painter(this);

  /*  painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect, 4, 4);
    QPen pen(Qt::black, 1);
    painter.setPen(pen);
    painter.fillPath(path, Qt::white);
    painter.drawPath(path);*/

    QPixmap pixmap(str);
    painter.drawPixmap(rect, pixmap);

}


void CDisegnoMacchina::BottoniDisegna(){

    ///SETTING
    if(!Processa->pData[IdMacchina].ProcessoIsOn){
       QRect rect (BottoneRect(dBottoni.at(MacchinaBottoni::Setting),dBottoni.at(MacchinaBottoni::Setting+1)));
       BottoniDisegna( rect,":/images/Macchina/Setting.png");
    }
    //return;

  /*  if(IdMacchina==1){
        DataType      pData=Processa->pData[IdMacchina];
        IdMacchina=1;
    }
    else{
       DataType      pData=Processa->pData[IdMacchina];
       IdMacchina=0;
    }*/

    ////VUOTO
    if(!Processa->Vuoto[IdMacchina].Camera.PortaChiusa->Segnale() | Processa->pData[IdMacchina].ProcessoIsOn ){
        QRect rect (BottoneRect(dBottoni.at(MacchinaBottoni::VuotoStart),dBottoni.at(MacchinaBottoni::VuotoStart+1)));
        BottoniDisegna( rect,":/images/Macchina/on-button-1.png");
    }else{
        QRect rect (BottoneRect(dBottoni.at(MacchinaBottoni::VuotoStart),dBottoni.at(MacchinaBottoni::VuotoStart+1)));
        if((Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::InStopVuoto  )){
            BottoniDisegna( rect,":/images/Macchina/on-button-3.png");
        }else if((Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::EnabledVent )
                | (Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::StopVuoto ) ){
            BottoniDisegna( rect,":/images/Macchina/on-button-3.png");
        }else{
            BottoniDisegna( rect,":/images/Macchina/on-button-2.png");
        }
    }

    ///VENTING
 /*   if(!Processa->Vuoto[IdMacchina].Camera.PortaChiusa->Segnale() | Processa->pData[IdMacchina].ProcessoIsOn ){
        QRect rect(BottoneRect(dBottoni.at(MacchinaBottoni::Venting),dBottoni.at(MacchinaBottoni::Venting+1)));
        BottoniDisegna( rect,":/images/Macchina/on-button-1.png");
    }else{
        QRect rect (BottoneRect(dBottoni.at(MacchinaBottoni::Venting),dBottoni.at(MacchinaBottoni::Venting+1)));
        if((Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::EnabledVent  )){
            BottoniDisegna( rect,":/images/Macchina/on-button-3.png");
        }else{
            BottoniDisegna( rect,":/images/Macchina/on-button-1.png");
        }
    }*/

    ///PROCESSO
    if(Processa->pData[IdMacchina].ProcessoIsOn | (Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::PreSet)){
        QRect rect(BottoneRect(dBottoni.at(MacchinaBottoni::ProcessoStart),dBottoni.at(MacchinaBottoni::ProcessoStart+1)));
        BottoniDisegna( rect,":/images/Macchina/on-button-2.png");
    }else{
        QRect rect(BottoneRect(dBottoni.at(MacchinaBottoni::ProcessoStart),dBottoni.at(MacchinaBottoni::ProcessoStart+1)));
        if(Processa->pData[IdMacchina].ProcessoIsEnabled
                & (Processa->pData[IdMacchina].StatoMacchina!=CStatoMacchina::Manuale) & !Processa->pData[IdMacchina].ProcessoCorrotto) {
           BottoniDisegna( rect,":/images/Macchina/on-button-3.png");
        }else{
            BottoniDisegna( rect,":/images/Macchina/on-button-1.png");
        }
        //rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::ProcessoScegli),dBottoni.at(MacchinaBottoni::ProcessoScegli+1)));
        //BottoniDisegna( rect,":/images/Macchina/sort-down-2.png");
    }

    ///MANULE AUTOMATICO
    if(Processa->pData[IdMacchina].ProcessoIsEnabled & !Processa->pData[IdMacchina].ProcessoIsOn & (Processa->pData[IdMacchina].StatoMacchina!=CStatoMacchina::PreSet)) {
        QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Manuale),dBottoni.at(MacchinaBottoni::Manuale+1)));
        if(Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::Manuale)
           BottoniDisegna( rect,":/images/Macchina/hold-2.png");
        else
           BottoniDisegna( rect,":/images/Macchina/hold-3.png");
    }else{
        QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Manuale),dBottoni.at(MacchinaBottoni::Manuale+1)));
        BottoniDisegna( rect,":/images/Macchina/hold-1.png");
    }

    ////ERRORE
    if(Processa->pData[IdMacchina].ErrCount){ //if(Processa->pData[IdMacchina].ErrPreProcesso | Processa->pData[IdMacchina].ErrProcesso){
        QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Errore),dBottoni.at(MacchinaBottoni::Errore+1)));
        BottoniDisegna( rect,":/images/Macchina/error-2.png");
    }else{
        QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Errore),dBottoni.at(MacchinaBottoni::Errore+1)));
        BottoniDisegna( rect,":/images/Macchina/error-1.png");

    }

    ////MESSAGGI
    if(!Processa->VersioneRidotta){
        if(Processa->pData[IdMacchina].EventoProcessoCount){
            QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Note),dBottoni.at(MacchinaBottoni::Note+1)));
            BottoniDisegna( rect,":/images/Macchina/question-2.png");
        }else{
            QRect rect = QRect(BottoneRect(dBottoni.at(MacchinaBottoni::Note),dBottoni.at(MacchinaBottoni::Note+1)));
            BottoniDisegna( rect,":/images/Macchina/question-1.png");
        }
    }



}


void CDisegnoMacchina::MouseEvento_Bottoni(QMouseEvent *event){

    if(Processa->pData[IdMacchina].ProcessoIsOn & (event->button()==Qt::RightButton)){
        for(int i=0; i< Processa->SorgenteCount ;i++){
            if(pTarget[i].Rect.contains(event->pos().x(),event->pos().y())){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("CDisegnoMacchina");
                msgBox->setText( QString(Processa->Bias[IdMacchina].Sorgente[i].SetI->Descrizione()) +  " Spengo ");
                msgBox->setIcon(QMessageBox::Critical);
                msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox->setDefaultButton(QMessageBox::Yes);
                if(msgBox->exec()==QMessageBox::Yes){
                    Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::SpegniCatodo,i);
                }
            }
         }
    }

    /// VUOTO////
    if(BottoneRect(dBottoni.at(MacchinaBottoni::VuotoStart),dBottoni.at(MacchinaBottoni::VuotoStart+1)).contains(event->pos().x(),event->pos().y())){
        //int stato=Processa->pData[IdMacchina].StatoMacchina;
        if(!Processa->Vuoto[IdMacchina].Camera.PortaChiusa->Segnale() | Processa->pData[IdMacchina].ProcessoIsOn){
            ;
        }else{
            if((Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::EnabledVent )
                    | (Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::StopVuoto )
                    | (Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::InStopVuoto )
                    | (Processa->pData[IdMacchina].StatoVuoto==CStatoMacchina::Ventila )){
                Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::Vacuum);
                pTurbo->Colore=Qt::darkYellow;
                update();
            }else{
                Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::StopVuoto);
                update();
            }
        }
    }

    ///VENTING
    for(int i=0 ;i<Processa->GasCount+VALVOLE_COUNT; i++){
        if(pValvole[i].Rect.contains(event->pos().x(),event->pos().y())){
           if(i==ValvolaVenting){
               if(Processa->pData[IdMacchina].ProcessoIsOn){
                   QMessageBox *msgBox  =new QMessageBox;
                   msgBox->setWindowTitle("CDisegnoMacchina");
                   msgBox->setText("Venting Non Ammessa");
                   msgBox->exec();
               }else{
                   if(Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::EnabledVent ){
                      Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::Ventila);
                   }else{
                       if(Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::Ventila){
                           QMessageBox *msgBox  =new QMessageBox;
                           msgBox->setWindowTitle("CDisegnoMacchina");
                           msgBox->setText("Comando Venting Avviato");
                           msgBox->exec();
                       }else{
                           QMessageBox *msgBox  =new QMessageBox;
                           msgBox->setWindowTitle("CDisegnoMacchina");
                           msgBox->setText("Venting Non Ammessa Attendere");
                           msgBox->exec();
                       }
                   }
               }
           }
           break;
        }
    }

    ///START PROCESSO///
    if(BottoneRect(dBottoni.at(MacchinaBottoni::ProcessoStart),dBottoni.at(MacchinaBottoni::ProcessoStart+1)).contains(event->pos().x(),event->pos().y())){
        if(Processa->pData[IdMacchina].ProcessoIsOn){
            QMessageBox *msgBox  =new QMessageBox;
            msgBox->setWindowTitle("CDisegnoMacchina");
            msgBox->setText( "Fermare il processo " + QString(Processa->pData[IdMacchina].File.ProcessoNome) + " ? ");
            msgBox->setIcon(QMessageBox::Critical);
            msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            msgBox->setDefaultButton(QMessageBox::No);
            if(msgBox->exec()==QMessageBox::Yes){
               Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::StopProcesso);
               emit StartProcesso();
            }

        }else{
            if(Processa->pData[IdMacchina].ProcessoIsEnabled ) {
                QByteArray NomeFile=Processa->pData[IdMacchina].File.ProcessoNome;
                int id=Processa->pData[IdMacchina].File.ProcessoId;
                NomeFile.resize(UTENTE_SIZE);
                QString path = QDir::homePath() + "/run" + PLC_PROCESSO_PATH + "/";
                path+= QString::number(id)+ "_" + QString(NomeFile) + "_"+ QString::number(IdMacchina)
                        + "_" + QString::number(getImpianto())
                                        + PLC_PROCESSO_EXT;
                qDebug() << path;
                if(!QFile::exists(path)){
                    QMessageBox *msgBox  =new QMessageBox;
                    msgBox->setWindowTitle("CDisegnoMacchina");
                    msgBox->setText( path + " File Inesistente");
                    msgBox->setIcon(QMessageBox::Critical);
                    msgBox->exec();

                }else{
                    Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::StartProcesso,
                                     id,NomeFile);
                    emit StartProcesso();

                }


            }
        }
    }

    ///MANUALE///
    if(BottoneRect(dBottoni.at(MacchinaBottoni::Manuale),dBottoni.at(MacchinaBottoni::Manuale+1)).contains(event->pos().x(),event->pos().y())){
        if(Processa->pData[IdMacchina].ProcessoIsEnabled & !Processa->pData[IdMacchina].ProcessoIsOn) {
            if(Processa->pData[IdMacchina].StatoMacchina==CStatoMacchina::Manuale){
               Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::Automatico);
               for(int i=0; i< Processa->SorgenteCount; i++)
                   Processa->Bias[IdMacchina].Sorgente[i].Enabled=true;
            }else{
               Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::Manuale);
            }

        }
    }

    ///ERRORE///
    if(Processa->pData[IdMacchina].ErrCount!=0){
        if(BottoneRect(dBottoni.at(MacchinaBottoni::Errore),dBottoni.at(MacchinaBottoni::Errore+1)).contains(event->pos().x(),event->pos().y())){
            emit MostraErrore();
        }
    }


    ///EVENTI//
    if(!Processa->VersioneRidotta){
        if(BottoneRect(dBottoni.at(MacchinaBottoni::Note),dBottoni.at(MacchinaBottoni::Note+1)).contains(event->pos().x(),event->pos().y())){
            emit MostraEvento();
        }
    }

    ///SETMACHINA//
    if(!Processa->pData[IdMacchina].ProcessoIsOn){
        if(BottoneRect(dBottoni.at(MacchinaBottoni::Setting),dBottoni.at(MacchinaBottoni::Setting+1)).contains(event->pos().x(),event->pos().y())){
            QSetting dialog(Processa,IdMacchina);
            dialog.exec();
            //qDebug() << "eccomi";
        }
    }

}


void CDisegnoMacchina::paintEvent(QPaintEvent *event){

    if(IdMacchina==-1)
        return;

    Q_UNUSED(event);
    wFoglio=this->width();
    hFoglio=this->height();
    xTrasla=wFoglio;
    xTrasla/=2;
    xTrasla/=Fattore;
    yTrasla=hFoglio;
    yTrasla/=Fattore;
    yTrasla/=2;



  /*  for(int i=0; i<dBottoni.count();i+=2){
        DisegnaTesto(dBottoni.at(i),dBottoni.at(i+1),
                     QString::number(i) ,Qt::blue);
    }

    return;*/
    //CVuoto pVuoto=Processa->Vuoto[IdMacchina];
    DisegnaMacchina();
    DisegnaComponente(dCaviGas->Turbo,0,dTurbo,pTurbo);
    DisegnaTesto(pTurbo->Label.at(0),pTurbo->Label.at(1), QString::number(Processa->Vuoto[IdMacchina].Turbo.Temp->DSegnale()) + " °C",Qt::red);


    for(int i=0;i< dCaviGas->Cavo.count();i++){
        DisegnaComponentiVuoto(i);
    }

    DisegnaSensoreLabel(Penning);
    if(Processa->Vuoto[IdMacchina].Baratron.Enabled)
      DisegnaSensoreLabel(Baratron);

    if(Processa->pData[IdMacchina].PowerIsEnabled){
        if(ProcessoIsEnabled){
            for(int i=0; i<Processa->GasCount;i++){
                DisegnaTesto(pGas[i].Label.at(0),pGas[i].Label.at(1),
                             QString::number(pGas[i].pSegnale->DSegnale(), 'f',1) ,Qt::black);
                DisegnaTesto(pGas[i].Label.at(2),pGas[i].Label.at(3),
                             pGas[i].pSegnale->Descrizione() ,Qt::black);
            }
        }

        for(int i = 0; i< SorgenteCount ;i++){
             DisegnaCatodo(i,true);
             QDisegnoSorgente *pCatodo;
             pCatodo=dSorgente.at(i);
             if(ProcessoIsEnabled){
                 //=Processa->Bias[IdMacchina].Sorgente[i].Catodo->GetI->Segnale();
                 //DSegnale*=Processa->Bias[IdMacchina].Sorgente[i].Catodo->GetI->Fattore();
                 double DSegnale=Processa->Bias[IdMacchina].Sorgente[i].Catodo->GetI->DSegnale();
                 if(!Processa->Bias[IdMacchina].Sorgente[i].Enabled)
                     DSegnale=0;
                 DisegnaTesto(pCatodo->Label.at(0),pCatodo->Label.at(1),
                              QString::number(DSegnale, 'f',1) + " A",Qt::darkGreen, Qt::white,18);

                 if(Processa->Bias[IdMacchina].Sorgente[i].Catodo->IsTargetConsumato(Processa->Bias[IdMacchina].Sorgente[i].SetI))
                    DisegnaTesto(pCatodo->Label.at(2),pCatodo->Label.at(3),
                              QString::number(Processa->Bias[IdMacchina].Sorgente[i].Catodo->OnTime(), 'f',1) + " Ah",Qt::darkRed, Qt::red,18);
                 else
                     DisegnaTesto(pCatodo->Label.at(2),pCatodo->Label.at(3),
                               QString::number(Processa->Bias[IdMacchina].Sorgente[i].Catodo->OnTime(), 'f',1) + " Ah",Qt::darkGreen, Qt::white,18);
             }
        }
    }


    for(int i=0; i< ResistenzeCount;i++){
       DisegnaResistenza(i);
    }


    DisegnaTavola();
    double DSegnale=Processa->Bias[IdMacchina].DSegnale(Processa->Bias[IdMacchina].GetV);
    DisegnaTesto(dTavola->Label.at(0),dTavola->Label.at(1),
                 QString::number(DSegnale, 'f',2) + " V",Qt::darkGreen, Qt::white,18);
    DSegnale=Processa->Bias[IdMacchina].DSegnale(Processa->Bias[IdMacchina].GetI);
    DisegnaTesto(dTavola->Label.at(2),dTavola->Label.at(3),
                 QString::number(DSegnale, 'f',2) + " A",Qt::darkGreen, Qt::white,18);
    DisegnaTesto(dTavola->Label.at(4),dTavola->Label.at(5),
                 QString::number(Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->DSegnale()) + " °C",Qt::red);
    DisegnaTesto(dTavola->Label.at(6),dTavola->Label.at(7),
                 QString::number(Processa->Vuoto[IdMacchina].Camera.Temp->DSegnale()) + " °C",Qt::red);


    QString Step=QString::number(Processa->pData[IdMacchina].StepIndex);
    Step+= " Step :" + Processa->getTipoStep(0);
    Step+= " " + QString::number(Processa->pData[IdMacchina].Repit);
    Step+= "/" + QString::number(Processa->pData[IdMacchina].RepitCount);

    DisegnaTesto(dLabel.at(MacchinaLabel::MainTime),dLabel.at(MacchinaLabel::MainTime+1),
                 "Plc Tempo "+ QDateTime::fromTime_t(Processa->MainTimer).toUTC().toString("hh:mm:ss"),Qt::blue);

    DisegnaTesto(dLabel.at(MacchinaLabel::Ciclo),dLabel.at(MacchinaLabel::Ciclo+1),
                 "Ciclo : " + QString::number(Processa->pData[IdMacchina].File.Ciclo),Qt::blue);

    if(!Processa->pData[IdMacchina].ProcessoCorrotto){
        DisegnaTesto(dLabel.at(MacchinaLabel::Processo),dLabel.at(MacchinaLabel::Processo+1),
                     QString(Processa->pData[IdMacchina].File.ProcessoNome),Qt::darkBlue, Qt::white,24);
        IdProcesso=Processa->pData[IdMacchina].File.ProcessoId;
    }else{
        DisegnaTesto(dLabel.at(MacchinaLabel::Processo),dLabel.at(MacchinaLabel::Processo+1),
                     "",Qt::darkBlue, Qt::white,24);
        IdProcesso=0;
    }



    DisegnaTesto(dLabel.at(MacchinaLabel::Firma),dLabel.at(MacchinaLabel::Firma+1),
                 "Dr. Riccardo Villa" ,Qt::blue);
    DisegnaTesto(dLabel.at(MacchinaLabel::Sito),dLabel.at(MacchinaLabel::Sito+1),
                 "www.pvdlab.com" ,Qt::blue);
    QColor Colore;
    if(Processa->pData[IdMacchina].TipoStep==CTipoStep::Riscaldo)
        Colore =Qt::white;
    else
        Colore =Qt::darkBlue;


    if(Processa->pData[IdMacchina].ProcessoIsOn){
        DisegnaTesto(dLabel.at(MacchinaLabel::ProcessoPlc),dLabel.at(MacchinaLabel::ProcessoPlc+1),
                     QString(Processa->pData[IdMacchina].File.ProcessoNome),Colore);
        DisegnaTesto(dLabel.at(MacchinaLabel::Step),dLabel.at(MacchinaLabel::Step+1),
                     Step,Colore);
        DisegnaTesto(dLabel.at(MacchinaLabel::MenoStepProcesso),dLabel.at(MacchinaLabel::MenoStepProcesso+1),
                     "Step " + QDateTime::fromTime_t(Processa->pData[IdMacchina].StepTimeStop-Processa->pData[IdMacchina].StepTime).toUTC().toString("hh:mm:ss") ,Colore);
        DisegnaTesto(dLabel.at(MacchinaLabel::MenoTempoProcesso),dLabel.at(MacchinaLabel::MenoTempoProcesso+1),
                     "Proc " + QDateTime::fromTime_t(Processa->pData[IdMacchina].TimeStop-Processa->pData[IdMacchina].Time).toUTC().toString("hh:mm:ss") ,Colore);
    }else{
        DisegnaTesto(dLabel.at(MacchinaLabel::Step),dLabel.at(MacchinaLabel::Step+1),
                     Processa->getStatoMacchina(IdMacchina) ,Colore);

        DisegnaTesto(dLabel.at(MacchinaLabel::MenoStepProcesso),dLabel.at(MacchinaLabel::MenoStepProcesso+1),
                     QDateTime::fromTime_t(Processa->pData[IdMacchina].TimeEventStop-Processa->pData[IdMacchina].TimeEvent ).toUTC().toString("hh:mm:ss") ,Colore);
    }

    BottoniDisegna();

}


void CDisegnoMacchina::DisegnaMacchina(){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);

    QPunto *start,*stop;
    QPen pen(Qt::black);
    pen.setWidth(INTERCAPEDINE);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    QPainterPath path;
    start=dMacchiana.at(0);
    path.moveTo(start->x,hFoglio/Fattore-start->y );
    for(int i=0;i< dMacchiana.count()-1;i++){
        start=dMacchiana.at(i);
        stop=dMacchiana.at(i+1);
        path.lineTo(stop->x,hFoglio/Fattore-stop->y);
    }

    start=stop;
    stop=dMacchiana.at(0);
    path.lineTo(stop->x,hFoglio/Fattore-stop->y);
    QLinearGradient gradient(QPoint(0,-hFoglio/2),QPoint(0,hFoglio/2));
    gradient.setColorAt(0, Qt::darkGray);
    gradient.setColorAt(1, ColoreCamera);
    painter.fillPath(path,  gradient);

    for(int i=0;i< dMacchiana.count()-1;i++){
        start=dMacchiana.at(i);
        stop=dMacchiana.at(i+1);
        painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
    }
    start=stop;
    stop=dMacchiana.at(0);
    painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);

    pen.setColor(Qt::blue);
    pen.setWidth(INTERCAPEDINE-1);
    painter.setPen(pen);
    for(int i=0;i< dMacchiana.count()-1;i++){
        start=dMacchiana.at(i);
        stop=dMacchiana.at(i+1);
        painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
    }
    start=stop;
    stop=dMacchiana.at(0);
    painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);

}


QComponente * CDisegnoMacchina::NextValvola(){

    if(ValvolaIndex>= ValvolaCount)
        ValvolaIndex=0;
    else
        ValvolaIndex++;
    return &pValvole[ValvolaIndex];
}

QComponente * CDisegnoMacchina::NextSensori(){

    if(SensoreIndex>= SensoreCount)
        SensoreIndex=0;
    else
        SensoreIndex++;
    pSensori[SensoreIndex].TipoCavo=QTipoCavoGas::Sensore;
    return &pSensori[SensoreIndex];
}

void CDisegnoMacchina::DisegnaComponentiVuoto(int Index){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);

    QPunto *start,*stop;
    QCavo *pCavo;
    pCavo=dCaviGas->Cavo.at(Index);

    QPen pen(Qt::black);
    pen.setWidth(INTERCAPEDINE);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    for(int i=0;i< pCavo->dData.count()-1;i++){
        start=pCavo->dData.at(i);
        stop=pCavo->dData.at(i+1);
        painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
    }
    pen.setColor(pCavo->Colore);
    pen.setWidth(INTERCAPEDINE-2);
    painter.setPen(pen);
    for(int i=0;i< pCavo->dData.count()-1;i++){
        start=pCavo->dData.at(i);
        stop=pCavo->dData.at(i+1);
        painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
    }


    if(dCaviGas->Cavo.at(Index)->TipoCavo==QTipoCavoGas::Gas){
        QPunto *pCentro = new QPunto();
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dValvola,NextValvola());
        qreal len=dValvola->Centro.at(1)->x-dValvola->Centro.at(0)->x;
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x-len;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x+len;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dFlussimetro,&pGas[Index]);
    }else if(dCaviGas->Cavo.at(Index)->TipoCavo==QTipoCavoGas::Sensore){
        QPunto *pCentro = new QPunto();
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dSensore,NextSensori());
    }else if(dCaviGas->Cavo.at(Index)->TipoCavo==QTipoCavoGas::SensoreValvola){
        QPunto *pCentro = new QPunto();
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dValvola,NextValvola());
        qreal len=dValvola->Centro.at(1)->x-dValvola->Centro.at(0)->x;
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x-len;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x+len;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dSensore,NextSensori());
    }else{
        QPunto *pCentro = new QPunto();
        if(pCavo->dData.at(2)->x<0){
            pCentro->x=pCavo->dData.at(2)->x-dValvola->Centro.at(1)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }else{
            pCentro->x=pCavo->dData.at(2)->x;
            pCentro->y=pCavo->dData.at(2)->y;
        }
        DisegnaComponente(pCentro,0,dValvola,NextValvola());
    }
}

void CDisegnoMacchina::DisegnaCatodo(int Index, bool ConCavi){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);


    QPunto *start,*stop;
    QDisegnoSorgente *pCatodo;
    pCatodo=dSorgente.at(Index);

    if(ConCavi){
        QPen pen(Qt::black);
        pen.setWidth(INTERCAPEDINE);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
        for(int i=0;i< pCatodo->Cavi.count()-1;i++){
            start=pCatodo->Cavi.at(i);
            stop=pCatodo->Cavi.at(i+1);
            painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
        }
        pen.setColor(pCatodo->Colore);
        pen.setWidth(INTERCAPEDINE-1);
        painter.setPen(pen);
        for(int i=0;i< pCatodo->Cavi.count()-1;i++){
            start=pCatodo->Cavi.at(i);
            stop=pCatodo->Cavi.at(i+1);
            painter.drawLine(start->x,hFoglio/Fattore-start->y ,stop->x,hFoglio/Fattore-stop->y);
        }
    }

    double y=pCatodo->Target.at(0)->y-pCatodo->Target.at(1)->y;
    double x=pCatodo->Target.at(0)->x-pCatodo->Target.at(1)->x;
    double Tangente=y;
    Tangente/=x;
    double angolo = atan (Tangente) * (180.0 / PI);
    if(Index>1)
        angolo-=180;
    painter.rotate(-angolo);
    pTarget[Index].Descrizione="Sorgente ["+ QString::number(Index) + "]";
    DisegnaComponente(pCatodo->Target.at(1),angolo,dTarget,&pTarget[Index]);

}


QString CDisegnoMacchina::Descrizione(QComponente pComp){


        if(!pComp.pSegnale)
            return QString(pComp.pSegnale->Descrizione());
        else
           return "";


}

void CDisegnoMacchina::DisegnaComponente(QPunto * pCentro, qreal angolo,QDisegnoComponente  *pComponente,QComponente *plabel){

    QPunto *start,*stop;
    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);

    if(plabel->TipoCavo==QTipoCavoGas::Sensore){
        QPen pen(plabel->Colore);
        pen.setWidth(INTERCAPEDINE);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
    }else{
        QPen pen(Qt::black);
        pen.setWidth(PENNA);
        pen.setCapStyle(Qt::RoundCap);
        painter.setPen(pen);
    }


    plabel->Trasla->x=pCentro->x;
    plabel->Trasla->y=hFoglio/Fattore-pCentro->y;
    plabel->Angolo=-angolo;
    if(plabel->Label.count())
       plabel->Label.clear();
    painter.translate(pCentro->x,hFoglio/Fattore-pCentro->y);
    painter.rotate(-angolo);


    qreal max_y,max_x;
    qreal min_y,min_x;

    max_y=-hFoglio/2;
    max_x=-wFoglio/2;
    min_y=hFoglio/2;
    min_x=wFoglio/2;
    start=pComponente->dData.at(0);

    QPainterPath path;
    path.moveTo(start->x,-start->y );
    for(int i=0;i< pComponente->dData.count()-1;i++){
        start=pComponente->dData.at(i);
        stop=pComponente->dData.at(i+1);
        painter.drawLine(start->x,-start->y ,stop->x,-stop->y);
        path.lineTo(stop->x,-stop->y);
        max_y=qMax(max_y,stop->y);
        max_x=qMax(max_x,stop->x);
        min_y=qMin(min_y,stop->y);
        min_x=qMin(min_x,stop->x);
    }



    start=stop;
    stop=pComponente->dData.at(0);
    painter.drawLine(start->x,-start->y ,stop->x,-stop->y);
    path.lineTo(stop->x,-stop->y);
    QLinearGradient gradient(QPoint(0,-min_y),QPoint(0,-max_y));
    gradient.setColorAt(0, Qt::darkGray);
    gradient.setColorAt(1, plabel->Colore);
    //gradient.setColorAt(1, Qt::blue);
    if(plabel->TipoCavo!=QTipoCavoGas::Sensore)
       painter.fillPath(path,  gradient);


    min_x=pCentro->x+min_x;
    min_x*=Fattore;
    min_x+=wFoglio/2;

    max_x=pCentro->x+max_x;
    max_x*=Fattore;
    max_x+=wFoglio/2;

    min_y=pCentro->y+min_y;
    min_y*=Fattore;
    min_y=hFoglio/2-min_y;

    max_y=pCentro->y+max_y;
    max_y*=Fattore;
    max_y=hFoglio/2-max_y;

    int h=min_y-max_y;
    int w=max_x-min_x;

    plabel->Rect= QRect(min_x,max_y,w,h);

    for(int i=0;i<pComponente->Label.count();i++){
        QPunto * pTo= new QPunto(pCentro->x+pComponente->Label.at(i)->x,
                                 (pCentro->y+pComponente->Label.at(i)->y));
        plabel->Label.append(pTo);
    }

    if(plabel->pSegnale!=NULL)
       plabel->Descrizione=QString(plabel->pSegnale->Descrizione());

}


QRect CDisegnoMacchina::BottoneRect(QPunto *pStart,QPunto *pStop){

    qreal max_y,max_x;
    qreal min_y,min_x;

    min_x=pStart->x;
    min_y=pStart->y;

    max_x=pStop->x;
    max_y=pStop->y;

    min_x*=Fattore;
    min_x+=wFoglio/2;

    max_x*=Fattore;
    max_x+=wFoglio/2;

    min_y*=Fattore;
    min_y=hFoglio/2-min_y;

    max_y*=Fattore;
    max_y=hFoglio/2-max_y;

    int h=min_y-max_y;
    int w=max_x-min_x;

    return QRect(min_x,max_y,w,h);

}


QColor CDisegnoMacchina::getColoreVuoto(){

    QColor Colore =QColor(152,204,255,255);

    return Colore;
}


QColor CDisegnoMacchina::getColoreGas(int Index){

    if(Index==0)
        return QColor(51,51,255,255);
    else if(Index==1)
       return QColor(255,0,255,255);
    else
       return QColor(224,224,224,255);
}


QColor CDisegnoMacchina::getColore(int newstato){

    switch (newstato) {
    case QTipoStatoComponente::Acceso :
        return Qt::green;
        break;
    case QTipoStatoComponente::Spento :
        return Qt::darkGray;
        break;
    case QTipoStatoComponente::Init :
        return Qt::yellow;
        break;
    case QTipoStatoComponente::Errore :
        return Qt::red;
        break;
    default:
        break;
    }
    return Qt::black;
}


void CDisegnoMacchina::DisegnaResistenza(int Index){

    QDisegnoResistenza *dRes;
    dRes=dRiscaldo.at(Index);
    double y=dRes->dData.at(0)->y-dRes->dData.at(1)->y;
    double x=dRes->dData.at(0)->x-dRes->dData.at(1)->x;
    double Tangente=y;
    Tangente/=x;
    double angolo = atan (Tangente) * (180.0 / PI);
    if(Index==0)
        angolo-=180;
    DisegnaComponente(dRes->dData.at(1),angolo,dResistenza,&pRes[Index]);

}

void CDisegnoMacchina::DisegnaSensoreLabel(int Index){

    double pressione;
    if(Index==Penning){
        pressione=Processa->Vuoto[IdMacchina].Penning->Pressione();
    }else{
        pressione=Processa->Vuoto[IdMacchina].Baratron.Pressione();
    }
    QString str=QString::number(pressione, 'e',2);

    int pos=str.indexOf("e");
    QString Prima=str.left(pos) + " 10";
    QString Seconda=str.mid(pos+1,str.length()) ;

    DisegnaTesto(pSensori[Index].Label.at(0),pSensori[Index].Label.at(1),Prima ,Qt::black);
    DisegnaTesto(pSensori[Index].Label.at(4),pSensori[Index].Label.at(5),Seconda ,Qt::black);

}

void CDisegnoMacchina::DisegnaTesto(QPunto * Start,QPunto * Stop,QString Testo ,QColor colore){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);

    QPen pen(colore);
    pen.setWidth(INTERCAPEDINE/5);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    QFont font;
    font.setPixelSize(FONT_SIZE);
    painter.setFont(font);


    QRect rect ;
    rect.adjust(Start->x,hFoglio/Fattore-Start->y,Stop->x,hFoglio/Fattore-Stop->y);
    painter.drawText(rect, Qt::AlignCenter, Testo);
}


void CDisegnoMacchina::DisegnaTesto(QPunto * Start,QPunto * Stop,QString Testo ,QColor Colore,QColor Backcolore, int FontSize){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);


    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    QRect rect ;
    rect.adjust(Start->x,hFoglio/Fattore-Start->y,Stop->x,hFoglio/Fattore-Stop->y);
    path.addRoundedRect(rect, 2, 2);
    QPen pen(Colore, 2);
    painter.setPen(pen);
    painter.fillPath(path, Backcolore);
    painter.drawPath(path);

    pen =QPen(Colore);
    pen.setWidth(INTERCAPEDINE/5);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);
    QFont font;
    font.setPixelSize(FontSize);
    painter.setFont(font);


    painter.drawText(rect, Qt::AlignCenter, Testo);
}
void CDisegnoMacchina::DisegnaTavola(){

    QPainter painter(this);
    painter.resetTransform();
    painter.scale(Fattore,Fattore);
    painter.translate(xTrasla,-yTrasla);
    int diam=dTavola->Raggio;

    QPen pen(Qt::lightGray);
    pen.setWidth(INTERCAPEDINE/3);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    QPunto *centro = dTavola->Centro;
    painter.translate(centro->x,hFoglio/Fattore-centro->y);
    painter.rotate(TavolaGradi);


    //QRadialGradient g(diam/2, diam/2, diam*0.4,
    //    diam*0.4, diam*0.4);
    QRadialGradient g(0, 0, diam,
        diam, diam);

    QColor Colore;
    //int tipostep=Processa->pData[IdMacchina].TipoStep;
    if(Processa->pData[IdMacchina].TipoStep==CTipoStep::Riscaldo){
        Colore=QColor(51,0,0,255);
    }else if((Processa->pData[IdMacchina].TipoStep==CTipoStep::Coating) | (Processa->pData[IdMacchina].TipoStep==CTipoStep::Etching))
        Colore=Qt::yellow;
    else{
       if(Processa->Vuoto[IdMacchina].Tavola.Errore())
           Colore=Qt::red;
       else{
           if(Processa->Vuoto[IdMacchina].Tavola.OnOff->Segnale())
               Colore=Qt::green;
           else
               Colore=Qt::lightGray;
       }

    }


    g.setColorAt(0, Qt::white);
    g.setColorAt(1, Colore);
    QBrush brush(g);
    painter.setPen(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(brush);
    painter.drawEllipse(QPointF(0,0), diam, diam);
    painter.drawEllipse(QPointF(0,0), 10, 10);

    int dsat=dTavola->Raggio/4;
    QRadialGradient g1(0, 0, dsat,
        dsat, dsat);
    painter.setPen(Qt::black);
    int angolo=60;
    diam-=dTavola->Raggio*0.4;
    for(int i=0 ; i<6;i++){
        double x,y;
        x=diam*cos(angolo*PI/180);
        y=diam*sin(angolo*PI/180);
        g1.setColorAt(0, Qt::white);
        g1.setColorAt(1, Colore);
        QBrush brush(g1);
        painter.setPen(Qt::white);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(brush);
        painter.drawEllipse(QPointF(x,y), dsat, dsat);
        //painter.drawEllipse(QPointF(0,0), 2, 2);
        angolo+=60;
    }

    int x=wFoglio/2-dTavola->Raggio*0.7*Fattore;
    int y=hFoglio/2-dTavola->Raggio*0.7;
    int w=2*dTavola->Raggio*0.7*Fattore;

    RectTavola=QRect(x,y,w,w);

}

void CDisegnoMacchina::resizeEvent(QResizeEvent *event){

    image = new QImage( this->width(),this->height(),QImage::Format_RGB16);
    image->fill(Qt::lightGray);

    update();
    QWidget::resizeEvent(event);
}

void CDisegnoMacchina::TavolaTimeout()
{

    if(Processa->Vuoto[IdMacchina].Tavola.OnOff->Segnale()){
        if(Ruota!=Processa->Vuoto[IdMacchina].Tavola.Ruota->Segnale()){
           if(RuotaGradi>=360)
               TimerTavola->stop();
           Ruota=Processa->Vuoto[IdMacchina].Tavola.Ruota;
           RuotaGradi=0;
        }
        RuotaGradi+=10;
        TavolaGradi+=10;
        if(TavolaGradi>=360)
           TavolaGradi=0;
        dTavolaColore=getColore(QTipoStatoComponente::Acceso);
        update();
    }

}


