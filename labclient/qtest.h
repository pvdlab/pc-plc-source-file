#ifndef QTEST_H
#define QTEST_H

#include <QDialog>

namespace Ui {
class QTest;
}

class QTest : public QDialog
{
    Q_OBJECT

public:
    explicit QTest(QWidget *parent = 0);
    ~QTest();

private slots:
    void on_ButtonChiudi_clicked();

private:
    Ui::QTest *ui;
};

#endif // QTEST_H
