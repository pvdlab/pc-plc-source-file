#include "qmostraeventi.h"
#include "ui_qmostraeventi.h"
#include <QDebug>


QMostraEventi::QMostraEventi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QMostraEventi)
{
    ui->setupUi(this);



    QString str="background-color: rgba(41, 27, 27, 100);";
    str+="border-style: outset;";
    str+="border-width: 2px;";
    str+="border-radius: 10px;";
    str+="border-color: beige;";
    str+="padding: 6px;";

    this->setStyleSheet(str);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->ButtonAnnulla->setVisible(false);
    ui->ButtonElimina->setVisible(false);


}

QMostraEventi::~QMostraEventi()
{
    delete ui;
}

void QMostraEventi::addLista(QListWidget *newlista){

    if(ui->listWidget->count())
        return;


    for(int i=0; i<newlista->count();i++){
        QString str=newlista->item(i)->text();
        ui->listWidget->addItem(str);
        ui->listWidget->item(i)->setForeground(Qt::white);
    }

}

void QMostraEventi::addLista(QString str){

     ui->listWidget->addItem(str);

}

void QMostraEventi::clear(){

    ui->listWidget->clear();

}

void QMostraEventi::on_ButtonElimina_clicked()
{

    ui->listWidget->clear();
    close();
}

void QMostraEventi::on_ButtonAnnulla_clicked()
{
    close();
}

void QMostraEventi::on_pushButton_clicked()
{
    close();
}
