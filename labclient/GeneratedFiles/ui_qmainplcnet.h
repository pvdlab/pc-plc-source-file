/********************************************************************************
** Form generated from reading UI file 'qmainplcnet.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMAINPLCNET_H
#define UI_QMAINPLCNET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qimpianto.h"

QT_BEGIN_NAMESPACE

class Ui_QMainplcnet
{
public:
    QVBoxLayout *verticalLayout;
    QImpianto *Impianto;

    void setupUi(QWidget *QMainplcnet)
    {
        if (QMainplcnet->objectName().isEmpty())
            QMainplcnet->setObjectName(QStringLiteral("QMainplcnet"));
        QMainplcnet->resize(904, 562);
        verticalLayout = new QVBoxLayout(QMainplcnet);
        verticalLayout->setSpacing(1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(1, 1, 1, 1);
        Impianto = new QImpianto(QMainplcnet);
        Impianto->setObjectName(QStringLiteral("Impianto"));

        verticalLayout->addWidget(Impianto);


        retranslateUi(QMainplcnet);

        QMetaObject::connectSlotsByName(QMainplcnet);
    } // setupUi

    void retranslateUi(QWidget *QMainplcnet)
    {
        QMainplcnet->setWindowTitle(QApplication::translate("QMainplcnet", "QMainplcnet", 0));
    } // retranslateUi

};

namespace Ui {
    class QMainplcnet: public Ui_QMainplcnet {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMAINPLCNET_H
