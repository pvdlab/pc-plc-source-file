/********************************************************************************
** Form generated from reading UI file 'qmstart.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMSTART_H
#define UI_QMSTART_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QMStart
{
public:
    QVBoxLayout *verticalLayout;
    QProgressBar *progressBar;
    QListWidget *listWidget;

    void setupUi(QWidget *QMStart)
    {
        if (QMStart->objectName().isEmpty())
            QMStart->setObjectName(QStringLiteral("QMStart"));
        QMStart->resize(710, 418);
        verticalLayout = new QVBoxLayout(QMStart);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        progressBar = new QProgressBar(QMStart);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(24);

        verticalLayout->addWidget(progressBar);

        listWidget = new QListWidget(QMStart);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);


        retranslateUi(QMStart);

        QMetaObject::connectSlotsByName(QMStart);
    } // setupUi

    void retranslateUi(QWidget *QMStart)
    {
        QMStart->setWindowTitle(QApplication::translate("QMStart", "Form", 0));
        progressBar->setFormat(QString());
    } // retranslateUi

};

namespace Ui {
    class QMStart: public Ui_QMStart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMSTART_H
