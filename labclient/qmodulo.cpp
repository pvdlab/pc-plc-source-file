#include "qmodulo.h"
//#include "ui_qmodulo.h"
#include "qcostanti.h"


QSlot::QSlot(QWidget *parent) :
    QWidget(parent)
{
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("QSlot"));
    //this->resize(W_DEF, 500);

    Layout = new QVBoxLayout(this);
    Layout->setSpacing(0);
    Layout->setObjectName(QStringLiteral("Layout"));
    Layout->setContentsMargins(0, 0, 0, 0);
    Layout->setObjectName(QStringLiteral("Layout"));
    LastModulo=NULL;
    slot=-1;

}

QSlot::~QSlot(){

    Analogico.clear();
    Digitale.clear();

}

void QSlot::Edit(){

    if(IsDigitale){
       for(int i=0; i< Digitale.count();i++){
          QDigitale *pdig=Digitale.at(i);
         /* if(slot==2 && i==0){
             qDebug() << slot << i;
             pdig->Edit();
          }*/

          pdig->Edit();
       }
    }else{
        for(int i=0; i< Analogico.count();i++){
           QAnalogico *pdig=Analogico.at(i);
           pdig->EditSegnale();
        }
    }
}

void QSlot::setManuale(bool newManuale){

    //if(TipoSlot==CTipoSlot::InDigitale || TipoSlot==CTipoSlot::InAnalogico )
    //    return;

    if(IsDigitale){
       for(int i=0; i< Digitale.count();i++){
          QDigitale *pdig=Digitale.at(i);
          if(pdig->isOutPut)
             pdig->setManuale(newManuale);
       }
    }else{
        for(int i=0; i< Analogico.count();i++){
           QAnalogico *pdig=Analogico.at(i);
           if(pdig->isOutPut)
             pdig->setManuale(newManuale);
        }
    }
}

void QSlot::setSlot(int newslot,QList <CModulo *> lmodulo,QNetPlc *Processa){

    slot=newslot;

    int moduloCount=0;
    for(int j=0;j<lmodulo.count();j++){

        int h=0;
        CModulo *modulo=lmodulo[j];
        if(modulo->Index==12)
            modulo->Index=12;
        moduloCount+=modulo->Count;
        IsDigitale=modulo->IsDigitale;
        TipoSlot=modulo->TipoSlot;
        QMLabel *pdig= new QMLabel(this);
        switch (TipoSlot) {
        case CTipoSlot::InDigitale:
            pdig->setDescrizione(QString::number(modulo->Index) + " InD " + QString::number(modulo->Versione));
            break;
        case CTipoSlot::OutDigitale:
            pdig->setDescrizione(QString::number(modulo->Index) + " OutD " + QString::number(modulo->Versione));
            break;
        case CTipoSlot::InAnalogico:
            pdig->setDescrizione(QString::number(modulo->Index) + " InA " + QString::number(modulo->Versione));
            break;
        case CTipoSlot::OutAnalogico:
            pdig->setDescrizione(QString::number(modulo->Index) + " OutA " + QString::number(modulo->Versione));
            break;
        default:
            break;
        }

        Layout->addWidget(pdig);

        for(int i=0; i<modulo->Count;i++){
            if(modulo->IsDigitale){
                QDigitale *pdig= new QDigitale(this);
                if(modulo->TipoSlot==CTipoSlot::OutDigitale)
                    pdig->isOutPut=true;
                else
                    pdig->isOutPut=false;
                Digitale.append(pdig);
                QString str;
                if(modulo->pPoint[i].Descrizione!=NULL){
                    if(newslot==2)
                        qDebug() << "Descrizione!=NULL";
                    str="[" + QString::number(modulo->Slot) + "," +
                                 QString::number(i) + "] " +
                                 QString::fromUtf8(modulo->pPoint[i].Descrizione);

                }else{
                    if(newslot==2)
                        qDebug() << "Descrizione==NULL";
                    str="[" + QString::number(modulo->Slot) + "," +
                                 QString::number(i) + "] " ;
                }

                pdig->setDigitale(str,modulo->Slot,i,0,Processa);
                Layout->addWidget(pdig);
                h+=pdig->height()+1;
                connect(pdig, SIGNAL( CambiaSegnale(int,int,int)),
                         this, SLOT(CambiaSegnale(int,int,int)));
            }else{
                QAnalogico *pdig= new QAnalogico(this);
                if(modulo->TipoSlot==CTipoSlot::OutAnalogico)
                    pdig->isOutPut=true;
                else
                    pdig->isOutPut=false;
                Analogico.append(pdig);
                QString str="[" + QString::number(modulo->Slot) + "," +
                             QString::number(i) + "] " ;
                pdig->setAnalogico(QString::fromUtf8(modulo->pPoint[i].Descrizione),str,modulo->Slot,i,Processa);
                Layout->addWidget(pdig);
                h+=pdig->height()+1;
                connect(pdig, SIGNAL( CambiaSegnale(int,int,int)),
                         this, SLOT(CambiaSegnale(int,int,int)));
            }


        }
    }

    this->resize(W_DIG+0, moduloCount*(H_DIG+0));
}

