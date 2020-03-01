#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "qpotenza.h"
#include "qcostanti.h"

QBias::QBias(QWidget *parent) :
    QWidget(parent)
{
    Allarme=NULL;
    Arco=NULL;
    PowIsOn=NULL;
    PowErr=NULL;
    IsHVolt=NULL;
    OnEnable=NULL;
    IsLVolt=NULL;
    SetV=NULL;
    PowOn=NULL;
    HVolt=NULL;
    LVolt=NULL;

    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("Bias"));
    this->resize(3*W_DIG+4, 6*(H_DIG+4));

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

    widSx = new QVBoxLayout(this);
    widSx->setObjectName(QStringLiteral("widSx"));
    horizontalLayout->addLayout(widSx);

    widCentro = new QVBoxLayout(this);
    widCentro->setObjectName(QStringLiteral("widCentro"));
    horizontalLayout->addLayout(widCentro);

    widDx = new QVBoxLayout(this);
    widDx->setObjectName(QStringLiteral("widDx"));
    horizontalLayout->addLayout(widDx);

    verticalLayout->addLayout(horizontalLayout);

}

QBias::~QBias(){


}

void QBias::Edit(){

    if(Processa->Bias[IdMacchina].OnEnable ){
        SetV->Edit();
        GetV->Edit();
        GetI->Edit();
    }else{
        if(!Processa->Bias[IdMacchina].IsHVolt->Segnale()){
           SetV->EditSecondo();
           GetV->EditSecondo();
           GetI->EditSecondo();
        }else{
           SetV->Edit();
           GetV->Edit();
           GetI->Edit();
        } 
    }
        
    PowIsOn->Edit();
    PowErr->Edit();
    IsHVolt->Edit();
    OnEnable->Edit();
    IsLVolt->Edit();

    HVolt->Edit();
    LVolt->Edit();
    Allarme->Edit();
    Arco->Edit();

    AcquaIson->Edit();
    Interlock->Edit();
    PortaChiusa->Edit();
    TavRuota->Edit();

    AcquaCamera->Edit();
    AcquaTurbo->Edit();

    Termostato->Edit();

}

