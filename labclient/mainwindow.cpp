#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MainCostanti.h"
#include "Segnale.h"
#include <QSqlError>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include  <QHostInfo>
#include <QtConcurrent/QtConcurrent>
#include "qstartprocesso.h"
#include "qdialogtavola.h"
#include "qconnettidb.h"
#include "qsimula.h"


#define MYSQL
#define RESTARTSERVER

//#define TEST_VESIONE_RIDOTTA


//http://thebeautifullmind.com/2014/06/18/creating-a-application-launcher-in-ubuntu-14-04/
// cd /home/ricvb/Dropbox/run/consoleplc ; ./consoleplc


void MainWindow::on_DbConnesso(QSqlDatabase *newDb,  QSqlDatabase *newDbProcesso,
                               QSqlQuery *queryEvento, QSqlQuery *newqueryData, QSqlQuery *newqueryLeek){


    if(!DbIsConnesso){
        QDesktopWidget desktop;
        int desktopHeight=desktop.geometry().height();
        int desktopWidth=desktop.geometry().width();
        this->resize(desktopWidth,desktopHeight);
    }
   
    QNetPlc       *mProcessa=Processa;
    MonitorizzaViewInit();
    for(int i=0; i< Processa->CamereCount; i++ ){
        Processa->pData[i].File.ProcessoId=0;
        Processa->pData[i].ProcessoIsOn=false;
    }

    Processa->queryData=*newqueryData;

    DbData=newDb;
    DbProcesso=newDbProcesso;
    while (queryEvento->next()) {
        ui->ListaErrori->addItem(queryEvento->record().value("EVENTO").toString());
    }



    ui->LeekView->Init(Processa,*DbData,0);
    if(Processa->CamereCount==2)
       ui->LeekViewDue->Init(Processa,*DbData,1);
    else
       ui->LeekViewDue->setVisible(false);

    TipoSteoMod = new QSqlQueryModel();
    TipoSteoMod->setQuery("SELECT TIPO_STEP, ID_TIPO_STEP FROM TIPO_STEP ",*DbData);
    ui->ListaCTipoStep->setModel(TipoSteoMod);

    int IdProcesso;
    QSqlQuery query(*DbData) ;
    query.exec("SELECT * FROM CICLO WHERE ANNO = '" + QDate::currentDate().toString("yyyy") + "'");
    if(!query.last()){
        Ciclo=1;
        IdMacchina=0;
    }else{
        Ciclo=query.record().value("CICLO").toInt();
        IdMacchina=query.record().value("MACCHINA").toInt();
        IdProcesso=query.record().value("ID_PROCESSO").toInt();
    }

    if(!VersioneRidotta){
       if(DbProcesso!=NULL)
           ui->ProcessoView->setDbProcesso(DbProcesso,IdMacchina,IdProcesso);
    }

    this->setWindowTitle("Connesso su : "+ DbData->hostName());


    this->setCursor(QCursor(Qt::ArrowCursor));
    Processa->VersioneRidotta=VersioneRidotta;
    if(VersioneRidotta){
       if(!DbIsConnesso){
           ui->widRidotta->setVisible(true);
           ui->Bottoni->setVisible(false);
           ui->WidComandi->setVisible(false);
           ui->tabImpianto->setVisible(false);
           ui->statusBar->setVisible(false);
           int wFoglio,hFoglio;
           getFoglio(&wFoglio,&hFoglio);
           QHBoxLayout *horizontalLayout;
           if (this->objectName().isEmpty())
               this->setObjectName(QStringLiteral("BOOO"));
           horizontalLayout = new QHBoxLayout(ui->widRidotta);
           horizontalLayout->setSpacing(0);
           horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
           horizontalLayout->setContentsMargins(0, 0, 0, 0);
           Macchina = new CDisegnoMacchina(ui->widRidotta);
           QDesktopWidget desktop;
           int desktopHeight=desktop.geometry().height();
           int desktopWidth=desktop.geometry().width();
           Macchina->resize(desktopWidth,desktopHeight);
           Macchina->setMinimumSize(QSize(wFoglio, hFoglio));
           horizontalLayout->addWidget(Macchina);
           Macchina->Init(Processa,0);
       }
       connect(Macchina, SIGNAL(MostraEvento()),
                this, SLOT(mostraMsgPlc()));

       connect(Macchina, SIGNAL(MostraErrore()),
                this, SLOT(mostraListaErrori()));
    }else{
        Edit();
        //ui->Bottoni->setVisible(false);
        ui->ButtonVenting->setVisible(false);
        ui->ButtonVuoto->setVisible(false);
        ui->ButtonStopVuoto->setVisible(false);
        ui->ButtonAutomatico->setVisible(false);
        ui->ButtonManule->setVisible(false);
        ui->ButtonStartProcesso->setVisible(false);
        ui->ButtonStopProcesso->setVisible(false);
        //ui->ButtonApriCiclo->setVisible(false);
        ui->widRidotta->setVisible(false);
        QFont f( "Arial", 14, QFont::Bold);
        ui->Stato->setFont(f);
        ui->ProcessoPlc->setFont(f);
        ui->MainTime->setFont(f);
        ui->StepTime->setFont(f);
        ui->Ciclo->setFont(f);
        ui->EventTime->setFont(f);
        ui->RepiStep->setFont(f);
        ui->Time->setFont(f);
    }

    DbIsConnesso=true;

}




