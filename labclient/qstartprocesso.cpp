#include "qstartprocesso.h"
#include "ui_qstartprocesso.h"


QStartProcesso::QStartProcesso(QWidget *parent,QNetPlc *newProcessa) :
    QDialog(parent),
    ui(new Ui::QStartProcesso)
{
    ui->setupUi(this);

    QString str="background-color: rgba(230, 155, 10, 100);";
    str+="border-style: outset;";
    str+="border-width: 2px;";
    str+="border-radius: 10px;";
    str+="border-color: beige;";
    str+="padding: 6px;";
    this->setStyleSheet(str);
    this->setWindowFlags(Qt::FramelessWindowHint);

    Processa=newProcessa;
    timer.start(100,this);

    for(int i=0 ; i < Processa->GasCount; i++){
        Gas[i] = new QGas(ui->widGas);
        Gas[i]->Init(Processa,i);
    }

    Catodo[0] = new QCatodi(ui->widCatodiSx);
    Catodo[0]->Init(Processa,0);
    Catodo[1] = new QCatodi(ui->widCatodiSx);
    Catodo[1]->Init(Processa,1);

    Catodo[2] = new QCatodi(ui->widCatodiDx);
    Catodo[2]->Init(Processa,2);
    Catodo[3] = new QCatodi(ui->widCatodiDx);
    Catodo[3]->Init(Processa,3);

    Bias= new QBias(ui->widBias);
    Bias->Init(Processa);

}

QStartProcesso::~QStartProcesso()
{
    for(int i=0 ; i < Processa->GasCount; i++){
        delete Gas[i] ;
    }
    delete ui;
}


void QStartProcesso::timerEvent(QTimerEvent *event){

    if(event->timerId() == timer.timerId()){
        if(Processa->pData->ProcessoIsOn )
            this->close();
        if(Processa->pData->ErrPreProcesso)
            this->close();
        double perc=Processa->EventTime;
        if(Processa->EventStopTime){
           perc/=Processa->EventStopTime;
           perc*=100;
           ui->progressBar->setValue(perc);
        }

        ui->label->setText("Preset " + QDateTime::fromTime_t(Processa->EventTime).toUTC().toString("hh:mm:ss") + " di " +
                             QDateTime::fromTime_t(Processa->EventStopTime).toUTC().toString("hh:mm:ss"));
        for(int i=0 ; i < Processa->GasCount; i++){
           Gas[i]->Edit();
        }
        for(int i=0 ; i < 3; i++){
           Catodo[i]->Edit();
        }
        Bias->Edit();
        if(Processa->pData->ErrPreProcesso)
            this->close();
    }
    else {
         QWidget::timerEvent(event);
    }

}

void QStartProcesso::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);

    int top=(ui->widGas->height()-3*(Gas[0]->height())-10)/2;
    int left=(ui->widGas->width()-Gas[0]->width())/2;
    Gas[0]->move(left,top);
    for(int i=1 ; i < Processa->GasCount; i++){
      Gas[i]->move(left,Gas[i-1]->height()+Gas[i-1]->pos().ry()+5);
    }

    top=(ui->widCatodiSx->height()-2*(Catodo[0]->height())-5)/2;
    left=(ui->widCatodiSx->width()-Catodo[0]->width())/2;
    Catodo[0]->move(left,top);
    Catodo[1]->move(left,Catodo[0]->height()+Catodo[0]->pos().ry()+5);

    top=(ui->widCatodiDx->height()-2*(Catodo[2]->height())-5)/2;
    left=(ui->widCatodiDx->width()-Catodo[2]->width())/2;
    Catodo[2]->move(left,top);
    Catodo[3]->move(left,Catodo[2]->height()+Catodo[2]->pos().ry()+5);

    top=(ui->widBias->height()-(Bias->height()))/2;
    left=(ui->widBias->width()-Bias->width())/2;
    Bias->move(left,top);

}