void QBias::Init(QNetPlc *newProcessa, int newIdMacchina){

    Processa=newProcessa;
    IdMacchina=newIdMacchina;

    label = new QMLabel(this);
    label->setDescrizione("Bias");
    widSx->addWidget(label);


    SetV= new QAnalogico(this);
    SetV->setAnalogico("SetV","V",Processa->Bias[IdMacchina].SetV);
    widSx->addWidget(SetV);

    GetV= new QAnalogico(this);
    GetV->setAnalogico("GetV","V",Processa->Bias[IdMacchina].GetV);
    widSx->addWidget(GetV);

    PowOn= new QDigitale(this);
    PowOn->setDigitale("PowOn",Processa->Bias[IdMacchina].PowOn);
    widSx->addWidget(PowOn);

    PowIsOn= new QDigitale(this);
    PowIsOn->setDigitale("PowIsOn",Processa->Bias[IdMacchina].PowIsOn);
    widSx->addWidget(PowIsOn);

    PowErr= new QDigitale(this);
    PowErr->setDigitale("PowErr",Processa->Bias[IdMacchina].PowErr);
    widSx->addWidget(PowErr);
    
    if(Processa->Bias[IdMacchina].Allarme){
        Allarme= new QDigitale(this);
        Allarme->setDigitale("Allarme",Processa->Bias[IdMacchina].Allarme);
        widSx->addWidget(Allarme);
    }else
        Allarme=NULL;

    if(Processa->Bias[IdMacchina].Arco){
        Arco= new QDigitale(this);
        Arco->setDigitale("Arco",Processa->Bias[IdMacchina].Arco);
        widSx->addWidget(Arco);
    }else
        Arco=NULL;

    label = new QMLabel(this);
    label->setDescrizione("Bias");
    widCentro->addWidget(label);

    GetI= new QAnalogico(this);
    GetI->setAnalogico("GetI","A",Processa->Bias[IdMacchina].GetI);
    widCentro->addWidget(GetI);

    if(Processa->Bias[IdMacchina].IsHVolt){
      IsHVolt= new QDigitale(this);
      IsHVolt->setDigitale("IsHVolt",Processa->Bias[IdMacchina].IsHVolt);
      widCentro->addWidget(IsHVolt);
    }
    
    if(Processa->Bias[IdMacchina].OnEnable){
       OnEnable= new QDigitale(this);
       OnEnable->setDigitale("OnEnable",Processa->Bias[IdMacchina].OnEnable);
       widCentro->addWidget(OnEnable);
    }else
        OnEnable=NULL;

    if(Processa->Bias[IdMacchina].IsLVolt){
       IsLVolt= new QDigitale(this);
       IsLVolt->setDigitale("IsLVolt",Processa->Bias[IdMacchina].IsLVolt);
       widCentro->addWidget(IsLVolt);
    }else
        IsLVolt=NULL;


    HVolt= new QDigitale(this);
    HVolt->setDigitale("HVolt",Processa->Bias[IdMacchina].HVolt);
    widCentro->addWidget(HVolt);

    LVolt= new QDigitale(this);
    LVolt->setDigitale("LVolt",Processa->Bias[IdMacchina].LVolt);
    widCentro->addWidget(LVolt);

    /////////////////////////////////
    label = new QMLabel(this);
    label->setDescrizione("Resistenza");
    widDx->addWidget(label);

    Interlock= new QDigitale(this);
    Interlock->setDigitale("Interlock",Processa->Vuoto[0].Camera.Interlock);
    widDx->addWidget(Interlock);

    PortaChiusa= new QDigitale(this);
    PortaChiusa->setDigitale("Porta Chiusa",Processa->Vuoto[IdMacchina].Camera.PortaChiusa);
    widDx->addWidget(PortaChiusa);

    AcquaIson= new QDigitale(this);
    AcquaIson->setDigitale("AcquaIson",Processa->Vuoto[0].Camera.AcquaIsOn);
    widDx->addWidget(AcquaIson);

    AcquaTurbo= new QDigitale(this);
    AcquaTurbo->setDigitale("AcquaTurbo",Processa->Vuoto[0].Turbo.Acqua);
    widDx->addWidget(AcquaTurbo);

    AcquaCamera= new QDigitale(this);
    AcquaCamera->setDigitale("AcquaCamera",Processa->Vuoto[0].Camera.Acqua);
    widDx->addWidget(AcquaCamera);

    TavRuota= new QDigitale(this);
    TavRuota->setDigitale("Tav. Ruota",Processa->Vuoto[IdMacchina].Tavola.Ruota);
    widDx->addWidget(TavRuota);

    Termostato = new QDigitale(this);
    Termostato->setDigitale("Termostato",Processa->Vuoto[0].Camera.Termostato);
    widDx->addWidget(Termostato);


}


QCatodi::QCatodi(QWidget *parent) :
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
    Schermo=NULL;

}

QCatodi::~QCatodi(){

    delete SetI;
    delete GetI;
    delete Trigger;
    delete OnPow;
    delete OnMainPow;
    delete IsOnArc;
    delete ErrPow;
    delete Layout;
    delete label;

}

void QCatodi::Edit(){

    if(!Processa->Bias[IdMacchina].Sorgente[SorIndex].Enabled){
        SetI->setEnabled(false);
        GetI->setEnabled(false);
        Trigger->Edit(false);
        OnPow->Edit(false);
        OnMainPow->Edit(false);
        IsOnArc->Edit(false);
        ErrPow->Edit(false);
        Schermo->Edit(false);
        SetI->Edit(0);
        GetI->Edit(0);
        Temp->Edit(0);
        Processa->Bias[IdMacchina].Sorgente[SorIndex].Catodo->Enabled=false;
        Processa->Bias[IdMacchina].Sorgente[SorIndex].Catodo->OnTime();
    }else{
        SetI->setEnabled(true);
        GetI->setEnabled(true);
        SetI->Edit();
        GetI->Edit();
        Trigger->Edit();
        /*if(SorIndex==5){
            bool segnale=(bool)Processa->Bias[IdMacchina].Sorgente[SorIndex].Catodo->OnPow->Segnale();
            //if(segnale)
               qDebug() << SorIndex << segnale;
        }*/
        OnPow->Edit();
        OnMainPow->Edit();
        IsOnArc->Edit();
        ErrPow->Edit();
        Schermo->Edit();
        Temp->Edit();
        Processa->Bias[IdMacchina].Sorgente[SorIndex].Catodo->Enabled=true;
        OnTime->setDescrizione( QString::number(Processa->Bias[IdMacchina].Sorgente[SorIndex].Catodo->OnTime(),'f',2));
    }

    if(Processa->pData->ProcessoIsOn){
      // labelTrig->setDescrizione("TrigCount: "
      //                           + QString::number(Processa->Sorgente[SorIndex].Catodo[1]->pErrore.Count()));
    }

}

