#include "qmstart.h"
#include "ui_qmstart.h"
#include <QTimer>
#include <QDateTime>

QMStart::QMStart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QMStart)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()),
                     this, SLOT(Scan()));
    timer->start(100);
    perc=0;
}

QMStart::~QMStart()
{
    delete ui;
}


void QMStart::Scan(){

    if(perc+10>100)
        perc=0;
    else
        perc+=10;

    ui->progressBar->setValue(perc);

}

void QMStart::MostraEvento(QString evento){

    ui->listWidget->addItem(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + " " + evento);

}
