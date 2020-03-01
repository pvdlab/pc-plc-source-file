#include "qsimula.h"
#include "ui_qsimula.h"

QSimula::QSimula(QNetPlc *newProcessa,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSimula)
{
    Processa=newProcessa;
    QString str="background-color: rgba(230, 155, 10, 100);";
    str+="border-style: outset;";
    str+="border-width: 2px;";
    str+="border-radius: 10px;";
    str+="border-color: beige;";
    str+="padding: 6px;";

    this->setStyleSheet(str);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
}

QSimula::~QSimula()
{
    delete ui;
}



void QSimula::on_BiasLow_clicked()
{
    Processa->AddMsg(CTipoMessaggio::Simula,Simula_BiasLow,ui->BiasLowTempo->value());
    this->close();
}

void QSimula::on_chiudi_clicked()
{
    this->close();
}