void QSlot::CambiaSegnale(int newslot, int newpoint, int newsegnale){

    emit sendCambiaSegnale(newslot,newpoint,newsegnale);
}

void QSlot::paintEvent(QPaintEvent *event){

    Q_UNUSED(event)

}

QModulo::QModulo(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *VericalLayout;

    if (this->objectName().isEmpty())
         this->setObjectName(QStringLiteral("QVuoto"));
    this->resize(924, 558);
    VericalLayout = new QVBoxLayout(this);
    VericalLayout->setSpacing(0);
    VericalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    VericalLayout->setContentsMargins(0, 0, 0, 0);

    widUp = new QWidget(this);
    VericalLayout->addWidget(widUp);
    widDown = new QWidget(this);
    VericalLayout->addWidget(widDown);
    this->setWindowTitle(QApplication::translate("QVuoto", "Form", 0));

}

QModulo::~QModulo()
{
    Slot.clear();
}

void QModulo::Edit(){

    for(int i=0; i< Slot.count();i++){
        QSlot *pSlot=Slot.at(i);
        pSlot->Edit();
    }

}

void QModulo::setManuale(bool newManuale){

    for(int i=0; i< Slot.count();i++){
        QSlot *pSlot=Slot.at(i);
        pSlot->setManuale(newManuale);
    }

}



void QModulo::Init(QNetPlc *newProcessa){

    Processa=newProcessa;


    for(int i=0; i< Processa->ModuloCount;i++){
        CModulo *modulo=&Processa->pModulo[i];
        QList <CModulo *> lmodulo;

        if(!modulo->Posizionato){
            lmodulo.append(modulo);
            modulo->Posizionato=true;
            ModuloCount++;
            for(int j=0; j< Processa->ModuloCount;j++){
                CModulo *next=&Processa->pModulo[j];
                if(!next->Posizionato & (modulo->Index==next->Index)){
                    next->Posizionato=true;
                    lmodulo.append(next);
                }
            }

        }
        if(lmodulo.count()){
            if(ModuloCount<=MODULO_MAX){
                QSlot *pSlot = new QSlot(widUp);
                pSlot->setSlot(i,lmodulo,Processa);
                Slot.append(pSlot);
                connect(pSlot, SIGNAL( sendCambiaSegnale(int,int,int)),
                         this, SLOT(CambiaSegnale(int,int,int)));
            }else{
                QSlot *pSlot = new QSlot(widDown);
                pSlot->setSlot(i,lmodulo,Processa);
                Slot.append(pSlot);
                connect(pSlot, SIGNAL( sendCambiaSegnale(int,int,int)),
                         this, SLOT(CambiaSegnale(int,int,int)));
            }
        }


    }


}

void QModulo::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);
    QSlot *pSlot=Slot.at(0);
    int spazio=this->width()-pSlot->width()*MODULO_MAX;
    spazio/=MODULO_MAX+1;
    int left=spazio;

    for(int i=0; i< MODULO_MAX;i++){
       QSlot *pSlot=Slot.at(i);
       pSlot->move(left,4);
       left+=spazio+pSlot->width();
    }

    left=spazio;
    for(int i=MODULO_MAX; i< ModuloCount;i++){
       QSlot *pSlot=Slot.at(i);
       pSlot->move(left,4);
       left+=spazio+pSlot->width();
    }
}


void QModulo::CambiaSegnale( int newslot,int newpoint, int newsegnale){

    emit sendCambiaSegnale(newslot,newpoint, newsegnale);
}
