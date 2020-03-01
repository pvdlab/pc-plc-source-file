#include <QtWidgets>
#include "qgraficoriquadro.h"

#define D_TIME 120
#define D_VEDO 900

QGraficoLabel::QGraficoLabel(QWidget *parent) :
    QWidget(parent)
{

    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StaticContents);
    pixmap= new QPixmap(this->width(),this->height());
    pixmap->fill(Qt::lightGray);
    spazio=110;

}

QGraficoLabel::~QGraficoLabel(){


}

void QGraficoLabel::Edit(QSqlRecord record){

    CIOSegnale *pSegnale;

    for(int i=0 ; i<pListaSegnali.count() ; i++){
       pSegnale=pListaSegnali.at(i);
       QString campo=pSegnale->Descrizione();
       pSegnale->DSegnale(record.value(campo).toDouble());
       pListaSegnali.replace(i,pSegnale);
    }
    update();
}


void QGraficoLabel::paintEvent(QPaintEvent *event){

    CIOSegnale *pSegnale;
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawPixmap(dirtyRect, *pixmap, dirtyRect);
    //painter.eraseRect(dirtyRect);

    for(int i=0 ; i<pListaSegnali.count() ; i++){
        pSegnale=pListaSegnali.at(i);
        painter.setPen(QPen(pSegnale->PenColor, pSegnale->PenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        QString str=pSegnale->Descrizione();
        double val;
        if(pSegnale->TipoComp==CTipoComDef::BIAS_V){
            val=getBias_V();
        }else if(pSegnale->TipoComp==CTipoComDef::BIAS_I){
            val=getBias_I();
        }else if(pSegnale->TipoComp==CTipoComDef::PENNING){
            val=getPenning();
        }else
            val=pSegnale->DSegnale();
        if(TipoScala==QTipoScala::Logaritmica)
           str+= " " + QString::number(val,'e',2);
        else
           str+= " " + QString::number(val,'f',1);
        qreal x=i*spazio;
        painter.drawText(QRect(x,0,spazio,this->height()), Qt::AlignLeft | Qt::AlignHCenter, str);
    }

}


void QGraficoLabel::resizeEvent(QResizeEvent *event)
{
    pixmap= new QPixmap(this->width(),this->height());
    pixmap->fill(qRgb(255, 255, 255));
    update();
    QWidget::resizeEvent(event);
}

void QGraficoLabel::resizePixmap(QImage *image, const QSize &newSize)
{
    if (pixmap->size() == newSize)
        return;

    QPixmap newpixmap(newSize);
    newpixmap.fill(qRgb(255, 255, 255));
    QPainter painter(&newpixmap);
    painter.drawPixmap(QPoint(0, 0), *pixmap);
    *pixmap = newpixmap;

}

QGrafico::QGrafico(QWidget *parent) :
    QWidget(parent)
{

    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setAttribute(Qt::WA_StaticContents);
    myPenWidth = 10;
    myPenColor = Qt::blue;
    ymax=1000;
    ydiv=100;
    ydivcount=ymax/ydiv;
    tmax=3600;
    tdiv=D_TIME;
    tdivcount=tmax/tdiv;
    TipoScala=QTipoScala::Lineare;
    image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
    image->fill(Qt::lightGray);
    tzoom=1;
    DisegnaGriglia();

}

QGrafico::~QGrafico(){

    //delete image;
}

void  QGrafico::setSegnali(QList<CIOSegnale*> pnewListaSegnali){

    pListaSegnali=pnewListaSegnali;
    for(int i=0; i< pListaSegnali.count(); i++){
        CIOSegnale *pSegnale=pListaSegnali.at(i);
        //pSegnale->lastPoint.setY(this->height());
        pSegnale->lastPoint.setY(-1);
        pSegnale->lastPoint.setX(0);
    }

}

void QGrafico::setMaxOut(qreal newymaxout,int newtiposcala){

    ymaxout=newymaxout;
    yscala= ymaxout/ymax;
    TipoScala=newtiposcala;
    if(newtiposcala==QTipoScala::Logaritmica)
        ydivcount=ymaxout;

}



void QGrafico::setTempo(int newtempo)
{

    if(newtempo<1800)
        newtempo=1800;
    tmax=newtempo;
    int y=this->height();
    this->resize(tmax,y);

    image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
    image->fill(Qt::lightGray);

    DisegnaGriglia();

}

void QGrafico::setTempo(int newtempo,qreal newtzoom)
{
    tzoom=newtzoom;
    if(newtempo<1800)
       newtempo=1800 ;
    tmax=newtempo;
    //tmax*=tzoom;
    int y=this->height();
    this->resize(tmax,y);

    image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
    image->fill(Qt::lightGray);

    DisegnaGriglia();
}

void QGrafico::addTempo(int newtempo)
{
    tmax+=newtempo;
    int y=this->height();
    this->resize(tmax,y);

    QSize newSize;
    newSize.setHeight(y);
    newSize.setWidth(tmax);

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(Qt::lightGray);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;

    DisegnaGriglia();

}

void QGrafico::DisegnaGriglia(){

    myPenColor=Qt::black;

    //qDebug() << this->width();
    QPainter painter(image);
    tdivcount=this->width()/D_TIME;
    ydiv=this->height()/ydivcount;
    ymax=this->height();
    yscala=ymaxout/ymax;
    painter.setPen(QPen(Qt::black, 1));
    for(int i=0 ; i< tdivcount+1 ;i++){
       qreal tempo=D_TIME*i;
       //tempo*=tzoom;
       painter.drawLine(tempo, 0,tempo,this->height());
    }

    for(int i=0 ; i< ydivcount+1 ;i++){
       painter.drawLine(0,this->height()-ydiv*i,this->width(), this->height()-ydiv*i);
    }


    update();

}



QString QGrafico::getVal(qreal newval)
{
    newval*=yscala;
    return QString::number(newval) ;

}

void QGrafico::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        update();
    }

}



