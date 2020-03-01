#include "Test.h"
#include "stdio.h"
#include "string.h"


ctest::ctest(CMacchina *newProcessa, QString newTest,QObject *parent) : QThread(parent)
{

    Processa=newProcessa;
    Test=newTest;

}


void ctest::run(){

    if(Test=="power")
       testPower();
}


void ctest::mainComando(BYTE comando){

 /*   char buffer[10];
    char *pBuffer=buffer;
    int len=0;
    memset(pBuffer,'\0',10);
    pBuffer=ByteStr(pBuffer,comando,&len);

    pBuffer=buffer;
    while (Processa->getComando()!=comando) {
      Processa->Macchi(pBuffer);
      msleep(10);
    }*/

}


void ctest::testPower(){

    int index=0;
    Processa->Debug=true;
    mainComando(CTipoComando::StendBy);

    MostraEvento("INIT TEST POWER",Color::FG_DEFAULT);
    msleep(1000);

    //Processa->resetTimeOut();

    while (!Processa->Vuoto[Processa->IdVuoto].Camera.Interlock->Set(true)) {
       msleep(1);
    }

    while (!Processa->Macchina_PowOn(index)) {
       msleep(1);
    }
    if(!Processa->Macchina_PowIsOn(index)){
        MostraEvento("ERRORE POWER ON",Color::FG_DEFAULT);
    }else{
        msleep(200);

        while (!Processa->Bias[Processa->IdCiclo].SetTap(1)) {
           msleep(1);
        }

        msleep(20000);

        while (!Processa->Macchina_PowOff(index)) {
           msleep(1000);
        }
        if(!Processa->Macchina_PowIsOn(index)){
            MostraEvento("ERRORE POWER OFF",Color::FG_DEFAULT);
        }
    }

    while (!Processa->Vuoto[Processa->IdVuoto].Camera.Interlock->Set(false)) {
       msleep(1);
    }

    MostraEvento("END TEST POWER",Color::FG_DEFAULT);
    Processa->Debug=false;

    mainComando(CTipoComando::Reset);


}