void QCatodi::Init(QNetPlc *newProcessa, int index,int newIdMacchina){

    Processa=newProcessa;
    SorIndex=index;
    IdMacchina=newIdMacchina;

    label = new QMLabel(this);
    label->setDescrizione(Processa->Bias[IdMacchina].Sorgente[index].pErrore.Descrizione());
    Layout->addWidget(label);

    OnMainPow= new QDigitale(this);
    OnMainPow->setDigitale("OnMainPow",Processa->Bias[IdMacchina].Sorgente[index].PowOn);
    Layout->addWidget(OnMainPow);

    SetI= new QAnalogico(this);
    SetI->setAnalogico("SetI","A",Processa->Bias[IdMacchina].Sorgente[index].SetI);
    Layout->addWidget(SetI);

    GetI= new QAnalogico(this);
    GetI->setAnalogico("GetI","A",Processa->Bias[IdMacchina].Sorgente[index].Catodo->GetI);
    Layout->addWidget(GetI);
    
    TrigCount = new QMLabel(this);
    TrigCount->setDescrizione("TrigCount: ");
    Layout->addWidget(TrigCount);
    OnTime = new QMLabel(this);
    OnTime->setDescrizione("Ah: ");
    Layout->addWidget(OnTime);
    
    if(Processa->Bias[IdMacchina].Sorgente[index].Catodo->Temp){
        Temp= new QAnalogico(this);
        Temp->setAnalogico("Temp","°C",Processa->Bias[IdMacchina].Sorgente[index].Catodo->Temp);
        Layout->addWidget(Temp);
    }else{
        Temp=NULL;
    }

    Trigger= new QDigitale(this);
    Trigger->setDigitale("Trigger",Processa->Bias[IdMacchina].Sorgente[index].Catodo->Trigger);
    Layout->addWidget(Trigger);

    OnPow= new QDigitale(this);
    OnPow->setDigitale("OnPow",Processa->Bias[IdMacchina].Sorgente[index].Catodo->OnPow);
    Layout->addWidget(OnPow);

    IsOnArc= new QDigitale(this);
    IsOnArc->setDigitale("IsOnArc",Processa->Bias[IdMacchina].Sorgente[index].Catodo->IsOnArc);
    Layout->addWidget(IsOnArc);

    ErrPow= new QDigitale(this);
    ErrPow->setDigitale("ErrPow",Processa->Bias[IdMacchina].Sorgente[index].Catodo->ErrPow);
    Layout->addWidget(ErrPow);

    if(Processa->Bias[IdMacchina].Sorgente[index].Schermo){
        Schermo= new QDigitale(this);
        Schermo->setDigitale("Schermo",Processa->Bias[IdMacchina].Sorgente[index].Schermo);
        Layout->addWidget(Schermo);
    }


    TrigCount=0;

}


QPotenza::QPotenza(QWidget *parent) :
    QWidget(parent)
{

}


QPotenza::~QPotenza()
{
}

void QPotenza::Edit(){

    Bias[IdMacchina].Edit();
    if(!VersioneRidotta)
        Macchina->Edit();
    if(Processa->CamereCount==1){
        for(int i=0 ; i < 6; i++){
           Catodo[i]->Edit();
        }
    }else
        MacchinaDue->Edit();

}

void QPotenza::setVersioneRidotta(){

    VersioneRidotta=true;
    widSx->setVisible(false);
    widDx->setVisible(false);
    for(int i=0;i<6;i++)
       Catodo[i]->setVisible(false);
    Bias[IdMacchina].setVisible(false);

}



