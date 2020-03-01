#include "initdialog.h"
#include "ui_initdialog.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QTimer>

void scrivi(){

    QString path=QDir::homePath() + "/labclientnet";
    QFile file(path);

    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        QTextStream out(&file);
        out << "Primo " << "cnet->host " << "cnet->porta " << "cnet->impianto " << "\n";
        out << "Secondo " << "cnet->host " << "cnet->porta " << "cnet->impianto "<< "\n";
        file.close();

    }
}




InitDialog::InitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QInitDialog)
{
    ui->setupUi(this);

}

InitDialog::~InitDialog()
{
    delete ui;
}

int InitDialog::ApriFile(){

    QString path=QDir::homePath() + "/labclientnet";
    QFile file(path);

    if(file.open(QIODevice::ReadOnly )){
        QTextStream out(&file);
        while (!out.atEnd()) {
            CNetDb *cnet= new CNetDb();
            out >> cnet->wFoglio>> cnet->hFoglio >>
                    cnet->Nome >> cnet->Impianto >> cnet->PortaPlc >>
                   cnet->DbDatiHost >> cnet->DbDatiClient >> cnet->DbDatiPass >> cnet->DbDatiPorta >>
                   cnet->DbProcessoHost >> cnet->DbProcessoClient >> cnet->DbProcessoPass >> cnet->DbProcessoPorta;
            if(cnet->DbProcessoClient.isEmpty())
                cnet->IsRidotta=true;
            else
                cnet->IsRidotta=false;
            if(!cnet->Nome.isEmpty()){
                pNet.append(cnet);
                ui->Nome->addItem(cnet->Nome);
                if(pNet.count() ==1){
                    Aggiorna(0);
                }
            }
        }
        file.close();
        return pNet.count();
    }
    return 0;
}

void InitDialog::Aggiorna(int index){


    Net=*pNet.at(index);
    ui->Host->setText(pNet.at(index)->DbDatiHost);
    ui->Impianto->setText(pNet.at(index)->Impianto);
    ui->PlcPorta->setValue(pNet.at(index)->PortaPlc.toInt());
    ui->DatiUtente->setText(pNet.at(index)->DbDatiClient);
    ui->DatiPass->setText(pNet.at(index)->DbDatiPass);
    ui->DatiPorta->setValue(pNet.at(index)->DbDatiPorta.toInt());

}

void InitDialog::on_Salva_clicked()
{

}

void InitDialog::on_Annulla_clicked()
{
    Connetti=false;
    this->close();
}

void InitDialog::on_Connetti_clicked()
{
    Connetti=true;
    this->close();
}


void InitDialog::on_Nome_activated(int index)
{
    Aggiorna(index);
}
