#ifndef QQLed_H
#define QQLed_H

#include <QtDesigner/QtDesigner>
#include <QWidget>

class QTimer;

class QLed : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(double diameter READ diameter WRITE setDiameter) // mm
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
    Q_PROPERTY(bool state READ state WRITE setState)
    Q_PROPERTY(bool flashing READ isFlashing WRITE setFlashing)
    Q_PROPERTY(int flashRate READ flashRate WRITE setFlashRate)

public:
    explicit QLed(QWidget* parent=0);
    ~QLed();

    double diameter() const;
    void setDiameter(double diameter);

    QColor color() const;
    void setColor(const QColor& color);
    void setSegnale(bool );

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);
    bool state() const;
    bool isFlashing() const;
    int flashRate() const;
    void  setManuale(bool newManuale){isManuale=newManuale;}

signals:
    void mousePress(bool);

public slots:
    void setState(bool state);
    void toggleState();
    void setFlashing(bool flashing);
    void setFlashRate(int rate);
    void startFlashing();
    void stopFlashing();

public:
    int heightForWidth(int width) const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent * event);

private:
    double diameter_;
    QColor color_;
    Qt::Alignment alignment_;
    bool initialState_;
    bool state_;
    int flashRate_;
    bool flashing_;
    int pixX_, pixY_;
    int diamX_, diamY_;
    QRadialGradient gradient_;
    QTimer* timer_;
    QString Descrizione_;
    bool       isManuale;
};

#endif // QQLed_H
