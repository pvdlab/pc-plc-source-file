#include "qanalogico.h"
#include "qcostanti.h"
#include <QtWidgets>
#include <QHBoxLayout>



QMLabel::QMLabel(QWidget *parent) :
    QWidget(parent){

  //setGeometry(QRect(0, 0, W_DIG, H_DIG));
  setMinimumSize(QSize(W_DIG, H_DIG));
  setMaximumSize(QSize(W_DIG, H_DIG));
  setBackgroundRole(QPalette::Midlight);
  setAutoFillBackground(true);
  QFont newFont = font();
  newFont.setPointSize(H_FONT_DIG);
  setFont(newFont);
  Descrizione="Label" ;

}

void QMLabel::setDescrizione( QString newdescrizione){

  Descrizione=newdescrizione;
  update();

}

void QMLabel::paintEvent(QPaintEvent * /* event */)
{

    QColor color;
    QPainter painter(this);
    color.setHsv(FONT_COLORE);
    painter.fillRect(QRect(0,0,W_DIG,H_DIG),QColor(SFONDO_TITOLO));
    painter.drawText(QRect(0,0,W_DIG,H_DIG), Qt::AlignCenter | Qt::AlignHCenter, Descrizione);


}


QAnalogico::QAnalogico(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;

    QFont newFont = font();
    newFont.setPointSize(H_FONT_DIG);
    setFont(newFont);
    setObjectName(QStringLiteral("widdigitale"));
    setGeometry(QRect(20, 190, W_DIG, H_DIG));
    setMaximumSize(QSize(W_DIG, H_DIG));
    setBaseSize(QSize(W_DIG, H_DIG));
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    EditBox = new QDoubleSpinBox(this);
    EditBox->setObjectName(QStringLiteral("EditBox"));
    horizontalLayout->addWidget(EditBox);
    EditBox->setMaximum(10000);
    EditBox->hide();

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);
    Segnale=0;
    isManuale=false;

    connect(EditBox, SIGNAL(editingFinished()),
            this, SLOT(editingFinished()));

    pSegnale=NULL;

}


QAnalogico::~QAnalogico()
{
    delete EditBox;
}

void QAnalogico::editingFinished(){

    if(isManuale){
        if(pSegnale!=NULL){
            double val=EditBox->value();
            double valtosend=val;
            if(pSegnale->Fattore())
              valtosend/=pSegnale->Fattore();
            int newval=valtosend;
            emit CambiaSegnale(pSegnale->Slot(),pSegnale->Point(),newval);
            //val=Segnale;
            //val*=pSegnale->Fattore();
            //EditBox->setValue(val);
        }

    }
}

void QAnalogico::setManuale(bool newManuale){

    isManuale=newManuale;
    if(isManuale){
        EditBox->show();
        EditBox->setValue(DSegnale);
    }
    else
        EditBox->hide();

}

void QAnalogico::setAnalogico(QString newdescrizione, QString newmisura, CIOSegnale *newpSegnale){

    Descrizione=newdescrizione;
    Misura= newmisura;
    if(newpSegnale)
       pSegnale=newpSegnale;
    update();
}

void QAnalogico::setAnalogico(QString newdescrizione, QString newmisura,int newslot,int newpoint,  CMacchina *Processa){

    Descrizione=newdescrizione;
    Misura= newmisura;
    pSegnale= new CIOSegnale;
    pSegnale =Processa->Segnale(Descrizione.toUtf8().data(),newslot,newpoint,0,0,0);
    update();
}

void QAnalogico::EditSegnale(){

    if(pSegnale==NULL)
        return;

    if(Segnale!=pSegnale->Segnale()){
        Segnale=pSegnale->Segnale();
        if(Segnale<0)
           Segnale=0;
        DSegnale=Segnale;
        pSegnale->DSegnale(Segnale);
        update();
    }
}

void QAnalogico::Edit(int newSegnale){

    if(!this)
        return;

    Segnale=newSegnale;
    pSegnale->DSegnale(0);
    update();
}

void QAnalogico::Edit(){
    
    if(!this)
        return;

    if(!pSegnale)
        return;

    if(Segnale!=pSegnale->Segnale()){
        Segnale=pSegnale->Segnale();
        if(Segnale<0)
           Segnale=0;
        DSegnale=Segnale;
        DSegnale*=pSegnale->Fattore();
        pSegnale->DSegnale(DSegnale);
        update();
    }
}

void QAnalogico::EditSecondo(){

    if(pSegnale==NULL)
        return;

    if(Segnale!=pSegnale->Segnale()){
        Segnale=pSegnale->Segnale();
        if(Segnale<0)
           Segnale=0;
        DSegnale=Segnale;
        DSegnale*=pSegnale->Fattore();
        DSegnale*=pSegnale->SecondoFattore();
        pSegnale->DSegnale(DSegnale);
        update();
    }
}

void QAnalogico::paintEvent(QPaintEvent* event){

    QColor color;


    QPainter painter(this);

    color.setHsv(FONT_COLORE);
    painter.drawText(QRect(0,0,W_DIG/3,H_DIG), Qt::AlignLeft  | Qt::AlignHCenter, Descrizione);


    color.setHsv(FONT_COLORE);
    painter.fillRect(QRect(W_DIG/3,0,W_DIG/3+H_DIG,H_DIG),QColor(255,255,255));
    painter.drawText(QRect(W_DIG/3,0,W_DIG/3+H_DIG,H_DIG), Qt::AlignCenter | Qt::AlignHCenter, QString::number(DSegnale,'f',2));

    color.setHsv(FONT_COLORE);
    painter.drawText(QRect(2*W_DIG/3+H_DIG,0,W_DIG/3-H_DIG,H_DIG), Qt::AlignRight  | Qt::AlignHCenter, Misura);

    EditBox->resize(W_DIG/3+20,H_DIG);
    EditBox->move(W_DIG/3,0);
}