void QGrafico::mouseMoveEvent(QMouseEvent *event)
{

    if ((event->buttons() & Qt::LeftButton) ){
       lastCursore=event->pos();
       drawCursore(event->pos());
    }

}

void QGrafico::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton ) {
        drawCursore(event->pos());
    }
}


void QGrafico::drawCursore(const QPoint &endPoint)
{

    lastPoint = endPoint;
    int tempo=endPoint.x();
    emit gotoTempo(tempo);
    update();


}


void QGrafico::cursoreMoveto(int x){

    lastCursore.setX(x);
    update();
}

void QGrafico::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, *image, dirtyRect);

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::yellow, 2));
    painter.drawLine(lastCursore.rx(),0,lastCursore.rx(),this->height());
    painter.drawText(QRect(lastCursore.rx(),0,60,32), Qt::AlignCenter | Qt::AlignHCenter,
                     QDateTime::fromTime_t(lastCursore.rx()/tzoom).toUTC().toString("hh:mm:ss") );

}

void QGrafico::resizeEvent(QResizeEvent *event)
{

        if(image->height()!=this->height()){
           // qDebug() << this->width() << this->height() ;
            image = new QImage(this->width(),this->height(),QImage::Format_RGB16);
            image->fill(Qt::lightGray);

            DisegnaGriglia();
        }




    update();
    QWidget::resizeEvent(event);

}

void QGrafico::Edit(int newtime, int index,qreal val){

    if(newtime>tmax)
        return;

    CIOSegnale *pSegnale;
    QPainter painter(image);
    pSegnale=pListaSegnali.at(index);
    if(pSegnale->lastPoint.rx()>newtime){
       pSegnale->lastPoint.setX(newtime);
    }

    painter.setPen(QPen(pSegnale->PenColor, pSegnale->PenWidth, Qt::SolidLine, Qt::RoundCap,
                      Qt::RoundJoin));
    if(TipoScala==QTipoScala::Logaritmica){
        if(val!=0 && val < 1){
            val=log10(val);
            if(val<0)
               val*=-1;
            if(ymaxout)
              val/=ymaxout;
            val*=this->height();
            QPointF  endPoint(newtime,val);
            if(val){
              painter.drawLine(pSegnale->lastPoint, endPoint);
              update();
            }
            pSegnale->lastPoint = endPoint;
        }

    }else{

        if(ymaxout)
          val/=ymaxout;
        val*=this->height();
        QPointF  endPoint(newtime,this->height()-val);
        if(val){
          painter.drawLine(pSegnale->lastPoint, endPoint);
          update();
        }
        pSegnale->lastPoint = endPoint;
    }
}


void QGrafico::Edit(int newtime, int index){

    if(newtime>tmax)
        return;

    CIOSegnale *pSegnale;
    pSegnale=pListaSegnali.at(index);
    if(pSegnale->lastPoint.rx()>newtime){
       pSegnale->lastPoint.setX(newtime);
    }
    qreal val;
    if(pSegnale->TipoComp==CTipoComDef::BIAS_V){
        val=getBias_V();
    }else if(pSegnale->TipoComp==CTipoComDef::BIAS_I){
        val=getBias_I();
    }else if(pSegnale->TipoComp==CTipoComDef::PENNING){
        val=getPenning();
    }else
        val=pSegnale->DSegnale();
    if(val<0)
        val=0;

    Edit(newtime, index,val);

}