void QPotenza::Init(QNetPlc *newProcessa,int newIdMacchina){

    IdMacchina=newIdMacchina;
    Processa=newProcessa;
    getFoglio(&wFoglio,&hFoglio);
    VersioneRidotta=false;

    if(Processa->CamereCount==1){
        QHBoxLayout *horizontalLayout;
        QVBoxLayout *verticalLayout;
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
        widBias = new QWidget(widCentro);
        widBias->setObjectName(QStringLiteral("widBias"));
        verticalLayout->addWidget(widBias);
        widDx = new QWidget(this);
        horizontalLayout->addWidget(widDx);
        this->setWindowTitle(QApplication::translate("QPotenza", "Form", 0));
    }else{
        QHBoxLayout *horizontalLayout;
        QVBoxLayout *verticalLayout;
        if (this->objectName().isEmpty())
             this->setObjectName(QStringLiteral("QVuoto"));
        this->resize(924, 558);
        horizontalLayout = new QHBoxLayout(this);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);

        widSx = new QWidget(this);
        horizontalLayout->addWidget(widSx);

        //widCentro = new QWidget(this);
        //horizontalLayout->addWidget(widCentro);
        verticalLayout = new QVBoxLayout(widSx);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        Macchina = new CDisegnoMacchina(widSx);
        Macchina->setMinimumSize(QSize(wFoglio, hFoglio));
        verticalLayout->addWidget(Macchina);

        widBias = new QWidget(widCentro);
        widBias->setObjectName(QStringLiteral("widBias"));
        verticalLayout->addWidget(widBias);

        widDx = new QWidget(this);
        horizontalLayout->addWidget(widDx);
        verticalLayout = new QVBoxLayout(widDx);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        MacchinaDue = new CDisegnoMacchina(widDx);
        MacchinaDue->setMinimumSize(QSize(wFoglio, hFoglio));
        verticalLayout->addWidget(MacchinaDue);
        this->setWindowTitle(QApplication::translate("QPotenza", "Form", 0));
    }

    if(Processa->CamereCount==1){
        Catodo[0] = new QCatodi(widSx);
        Catodo[0]->Init(Processa,3,IdMacchina);
        Catodo[0]->move(10,10);
        Catodo[1] = new QCatodi(widSx);
        Catodo[1]->Init(Processa,5,IdMacchina);
        Catodo[2] = new QCatodi(widSx);
        Catodo[2]->Init(Processa,0,IdMacchina);

        Catodo[3] = new QCatodi(widDx);
        Catodo[3]->Init(Processa,2,IdMacchina);
        Catodo[3]->move(10,10);
        Catodo[4] = new QCatodi(widDx);
        Catodo[4]->Init(Processa,4,IdMacchina);
        Catodo[5] = new QCatodi(widDx);
        Catodo[5]->Init(Processa,1,IdMacchina);
    }

    Bias = new QBias(widBias);

    Bias->Init(Processa,IdMacchina);
    Macchina->Init(Processa,0);
    if(Processa->CamereCount==2)
        MacchinaDue->Init(Processa,1);

    if(Processa->CamereCount==1){
        for(int i=0; i<4;i++){
            connect(Catodo[i]->OnMainPow, SIGNAL( CambiaSegnale(int,int,int)),
                     this, SLOT(CambiaSegnale(int,int,int)));
            connect(Catodo[i]->OnPow, SIGNAL( CambiaSegnale(int,int,int)),
                     this, SLOT(CambiaSegnale(int,int,int)));
            connect(Catodo[i]->SetI, SIGNAL( CambiaSegnale(int,int,int)),
                     this, SLOT(CambiaSegnale(int,int,int)));
        }
    }

    
    connect(Bias[IdMacchina].PowOn, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Bias[IdMacchina].SetV, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Bias[IdMacchina].LVolt, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));
    connect(Bias[IdMacchina].HVolt, SIGNAL( CambiaSegnale(int,int,int)),
             this, SLOT(CambiaSegnale(int,int,int)));

}

void QPotenza::CambiaSegnale( int newslot,int newpoint, int newsegnale){

    emit sendCambiaSegnale(newslot,newpoint,  newsegnale);
}

void QPotenza::paintEvent(QPaintEvent *event){

     Q_UNUSED(event);

    int left,spazio;
    if(Processa->CamereCount==1){
        left=( widSx->width()-Catodo[0]->width())/2;
        spazio=( widSx->height()-3*Catodo[0]->height())/4;
        Catodo[0]->move(left,spazio);
        Catodo[1]->move(left,Catodo[0]->height()+Catodo[0]->pos().ry()+spazio);
        Catodo[2]->move(left,Catodo[1]->height()+Catodo[1]->pos().ry()+spazio);

        left=( widDx->width()-Catodo[3]->width())/2;
        spazio=( widSx->height()-3*Catodo[3]->height())/4;
        Catodo[3]->move(left,spazio);
        Catodo[4]->move(left,Catodo[3]->height()+Catodo[3]->pos().ry()+spazio);
        Catodo[5]->move(left,Catodo[4]->height()+Catodo[4]->pos().ry()+spazio);
    }


    left=( Macchina->width()-Bias->width())/2;
    //int top=(widBias[IdMacchina].height()- Bias[IdMacchina].height())/2;
    //Bias[IdMacchina].move(left ,top);

}
