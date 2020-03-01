
#include <QApplication>
#include <QMessageBox>
#include <QSplashScreen>
#include "mainwindow.h"
#include "qlabservernet.h"
#include <unistd.h>
#include <QHostInfo>
#include <QSqlError>
#include "NetPlc.h"
#include "initdialog.h"
#include "StdAfx.h"

void Onda(){

    double Periodo=10;
    double Volt=12;
    double ton=4;
    double Salita=0.01;
    double t=0;

    for(int i=0;i<30;i++){
        qDebug() << QString::number(t+=0,'f',2) + "u" << ", " << "0";
        qDebug() << QString::number(t+=(Periodo/2-ton-0.01),'f',2) + "u" << ", " << "0";
        qDebug() << QString::number(t+=Salita,'f',2) + "u" << ", " << QString::number(Volt);
        qDebug() << QString::number(t+=(ton-Salita),'f',2) + "u" << ", " << QString::number(Volt);
        qDebug() << QString::number(t+=Salita,'f',2) + "u" << ", " << "0";
        qDebug() << QString::number(t+=(Periodo/2-ton-Salita),'f',2) + "u" << ", " << "0";
        qDebug() << QString::number(t+=Salita,'f',2) + "u" << ", " << "-" + QString::number(Volt);
        qDebug() << QString::number(t+=(ton-Salita),'f',2) + "u" << ", " << "-" + QString::number(Volt);
        qDebug() << QString::number(t+=(Salita),'f',2) + "u" << ", " << "0";
    }
}

#define NUMERO 5

int main(int argc, char *argv[])
{

    //Onda();

    char buffer[NUMERO];
    char *pbuffer=buffer;
    mempcpy(pbuffer,"01234",NUMERO);
    memset(pbuffer,'\0',NUMERO);

    bool IsCompleto=true;
    setIsCompleto(IsCompleto);
    QApplication app(argc, argv);

    InitDialog pInitDialog(0);
    if(pInitDialog.ApriFile()>1){
        pInitDialog.exec();
        if(!pInitDialog.Connetti)
           return 0;
    }


    setFoglio(pInitDialog.Net.wFoglio,pInitDialog.Net.hFoglio);

    if(pInitDialog.Net.DbDatiHost=="localhost"){
        int count=0;
        bool trovato=false;
        for(;;){
            std::string msg="pgrep consoleplc";
            std::vector<std::string> processi=mSystem(msg );
            if(processi.size() !=0){
                trovato=true;
                break;
            }
            if(count++>4)
                break;
            usleep(150000);
        }
        if(!trovato){
            for(;;){
                std::string msg="pgrep stabile";
                std::vector<std::string> processi=mSystem(msg );
                if(processi.size() !=0){
                    trovato=true;
                    break;
                }
                if(count++>4)
                    break;
                usleep(150000);
            }
            if(trovato==false){
                QMessageBox *msgBox  =new QMessageBox;
                msgBox->setWindowTitle("Main");
                msgBox->setText("Impossibile Trovare il Plc locale");
                msgBox->setStandardButtons(QMessageBox::Ok);
                msgBox->exec();
                return 0;
            }

        }
    }

    QPixmap pixmap(":/LOGO-1.png");
    QSplashScreen * splash = new QSplashScreen(pixmap);
    QNetPlc *Processa = new QNetPlc(CTipoApp::Client,pInitDialog.Net.Impianto.toInt(),argc,argv);
    splash->showMessage(QObject::tr("Dr Riccardo Villa"),
                             Qt::AlignBottom | Qt::AlignRight  , Qt::blue);

    if(pInitDialog.Net.DbDatiHost=="labserver"){
        QString path=QDir::homePath() + "/Dropbox/labserver/labserver.net";
        QFile file(path);
        if(file.open(QIODevice::ReadOnly )){
            QTextStream out(&file);
            QString titolo;
            out >> titolo >> pInitDialog.Net.DbDatiHost;
        }else{
            QMessageBox *msgBox  =new QMessageBox;
            msgBox->setWindowTitle("Main");
            msgBox->setText("Impossibile Trovere il file " + path);
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->exec();
            return 0;
        }
        file.close();
        if(pInitDialog.Net.DbProcessoHost=="labserver")
            pInitDialog.Net.DbProcessoHost=pInitDialog.Net.DbDatiHost;
    }

    QLabserverNet *labnet= new QLabserverNet(Processa,pInitDialog.Net.DbDatiHost,pInitDialog.Net.PortaPlc.toInt());


    MainWindow window(Processa,labnet,CTipoApp::Client,&pInitDialog.Net);
    splash->show();

    int count=1;
    int TimeOut=0;
    while(!labnet->IsConnesso | !window.DbIsConnesso){
        count++;
        if(count==7)
            count=1;
        QString str=":/LOGO-" + QString::number(count) + ".png";
        QPixmap pixmap(str);
        splash->setPixmap(pixmap);
        app.processEvents();
        TimeOut++;
        if(TimeOut>150){
            QMessageBox *msgBox  =new QMessageBox;
            msgBox->setWindowTitle("Main");
            msgBox->setText("Impossibile Connettersi al Server");
            msgBox->setStandardButtons(QMessageBox::Ok);
            msgBox->exec();
            return 0;
        }
        app.processEvents();
        usleep(300000);

    }


    if(Processa->Impianto!=pInitDialog.Net.Impianto.toInt()){
        QMessageBox *msgBox  =new QMessageBox;
        msgBox->setWindowTitle("Main");
        msgBox->setText(pInitDialog.Net.Impianto + " Impianto Errato");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
        return 0;
    }

    setTipoApp(CTipoApp::Client);
    window.show();
    splash->finish(&window);
    return app.exec();

}