void QGrafico::Edit(int newtime){

    for(int i=0; i< pListaSegnali.count(); i++){
        Edit(newtime,i);
    }

}

void QGrafico::Load(qreal newtime,QSqlRecord record){

    CIOSegnale *pSegnale;
    QPainter painter(image);
    QString campo;

    for(int i=0; i< pListaSegnali.count(); i++){
        pSegnale=pListaSegnali.at(i);
        campo=pSegnale->Descrizione();
        qreal val=record.value(campo).toDouble() ;
        if((pSegnale->TipoComp==CTipoComDef::PENNING) | (pSegnale->TipoComp==CTipoComDef::BARATRON)){
            val/=ZIP_PRESSIONE;
        }else
            val/=pSegnale->Zip;
        painter.setPen(QPen(pSegnale->PenColor, pSegnale->PenWidth, Qt::SolidLine, Qt::RoundCap,
                          Qt::RoundJoin));

        if(TipoScala==QTipoScala::Logaritmica){
            val=log10(val);
            if(val<0)
               val*=-1;
            if(ymaxout)
              val/=ymaxout;
            val*=this->height();
            QPointF  endPoint(newtime,val);
            if(val){
              if(pSegnale->lastPoint.rx()> endPoint.rx())
                 pSegnale->lastPoint.setX(endPoint.rx());
              if(pSegnale->lastPoint.ry()!=0)
                 painter.drawLine(pSegnale->lastPoint, endPoint);
              update();
            }
            pSegnale->lastPoint = endPoint;
        }else{
            if(ymaxout)
              val/=ymaxout;
            val*=this->height();
            QPointF  endPoint(newtime,this->height()-val);
            if(pSegnale->lastPoint.rx()> endPoint.rx())
               pSegnale->lastPoint.setX(endPoint.rx());
            if(val){
              if(pSegnale->lastPoint.ry()!=0)
                 painter.drawLine(pSegnale->lastPoint, endPoint);
              update();
            }
            pSegnale->lastPoint = endPoint;
        }
    }
}

void QGrafico::drawCompStep(qreal t,int index){

   qreal tempo;
   CIOSegnale *pSegnale;
   QPainter painter(image);
   QPointF  endPoint;

   for(int i=0; i< pListaSegnali.count(); i++){
      pSegnale=pListaSegnali.at(i);
      if(pSegnale==NULL){
          i++;
          pSegnale=pListaSegnali.at(i);
      }
      tempo=t;
      tempo*=tzoom;
      painter.setPen(QPen(pSegnale->PenColor, pSegnale->PenWidth, Qt::SolidLine, Qt::RoundCap,
                          Qt::RoundJoin));
      for(int j=0;j<pSegnale->m_Step[index].StepCount;j++){
          qreal da= pSegnale->m_Step[index].Step[j].Start;
          da*=pSegnale->Fattore();
          if(ymaxout)
            da/=ymaxout;
          da*=this->height();
          endPoint.setX(tempo);
          endPoint.setY(this->height()-da);
          if(j==0){
              if(pSegnale->lastPoint.rx()==endPoint.rx())
                  painter.drawLine(pSegnale->lastPoint, endPoint);
              else{
                  //painter.drawLine(endPoint.rx(),this->height(), endPoint.rx(),this->height()-endPoint.ry());
                  //qDebug() << pSegnale->lastPoint << endPoint;//
              }

          }
          else
            painter.drawLine(pSegnale->lastPoint, endPoint);
          pSegnale->lastPoint = endPoint;
          qreal a= pSegnale->m_Step[index].Step[j].Stop;
          a*=pSegnale->Fattore();
          if(ymaxout)
            a/=ymaxout;
          a*=this->height();
          qreal rampa= pSegnale->m_Step[index].Step[j].Rampa;
          rampa*=tzoom;
          tempo+=rampa/1000;
          endPoint.setX(tempo);
          endPoint.setY(this->height()-a);
          painter.drawLine(pSegnale->lastPoint, endPoint);
          pSegnale->lastPoint = endPoint;

      }

   }
   update();

}




