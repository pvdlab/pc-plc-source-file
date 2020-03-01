#include "qprocessosend.h"

QProcessoSend::QProcessoSend(CProcessa *newProcessa,QObject *parent) :
    QThread(parent) ,Processa(newProcessa)
{
}
