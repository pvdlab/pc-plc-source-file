#include "qtest.h"
#include "ui_qtest.h"

QTest::QTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QTest)
{
    ui->setupUi(this);
}

QTest::~QTest()
{
    delete ui;
}

void QTest::on_ButtonChiudi_clicked()
{
    this->close();
}
