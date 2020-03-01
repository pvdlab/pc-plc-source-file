#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "qvuoto.h"
#include "qanalogico.h"
#include "qcostanti.h"

QGas::QGas(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("QResistenza"));
    this->resize(W_DIG+4, 4*(H_DIG+4));

    Layout = new QVBoxLayout(this);
    Layout->setSpacing(2);
    Layout->setObjectName(QStringLiteral("Layout"));
    Layout->setContentsMargins(2, 2, 2, 2);
    Layout->setObjectName(QStringLiteral("Layout"));

}


QGas::~QGas(){

    delete GetGas;
    delete SetGas;
    delete Valv;
    delete label;
    delete Layout;
}

void QGas::Edit(){

    GetGas->Edit();
    SetGas->Edit();
    Valv->Edit();

}

void QGas::Init(QNetPlc *newProcessa,int index){

    Processa=newProcessa;

    label = new QMLabel(this);
    label->setDescrizione(Processa->GGas[index].pErrore.Descrizione());
    Layout->addWidget(label);


    SetGas= new QAnalogico(this);
    SetGas->setAnalogico("SetGas","sccm",Processa->GGas[index].SetG);
    Layout->addWidget(SetGas);

    GetGas = new QAnalogico(this);
    GetGas->setAnalogico("GetGas","sccm",Processa->GGas[index].GetG);
    Layout->addWidget(GetGas);

    Valv= new QDigitale(this);
    Valv->setDigitale("Valv",Processa->GGas[index].Valvola);
    Layout->addWidget(Valv);

}

//////////////Meccanica//////////////////////
QMeccanica::QMeccanica(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("QResistenza"));
    this->resize(W_DIG+4, 4*(H_DIG+4));

    Layout = new QVBoxLayout(this);
    Layout->setSpacing(2);
    Layout->setObjectName(QStringLiteral("Layout"));
    Layout->setContentsMargins(2, 2, 2, 2);
    Layout->setObjectName(QStringLiteral("Layout"));

}


QMeccanica::~QMeccanica(){

    delete OnPwer;
    delete IsOnPwer;
    delete Errore;
    delete Layout;
}

void QMeccanica::Edit(){

    OnPwer->Edit();
    IsOnPwer->Edit();
    Errore->Edit();
    double Tempo=Processa->Vuoto->Meccanica.OnTime();
    Tempo/=3600;
    OnTime->setDescrizione( QString::number(Tempo,'f',2));

}

void QMeccanica::Init(QNetPlc *newProcessa){

    Processa=newProcessa;

    label = new QMLabel(this);
    label->setDescrizione("Meccanica");
    Layout->addWidget(label);


    OnPwer= new QDigitale(this);
    OnPwer->setDigitale("OnPwer",Processa->Vuoto->Meccanica.PowOn );
    Layout->addWidget(OnPwer);

    IsOnPwer= new QDigitale(this);
    IsOnPwer->setDigitale("IsOnPwer",Processa->Vuoto->Meccanica.PowIsOn);
    Layout->addWidget(IsOnPwer);


    Errore= new QDigitale(this);
    Errore->setDigitale("Errore",Processa->Vuoto->Meccanica.PowErr);
    Layout->addWidget(Errore);

    OnTime = new QMLabel(this);
    OnTime->setDescrizione("Ore: ");
    Layout->addWidget(OnTime);


}
//////////////turbo//////////////////////
QTurbo::QTurbo(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("QResistenza"));
    this->resize(W_DIG+4, 7*(H_DIG+4));

    Layout = new QVBoxLayout(this);
    Layout->setSpacing(2);
    Layout->setObjectName(QStringLiteral("Layout"));
    Layout->setContentsMargins(2, 2, 2, 2);
    Layout->setObjectName(QStringLiteral("Layout"));

}


QTurbo::~QTurbo(){

    delete Start;
    delete Stop;
    delete Accelerazione;
    delete Normale;
    delete Errore;
    delete Layout;
}

void QTurbo::Edit(){

    Temperatura->Edit();
    Start->Edit();
    Stop->Edit();
    Accelerazione->Edit();
    Normale->Edit();
    Errore->Edit();
    double Tempo=Processa->Vuoto->Turbo.OnTime()/3600;
    OnTime->setDescrizione(QString::number(Tempo,'f',2));

}