MainWindow::MainWindow(QNetPlc *newProcessa,QLabserverNet *newNetServer,int newtipoapp,CNetDb *newpNet,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    VersioneRidotta=-1;
    IdMacchina=0;

    for(;;){
        if(newProcessa->Impianto!=0)
            break;
        usleep(100);
    }
    Processa = newProcessa;
    Processa->ResetErrore();

    ui->setupUi(this);
    ui->Bottoni->setVisible(true);
    ui->WidComandi->setVisible(true);
  /*  QDesktopWidget desktop;
    int desktopHeight=desktop.geometry().height();
    int desktopWidth=desktop.geometry().width();
    this->resize(desktopWidth,desktopHeight);*/

    IsMsgPlcCaricato=false;
    IsVisibile=false;
    DbIsConnesso=false;
    pNet=newpNet;
    DbData=NULL;
    Impianto=pNet->Impianto;
    IsLocale=pNet->IsLocale;


    host=newNetServer->ServerIp;
    TipoApp=newtipoapp;

    connect(newNetServer->Client, SIGNAL(StepSpediti(int,int)), this, SLOT(on_StepSpediti(int,int)));
    connect(newNetServer->Client, SIGNAL(processoCaricato()), this, SLOT(on_processoCaricato()));
    connect(newNetServer->Client, SIGNAL(processoSpedito()), this, SLOT(on_processoSpedito()));
    connect(newNetServer->Client, SIGNAL(Errore(QString)), this, SLOT(mostraEvento(QString)));

    ui->ButtonErrore->setToolTip("Non Connesso");
    ui->ButtonErrore->setIcon( QIcon(":/images/Errore.png"));
    ui->ButtonErrore->setIconSize(QSize(32, 32));
    ui->ButtonErrore->setDisabled(true);
    connect(ui->ButtonErrore, SIGNAL(clicked()), this, SLOT(mostraListaErrori()));

    ui->ButtonMsgPlc->setToolTip("Non Connesso");
    ui->ButtonMsgPlc->setIcon( QIcon(":/images/edit.png"));
    ui->ButtonMsgPlc->setIconSize(QSize(32, 32));
    connect(ui->ButtonMsgPlc, SIGNAL(clicked()), this, SLOT(mostraMsgPlc()));

    ui->tabImpianto->setCurrentIndex(0);
    ui->VuotoView->Init(Processa);
    ui->PotenzaView->Init(Processa,0);
    ui->MonitorizzaView->Init(Processa);
    ui->ProcessoView->Init(Processa);
    MonitorizzaViewInit();

    connect(newNetServer->Client, SIGNAL(processiDisponibili(int,QString)),
             this, SLOT( processiDisponibili(int ,QString)));
    connect(newNetServer->Client, SIGNAL(Errore(QString)),
             this, SLOT( on_Errore(QString)));
    connect(Processa, SIGNAL(ComandoAttivato(int,int)),
             this, SLOT(ComandoAttivato(int,int)));
    connect(Processa, SIGNAL(ComandoRicevuto(int,int)),
             this, SLOT(ComandoRicevuto(int,int)));
    connect(Processa, SIGNAL(ComandoEseguito(int,int)),
             this, SLOT(ComandoEseguito(int,int)));
    connect(Processa, SIGNAL(StatoMacchina()),
             this, SLOT(StatoMacchina()));
    connect(Processa, SIGNAL(StatoVuoto(int)),
             this, SLOT(StatoVuoto(int)));
    connect(Processa, SIGNAL(ProcessoIsOn(int)),
             this, SLOT(ProcessoIsOn(int)));
    connect(Processa, SIGNAL(ProcessoIsOff(int)),
             this, SLOT(ProcessoIsOff(int)));
    connect(Processa, SIGNAL(Processo(int,int)),
             this, SLOT(Processo(int,int)));
    connect(Processa, SIGNAL(AddTempo(int)),
             this, SLOT(AddTempo(int)));
    connect(Processa, SIGNAL(nuovoMsg(QByteArray)),
             this, SLOT(nuovoMsg(QByteArray)));
    connect(Processa, SIGNAL(NuovaData()),
             this, SLOT(Edit()));
    connect(Processa, SIGNAL(CompStepLoad(int)),
             this, SLOT(CompStepLoad(int)));
    connect(Processa, SIGNAL(nuovoErrore(int)),
             this, SLOT(nuovoErrore(int)));
    connect(Processa, SIGNAL(ProcessoCorrotto(int,int)),
             this, SLOT(ProcessoCorrotto(int,int)));
    connect(Processa, SIGNAL(MsgRicevuti(int,int,QString)),
             this, SLOT(on_MsgRicevuti(int,int,QString)));
    connect(Processa, SIGNAL(MacchinainInManuale(bool)),
             this, SLOT(on_MacchinainInManuale(bool)));

    connect(ui->VuotoView, SIGNAL(sendCambiaSegnale(int,int,int)),
             this, SLOT(sendCambiaSegnale(int,int,int)));
    connect(ui->PotenzaView, SIGNAL(sendCambiaSegnale(int,int,int)),
             this, SLOT(sendCambiaSegnale(int,int,int)));

    connect(ui->PotenzaView->Macchina, SIGNAL(MostraEvento()),
             this, SLOT(mostraMsgPlc()));
    connect(ui->PotenzaView->Macchina, SIGNAL(MostraErrore()),
             this, SLOT(mostraListaErrori()));
    connect(ui->PotenzaView->Macchina, SIGNAL(StartProcesso()),
             this, SLOT(on_ButtonStartProcesso_clicked()));
    if(Processa->CamereCount!=1){
        connect(ui->PotenzaView->MacchinaDue, SIGNAL(MostraEvento()),
                 this, SLOT(mostraMsgPlc()));
        connect(ui->PotenzaView->MacchinaDue, SIGNAL(MostraErrore()),
                 this, SLOT(mostraListaErrori()));
        connect(ui->PotenzaView->MacchinaDue, SIGNAL(StartProcesso()),
                 this, SLOT(on_ButtonStartProcesso_clicked()));
    }


    connect(ui->ModuloView, SIGNAL(sendCambiaSegnale(int,int,int)),
             this, SLOT(sendCambiaSegnale(int,int,int)));

    connect(ui->LeekView ,SIGNAL(caricaRecord(int)),
             this, SLOT(setprogressBar(int)));

    connect(ui->VuotoView, SIGNAL(StartProcesso()),
             this, SLOT(on_ButtonStartProcesso_clicked()));


    Processa->ResetComando(0);
    Processa->ResetErrore();
    m_pData = new DataType;
    m_pData[Processa->IdPro].Comando=1;
    m_pData[Processa->IdPro].StatoVuoto=-1;
    m_pData[Processa->IdPro].StatoMacchina=-1;
    m_pData[Processa->IdPro].ComandoAttivato=-1;
    m_pData[Processa->IdPro].ComandoEseguito=-1;
    m_pData[Processa->IdPro].ComandoRicevuto=-1;
    m_pData[Processa->IdPro].File.ProcessoId=-1;
    m_pData[Processa->IdPro].LastComandoLen=0;
    m_pData[Processa->IdPro].LastComandoIndex=0;
    m_pData[Processa->IdPro].ProcessoIsOn=false;


    enableNet=true;
    RicevutoData=true;
    PathCicli=QDir::homePath() + "/Dropbox/DATABASE/CICLI/" + QDate::currentDate().toString("yyyy") + "/";
    PathProcessi=QDir::homePath() + "/Dropbox/DATABASE/PROCESSI/";

    ui->tabImpianto->setCurrentIndex(4);
    ui->tabImpianto->setCurrentIndex(0);
    ui->ButtonManule->hide();
    ui->ButtonAutomatico->hide();
    ui->ButtonStartProcesso->setDisabled(true);
    ui->ButtonStopProcesso->setDisabled(true);
    ui->ButtonVenting->setDisabled(true);
    ui->ButtonVuoto->setDisabled(true);
    ui->ButtonStopVuoto->setDisabled(true);
    ui->ButtonAutomatico->setDisabled(true);
    ui->ButtonImpara->setDisabled(false);

    recordCount=0;

    ui->ListaErrori->addItem(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss")
                                + " Net To " + host);
    ui->ButtonManule->show();
    ui->ButtonAutomatico->show();
    ui->tabImpianto->removeTab(0);

    ui->ProcessoView->SetTipoApp(TipoApp);
    connect(ui->ProcessoView ,SIGNAL(getNuovaConnesione()),
             this, SLOT(ConnettiToDatabase()));

    ConnettiToDatabase();

}


