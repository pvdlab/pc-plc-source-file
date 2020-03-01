#include "qmostraerrore.h"
#include "ui_qmostraerrore.h"



QMostraErrore::QMostraErrore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QMostraErrore)
{
    ui->setupUi(this);

    QString str="background-color: rgba(204, 0, 0, 100);";
    str+="border-style: outset;";
    str+="border-width: 2px;";
    str+="border-radius: 10px;";
    str+="border-color: beige;";
    str+="padding: 6px;";
    this->setStyleSheet(str);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

QMostraErrore::~QMostraErrore()
{
    delete ui;
}

void QMostraErrore::addErrore(QString errore){

    if(lasterrore!=errore){
      ui->listWidget->addItem(errore);
      int count=ui->listWidget->count()-1;
      ui->listWidget->item(count)->setForeground(Qt::white);
    }
    lasterrore=errore;
}

void QMostraErrore::on_ButtonOk_clicked()
{
    this->close();
}
