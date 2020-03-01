#include "qsetting.h"
#include "ui_qsetting.h"

QSetting::QSetting(QNetPlc *newProcessa,int newIdMacchina,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSetting)
{
    ui->setupUi(this);
    IdMacchina=newIdMacchina;
    Processa=newProcessa;
}

QSetting::~QSetting()
{
    delete ui;
}

void QSetting::on_Impara_clicked()
{
    Processa->AddMsg(CTipoMessaggio::SetComando,IdMacchina,CTipoComando::Impara);
    this->close();
}

void QSetting::on_Chiudi_clicked()
{
    this->close();
}