void  MainWindow::MonitorizzaViewInit(){


    ui->ModuloView->Init(Processa);

    ui->MonitorizzaView->Clear();

    ui->MonitorizzaView->setMaxOut(QTipoGrafico::Vuoto,6,QTipoScala::Logaritmica);
    ui->MonitorizzaView->setMaxOut(QTipoGrafico::Riscaldo,600,QTipoScala::Lineare);
    ui->MonitorizzaView->setMaxOut(QTipoGrafico::Gas,500,QTipoScala::Lineare);
    ui->MonitorizzaView->setMaxOut(QTipoGrafico::LowI,10,QTipoScala::Lineare);
    ui->MonitorizzaView->setMaxOut(QTipoGrafico::Volt,1100,QTipoScala::Lineare);
    ui->MonitorizzaView->setMaxOut(QTipoGrafico::HeghtI,100,QTipoScala::Lineare);

    ui->MonitorizzaView->append(QTipoGrafico::Vuoto,Processa->Vuoto[Processa->IdPro].Penning->Segnale());
    if(Processa->Vuoto->Baratron.Segnale)
      ui->MonitorizzaView->append(QTipoGrafico::Vuoto,Processa->Vuoto[Processa->IdPro].Baratron.Segnale);
    //if(Processa->Vuoto->Baratron.Pirani)
    //  ui->MonitorizzaView->append(QTipoGrafico::Vuoto,Processa->Vuoto[Processa->IdPro].Baratron.Pirani);

    ui->MonitorizzaView->append(QTipoGrafico::Riscaldo,Processa->Vuoto[Processa->IdPro].Resistenza->Temp_Resistenza);
    ui->MonitorizzaView->append(QTipoGrafico::Riscaldo,Processa->Vuoto[Processa->IdPro].Turbo.Temp);
    ui->MonitorizzaView->append(QTipoGrafico::Riscaldo,Processa->Vuoto[Processa->IdPro].Camera.Temp);
    //ui->MonitorizzaView->append(QTipoGrafico::Riscaldo,Processa->Vuoto[Processa->IdPro].Camera.TempCatodo);

    for(int i=0; i< Processa->GasCount;i++){
        ui->MonitorizzaView->append(QTipoGrafico::Gas,Processa->GGas[i].GetG);
    }

    ui->MonitorizzaView->append(QTipoGrafico::LowI,Processa->Bias[Processa->IdPro].GetI);
    ui->MonitorizzaView->append(QTipoGrafico::Volt,Processa->Bias[Processa->IdPro].GetV);

    for(int i=0; i< Processa->SorgenteCount;i++){
        ui->MonitorizzaView->append(QTipoGrafico::HeghtI,Processa->Bias[Processa->IdPro].Sorgente[i].Catodo->GetI);
    }
}

void MainWindow::processiDisponibili(int count,QString str){

    ui->ProcessoView->processiDisponibili(count,str);
}


void MainWindow::ConnettiToDatabase(){

    this->setCursor(QCursor(Qt::WaitCursor));
    QConnettiDB *thread = new QConnettiDB(pNet);
    connect(thread, SIGNAL(Connesso(QSqlDatabase *,QSqlDatabase *,QSqlQuery *,QSqlQuery *,QSqlQuery *)),
            this, SLOT(on_DbConnesso(QSqlDatabase *,QSqlDatabase *,QSqlQuery *,QSqlQuery *,QSqlQuery *)));
    connect(thread, SIGNAL(mostraEvento(QString)),
            this, SLOT(mostraEvento(QString)));
    connect(thread, SIGNAL(VersioneRidotta(bool)),
            this, SLOT(on_VersioneRidotta(bool)));

    thread->start();

}

