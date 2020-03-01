#include <QtWidgets>
#include "qled.h"


QLed::QLed(QWidget* parent) :
    QWidget(parent),
    diameter_(0),
    color_(QColor("red")),
    alignment_(Qt::AlignVCenter),
    initialState_(true),
    state_(true),
    flashRate_(200),
    flashing_(false)
{
    isManuale=false;
}

QLed::~QLed()
{
}


void QLed::mousePressEvent(QMouseEvent * event){

    Q_UNUSED(event);
    if(!isManuale)
        return;


    if(color_.name()=="#ff0000"){
      //setColor(QColor("green"));
      emit mousePress(true);
    }
    else{
      //setColor(QColor("red"));
      emit mousePress(false);
    }
    setColor(QColor("yellow"));
    update();


}

double QLed::diameter() const
{
    return diameter_;
}

void QLed::setDiameter(double diameter)
{
    diameter_ = diameter;

    pixX_ = round(double(height())/heightMM());
    pixY_ = round(double(width())/widthMM());

    diamX_ = diameter_*pixX_;
    diamY_ = diameter_*pixY_;

    update();
}


QColor QLed::color() const
{
    return color_;
}

void QLed::setColor(const QColor& color)
{
    color_ = color;
    update();
}

void QLed::setSegnale(bool segnale){

    if(segnale==true)
       setColor(QColor("green"));
    else
       setColor(QColor("red"));
    update();

}

Qt::Alignment QLed::alignment() const
{
    return alignment_;
}

void QLed::setAlignment(Qt::Alignment alignment)
{
    alignment_ = alignment;

    update();
}

void QLed::setFlashRate(int rate)
{
    flashRate_ = rate;

    update();
}

void QLed::setFlashing(bool flashing)
{
    flashing_ = flashing;

    update();
}

void QLed::startFlashing()
{
    setFlashing(true);
}

void QLed::stopFlashing()
{
    setFlashing(false);
}


void QLed::setState(bool state)
{
    state_ = state;
    update();
}

void QLed::toggleState()
{
    state_ = !state_;
    update();
}

int QLed::heightForWidth(int width) const
{
    return width;
}

QSize QLed::sizeHint() const
{
    return QSize(diamX_, diamY_);
}

QSize QLed::minimumSizeHint() const
{
    return QSize(diamX_, diamY_);
}

void QLed::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);

    QRect geo = geometry();
    int width = geo.width();
    int height = geo.height();

    int x=0, y=0;

    x = (width-diamX_)/2;
    y = (height-diamY_)/2;
    QRadialGradient g(x+diamX_/2, y+diamY_/2, diamX_*0.4,
        diamX_*0.4, diamY_*0.4);

    g.setColorAt(0, Qt::white);
    if ( state_ )
        g.setColorAt(1, color_);
    else
        g.setColorAt(1, Qt::black);
    QBrush brush(g);

    p.setPen(color_);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setBrush(brush);
    p.drawEllipse(x, y, diamX_-1, diamY_-1);


   /* if ( flashRate_ > 0 && flashing_ )
        timer_->start(flashRate_);
    else
        timer_->stop();*/
}

bool QLed::state() const
{
    return state_;
}

bool QLed::isFlashing() const
{
    return flashing_;
}

int QLed::flashRate() const
{
    return flashRate_;
}
