#include "Initfile.h"
#include "StdAfx.h"
#include <QFile>
#include <QDataStream>

void mostraSubInit(QString primo,QString secondo){
    std::string str;
    str.append(primo.toUtf8());
    str.append(secondo.toUtf8());
    MostraEvento(str,Color::FG_GREEN);
}

CCInitFile::CCInitFile()
{

  Titolo="";
  Titolo+="";
  NomeFile="";

}

CCInitFile::CCInitFile(QString newtitolo, QString newnomeFile)
{

  Titolo=newtitolo;
  Titolo+="> ";
  NomeFile=newnomeFile;

}

bool CCInitFile::isSudo(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SUDO){
             QString buffer=argv[i+1];
             buffer=buffer.trimmed();
             if(buffer==PASSWORD)
                 return true;
           }

    }
    return false;
}

int CCInitFile::isMacchina(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_IMPIANTO){
             QString buffer=argv[i+1];
             buffer=buffer.trimmed();
             return buffer.toInt();
           }

    }
    return 0;
}


bool CCInitFile::isSimula(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_SIMULA){
              return true;
         }

    }
    return false;
}

bool CCInitFile::isDog(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_DOG){
              return true;
         }

    }
    return false;
}

bool CCInitFile::isSetSegnale(int argc, char *argv[],int *slot, int *point, int *valore){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_SEGNALE){
             buffer=argv[i+1];
             *slot =buffer.toInt();
             buffer=argv[i+2];
             *point =buffer.toInt();
             buffer=argv[i+3];
             *valore =buffer.toInt();
             return true;
         }

    }
    return false;
}

bool CCInitFile::isDebug(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_DEBUG){
              return true;
         }

    }
    return false;
}

bool CCInitFile::isManuale(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_MANUALE){
              return true;
         }

    }
    return false;
}

bool CCInitFile::isPorta(int argc, char *argv[]){

    for (int i = 0; i < argc; ++i) {
         QString buffer=argv[i];
         buffer=buffer.trimmed();
         if(buffer==SET_PORTA_APERTA){
              return true;
         }

    }
    return false;
}




void CCInitFile::appaned(QString newComando, QString newDescrizione){

    for(int i=0; i< Arg.count();i++){
        QString Comando=Arg[i]->Comando;
        if(Comando==newComando)
            return;
    }
    CArg *pArg= new CArg();
    pArg->Comando=newComando;
    pArg->Descrizione=newDescrizione;
    pArg->daSalvare="null";
    Arg.append(pArg);
    mostraSubInit("[" + pArg->Comando + "]\t"+ pArg->Descrizione, pArg->daSalvare);

}

void CCInitFile::mostra(){

    for(int i=0; i< Arg.count();i++)
        mostraSubInit("[" + Arg.at(i)->Comando + "]\t"+ Arg.at(i)->Descrizione, Arg.at(i)->daSalvare);
}

QString CCInitFile::at(QString comando){

    for(int i=0; i< Arg.count();i++){
        if(comando==Arg.at(i)->Comando){
            return Arg.at(i)->daSalvare;
        }
    }
    return "";

}


QString  CCInitFile::leggiInput(){

    char  buf[1024];

    bool trovato=false;

    printf("%s",Titolo.toUtf8().data());
    if (fgets(buf, sizeof(buf), stdin))
    {
        QString prima;
        QString str=QString::fromLocal8Bit(buf);
        str=str.left(str.length()-1);
        int pos;
        str=buf;
        do{
            QString start=str.left(str.length()-1).trimmed();
            str=str.trimmed();
            pos=str.indexOf(' ');
            prima=str.left(pos).trimmed();
            str=str.mid(pos,str.length()-pos).trimmed();
            pos=str.indexOf(' ');
            QString seconda=str.left(pos).trimmed();
            str=str.mid(pos,str.length()-pos).trimmed();
            for(int i=0; i< Arg.count();i++){
                if(Arg.at(i)->Comando==prima){
                   CArg *pArg= Arg.at(i);
                   pArg->daSalvare=seconda;
                   Arg.replace(i,pArg);
                   trovato=true;
                }
            }
            if(!trovato){
               return start;
            }

        }while (pos>0);

    }
    mostra();
    setFile();

    return "";

}

void CCInitFile::setFile(){

    QFile file(NomeFile);
    file.open(QIODevice::WriteOnly);
    QDataStream Out(&file);
    Out << Arg.count();
    for(int i=0; i< Arg.count();i++){
       Out <<  Arg.at(i)->Comando << Arg.at(i)->Descrizione << Arg.at(i)->daSalvare;
    }
    file.close();
}

bool CCInitFile::getFile(){

    Arg.clear();
    QFile file(NomeFile);
    if(file.open(QIODevice::ReadOnly)){
        QDataStream in(&file);
        int count;
        in >> count;
        for(int i=0; i< count;i++){
           QString  Comando,Descrizione, daSalvare;
           in >> Comando >> Descrizione >> daSalvare;
           CArg *pArg= new CArg();
           pArg->Comando=Comando;
           pArg->Descrizione=Descrizione;
           pArg->daSalvare=daSalvare;
           Arg.append(pArg);
        }
        file.close();
        return true;
    };
    return false;


}