void MainWindow::on_VersioneRidotta(bool newVersioneRidotta){

    VersioneRidotta=newVersioneRidotta;

}

QString OldMsg;

void MainWindow::on_MsgRicevuti(int step, int di,QString msg){


    if(OldMsg==msg)
        return;

    OldMsg=msg;

    IsMsgPlcCaricato=true;

    if(di){
        if(!Processa->pData[Processa->IdPro].ProcessoIsOn){
            double percent=100;
            percent*=step+1;
            percent/=di;
            if(di==step){
               QThread::msleep(100);
               ui->progressBar->setValue(0);
            }else{
               ui->progressBar->setValue((int)percent);
            }
        }

        int pos =msg.indexOf(FINE_MSG);
        if(pos>0){
            msg=msg.left(pos);
            int pos=msg.indexOf("\n");
            if(pos>0)
                msg=msg.left(pos);
        }
        if(msg.length()>1){
            ui->ListaErrori->addItem(msg);
            ui->ListaErrori->scrollToBottom();
        }

        //if(!Processa->pData[Processa->IdPro].ProcessoIsOn){
            if(DlgEventi.isVisible())
                DlgEventi.addLista(msg);
        //}


    }

}

void MainWindow::on_processoCaricato(){

    if(!VersioneRidotta)
      ui->ProcessoView->ProcessoCaricaDaPlc();
    else{
      Processa->Processo_LoadDaFile() ;
    }

    ui->progressBar->setValue(0);


}

void MainWindow::on_processoSpedito(){

    ui->progressBar->setValue(0);


}

void MainWindow::on_StepSpediti(int step, int di){

    if(di){
        double percent=100;
        percent*=step;
        percent/=di;
        if(di==step){
           QThread::msleep(100);
           ui->progressBar->setValue(0);
        }else{
           ui->progressBar->setValue((int)percent);
        }
    }

}

void MainWindow::nuovoErrore(int count){

    qDebug() << count;
}

void MainWindow::mostraErrore(QString msg){

    QMessageBox MsgBox;
    MsgBox.setWindowTitle("Errore di Connessione al database");
    MsgBox.setText(msg + " Mi Connetto Localmente? ");
    MsgBox.setIcon(QMessageBox::Critical);
    MsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    MsgBox.setDefaultButton(QMessageBox::No);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);


    if(!IsVisibile){
        IsVisibile=true;
        QTimer *aspetta = new QTimer(this);
        QObject::connect(aspetta, SIGNAL(timeout()),
                            this, SLOT(onTimeout()));
        aspetta->start(100);
    }

}


void MainWindow::onTimeout(){

    Processa->IsVisibile=true;
    //Processa->ResetErrore();

}


void MainWindow::resizeEvent(QResizeEvent *event){

    Q_UNUSED(event)

    DisegnaGrafico();

}

void MainWindow::Edit(){

    setBiasIsLV(Processa->Bias[Processa->IdPro].IsLVolt->Segnale());
    setPenning(Processa->Vuoto[Processa->IdPro].Penning->Pressione());
    setBias_V(Processa->Bias[Processa->IdPro].DSegnale(Processa->Bias[Processa->IdPro].GetV));
    setBias_I(Processa->Bias[Processa->IdPro].DSegnale(Processa->Bias[Processa->IdPro].GetI));

    //ui->VuotoView->Edit();

    if(!VersioneRidotta){
       ui->PotenzaView->Edit();
       ui->ProcessoView->Edit();
       ui->ModuloView->Edit();
       ui->LeekView->Edit();
       if(Processa->CamereCount==2)
           ui->LeekViewDue->Edit();
    }else
       Macchina->Edit();

    ui->MainTime->setText("Tempo On " + QDateTime::fromTime_t(Processa->MainTimer).toUTC().toString("hh:mm:ss"));

    ui->EventTime->setText(QDateTime::fromTime_t(Processa->EventTime).toUTC().toString("hh:mm:ss") + " di " +
                         QDateTime::fromTime_t(Processa->TimeEventStop).toUTC().toString("hh:mm:ss"));

    ui->StepTime->setText("Tempo Step " + QDateTime::fromTime_t(Processa->pData[Processa->IdPro].StepTime).toUTC().toString("hh:mm:ss") + " di " +
                         QDateTime::fromTime_t(Processa->pData[Processa->IdPro].StepTimeStop).toUTC().toString("hh:mm:ss"));
    ui->Time->setText("Tempo Proc " + QDateTime::fromTime_t(Processa->pData[Processa->IdPro].Time).toUTC().toString("hh:mm:ss") + " di " +
                         QDateTime::fromTime_t(Processa->pData[Processa->IdPro].TimeStop).toUTC().toString("hh:mm:ss"));

    //qDebug() << Processa->Bias->Sorgente[2].Catodo->GetI->DSegnale() << Processa->Bias->Sorgente[2].Catodo->GetI->Segnale();

    mostraErrore(&Processa->pErrore);
    mostraErrore(&Processa->pErroreDue);
    mostraErrore(&Processa->pErroreModulo);
    for(int i=0; i< Processa->CamereCount;i++){
       mostraErrore(&Processa->Vuoto[i].pErrore);
       mostraErrore(&Processa->Vuoto[i].Resistenza->pErrore);
       mostraErrore(&Processa->Bias[i].pErrore);
       mostraErrore(&Processa->Vuoto[i].Resistenza->pErrore);
       for(int j=0 ; j<Processa->SorgenteCount;j++ ){
            mostraErrore(&Processa->Bias[i].Sorgente[j].pErrore);
            mostraErrore(&Processa->Bias[i].Sorgente[j].Catodo->pErrore);
       }
    }
    for(int i=0 ; i<Processa->GasCount;i++)
         mostraErrore(&Processa->GGas[i].pErrore);


    ui->Ciclo->setText("Ciclo : " + QString::number(Processa->pData[Processa->IdPro].File.Ciclo));
    if(Processa->pData[Processa->IdPro].ProcessoIsOn | Processa->pData[Processa->IdPro].ErrProcesso){
       QString str=QString::number(Processa->pData[Processa->IdPro].StepIndex);
       str+= " Step :" + ui->ProcessoView->CTipoStep(Processa->pData[Processa->IdPro].TipoStep);
       str+= " " + QString::number(Processa->pData[Processa->IdPro].Repit);
       str+= "/" + QString::number(Processa->pData[Processa->IdPro].RepitCount);
       ui->RepiStep->setText(str);
       ui->MonitorizzaView->Edit(Processa->pData[Processa->IdPro].Time);
    }else{
       ui->RepiStep->setText( QString::number(0) + ")"
                             + "Step :" +
                             " " + QString::number(0) +
                             "/" + QString::number(0));
    }

    if(lastmsg!=Processa->pData[Processa->IdPro].LastComandoIndex){
        QByteArray lastcomando= Processa->GetStrMsgPerClient(0);
        int comando=Processa->ComandoToInt(lastcomando);
        if(comando){
            if(comando==CTipoComando::Load ){
                int percent=Processa->getPercentSpedito(lastcomando);
                if(percent==100)
                    percent=0;
                ui->progressBar->setValue(percent);
            }
        }
        int messaggio=Processa->MessaggioToInt(lastcomando);
        if(messaggio==CTipoMessaggio::Risposta){
            mostraEvento(Processa->GetStrMsgPerClient(0));
        }else if(messaggio==CTipoMessaggio::Errore){
            ;//mostraErrore(Processa->GetStrMsgPerClient(),"","");
        }
        lastmsg=Processa->pData[Processa->IdPro].LastComandoIndex;
    }
    //ui->statusBar->showMessage(" Modbus Scan Time = " + QString::number(Processa->Plc.ScanTime));
    ui->statusBar->showMessage("Plc "+ pNet->DbDatiHost + " Modbus Scan Time: " + QString::number(Processa->ScanTime) + " ms RecordCount : " + QString::number(Processa->pData[Processa->IdPro].RecordCount));

}

