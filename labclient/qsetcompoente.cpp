#include "qsetcompoente.h"
#include "ui_qsetcompoente.h"
#include <QDateTime>
#include "qdebug.h"



QSetCompoente::QSetCompoente(QNetPlc *newProcessa,int newIdMacchina,CIOSegnale *new_pSegnale,int newSegnale,int max, QString Descrizione,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSetCompoente)
{
    ui->setupUi(this);
    IdMacchina=newIdMacchina;
    Processa=newProcessa;
    pSegnale=new_pSegnale;
    TipoComp=QString::number(pSegnale->TipoComp).left(1).toInt();
    this->setWindowTitle(Descrizione);
    int TimeEventStop=Processa->pData[IdMacchina].TimeEventStop;
    int TimeEvent=Processa->pData[IdMacchina].TimeEvent;

    ui->ValoreSet->setMaximum(max);
    ui->Valore->setText(QString::number((int)(newSegnale*pSegnale->Fattore())));
    ui->ValoreSet->setValue((int)(newSegnale*pSegnale->Fattore()));
    if(Processa->pData[IdMacchina].TimeEventStop==0)
        Processa->pData[IdMacchina].TimeEventStop=3600;
    on_TimeOutSet_sliderMoved(Processa->pData[IdMacchina].TimeEventStop-Processa->pData[IdMacchina].TimeEvent);
    ui->TimeOutSet->setValue(Processa->pData[IdMacchina].TimeEventStop-Processa->pData[IdMacchina].TimeEvent);

}

QSetCompoente::QSetCompoente(QNetPlc *newProcessa,int newIdMacchina,CIOSegnale *newSegnale,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSetCompoente)
{
    ui->setupUi(this);
    IdMacchina=newIdMacchina;
    Processa=newProcessa;
    pSegnale=newSegnale;
    TipoComp=QString::number(pSegnale->TipoComp).left(1).toInt();
    this->setWindowTitle(QString(pSegnale->Descrizione()));


    if(QString::number(pSegnale->TipoComp).left(1)=="2")
       ui->ValoreSet->setMaximum(80);
    else
       ui->ValoreSet->setMaximum(1000);

    ui->Valore->setText(QString::number((int)pSegnale->DSegnale()));
    ui->ValoreSet->setValue((int)pSegnale->DSegnale());
    if(Processa->pData[IdMacchina].TimeEventStop==0)
        Processa->pData[IdMacchina].TimeEventStop=3600;
    on_TimeOutSet_sliderMoved(Processa->pData[IdMacchina].TimeEventStop-Processa->pData[IdMacchina].TimeEvent);
    ui->TimeOutSet->setValue(Processa->pData[IdMacchina].TimeEventStop-Processa->pData[IdMacchina].TimeEvent);

}

QSetCompoente::~QSetCompoente()
{
    delete ui;
}



void QSetCompoente::on_Set_clicked()
{
    int Segnale=0;
    int SegnaleSecondo=0;
    int Info=0;
    BYTE Enabled=0;
    SegnaleSecondo=0;
    BYTE Slot=0;
    BYTE Point=0;
    int Index=0;
    if((TipoComp==CTipoComp::Bias) | (TipoComp==CTipoComp::Gas) | (TipoComp==CTipoComp::Sorgente)){
        Slot=pSegnale->Slot();
        Point=pSegnale->Point();
        Index=QString::number(pSegnale->TipoComp).right(1).toInt();
        if((TipoComp==CTipoComp::Bias)){
            //pSegnale->DSegnale(ui->Valore->text().toInt());
            if(ui->Valore->text().toInt()>400)
                Info=1;
            else
                Info=0;
            Segnale=Processa->Bias[IdMacchina].Segnale(ui->Valore->text().toDouble());
        }else if((TipoComp==CTipoComp::Sorgente)){
            for(int i=0; i< Processa->SorgenteCount; i++){
                Enabled=SetBit(Enabled,i,Processa->Bias[IdMacchina].Sorgente[i].Enabled);
            }
            Processa->Bias[IdMacchina].Sorgente[Index].Enabled=true;
            Enabled=SetBit(Enabled,Index,Processa->Bias[IdMacchina].Sorgente[Index].Enabled);
            Segnale=ui->Valore->text().toInt()/pSegnale->Fattore();
        }else{
            Segnale=ui->Valore->text().toInt()/pSegnale->Fattore();
        }
    }else{
        Segnale=ui->Valore->text().toInt()/pSegnale->Fattore();
    }

    QByteArray msg(50,0);
    char *pBuffer=msg.data();
    int len=0;
    pBuffer=ByteStr(pBuffer,TipoComp,&len);
    pBuffer=ByteStr(pBuffer,Index,&len);
    pBuffer=ByteStr(pBuffer,Slot,&len);
    pBuffer=ByteStr(pBuffer,Point,&len);
    pBuffer=ByteStr(pBuffer,Enabled,&len);
    pBuffer=WORDStr(pBuffer,Segnale,&len);
    pBuffer=WORDStr(pBuffer,SegnaleSecondo,&len);
    pBuffer=WORDStr(pBuffer,Info,&len);
    pBuffer=DWORDStr(pBuffer,TimeOut,&len);
    msg.resize(len);
    Processa->AddMsg(CTipoMessaggio::Manuale, IdMacchina,msg);
    this->close();
}

void QSetCompoente::on_Chiudi_clicked()
{
    this->close();
}


void QSetCompoente::on_ValoreSet_sliderMoved(int position)
{
    ui->Valore->setText(QString::number(position));
}



void QSetCompoente::on_TimeOutSet_sliderMoved(int position)
{
   ui->TimeOut->setText( QDateTime::fromTime_t(position).toUTC().toString("hh:mm:ss"));
   TimeOut = position*1000;
   qDebug() << position;

}




void QSetCompoente::on_Valore_editingFinished()
{
    ui->ValoreSet->setValue(ui->Valore->text().toInt());
}