void QTurbo::Init(QNetPlc *newProcessa){

    Processa=newProcessa;

    label = new QMLabel(this);
    label->setDescrizione("Turbo");
    Layout->addWidget(label);

    Temperatura =new QAnalogico(this);
    Temperatura->setAnalogico("Temp","°C",Processa->Vuoto->Turbo.Temp);
    Layout->addWidget(Temperatura);

    Start= new QDigitale(this);
    Start->setDigitale("Start",Processa->Vuoto->Turbo.Start );
    Layout->addWidget(Start);

    Stop= new QDigitale(this);
    Stop->setDigitale("Stop",Processa->Vuoto->Turbo.Stop );
    Layout->addWidget(Stop);

    Accelerazione= new QDigitale(this);
    Accelerazione->setDigitale("Accelerazione",Processa->Vuoto->Turbo.Acc );
    Layout->addWidget(Accelerazione);

    Normale= new QDigitale(this);
    Normale->setDigitale("Normale",Processa->Vuoto->Turbo.Normale );
    Layout->addWidget(Normale);

    Errore= new QDigitale(this);
    Errore->setDigitale("Errore",Processa->Vuoto->Turbo.Errore );
    Layout->addWidget(Errore);

    Acqua= new QDigitale(this);
    Acqua->setDigitale("Acqua",Processa->Vuoto->Turbo.Acqua );
    Layout->addWidget(Acqua);

    OnTime = new QMLabel(this);
    OnTime->setDescrizione("Ore: ");
    Layout->addWidget(OnTime);

}
//////////

QResistenza::QResistenza(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("QResistenza"));
    this->resize(W_DIG+4, 9*(H_DIG+4));

    Layout = new QVBoxLayout(this);
    Layout->setSpacing(2);
    Layout->setObjectName(QStringLiteral("Layout"));
    Layout->setContentsMargins(2, 2, 2, 2);
    Layout->setObjectName(QStringLiteral("Layout"));


}

QResistenza::~QResistenza()
{

    //delete TempTurbo;
    delete TempCamera;
    delete TempResistenza;
    delete TavRuota;
    delete PortaChiusa;
    delete AcquaCamera;
    delete AriaCompressaOn;
    delete LVentValv;
    delete label;
    delete Layout;
}

void QResistenza::Edit(){

    TempCatodo->Edit();
    TempCamera->Edit();
    TempResistenza->Edit();
    TavRuota->Edit();
    PortaChiusa->Edit();
    AcquaCamera->Edit();
    AriaCompressaOn->Edit();
    LVentValv->Edit();

}

void QResistenza::Init(QNetPlc *newProcessa){

    Processa=newProcessa;

    label = new QMLabel(this);
    label->setDescrizione("Resistenza");
    Layout->addWidget(label);

    TempCamera = new QAnalogico(this);
    TempCamera->setAnalogico("Camera","°C",Processa->Vuoto->Camera.Temp);
    Layout->addWidget(TempCamera);

    TempResistenza= new QAnalogico(this);
    TempResistenza->setAnalogico("Res","°C",Processa->Vuoto->Resistenza->Temp_Resistenza);
    Layout->addWidget(TempResistenza);

    if(Processa->Vuoto->Camera.TempCatodo){
        TempCatodo =new QAnalogico(this);
        TempCatodo->setAnalogico("Catodo","°C",Processa->Vuoto->Camera.TempCatodo);
        Layout->addWidget(TempCatodo);
    }else
        TempCatodo=NULL;


    Interlock= new QDigitale(this);
    Interlock->setDigitale("Interlock",Processa->Vuoto->Camera.Interlock);
    //Interlock->setDigitale("Interlock",Processa->Vuoto->r;
    Layout->addWidget(Interlock);

    PortaChiusa= new QDigitale(this);
    PortaChiusa->setDigitale("Porta Chiusa",Processa->Vuoto->Camera.PortaChiusa);
    Layout->addWidget(PortaChiusa);

    AcquaCamera= new QDigitale(this);
    AcquaCamera->setDigitale("AcquaCamera",Processa->Vuoto->Camera.Acqua);
    Layout->addWidget(AcquaCamera);

    TavRuota= new QDigitale(this);
    TavRuota->setDigitale("Tav. Ruota",Processa->Vuoto->Tavola.Ruota);
    Layout->addWidget(TavRuota);

    AriaCompressaOn= new QDigitale(this);
    AriaCompressaOn->setDigitale("AriaCompressaOn",Processa->Vuoto->Camera.AriaComp);
    Layout->addWidget(AriaCompressaOn);

    LVentValv= new QDigitale(this);
    LVentValv->setDigitale("LVentValv",Processa->Vuoto->Camera.Valv);
    Layout->addWidget(LVentValv);
}