void  MainWindow::SetNetData()
{

  if(Processa->pData[Processa->IdPro].StatoMacchina!=m_pData[Processa->IdPro].StatoMacchina){
     StatoMacchina();
  }


  if(Processa->pData[Processa->IdPro].TipoStep!=m_pData[Processa->IdPro].TipoStep){
     CambiaCTipoStep(Processa->pData[Processa->IdPro].TipoStep);
  }


  if(Processa->pData[Processa->IdPro].File.ProcessoId!=m_pData[Processa->IdPro].File.ProcessoId){
    Processo(Processa->pData[Processa->IdPro].File.ProcessoId,Processa->IdPro);
  }

  if(Processa->pData[Processa->IdPro].ProcessoIsOn!=m_pData[Processa->IdPro].ProcessoIsOn ){
    if(Processa->pData[Processa->IdPro].ProcessoIsOn){
       ProcessoIsOn(Processa->IdPro);
    }
    if(!Processa->pData[Processa->IdPro].ProcessoIsOn && m_pData[Processa->IdPro].ProcessoIsOn){
       ProcessoIsOff(Processa->IdPro);
    }
  }
  memcpy(m_pData,&Processa->pData,sizeof(DataType));

}



void MainWindow::mostraMsgPlc(){

   if(!DlgEventi.isVisible()){
        DlgEventi.clear();
        DlgEventi.addLista(ui->ListaErrori);
        DlgEventi.exec();
   }


}

void MainWindow::mostraListaErrori(){

    if(!DlgErrore.isVisible()){
        DlgErrore.exec();
    }


}


void MainWindow::mostraErrore(CErrore *pErrore){

    if(!IsVisibile)
        return;

    std::vector<std::string> myStringList;
    int ora=pErrore->Edit(&myStringList);
    if(ora==-1)
        return;

    if(myStringList.size()){
        ui->ButtonErrore->setDisabled(false);
        ui->ListaErrori->setStyleSheet("* { background-color: red; }");
    }

    char Tmp[Processa->ErrSize];
    char *pTmp=Tmp;
    memcpy(pTmp,Processa->pBufferErrore,Processa->ErrSize);

    for (int i=0;i<(int)myStringList.size();i++){
        if((QString::fromUtf8(myStringList[i].c_str())!="E_PLC_SCRITTURA") & (QString::fromUtf8(myStringList[i].c_str())!="E_PLC_LETTURA")){
            QString Errorestr=QDateTime::fromTime_t(ora).toUTC().toString("hh:mm:ss") + "\t";
            Errorestr+=QString::fromUtf8(myStringList[i].c_str());
            mostraEvento(Errorestr);
            DlgErrore.addErrore(Errorestr);
        }
    }

    if(!DlgErrore.isVisible()){
        DlgErrore.exec();
    }

}

void MainWindow:: mostraEvento(QString msg){

    if(DlgEventi.isVisible())
       DlgEventi.addLista(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " " + msg);
}


void MainWindow::AddTempo(int tempo){

    QString str=QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss") + " AGGIUNGO "+
    QDateTime::fromTime_t(tempo).toUTC().toString("hh:mm:ss");
    mostraEvento(str);
    ui->MonitorizzaView->addTempo(tempo);

}

