#include "qmiomenu.h"
#include "ui_qmiomenu.h"
#include <QDebug>
#include <QDir>
#include "qstartprocesso.h"

qMioMenu::qMioMenu(QNetPlc *newProcessa,int IdMacchina, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::qMioMenu)
{
    ui->setupUi(this);
    Processa=newProcessa;


    this->setWindowFlags(Qt::FramelessWindowHint);

    QString path = QDir::homePath() + "/run" + PLC_PROCESSO_PATH + "/";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*_*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList()) {
        int pos=dirFile.indexOf(".");
        dirFile=dirFile.left(pos);
        ProcessoLista.append(dirFile);
        pos=dirFile.indexOf("_");
        if(pos>0){
           dirFile=dirFile.mid(pos+1,dirFile.length());
        }
        if(dirFile.right(1)==QString::number(IdMacchina)){
           pos=dirFile.indexOf("_");
           dirFile=dirFile.left(pos);
           ui->processo->addItem(dirFile);
        }
    }

    on_processo_activated(0);



}

void qMioMenu::paintEvent(QPaintEvent *event){


}

void qMioMenu::mousePressEvent(QMouseEvent *event){

    this->close();

}



qMioMenu::~qMioMenu()
{
    delete ui;
}



void qMioMenu::on_Chiudi_clicked()
{
    this->close();
}

void qMioMenu::on_processo_activated(int index)
{
    if(ProcessoLista.count()>index){
       QString str=ProcessoLista.at(index);
       int pos=str.indexOf("_");
       str=str.left(pos);
       ProcessoId=str.toInt();
    }else
       ProcessoId=0;

    ProcessoNome=ui->processo->currentText();

}
