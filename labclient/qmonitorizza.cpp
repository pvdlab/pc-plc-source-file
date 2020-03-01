#include "qmonitorizza.h"


QMonitorizza::QMonitorizza(QWidget *parent) :
    QWidget(parent)
{

     QHBoxLayout *horizontalLayout;
     QVBoxLayout *verticalLayoutSx;
     QVBoxLayout *verticalLayoutDx;

     horizontalLayout = new QHBoxLayout(this);
     horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
     verticalLayoutSx = new QVBoxLayout();
     verticalLayoutSx->setObjectName(QStringLiteral("verticalLayoutSx"));

     Vuoto = new QGraficoRiquadro(this);
     Vuoto->setObjectName(QStringLiteral("Vuoto"));
     verticalLayoutSx->addWidget(Vuoto);

     Riscaldamento = new QGraficoRiquadro(this);
     Riscaldamento->setObjectName(QStringLiteral("Riscaldamento"));
     verticalLayoutSx->addWidget(Riscaldamento);

     Gas = new QGraficoRiquadro(this);
     Gas->setObjectName(QStringLiteral("Gas"));
     verticalLayoutSx->addWidget(Gas);

     horizontalLayout->addLayout(verticalLayoutSx);

     verticalLayoutDx = new QVBoxLayout();
     verticalLayoutDx->setObjectName(QStringLiteral("verticalLayoutDx"));

     VBias = new QGraficoRiquadro(this);
     VBias->setObjectName(QStringLiteral("VBias"));
     verticalLayoutDx->addWidget(VBias);

     IBias = new QGraficoRiquadro(this);
     IBias->setObjectName(QStringLiteral("IBias"));
     verticalLayoutDx->addWidget(IBias);


     Catodi = new QGraficoRiquadro(this);
     Catodi->setObjectName(QStringLiteral("Catodi"));
     verticalLayoutDx->addWidget(Catodi);


     horizontalLayout->addLayout(verticalLayoutDx);


     connect(Vuoto, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));
     connect(Riscaldamento, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));
     connect(Gas, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));
     connect(IBias, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));
     connect(VBias, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));
     connect(Catodi, SIGNAL(gotoTempo(int,int)),
              this, SLOT(gotoTempo(int,int)));

     IsEdit=true;
}



QMonitorizza::~QMonitorizza()
{
 /*  delete Vuoto;
   delete Riscaldamento;
   delete Gas;
   delete VBias;
   delete IBias;
   delete Catodi;*/
}


void QMonitorizza::gotoZero(int newtempo){

    Vuoto->scrolltoTempo(0,  newtempo);
    Riscaldamento->scrolltoTempo(0,newtempo);
    Gas->scrolltoTempo(0,newtempo);
    VBias->scrolltoTempo(0,newtempo);
    IBias->scrolltoTempo(0,newtempo);
    Catodi->scrolltoTempo(0,newtempo);

}



void QMonitorizza::gotoTempo(int x, int newtempo){

    if(!IsEdit)
        return;

    Vuoto->scrolltoTempo(x,newtempo);
    Riscaldamento->scrolltoTempo(x,newtempo);
    Gas->scrolltoTempo(x,newtempo);
    VBias->scrolltoTempo(x,newtempo);
    IBias->scrolltoTempo(x,newtempo);
    Catodi->scrolltoTempo(x,newtempo);

    if(!queryData.isActive())
       return;

    QSqlRecord record;
    int tempo;
    if(newtempo>lastTempo){
       while(queryData.next()){
          tempo=queryData.record().value("TEMPO").toInt() ;
          if(tempo >= newtempo ){
             record= queryData.record();
             lastTempo=tempo;
             QThread::msleep(10);
             break;
          }

       }
    }else{
        while(queryData.previous()){
            tempo=queryData.record().value("TEMPO").toInt() ;
            if(tempo <= newtempo ){
               record= queryData.record();
               lastTempo=tempo;
               QThread::msleep(10);
               break;
            }
        }
    }

    Vuoto->LabelX->Edit(record);
    Riscaldamento->LabelX->Edit(record);
    Gas->LabelX->Edit(record);
    VBias->LabelX->Edit(record);
    IBias->LabelX->Edit(record);
    Catodi->LabelX->Edit(record);


}