void MainWindow::ComandoAttivato(int newComando,int index){

    if(newComando == CTipoComando::Vacuum){
        ui->ButtonVuoto->setDisabled(true);
        ui->ButtonStopVuoto->setDisabled(false);
    }else if(newComando == CTipoComando::StopVuoto){
        if(Processa->Vuoto->Camera.PortaChiusa->Segnale()){
            ui->ButtonVuoto->setDisabled(false);
            ui->ButtonStopVuoto->setDisabled(true);
        }
    }else if((newComando == CTipoComando::Manuale) | (newComando == CTipoComando::StartProcesso)){
        if(Processa->CamereCount>1){
           if(index==0){
               Processa->pData[0].PowerIsEnabled=true;
               Processa->pData[1].PowerIsEnabled=false;
           }else{
               Processa->pData[0].PowerIsEnabled=false;
               Processa->pData[1].PowerIsEnabled=true;
           }
        }
    }else if(newComando == CTipoComando::Automatico){
        if(Processa->CamereCount>1){
           Processa->pData[0].PowerIsEnabled=false;
           Processa->pData[1].PowerIsEnabled=false;
        }
    }

}

void MainWindow::ComandoEseguito(int newComando,int index){

   /* if(newComando == CTipoComando::StopVuoto){
       if(Processa->Vuoto->Camera.PortaChiusa->Segnale())
          ui->ButtonVenting->setDisabled(false);
    }else if(newComando == CTipoComando::Vacuum){
        if(!Processa->pData[Processa->IdPro].ProcessoIsOn){
          if(Processa->pData[Processa->IdPro].File.Processo>0)
            ui->ButtonStartProcesso->setDisabled(false);
        }else
          ui->ButtonStartProcesso->setDisabled(true);
    }*/
    //setStatoVuoto();

}

void MainWindow::ComandoRicevuto(int newComando,int index){

   Q_UNUSED(newComando)
}

void MainWindow::nuovoMsg(QByteArray msg){

    Q_UNUSED(msg)


}


void MainWindow::StatoMacchina(){

    ui->Stato->setText("Stato: "  + Processa->getStatoMacchina(0));

}


void MainWindow::StatoVuoto(int Stato){

    //=Processa->pData[Processa->IdPro].StatoVuoto;
    //DataType     pData=Processa->pData;
    //qDebug() << "pData[Processa->IdPro].StatoVuoto" << QString::number(Processa->pData[Processa->IdPro].StatoVuoto);
    if(Processa->pData[Processa->IdPro].ProcessoIsOn==false){
        if( (Stato == CStatoMacchina::Vacuum) || (Stato == CStatoMacchina::InVacuum)){
            ui->ButtonVuoto->setDisabled(true);
            ui->ButtonStopVuoto->setDisabled(false);
            if(!Processa->pData[Processa->IdPro].ProcessoIsOn){
              if(Processa->pData[Processa->IdPro].File.ProcessoId>0)
                ui->ButtonStartProcesso->setDisabled(false);
            }else
              ui->ButtonStartProcesso->setDisabled(true);
         }else{
            if((Processa->IsInVuoto()) ){
               ui->ButtonStopVuoto->setDisabled(false);
               ui->ButtonVuoto->setDisabled(true);
               if(!Processa->pData[Processa->IdPro].ProcessoIsOn){
                 if(Processa->pData[Processa->IdPro].File.ProcessoId>0)
                   ui->ButtonStartProcesso->setDisabled(false);
               }else
                 ui->ButtonStartProcesso->setDisabled(true);
             }else{
                if(Processa->Vuoto->Camera.PortaChiusa->Segnale()){
                    ui->ButtonStopVuoto->setDisabled(true);
                    ui->ButtonVuoto->setDisabled(false);
                }else{
                    ui->ButtonVuoto->setDisabled(true);
                }
                if(Processa->Vuoto->Camera.Valv->Segnale())
                    ui->ButtonVenting->setDisabled(true);
                else{
                    if(Processa->pData[Processa->IdPro].EnabledVenting)
                       ui->ButtonVenting->setDisabled(false);
                    else
                       ui->ButtonVenting->setDisabled(true);
                }

            }
         }
    }


}



void MainWindow::ProcessoCorrotto(int processo, int CompStep){

    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Question);
    msgbox.setWindowTitle("Carico Step");
    QString str="Processo Num " + QString::number(processo) +
            " CompStep " + QString::number(CompStep) + " Corrotto";
    msgbox.setText(str);
    msgbox.exec();
}

void MainWindow::ProcessoIsOff(int index){

    if(Processa->CamereCount>1){
       Processa->pData[0].PowerIsEnabled=false;
       Processa->pData[1].PowerIsEnabled=false;
    }

    if(!IsVisibile)
       Processa->pData[Processa->IdPro].ProcessoIsOn=true;

    ui->ButtonMsgPlc->setEnabled(true);
    ui->ButtonStopProcesso->setDisabled(true);
    ui->ButtonManule->setDisabled(false);
    ui->ButtonTest->setDisabled(false);
    ui->ButtonSimula->setDisabled(false);
    ui->ButtonApriCiclo->setDisabled(false);
    mostraEvento("Stop " +  ui->ProcessoView->getProcesso() );
    ui->ProcessoView->ProcessoIsOn();
    ui->MonitorizzaView->setEdit(true);
    timerUpdate.stop();
    timerProcesso.stop();
    ui->tabImpianto->setCurrentIndex(3);
    ui->LeekView->setFocus();
    Processa->pData[Processa->IdPro].StatoVuoto=0;

}

void MainWindow::CambiaCTipoStep(int CTipoStep){

    switch (CTipoStep) {
    case CTipoStep::Vuoto:
        break;
    case CTipoStep::Riscaldo:
        mostraEvento("Start RISCALDO");
        break;
    case CTipoStep::Etching:
        mostraEvento("Start ETCHING");
        break;
    case CTipoStep::Coating:
        mostraEvento("Start COATING");
        break;
    case CTipoStep::Venting:
        mostraEvento("Start VENTING");
        break;
    default:
        break;
    }
    if(CTipoStep-1>0)
      ui->ListaCTipoStep->setCurrentIndex(CTipoStep-1);
}