QVuoto::QVuoto(QWidget *parent) :
    QWidget(parent)
{
    VersioneRidotta=false;

    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    getFoglio(&wFoglio,&hFoglio);

    if (this->objectName().isEmpty())
         this->setObjectName(QStringLiteral("QVuoto"));
    this->resize(924, 558);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    widSx = new QWidget(this);
    horizontalLayout->addWidget(widSx);

    widCentro = new QWidget(this);
    horizontalLayout->addWidget(widCentro);
    verticalLayout = new QVBoxLayout(widCentro);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    Macchina = new CDisegnoMacchina(widCentro);
    Macchina->setMinimumSize(QSize(wFoglio, hFoglio));
    verticalLayout->addWidget(Macchina);
    widResistenza = new QWidget(widCentro);
    widResistenza->setObjectName(QStringLiteral("widBias"));
    verticalLayout->addWidget(widResistenza);
    widDx = new QWidget(this);
    horizontalLayout->addWidget(widDx);

    this->setWindowTitle(QApplication::translate("QVuoto", "Form", 0));


}

QVuoto::~QVuoto()
{
}


void QVuoto::setVersioneRidotta(){

    VersioneRidotta=true;
    widSx->setVisible(false);
    widDx->setVisible(false);
    Resistenza->setVisible(false);

    //Macchina->setMinimumSize(QSize(800, 0));

}


void QVuoto::CambiaSegnale( int newslot,int newpoint, int newsegnale){

    emit sendCambiaSegnale(newslot,newpoint,  newsegnale);
}

void QVuoto::Edit(){

    if(!VersioneRidotta)
       //Macchina->Edit();
    Resistenza->Edit();
    Penning->Edit();
    if(Processa->Vuoto->Baratron.Segnale)
       Baratron->Edit();
    if(Processa->Vuoto->Baratron.Pirani)
       Pirani->Edit();
    Turbo->Edit();
    Meccanica->Edit();

    for(int i=0 ; i < Processa->GasCount; i++){
       Gas[i]->Edit();
    }
}

void QVuoto::Init(QNetPlc *newProcessa){

    Processa=newProcessa;

    Resistenza = new QResistenza(widResistenza);
    Resistenza->Init(Processa);

    Penning=new QSensore(widDx);
    Penning->setAnalogico("Penn","mbar",
                          Processa->Vuoto->Penning->Segnale());

    if(Processa->Vuoto->Baratron.Segnale){
        Baratron=new QSensore(widDx);
        Baratron->setAnalogico("Bara","mbar",
                              Processa->Vuoto->Baratron.Segnale);
    }


    if(Processa->Vuoto->Baratron.Pirani){
        Pirani=new QSensore(widDx);
        Pirani->setAnalogico("Pira","mbar",
                              Processa->Vuoto->Baratron.Pirani);
    }


    Turbo= new QTurbo(widDx);
    Turbo->Init(Processa);

    Meccanica= new QMeccanica(widDx);
    Meccanica->Init(Processa);

    for(int i=0 ; i < Processa->GasCount; i++){
        Gas[i] = new QGas(widSx);
        Gas[i]->Init(Processa,i);
        connect(Gas[i]->Valv, SIGNAL( CambiaSegnale(int,int,int)),
                 this, SLOT(CambiaSegnale(int,int,int)));
        connect(Gas[i]->SetGas, SIGNAL( CambiaSegnale(int,int,int)),
                 this, SLOT(CambiaSegnale(int,int,int)));
    }

    connect(Turbo->Start, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Turbo->Stop, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Meccanica->OnPwer, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Resistenza->AcquaCamera, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Resistenza->AriaCompressaOn, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Resistenza->LVentValv, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));

    connect(Macchina, SIGNAL( StartProcesso()),
             this, SLOT(on_StartProcesso()));


    Macchina->Init(Processa,0);


}

void QVuoto::on_StartProcesso(){

    emit StartProcesso();

}

void QVuoto::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);

    int left=( widDx->width()-Penning->width())/2;

    Penning->move(left,50);
    int top=Penning->height()+ Penning->pos().ry();
    if(Processa->Vuoto->Baratron.Segnale){
      Baratron->move(left,top+50);
      top=Baratron->height()+Baratron->pos().ry();
    }
    if(Processa->Vuoto->Baratron.Pirani){
      Pirani->move(left,top+50);
      top=Pirani->height()+Pirani->pos().ry();
    }
    Turbo->move(left,top+50);
    Meccanica->move(left,Turbo->height()+Turbo->pos().ry()+50);

    left=( widSx->width()-Gas[0]->width())/2;
    Gas[0]->move(left,50);
    for(int i=1 ; i < Processa->GasCount; i++)
      Gas[i]->move(left,Gas[i-1]->height()+Gas[i-1]->pos().ry()+50);

    left=( Macchina->width()-Resistenza->width())/2;
    top=(widResistenza->height()- Resistenza->height())/2;
    Resistenza->move(left ,top);

}
