#include "qdialogtavola.h"
#include "ui_qdialogtavola.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>

QDialogTavola::QDialogTavola(QString newCoating,QString newCiclo,QString newIndirizzo,QString newdpuser,QString newdbpassword, QString newid,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QDialogTavola)
{
    ui->setupUi(this);

    Ciclo=newCiclo;
    Indirizzo=newIndirizzo;
    Coating=newCoating;
    dpuser=newdpuser;
    dbpassword=newdbpassword;
    id=newid;

    QString str="background-color: rgba(174, 205, 138, 1);";
    str+="border-style: outset;";
    str+="border-width: 2px;";
    str+="border-radius: 10px;";
    //str+="border-color: beige;";
    str+="padding: 6px;";
    this->setStyleSheet(str);
    this->setWindowFlags(Qt::FramelessWindowHint);

    Database = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","connectGestionale"));
    Database->setDatabaseName("GESTIONALE");
    Database->setHostName(Indirizzo);
    Database->setPort(-1);
    if (!Database->open(dpuser, dbpassword)) {
        QMessageBox msgBox;
        msgBox.setText( " GESTIONALE NON CONNESSO " );
        msgBox.exec();
        this->close();
    }

    TavolaMod=new QSqlQueryModel();
    TavolaMod->setQuery("SELECT TAVOLA, COATING FROM TAVOLA WHERE COATING = '" + Coating + "' AND CICLO = '0' GROUP BY TAVOLA ",*Database);
    qDebug() <<  "SELECT TAVOLA, COATING FROM TAVOLA WHERE COATING = '" + Coating + "' AND CICLO = '0' GROUP BY TAVOLA ";
    if(TavolaMod->rowCount()==0){
        this->deleteLater();
        return;
    }

   /* this->setAttribute(Qt::WA_DeleteOnClose);
    this->exec();
    ui->Tavola->setModel(TavolaMod);*/


    ui->Tavola->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->Tavola->setShowGrid(false);
    ui->Tavola->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    ui->Tavola->verticalHeader()->hide();
    ui->Tavola->verticalHeader()->setDefaultSectionSize(20);
    Tavola="";
    update();

    connect(ui->Tavola->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(RowChange(QModelIndex)));


}

void QDialogTavola::RowChange(QModelIndex index){

    Tavola=TavolaMod->record(index.row()).value("TAVOLA").toString();
}

QDialogTavola::~QDialogTavola()
{
    QString connection;
    connection=Database->connectionName();
    Database->close();
    delete Database;
    QSqlDatabase::removeDatabase(connection);
    delete ui;
}



void QDialogTavola::paintEvent(QPaintEvent *event){

    int with=ui->Tavola->width();
    int Dwith=with/3;
    ui->Tavola->setColumnWidth(1,Dwith);
    ui->Tavola->setColumnWidth(2,Dwith);


}
void QDialogTavola::on_OkButton_clicked()
{
    QSqlQuery query(*Database);
    QString str="UPDATE TAVOLA SET ID_CICLO = " + Ciclo + " WHERE TAVOLA = " + Tavola + " ";
    if( !query.exec(str) ){
        QMessageBox msgBox;
        msgBox.setText( query.lastError().text());
        msgBox.exec();
        this->close();
    }else{
        QString str="DELETE FROM TAVOLA_ATTUALE WHERE TAVOLA = " + Tavola;
        if( !query.exec(str) ){
            QMessageBox msgBox;
            msgBox.setText( query.lastError().text());
            msgBox.exec();
        }
    }
    this->close();
}

void QDialogTavola::on_ButtonChiudi_clicked()
{
    this->close();
}