void MainWindow::ProcessoIsOn(int index){

    if(VersioneRidotta)
        return;

    if(Processa->CamereCount>1){
       if(index==0){
           Processa->pData[0].PowerIsEnabled=true;
           Processa->pData[1].PowerIsEnabled=false;
       }else{
           Processa->pData[0].PowerIsEnabled=false;
           Processa->pData[1].PowerIsEnabled=true;
       }
    }

    IsMsgPlcCaricato=false;
    ui->ListaErrori->clear();
    DlgEventi.clear();
    Processa->pData[Processa->IdPro].EventoProcessoCount=0;

    //ui->ButtonMsgPlc->setEnabled(false);
    MonitorizzaViewInit();
    ui->MonitorizzaView->setEdit(false);
    ui->tabImpianto->setCurrentIndex(2);
    ui->MonitorizzaView->setFocus();
    ui->MonitorizzaView->gotoZero(Processa->pData[Processa->IdPro].TimeStop);
    ui->MonitorizzaView->setTempo(Processa->pData[Processa->IdPro].TimeStop);
    DisegnaGrafico();

    ui->ButtonStartProcesso->setDisabled(true);
    ui->ButtonVuoto->setDisabled(true);
    ui->ButtonVenting->setDisabled(true);
    ui->ButtonStopVuoto->setDisabled(true);
    ui->ButtonStopProcesso->setDisabled(false);
    ui->ButtonManule->setDisabled(true);
    ui->ButtonAutomatico->setDisabled(true);
    ui->ButtonApriCiclo->setDisabled(true);
    ui->ButtonImpara->setDisabled(true);
    ui->ButtonTest->setDisabled(true);
    ui->ButtonSimula->setDisabled(true);
    ui->ListaCTipoStep->setDisabled(true);

    ui->progressBar->setValue(0);
    ui->ProcessoView->ProcessoIsOn();
    ui->Stato->setText("Processo Avviato : " + ui->ProcessoView->getProcesso() );
    mostraEvento("Start " +  ui->ProcessoView->getProcesso() );

}

void MainWindow::Processo(int id, int macchinaId){


    if(!VersioneRidotta & Processa->CamereCount==1){
        if(ui->ProcessoView->ProcessoApri(id)==false){
             return;
         }
    }else{
        ui->ProcessoView->nuovoProcesso(QString::number(id),macchinaId);
    }


    if(Processa->IsInVuoto()){
        if(!Processa->pData[Processa->IdPro].ProcessoIsOn)
          ui->ButtonStartProcesso->setDisabled(false);
        else
          ui->ButtonStartProcesso->setDisabled(true);
    }else{
        ui->ButtonStartProcesso->setDisabled(true);
    }



    QString str=ui->ProcessoView->getProcesso();

    QPalette sample_palette;
    if(ui->ProcessoView->CoatingId==1)
      sample_palette.setColor(QPalette::WindowText, Qt::darkYellow);
    else if(ui->ProcessoView->CoatingId==2)
      sample_palette.setColor(QPalette::WindowText, Qt::darkBlue);
    else if(ui->ProcessoView->CoatingId==3)
      sample_palette.setColor(QPalette::WindowText, Qt::black);

    ui->ProcessoPlc->setAutoFillBackground(true);
    ui->ProcessoPlc->setPalette(sample_palette);
    ui->ProcessoPlc->setText("Processo PLC " + str);

}


void MainWindow::on_ButtonVuoto_clicked()
{
    ui->ButtonVuoto->setDisabled(true);
    ui->ButtonStopVuoto->setDisabled(false);
    SetAutomatico();
    ui->ButtonVenting->setDisabled(true);
    //Processa->ResetErrore();
    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::Vacuum);
}

void MainWindow::on_ButtonStopVuoto_clicked()
{

    SetAutomatico();
    ui->ButtonStartProcesso->setDisabled(true);
    //Processa->ResetErrore();
    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::StopVuoto);
}

void MainWindow::on_ButtonVenting_clicked()
{
    ui->ButtonStartProcesso->setDisabled(true);
    //Processa->ResetErrore();
    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::Ventila);

}

void MainWindow::on_ButtonStartProcesso_clicked()
{

    //ui->ProcessoView->Aggiorna();
    ui->Bottoni->setVisible(true);
    ui->WidComandi->setVisible(true);
    ui->ButtonVuoto->setDisabled(true);
    ui->ButtonStopVuoto->setDisabled(true);
    ui->ButtonVenting->setDisabled(true);
    ui->ButtonManule->setDisabled(false);
    ui->ButtonAutomatico->setDisabled(true);
    ui->ButtonImpara->setDisabled(true);
    ui->VuotoView->setManuale(false);
    ui->PotenzaView->setManuale(false);
    ui->ModuloView->setManuale(false);
    ui->ButtonImpara->setDisabled(true);
    ui->ButtonManule->setDisabled(true);
    ui->ButtonAutomatico->setDisabled(true);

    if(Processa->pData[Processa->IdPro].ProcessoIsOn)
        return;

    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::StartProcesso, 0);

    Processa->queryData.clear();
    ui->MonitorizzaView->setQuery(Processa->queryData);
    ui->ButtonStartProcesso->setDisabled(true);


    //QStartProcesso DlgStartProcesso(this,Processa);
    //DlgStartProcesso.exec();


}

void MainWindow::on_ButtonStopProcesso_clicked()
{
    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::StopProcesso);
    //ui->ButtonStopProcesso->setDisabled(true);
}

void MainWindow::on_Errore(QString str){

    QMessageBox msgbox;
    msgbox.setIcon(QMessageBox::Critical);
    msgbox.setWindowTitle("Errore");
    msgbox.setText(str);
    msgbox.exec();

}

