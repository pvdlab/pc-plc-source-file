#include "DisegnoMacchina.h"
#include <QtWidgets>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "qmiomenu.h"
#include "MainCostanti.h"

///MONTOR RONZONI 640X480

#define PI 3.14159265
//È RIFERITO AL FOGLIO DI SOLIDWORKS
//CON SOLIDWORKS H SAREBBE SUFFICINTE 600 MA QUI 630
#define W_FOGLIO         800
//#define hFoglio         630
#define INTERCAPEDINE    12
#define PENNA            2
#define RESISTENZA_COUNT 4
#define FONT_SIZE        16
#define PUNTI_CAVI 3
#define TAVOLA_LABEL 4
#define TARGET_LABEL 2
#define SENSORI_COUNT 2
#define VENTING_COUNT 1
#define TURBO_COUNT   1
#define VALVOLE_COUNT 2



CDisegnoMacchina::CDisegnoMacchina(QWidget *parent) :
    QWidget(parent)
{


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


void CDisegnoMacchina::Init(QNetPlc *newProcessa){

    getFoglio(&wFoglioDisegno,&hFoglioDisegno);
    Fattore=wFoglioDisegno;
    Fattore/=W_FOGLIO;
    QList <QPunto *> listafile;
    int Index=0;
    QString NoneFile;
    Processa=newProcessa;
    bool Errore=false;

    ResistenzeCount=getResistenzaCount();
    SorgenteCount=Processa->SorgenteCount;


    SensoreCount=SENSORI_COUNT-1;
    SensoreIndex=SensoreCount;
    ValvolaCount=Processa->GasCount+VALVOLE_COUNT-1;
    ValvolaIndex=ValvolaCount;
    ValvolaVenting=ValvolaCount-1;
    pTarget = new QComponente[Processa->SorgenteCount];
    pGas = new QComponente[Processa->GasCount];
    pValvole = new QComponente[Processa->GasCount+VALVOLE_COUNT];
    pRes = new QComponente[ResistenzeCount];
    pSensori = new QComponente[SENSORI_COUNT];
    pTurbo= new QComponente();
    pBias= new QComponente();


    //pVuoto = new QComponente[2*Processa->GasCount+SENSORI_COUNT+VENTING_COUNT+TURBO_COUNT];

    for(int i=0; i<Processa->GasCount;i++){
       pGas[i].pSegnale=Processa->GGas[i].GetG;
       pValvole[i].pSegnale=Processa->GGas[i].Valvola;
    }

    pSensori[0].pSegnale=Processa->Vuoto.Penning.Segnale;
    pSensori[1].pSegnale=Processa->Vuoto.Baratron.Segnale;
    pValvole[Processa->GasCount+1].pSegnale=Processa->Vuoto.Baratron.Valvola;
    pValvole[Processa->GasCount+2].pSegnale=Processa->Vuoto.Camera.Valv;
    pTurbo->pSegnale=Processa->Vuoto.Turbo.Temp;



    NoneFile=":/Componenti/Target.txt";
    listafile=parseFromFile(NoneFile,",");
    dTarget= new QDisegnoComponente();
    dTarget->Centro.append(listafile.at(0));
    for(int i=0;i<listafile.count(); i++)
        dTarget->dData.append(listafile.at(i));

    NoneFile=":/Componenti/Resistenza.txt";
    listafile=parseFromFile(NoneFile,",");
    dResistenza= new QDisegnoComponente();
    dResistenza->Centro.append(listafile.at(0));
    for(int i=0;i<listafile.count(); i++)
        dResistenza->dData.append(listafile.at(i));

    NoneFile=":/Componenti/Camera-" + QString::number(getImpianto()) + ".txt";
    dMacchiana=parseFromFile(NoneFile,",");

    Errore=false;
    listafile=parseFromFile(":/Componenti/Tavola.txt",",");
    if(listafile.count()<2*TAVOLA_LABEL+2){
        dTavola=NULL;
        QMessageBox *msgBox  =new QMessageBox;
        msgBox->setText("CDisegnoMacchina");
        msgBox->setInformativeText("(Tavola.txt) Tavola Punti Insufficenti" );
        msgBox->exec();
        Errore=true;
    }else{
        dTavola = new QDisegnoTavola();
        dTavola->Centro=listafile.at(0);
        dTavola->Raggio=listafile.at(1)->x-listafile.at(0)->x;
        int Index=2;
        for(int i=0;i<2*TAVOLA_LABEL;i++){
            dTavola->Label.append(listafile.at(Index++));
        }
    }


    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Target-" + QString::number(getImpianto()) + ".txt";
    listafile=parseFromFile(NoneFile,",");
    int punticount=listafile.size();
    punticount/=SorgenteCount;
    Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=new QDisegnoSorgente;
        sorgente->Descrizione="Sorgente [" + QString::number(i) + "]";
        sorgente->Colore=getColore(QTipoStatoComponente::Spento);
        for( int y = 0; y< punticount ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setText("CDisegnoMacchina");
                msgBox->setInformativeText(NoneFile + " Fuori Index" );
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
    NoneFile=":/Componenti/Cavi-Target-" + QString::number(getImpianto()) + ".txt";
    listafile=parseFromFile(NoneFile,",");
    Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=dSorgente.at(i);
        for( int y = 0; y< PUNTI_CAVI ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setText("CDisegnoMacchina");
                msgBox->setInformativeText(NoneFile + " Fuori Index"  );
                msgBox->exec();
                Errore=true;
                break;
            }
            sorgente->Cavi.append(listafile.at(Index++));
        }
    }

    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Label-Cavi-Target-" + QString::number(getImpianto()) + ".txt";
    listafile=parseFromFile(NoneFile,",");
    Index=0;
    for(int i = 0; i< SorgenteCount ;i++){
        QDisegnoSorgente *sorgente=dSorgente.at(i);
        for( int y = 0; y< 2*TARGET_LABEL ;y++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setText("CDisegnoMacchina");
                msgBox->setInformativeText(NoneFile + " Fuori Index" );
                msgBox->exec();
                Errore=true;
                break;
            }
            sorgente->Label.append(listafile.at(Index++));
        }
    }

    Errore=false;
    listafile.clear();
    NoneFile=":/Componenti/Resistenza-" + QString::number(getImpianto()) + ".txt";
    listafile=parseFromFile(NoneFile,",");
    punticount=listafile.size();
    punticount/=ResistenzeCount;
    Index=0;
    for(int i=0; i< ResistenzeCount; i++){
        QDisegnoResistenza *resistenza=new QDisegnoResistenza;
        resistenza->Colore=getColore(QTipoStatoComponente::Spento);
        for(int j=0 ; j< punticount;j++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setText("CDisegnoMacchina");
                msgBox->setInformativeText(NoneFile + " Fuori Index"  );
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
    NoneFile=":/Componenti/Cavi-Gas-" + QString::number(getImpianto()) + ".txt";
    listafile=parseFromFile(NoneFile,",");
    dCaviGas=new QDisegnoCaviGas();
    Index=0;
    for(int i=0; i< Processa->GasCount; i++){
        QCavo *pCavo= new QCavo(QTipoCavoGas::Gas);
        pCavo->Colore=getColore(QTipoStatoComponente::Spento);
        for(int j=0 ; j< PUNTI_CAVI;j++){
            if(Index>=listafile.count()){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setText("CDisegnoMacchina");
                msgBox->setInformativeText(NoneFile + " Fuori Index"  );
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
            msgBox->setText("CDisegnoMacchina");
            msgBox->setInformativeText(NoneFile + " Fuori Index"  );
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
             msgBox->setText("CDisegnoMacchina");
             msgBox->setInformativeText(NoneFile + " Fuori Index" );
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
             msgBox->setText("CDisegnoMacchina");
             msgBox->setInformativeText(NoneFile + " Fuori Index" );
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
    listafile=parseFromFile(NoneFile,",");
    Index=0;
    dTurbo= new QDisegnoComponente();
    dTurbo->Centro.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Label.append(listafile.at(Index++));
    dTurbo->Colore=getColore(QTipoStatoComponente::Spento);
    dTurbo->dData.append(dTurbo->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dTurbo->dData.append(listafile.at(Index++));


    listafile.clear();
    NoneFile=":/Componenti/ValvolaGas.txt";
    listafile=parseFromFile(NoneFile,",");
    Index=0;
    dValvola= new QDisegnoComponente();
    dValvola->Centro.append(listafile.at(Index++));
    dValvola->Centro.append(listafile.at(Index++));
    dValvola->Colore=getColore(QTipoStatoComponente::Spento);
    dValvola->dData.append(dValvola->Centro.at(0));
    for(int i=Index;i<listafile.count(); i++)
        dValvola->dData.append(listafile.at(Index++));

    listafile.clear();
    NoneFile=":/Componenti/Flussimetro.txt";
    listafile=parseFromFile(NoneFile,",");
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
    listafile=parseFromFile(NoneFile,",");
    Index=0;
    dSensore= new QDisegnoComponente();
    dSensore->Centro.append(listafile.at(Index++));
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
    TimerTavola->start(1000);
    TavolaGradi=0;
}

QList <QPunto *> CDisegnoMacchina::parseFromFile(const QString &filename, const QString &codec)
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
        msgBox->setText("CDisegnoMacchina");
        msgBox->setInformativeText(filename + " File Inesistente" );
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



void CDisegnoMacchina::Edit(){

    QColor  Colore;
    if(Processa->Resistenza.PowErr->Segnale()){
         Colore=getColore(QTipoStatoComponente::Errore);
         for(int i=0;i< ResistenzeCount;i++)
             pRes[i].Colore=Colore;
    }else{
        if(Processa->Resistenza.ReleDx->Segnale())
              Colore=Qt::green;
        else
              Colore=QColor(51,0,0,255);
        for(int i=0;i< ResistenzeCount;i++)
            pRes[i].Colore=Colore;
    }

    if(Processa->Vuoto.Turbo.Errore->Segnale()){
        pTurbo->Colore=getColore(QTipoStatoComponente::Errore);
    }else{
        if(Processa->Vuoto.Turbo.Start->Segnale()){
            if(Processa->Vuoto.Turbo.Acc->Segnale()){
                pTurbo->Colore=getColore(QTipoStatoComponente::Init);
                for(int i=0;i< dCaviGas->Cavo.count();i++){
                    dCaviGas->Cavo[i]->Colore=getColore(QTipoStatoComponente::Init);
                }
            }else{
                pTurbo->Colore=getColoreVuoto();
                for(int i=0;i< dCaviGas->Cavo.count();i++){
                    dCaviGas->Cavo[i]->Colore=getColoreVuoto();
                }
            }
        }else
           pTurbo->Colore=getColore(QTipoStatoComponente::Spento);
    }


    for(int i=0; i<SorgenteCount;i++){
        bool errore=Processa->Bias->Sorgente[i].Catodo->ErrPow->Segnale();
        if(errore){
           QDisegnoSorgente *pSorgente =dSorgente.at(i);
           pSorgente->Colore=getColore(QTipoStatoComponente::Errore);
           dSorgente.replace(i,pSorgente);
           pTarget[i].Colore=pSorgente->Colore;
        }else{
            bool IsOnArc=Processa->Bias->Sorgente[i].Catodo->IsOnArc->Segnale();
            bool OnPow=Processa->Bias->Sorgente[i].Catodo->OnPow->Segnale();
            QDisegnoSorgente *pSorgente =dSorgente.at(i);
            if(Processa->Bias->Sorgente[i].Enabled){
                if(OnPow & IsOnArc)
                    pSorgente->Colore=getColore(QTipoStatoComponente::Acceso);
                else if(OnPow & !IsOnArc)
                    pSorgente->Colore=getColore(QTipoStatoComponente::Init);
                else
                    pSorgente->Colore=getColore(QTipoStatoComponente::Spento);
            }else
                pSorgente->Colore=getColore(QTipoStatoComponente::Spento);
            dSorgente.replace(i,pSorgente);
            pTarget[i].Colore=pSorgente->Colore;
        }

    }
    update();

}




void CDisegnoMacchina::mousePressEvent(QMouseEvent *event){

    QPoint p = this->mapFromGlobal(QCursor::pos());
    QString px = QString::number(p.x());
    QString py = QString::number(p.y());
    QString x = QString::number(event->pos().x());
    QString y = QString::number(event->pos().y());
    //qDebug() this->t
    if(pTurbo->Rect.contains(event->pos().x(),event->pos().y())){
       qDebug() << "Turbo";
       Processa->AddMsg(CTipoMessaggio::SetComando,CTipoComando::Vacuum);
       pTurbo->Colore=Qt::darkYellow;
       update();
    }

    for(int i=0 ;i<Processa->GasCount+VALVOLE_COUNT; i++){
        if(pValvole[i].Rect.contains(event->pos().x(),event->pos().y())){
           qDebug() << pValvole[i].Rect.x() << pValvole[i].Rect.y() << pValvole[i].Rect.width() << pValvole[i].Rect.height();
           qDebug() << pValvole[i].Descrizione << i << event->pos().x() << event->pos().y();
           if(i==ValvolaVenting){
               if(!Processa->pData.EnabledVenting)
                  Processa->AddMsg(CTipoMessaggio::SetComando,CTipoComando::StopVuoto);
               else
                  Processa->AddMsg(CTipoMessaggio::SetComando,CTipoComando::Ventila);
               pTurbo->Colore=Qt::darkYellow;
               update();
           }
           break;
        }

    }

    for(int i=0 ;i<Processa->SorgenteCount; i++){
        if(pTarget[i].Rect.contains(event->x(),event->y())){
           qDebug() << pTarget[i].Rect.x() << pTarget[i].Rect.y() << pTarget[i].Rect.width() << pTarget[i].Rect.height();
           qDebug() << pTarget[i].Descrizione << i << event->x() << event->y();
           break;
        }
    }
   /* qMioMenu dialog(Processa);
    dialog.exec();
    if(dialog.StartProcesso)
       emit StartProcesso();*/


}


void CDisegnoMacchina::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);
    wFoglio=this->width();
    hFoglio=this->height();
    xTrasla=wFoglio;
    xTrasla/=2;
    xTrasla/=Fattore;
    yTrasla=hFoglio;
    yTrasla/=Fattore;
    yTrasla/=2;


    DisegnaMacchina();
    DisegnaComponente(dCaviGas->Turbo,0,dTurbo,pTurbo);

    for(int i=0;i< dCaviGas->Cavo.count();i++){
        DisegnaComponentiVuoto(i);
    }

    for(int i=0; i<Processa->GasCount;i++){
        DisegnaTesto(pGas[i].Label.at(0),pGas[i].Label.at(1),
                     QString::number(pGas[i].pSegnale->DSegnale(), 'f',1) ,Qt::black);
        DisegnaTesto(pGas[i].Label.at(2),pGas[i].Label.at(3),
                     pGas[i].pSegnale->Descrizione() ,Qt::black);
    }


    for(int i = 0; i< SorgenteCount ;i++){
         DisegnaCatodo(i,true);
         DisegnaTesto(dSorgente.at(i)->Label.at(0),dSorgente.at(i)->Label.at(1),
                      QString::number(Processa->Bias->Sorgente[i].Catodo->GetI->DSegnale(), 'f',1) + " A",Qt::black);
         DisegnaTesto(dSorgente.at(i)->Label.at(2),dSorgente.at(i)->Label.at(3),
                      QString::number(Processa->Bias->Sorgente[i].Catodo->OnTime(), 'f',1) + " Ah",Qt::black);
    }

    for(int i=0; i< ResistenzeCount;i++){
       DisegnaResistenza(i);
    }


    DisegnaTavola();
    DisegnaTavolaLabel();

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

    if(Processa->Vuoto.Meccanica.PowIsOn->Segnale()){
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
        if(Processa->Vuoto.Turbo.Acc->Segnale())
           gradient.setColorAt(1, getColore(QTipoStatoComponente::Init));
        else
           gradient.setColorAt(1, getColoreVuoto());
        painter.fillPath(path,  gradient);
    }


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

    painter.drawLine(0,hFoglio/Fattore,wFoglio/2,hFoglio/Fattore);

}

void CDisegnoMacchina::DisegnaCatodoLabel(int Index){

    QDisegnoSorgente *pCatodo;
    pCatodo=dSorgente.at(Index);

    DisegnaTesto(pCatodo->Label.at(0),pCatodo->Label.at(1),
                 QString::number(Processa->Bias->Sorgente[Index].Catodo->GetI->DSegnale(), 'f',1) + " A",Qt::black);
    DisegnaTesto(pCatodo->Label.at(2),pCatodo->Label.at(3),
                 QString::number(Processa->Bias->Sorgente[Index].Catodo->OnTime(), 'f',1) + " Ah",Qt::black);

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
    pen.setWidth(INTERCAPEDINE-1);
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

    QPen pen(Qt::black);
    pen.setWidth(PENNA);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

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
    //qDebug() << plabel->Rect.x() << plabel->Rect.y() << plabel->Rect.width() << plabel->Rect.height();

    for(int i=0;i<pComponente->Label.count();i++){
        QPunto * pTo= new QPunto(pCentro->x+pComponente->Label.at(i)->x,
                                 (pCentro->y+pComponente->Label.at(i)->y));
        plabel->Label.append(pTo);
    }

    if(plabel->pSegnale!=NULL)
       plabel->Descrizione=QString(plabel->pSegnale->Descrizione());

}



QColor CDisegnoMacchina::getColoreVuoto(){

    return QColor(152,204,255,255);
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


void CDisegnoMacchina::DisegnaTavolaLabel(){

    DisegnaTesto(dTavola->Label.at(0),dTavola->Label.at(1),QString::number(Processa->Bias->GetV->DSegnale()) + " V",Qt::black);
    DisegnaTesto(dTavola->Label.at(2),dTavola->Label.at(3),QString::number(Processa->Bias->GetI->DSegnale()) + " A",Qt::black);
    DisegnaTesto(dTavola->Label.at(4),dTavola->Label.at(5), QString::number(Processa->Resistenza.Temp->DSegnale()) + " °C",Qt::red);
    DisegnaTesto(dTavola->Label.at(6),dTavola->Label.at(7), QString::number(Processa->Vuoto.Camera.Temp->DSegnale()) + " °C",Qt::red);

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

void CDisegnoMacchina::DisegnaTavola(){

return;
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


    QRadialGradient g(diam/2, diam/2, diam*0.4,
        diam*0.4, diam*0.4);

    QColor Colore;
    //int tipostep=Processa->pData.TipoStep;
    if(Processa->pData.TipoStep==CTipoStep::Riscaldo){
        Colore=QColor(51,0,0,255);
    }else if((Processa->pData.TipoStep==CTipoStep::Coating) | (Processa->pData.TipoStep==CTipoStep::Etching))
        Colore=Qt::yellow;
    else{
       if(Processa->Vuoto.Tavola.Errore())
           Colore=Qt::red;
       else{
           if(Processa->Vuoto.Tavola.OnOff->Segnale())
               Colore=Qt::green;
           else
               Colore=Qt::lightGray;
       }

    }




    g.setColorAt(0, Qt::white);
    g.setColorAt(1, Colore);
    QBrush brush(g);
    painter.setPen(Colore);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(brush);
    painter.drawEllipse(QPointF(0,0), diam, diam);
    painter.drawEllipse(QPointF(0,0), 10, 10);

    int dsat=dTavola->Raggio/4;
    QRadialGradient g1(dsat/2, dsat/2, dsat*0.4,
        dsat*0.4, dsat*0.4);
    painter.setPen(Qt::black);
    int angolo=60;
    diam-=dTavola->Raggio*0.4;
    for(int i=0 ; i<6;i++){
        double x,y;
        x=diam*cos(angolo*PI/180);
        y=diam*sin(angolo*PI/180);
      /*  g.setColorAt(0, Colore);
        g.setColorAt(1, Colore);
        QBrush brush(g1);
        painter.setPen(Colore);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(brush);*/
        painter.drawEllipse(QPointF(x,y), dsat, dsat);
        angolo+=60;
    }

}

void CDisegnoMacchina::resizeEvent(QResizeEvent *event){

    image = new QImage( this->width(),this->height(),QImage::Format_RGB16);
    image->fill(Qt::lightGray);

    update();
    QWidget::resizeEvent(event);
}

void CDisegnoMacchina::TavolaTimeout()
{

    if(Processa->Vuoto.Tavola.OnOff->Segnale()){
        DisegnaTavola();
        update();
        TavolaGradi+=10;
        if(TavolaGradi>=360)
           TavolaGradi=0;
        dTavolaColore=getColore(QTipoStatoComponente::Acceso);
    }

}


