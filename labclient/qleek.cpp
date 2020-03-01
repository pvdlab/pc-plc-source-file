#include "qleek.h"


QLeek::QLeek(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QLeek)
{
    ui->setupUi(this);

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeEvent()));
    ui->Volume->addItem("154");
    ui->Volume->addItem("170");
    ui->Volume->addItem("27");
    ui->Volume->addItem("8");
    ui->Volume->addItem("2");
    ui->Volume->setEnabled(false);
    ui->ButtonStatoVuoto->setEnabled(false);
    Volume= 154;
    Processa=NULL;
    lastleek=0;
    m_MainTime=0;
    mStatoVuoto=0;
    pvuoto=NULL;
    pventing=NULL;
    pleek=NULL;

}

QLeek::~QLeek()
{
    delete ui;
}


void QLeek::Init(QNetPlc *newProcessa,QSqlDatabase newDbProcesso, int newIdVuoto){

    if(Processa==NULL){
        Processa=newProcessa;
        pvuoto =Processa->Segnale("Vacu",0,0,10000,1.667,11.3);
        pventing=Processa->Segnale("Vent",0,0,10000,1.667,11.3);
        pleek =Processa->Segnale("Leek",0,0,10000,1.667,11.3);

        pvuoto[IdVuoto].setColor(1);
        pventing->setColor(3);
        pleek->setColor(5);

        ui->Grafico->setMaxOut(6,QTipoScala::Logaritmica);
        ui->Grafico->append(pvuoto);
        ui->Grafico->append(pventing);
        ui->Grafico->append(pleek);

        MainTime.setHMS(0,0,0,0);
        MainTime.start();
    }

    DbProcesso= newDbProcesso;
    IdVuoto= newIdVuoto;

    connect(Processa, SIGNAL(StatoVuoto(int)),
             this, SLOT(StatoVuoto(int)));


}

void QLeek::Edit(){

    if(Processa==NULL)
        return;

    if(Processa->pData->ProcessoIsOn)
        return;

    double Pressione=Processa->Vuoto[IdVuoto].Penning->Pressione();
    ui->Penning->setText( QString::number(Pressione,'e',2 ) );
    ui->Leek->setText(" Leek " + QString::number(lastleek,'e',2 ));
    ui->Tempo->setText(QDateTime::fromTime_t(MainTime.elapsed()/1000).toUTC().toString("hh:mm:ss"));


    if(mStatoVuoto ==  CStatoMacchina::Vacuum | mStatoVuoto ==  CStatoMacchina::InVacuum){
       pvuoto->DSegnale(Pressione);
       ui->Grafico->Grafico->Edit(MainTime.elapsed()/1000,0,Pressione);
    }
    else{
       pventing->DSegnale(Pressione);
       ui->Grafico->Grafico->Edit(MainTime.elapsed()/1000,1,Pressione);
    }

    m_MainTime = MainTime.elapsed()/1000;

}

void QLeek::timeEvent()
{

    lastTempo+=timer.interval()/1000;

    double Pressione=Processa->Vuoto[IdVuoto].Penning->Pressione();
    if(Processa->pData->StatoVuoto ==  CStatoMacchina::Vacuum){
       pvuoto->DSegnale(Pressione);
       pventing->DSegnale(0);

     }else if((Processa->pData->StatoVuoto ==CStatoMacchina::StopVuoto)
              |  (Processa->pData->StatoVuoto ==CStatoMacchina::EnabledVent)
              | ((Manuale==true ) & (stato == CStatoMacchina::StopVuoto) )){
       if( LastVent>0){
           int intervallo;
           if(Pressione<0.01){
             intervallo=timer.interval();
             lastleek=Pressione;
             lastleek-=LastVent;
             lastleek*=Volume;
             if(intervallo)
               lastleek/=intervallo;
             lastleek*=1000;
           }else{
             intervallo=MainTime.elapsed();
             lastleek=Pressione;
             lastleek-=LastVent;
             lastleek*=Volume;
             if(intervallo)
               lastleek/=intervallo;
             lastleek*=1000;
           }


           pleek->DSegnale(lastleek);
           pventing->DSegnale(Pressione);
           pvuoto->DSegnale(0);
           //ui->Grafico->Edit(lastTempo);
           pventing->DSegnale(Pressione);
           pleek->DSegnale(lastleek);
           pvuoto->DSegnale(0);
        }

        if((Pressione<0.01) | (LastVent==0))
           LastVent=Pressione;
        ui->Grafico->Grafico->Edit(lastTempo,2,lastleek);

    }

    if(MainTime.elapsed()/1000>1200)
        timer.setInterval(500000);
    else if(MainTime.elapsed()/1000>7200)
        timer.stop();

}

void QLeek::StatoVuoto(int newmStatoVuoto){

   if(Processa->pData->ProcessoIsOn)
        return;

   if(newmStatoVuoto ==CStatoMacchina::EnabledVent){
      newmStatoVuoto =CStatoMacchina::StopVuoto ;
   }

   if(mStatoVuoto==newmStatoVuoto)
       return;

   mStatoVuoto=newmStatoVuoto;
   if((mStatoVuoto == CStatoMacchina::InVacuum) | (mStatoVuoto == CStatoMacchina::Vacuum)){
     if(timer.isActive())
         return;
     timer.start(50000);
     MainTime.restart();
     m_MainTime=0;
     ui->Grafico->Grafico->setTempo(7200,0.5);
     lastTempo=0;
     ui->ButtonStatoVuoto->setText("Vuoto");
   }else if((mStatoVuoto ==CStatoMacchina::StopVuoto)  | (mStatoVuoto ==CStatoMacchina::EnabledVent)){
     timer.start(50000);
     MainTime.restart();
     m_MainTime=0;
     LastVent=0;
     lastTempo=0;
     ui->ButtonStatoVuoto->setText("Stop Vuoto");
   }else{
     timer.stop();
   }

}

void QLeek::resizeEvent(QResizeEvent *event){

    Q_UNUSED(event)

    QSqlQuery query(DbProcesso);
    query.exec("SELECT * FROM LEEK");
    if(!query.last())
        return;
    QSqlRecord record=query.record();
    int num=record.value("ID_LEEK").toInt();
    query.first();
    int i=0;
    while (query.next()) {
        QSqlRecord record=query.record();
        ui->Grafico->Grafico->Load(record.value("TEMPO").toInt(),record);
        emit caricaRecord((i++)*100/num);
    }
    emit caricaRecord(0);

}

void QLeek::on_ButtonAutomatico_clicked()
{
    if(ui->ButtonAutomatico->text()=="Automatico"){
       ui->ButtonAutomatico->setText("Manuale");
       ui->ButtonStatoVuoto->setEnabled(true);
       ui->Volume->setEnabled(true);
       Manuale=true;
    }else{
       ui->ButtonAutomatico->setText("Automatico");
       ui->ButtonStatoVuoto->setEnabled(false);
       ui->Volume->setEnabled(false);
       Manuale=false;
    }
}

void QLeek::on_ButtonStatoVuoto_clicked()
{

    if(ui->ButtonStatoVuoto->text()=="Vuoto"){
       ui->ButtonStatoVuoto->setText("Stop Vuoto");
       stato=CStatoMacchina::StopVuoto;
       StatoVuoto( CStatoMacchina::StopVuoto);
    }else{
       ui->ButtonStatoVuoto->setText("Vuoto");
       stato=CStatoMacchina::Vacuum;
       StatoVuoto( CStatoMacchina::Vacuum );
    }

}

void QLeek::on_Volume_currentIndexChanged(const QString &arg1)
{

    Volume=arg1.toInt();
}
