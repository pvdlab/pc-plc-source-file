/********************************************************************************
** Form generated from reading UI file 'qmonitorizza.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMONITORIZZA_H
#define UI_QMONITORIZZA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMonitorizza
{
public:

    void setupUi(QWidget *QMonitorizza)
    {
        if (QMonitorizza->objectName().isEmpty())
            QMonitorizza->setObjectName(QStringLiteral("QMonitorizza"));
        QMonitorizza->resize(579, 470);

        retranslateUi(QMonitorizza);

        QMetaObject::connectSlotsByName(QMonitorizza);
    } // setupUi

    void retranslateUi(QWidget *QMonitorizza)
    {
        QMonitorizza->setWindowTitle(QApplication::translate("QMonitorizza", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class QMonitorizza: public Ui_QMonitorizza {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMONITORIZZA_H