void QMonitorizza::Edit(int newtime){

    if(newtime==0)
        return;


    Vuoto->Edit(newtime);
    Riscaldamento->Edit(newtime);
    Gas->Edit(newtime);
    IBias->Edit(newtime);
    VBias->Edit(newtime);
    Catodi->Edit(newtime);

    Vuoto->scrollTempo(newtime);
    Riscaldamento->scrollTempo(newtime);

    Gas->scrollTempo(newtime);
    IBias->scrollTempo(newtime);
    VBias->scrollTempo(newtime);
    Catodi->scrollTempo(newtime);

}

void QMonitorizza::Load(qreal newtime,QSqlRecord record){

    Vuoto->Grafico->Load(newtime,record);
    Riscaldamento->Grafico->Load(newtime,record);
    Gas->Grafico->Load(newtime,record);
    IBias->Grafico->Load(newtime,record);
    VBias->Grafico->Load(newtime,record);
    Catodi->Grafico->Load(newtime,record);

}

void QMonitorizza::Init(QNetPlc *newProcessa){

   Q_UNUSED(newProcessa)

}

void QMonitorizza::Clear(){

    Vuoto->Grafico->Clear();
    Riscaldamento->Grafico->Clear();
    Gas->Grafico->Clear();
    IBias->Grafico->Clear();
    VBias->Grafico->Clear();
    Catodi->Grafico->Clear();
}

void   QMonitorizza::append(int index ,CIOSegnale* pSegnale){

    if(!pSegnale)
        return;

    pSegnale->GraficoIndex=index;
    switch (index) {
    case QTipoGrafico::Vuoto :
        Vuoto->append(pSegnale);
        break;
    case QTipoGrafico::Riscaldo:
        Riscaldamento->append(pSegnale);
        break;
    case QTipoGrafico::Gas:
        Gas->append(pSegnale);
        break;
    case QTipoGrafico::LowI :
        IBias->append(pSegnale);
        break;
    case QTipoGrafico::Volt:
        VBias->append(pSegnale);
        break;
    case QTipoGrafico::HeghtI:
        Catodi->append(pSegnale);
        break;
    default:
        break;
    };
}

void QMonitorizza::setTempo(int newTempo){

    Vuoto->Grafico->setTempo(newTempo);
    Riscaldamento->Grafico->setTempo(newTempo);
    Gas->Grafico->setTempo(newTempo);
    IBias->Grafico->setTempo(newTempo);
    VBias->Grafico->setTempo(newTempo);
    Catodi->Grafico->setTempo(newTempo);

    Vuoto->scrollTempo(0);
    Riscaldamento->scrollTempo(0);
    Gas->scrollTempo(0);
    IBias->scrollTempo(0);
    VBias->scrollTempo(0);
    Catodi->scrollTempo(0);

}

void QMonitorizza::addTempo(int newTempo){

    Vuoto->Grafico->addTempo(newTempo);
    Riscaldamento->Grafico->addTempo(newTempo);
    Gas->Grafico->addTempo(newTempo);
    IBias->Grafico->addTempo(newTempo);
    VBias->Grafico->addTempo(newTempo);
    Catodi->Grafico->addTempo(newTempo);

}



void   QMonitorizza::setMaxOut(int index ,qreal max, int tiposcala){

    switch (index) {
    case QTipoGrafico::Vuoto :
        Vuoto->setMaxOut(max,tiposcala);
        //Vuoto->ProvaGrafico();
        break;
    case QTipoGrafico::Riscaldo:
        Riscaldamento->setMaxOut(max,tiposcala);
        break;
    case QTipoGrafico::Gas:
        Gas->setMaxOut(max,tiposcala);
        break;
    case QTipoGrafico::LowI :
        IBias->setMaxOut(max,tiposcala);
        break;
    case QTipoGrafico::Volt:
        VBias->setMaxOut(max,tiposcala);
        break;
    case QTipoGrafico::HeghtI:
        Catodi->setMaxOut(max,tiposcala);
        break;
    default:
        break;
    };
}