QDigitale::QDigitale(QWidget *parent) ://QString newdescrizione, int newpoint, int newslot
    QWidget(parent)
{

    isEdit=false;
    isEnabled=false;

    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;

    QFont newFont = font();
    newFont.setPointSize(H_FONT_DIG);
    setFont(newFont);
    setObjectName(QStringLiteral("widdigitale"));
    setGeometry(QRect(20, 190, W_DIG, H_DIG));
    setMaximumSize(QSize(W_DIG, H_DIG));
    setBaseSize(QSize(W_DIG, H_DIG));
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    led = new QLed(this);
    led->setObjectName(QStringLiteral("widled"));
    led->setMinimumSize(QSize(H_DIG, H_DIG));
    led->setMaximumSize(QSize(H_DIG, H_DIG));
    led->setDiameter(3);
    horizontalLayout->addWidget(led);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem( horizontalSpacer);

    connect(led, SIGNAL(mousePress(bool)),
            this, SLOT(mousePress(bool)));
    isManuale=false;
    pSegnale=NULL;

}


QDigitale::~QDigitale()
{
}

void QDigitale::setDigitale(QString newdescrizione,CIOSegnale *newpSegnale){

     Descrizione=newdescrizione;
     pSegnale= newpSegnale;
     update();

}

void QDigitale::setDigitale(QString newdescrizione, int newslot,int newpoint, int newfalsetrue, CMacchina *Processa){

    Descrizione=newdescrizione;
    pSegnale= new CIOSegnale; //QString Descrizione,int Slot,int Point,WORD FondoScala
    qDebug() << "Descrizione" << Descrizione;
    pSegnale =Processa->Segnale(Descrizione.toUtf8().data(),newslot,newpoint,newfalsetrue);
    //pSegnale->operator ()(newslot,newpoint,Descrizione,0,0,0 ,plc);
    update();
}

void QDigitale::Edit(){

    if(!this)
        return;

    //if(Segnale!=pSegnale->Segnale()){
       Segnale=(bool)pSegnale->Segnale();
       led->setSegnale(Segnale);
    //}

}

void QDigitale::Edit(bool newSegnale){

    if(!this)
        return;
    Segnale=(bool)newSegnale;
    led->setSegnale(Segnale);

}


void QDigitale::mousePress(bool newsegnale){

    if(isManuale){
        if(pSegnale!=NULL)
          emit CambiaSegnale(pSegnale->Slot(),pSegnale->Point(),newsegnale);
    }

}

void QDigitale::paintEvent(QPaintEvent * /* event */)
{
    QColor color;
    QFontMetrics metrics(font());
    int x = H_DIG+5;
    int y = (height() + metrics.ascent() - metrics.descent()) / 2;


    QPainter painter(this);

    color.setHsv(FONT_COLORE);
    painter.setPen(color);
    painter.drawText(x, y ,Descrizione );


}

QSensore::QSensore(QWidget *parent) :
    QWidget(parent)
{

    setMinimumSize(QSize(W_DIG, 2*H_DIG));
    setMaximumSize(QSize(W_DIG, 2*H_DIG));
    setBackgroundRole(QPalette::Midlight);
    setAutoFillBackground(true);
    QFont newFont = font();
    newFont.setPointSize(H_FONT_DIG);
    setFont(newFont);

}

void QSensore::setAnalogico(QString newdescrizione, QString newmisura, CIOSegnale *newpSegnale){

    Descrizione=newdescrizione;
    Misura= newmisura;
    pSegnale=newpSegnale;
    update();
}

void QSensore::Edit(){

    if(Segnale!=pSegnale->Segnale()){
        Segnale=pSegnale->Segnale();
        if(pSegnale->TipoComp==CTipoComDef::PENNING){
            double volt=pSegnale->Segnale();
            if(pSegnale->FondoScala())
              volt/=pSegnale->FondoScala();
            volt*=10;
            double exp=pSegnale->Fattore();
            exp*=volt;
            exp-=pSegnale->SecondoFattore();
            DSegnale=pow(10,exp);
            pSegnale->DSegnale(DSegnale);
        }
        else if(pSegnale->TipoComp==CTipoComDef::PIRANI){
            DSegnale=Segnale;
            DSegnale*=pSegnale->Fattore();
            DSegnale*=pSegnale->SecondoFattore();
            pSegnale->DSegnale(DSegnale);
        }else if(pSegnale->TipoComp==CTipoComDef::BARATRON){
            DSegnale=Segnale;
            DSegnale*=pSegnale->Fattore();
            pSegnale->DSegnale(DSegnale);
        }
        update();
    }

}

void QSensore::paintEvent(QPaintEvent * /* event */)
{
    QColor color;
    QPainter painter(this);

    color.setHsv(TITOLO);
    painter.fillRect(QRect(0,0,W_DIG,H_DIG),QColor(SFONDO_TITOLO));
    painter.drawText(QRect(0,0,W_DIG,H_DIG), Qt::AlignCenter | Qt::AlignHCenter, Descrizione);

    color.setHsv(FONT_COLORE);
    painter.fillRect(QRect(0,H_DIG,W_DIG,H_DIG),QColor(255,255,255));
    painter.drawText(QRect(0,H_DIG,W_DIG,H_DIG), Qt::AlignCenter | Qt::AlignHCenter, QString::number(DSegnale,'e',2));

}