QGraficoRiquadro::QGraficoRiquadro(QWidget *parent) :
    QWidget(parent)
{

    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;

    myPenWidth = 10;
    myPenColor = Qt::blue;


    this->resize(3600,1000);

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(11, 11, 11, 11);
    verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
    LabelY = new QWidget(this);
    LabelY->setObjectName(QStringLiteral("Label"));
    LabelY->setMinimumSize(QSize(60, 0));
    LabelY->setMaximumSize(QSize(60, 16777215));

    horizontalLayout->addWidget(LabelY);

    int h=620 ;//height();
    int hlabel=20;
    MaxTempo=3600;
    OldTempo=0;
    ScrollArea = new QScrollArea(this);
    ScrollArea->resize(D_VEDO-20,h-hlabel);

    Grafico = new QGrafico(ScrollArea);
    Grafico->setBackgroundRole(QPalette::Dark);
    Grafico->resize(MaxTempo, h-hlabel);
    ydivcount=10;
    Grafico->setMaxOut(500,QTipoScala::Lineare);
    ScrollArea->setWidget(Grafico);


    horizontalLayout->addWidget(ScrollArea);
    verticalLayout->addLayout(horizontalLayout);

    LabelX = new QGraficoLabel(this);
    LabelX->setObjectName(QStringLiteral("widget"));
    LabelX->setMinimumSize(QSize(0, hlabel));
    LabelX->setMaximumSize(QSize(16777215, 16777215));
    verticalLayout->addWidget(LabelX);
    TipoScala=QTipoScala::Lineare;

    connect(Grafico, SIGNAL(gotoTempo(int)),
             this, SLOT(goTo(int)));

    connect(ScrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int )),
             this, SLOT(valueChanged(int )));

    ScrollArea->horizontalScrollBar()->setSingleStep(1);

}

void QGraficoRiquadro::valueChanged(int value)
{
    //emit gotoTempo(event->pos().rx());
    emit gotoTempo(value,value+100);
}

void QGraficoRiquadro::goTo(int newtempo) {

    int x = ScrollArea->horizontalScrollBar()->value();
    emit gotoTempo(x,newtempo);

}



void QGraficoRiquadro::scrolltoTempo(int x, int newtempo){

    if(x==0)
        OldTempo=0;
    //if(x!=ScrollArea->horizontalScrollBar()->value())
    ScrollArea->horizontalScrollBar()->setValue( x );
    Grafico->cursoreMoveto(newtempo);

}



void QGraficoRiquadro::scrollTempo(int newtempo){

    if(newtempo>=OldTempo+1){
       int x=newtempo-ScrollArea->width()/2;
       OldTempo=newtempo;
       if(x>=0)
          ScrollArea->horizontalScrollBar()->setValue( x );
    }


}



QGraficoRiquadro::~QGraficoRiquadro(){

    delete Grafico;
    delete LabelX;
    delete LabelY;
    //delete ScrollArea;

}

void QGraficoRiquadro::Clear(){

    delete Grafico;
    delete LabelX;
    delete LabelY;
    //delete ScrollArea;

}

void QGraficoRiquadro::resizeEvent(QResizeEvent *event)
{

    //Grafico->resize(MaxTempo, height()-20-LabelX->height());
    Grafico->resize(Grafico->gettmax(), height()-20-LabelX->height());
    update();
    QWidget::resizeEvent(event);
}



void QGraficoRiquadro::paintEvent(QPaintEvent */* event */){

    QPainter painter(this);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    ydiv=Grafico->getydiv();
    ymax=Grafico->getymax();
    ydivcount=Grafico->getydivcount();
    qreal fonth = painter.fontInfo().pointSizeF();
    for(int i=0 ; i< ydivcount ;i++){
     QString str;
     if(TipoScala==QTipoScala::Logaritmica){
         double val=1;
         if(pow(10,ydivcount-i))
            val/=pow(10,ydivcount-i);
         str =QString::number(val,'e',2);
         painter.drawText(QRectF(0,ymax-ydiv*i-fonth,LabelY->width(),fonth+4), Qt::AlignRight | Qt::AlignHCenter, str);
     }
     else{
       str =Grafico->getVal(i*ydiv);
       painter.drawText(QRectF(0,ymax-ydiv*i-fonth,LabelY->width(),fonth+4), Qt::AlignRight | Qt::AlignHCenter, str);
     }
   }


}

void QGraficoRiquadro::setSegnali(QList<CIOSegnale *> pnewListaSegnali){

    Grafico->setSegnali(pnewListaSegnali);
    LabelX->setSegnali(pnewListaSegnali);
    update();

}


void   QGraficoRiquadro::append(CIOSegnale* pSegnale){

    Grafico->append(pSegnale);
    LabelX->append(pSegnale);
    update();

}