void MainWindow::on_MacchinainInManuale(bool newManuale){

    ui->VuotoView->setManuale(newManuale);
    ui->PotenzaView->setManuale(newManuale);
    ui->ModuloView->setManuale(newManuale);
}

void MainWindow::on_ButtonManule_clicked()
{
     OldComando=Processa->pData[Processa->IdPro].Comando;
     ui->ButtonManule->setDisabled(true);
     ui->ButtonAutomatico->setDisabled(false);
     ui->ButtonImpara->setDisabled(false);
     ui->VuotoView->setManuale(true);
     ui->PotenzaView->setManuale(true);
     ui->ModuloView->setManuale(true);
     Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::PlcManule);
}


void MainWindow::on_ButtonAutomatico_clicked()
{
    ui->ButtonManule->setDisabled(false);
    ui->ButtonAutomatico->setDisabled(true);
    ui->ButtonImpara->setDisabled(true);
    ui->VuotoView->setManuale(false);
    ui->PotenzaView->setManuale(false);
    ui->ModuloView->setManuale(false);
    Processa->AddMsg(CTipoMessaggio::SetComando,0,CTipoComando::Automatico);
}

void MainWindow::SetAutomatico(){

    ui->ButtonManule->setDisabled(false);
    ui->ButtonAutomatico->setDisabled(true);
    ui->ButtonImpara->setDisabled(true);
    ui->VuotoView->setManuale(false);
    ui->PotenzaView->setManuale(false);
    ui->ModuloView->setManuale(false);
    //Processa->AddMsg(CTipoMessaggio::SetComando,OldComando);

}

void MainWindow::on_ButtonApriCiclo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                               tr("Open File"), PathCicli,
                               tr("Msql (*.sql )"));
    if (fileName.isEmpty())
        return;

    QSqlDatabase DbProcessoMemoria = QSqlDatabase::addDatabase("QSQLITE");
    DbProcessoMemoria.setDatabaseName(":memory:");
    DbProcessoMemoria.open();
    QSqlQuery query(DbProcessoMemoria);

    this->setCursor(QCursor(Qt::WaitCursor));
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd()){
          QString line = in.readLine();
          query.exec(line);
       }
       inputFile.close();
    }
    this->setCursor(QCursor(Qt::ArrowCursor));

    Processa->queryData = QSqlQuery(DbProcessoMemoria);
    Processa->queryData.exec("SELECT * FROM CICLO_DATA");

    ui->tabImpianto->setCurrentIndex(3);
    ui->MonitorizzaView->setFocus();
    QSqlQuery     mquery;
    mquery.exec("SELECT MAX(TEMPO) FROM `CICLO_DATA`");
    mquery.next();
    QSqlRecord record=mquery.record();
    ui->MonitorizzaView->setTempo( record.value("MAX(TEMPO)").toInt() );

    ui->MonitorizzaView->setQuery(Processa->queryData);
    DisegnaGrafico();

}

void MainWindow::DisegnaGrafico(){

    if(!IsVisibile)
        return;

    if(!Processa->queryData.isActive())
        return;

    int num=Processa->queryData.numRowsAffected();
    if(!num)
        return;
    int i=0;
    int step=0;
    Processa->queryData.first();
    while (Processa->queryData.next()) {
        QSqlRecord record=Processa->queryData.record();
        ui->MonitorizzaView->Load(record.value("TEMPO").toReal(),record);
        ui->progressBar->setValue((i++)*100/num);
        if(step++==20){
          //QCoreApplication::processEvents();
          step=0;
        }


    }
    ui->progressBar->setValue(0);

}

void MainWindow::on_ListaErrori_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    if(ui->ListaErrori->count()==0)
       ui->ButtonErrore->setDisabled(true);

}



void MainWindow::on_tabImpianto_tabBarClicked(int index)
{

    ui->tabImpianto->setCurrentIndex(index);

    if(index==0){
        ;
    }else  if(index==1){
         ui->ProcessoView->getProcessi();
    }else if(index==2){
        if(!ui->MonitorizzaView->isActive() & !Processa->pData[Processa->IdPro].ProcessoIsOn){
            this->setCursor(QCursor(Qt::WaitCursor));
            ui->MonitorizzaView->setFocus();
            ui->MonitorizzaView->setQuery(Processa->queryData);
            Processa->queryData.last();
            if(!Processa->queryData.record().value("TEMPO").toInt()){
                this->setCursor(QCursor(Qt::ArrowCursor));
                return;
            }
            //if(!Processa->pData[Processa->IdPro].ProcessoIsOn)
            ui->MonitorizzaView->setTempo(Processa->queryData.record().value("TEMPO").toInt());
            DisegnaGrafico();
            this->setCursor(QCursor(Qt::ArrowCursor));
            ui->WidComandi->setVisible(true);
        }
    }else if(index==3){
        ;
    }else{
       ;
    }

}


void MainWindow::sendCambiaSegnale(int newslot, int newpoint, int newsegnale){

    Processa->AddMsgSetSegnale(newslot,newpoint,newsegnale);

}

void MainWindow::setprogressBar(int perc){

    ui->progressBar->setValue(perc);
}



void MainWindow::on_ButtonImpara_clicked()
{

    bool ok;
    int Impianto=QInputDialog::getInt(this, "Scegli Impianto", "Impianto", 1,1, 2,1, &ok);

    if(ok)
       Processa->AddMsg(CTipoMessaggio::SetComando,Impianto-1,CTipoComando::Impara);
}

void MainWindow::on_ButtonSimula_clicked()
{

    QSimula Simula(Processa);
    Simula.exec();

}

void MainWindow::on_ButtonTest_clicked()
{

    Processa->AddMsg(CTipoMessaggio::Test,0,CTipoComando::Impara);
}
