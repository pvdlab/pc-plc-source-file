/********************************************************************************
** Form generated from reading UI file 'qmostraeventi.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QMOSTRAEVENTI_H
#define UI_QMOSTRAEVENTI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QMostraEventi
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ButtonElimina;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *ButtonAnnulla;

    void setupUi(QDialog *QMostraEventi)
    {
        if (QMostraEventi->objectName().isEmpty())
            QMostraEventi->setObjectName(QStringLiteral("QMostraEventi"));
        QMostraEventi->setWindowModality(Qt::WindowModal);
        QMostraEventi->resize(800, 550);
        QMostraEventi->setMinimumSize(QSize(800, 550));
        QMostraEventi->setMaximumSize(QSize(800, 550));
        verticalLayout = new QVBoxLayout(QMostraEventi);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listWidget = new QListWidget(QMostraEventi);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ButtonElimina = new QPushButton(QMostraEventi);
        ButtonElimina->setObjectName(QStringLiteral("ButtonElimina"));

        horizontalLayout_2->addWidget(ButtonElimina);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        pushButton = new QPushButton(QMostraEventi);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        ButtonAnnulla = new QPushButton(QMostraEventi);
        ButtonAnnulla->setObjectName(QStringLiteral("ButtonAnnulla"));

        horizontalLayout_2->addWidget(ButtonAnnulla);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(QMostraEventi);

        QMetaObject::connectSlotsByName(QMostraEventi);
    } // setupUi

    void retranslateUi(QDialog *QMostraEventi)
    {
        QMostraEventi->setWindowTitle(QApplication::translate("QMostraEventi", "Dialog", 0));
        ButtonElimina->setText(QApplication::translate("QMostraEventi", "Elimina", 0));
        pushButton->setText(QApplication::translate("QMostraEventi", "Chiudi", 0));
        ButtonAnnulla->setText(QApplication::translate("QMostraEventi", "Annulla", 0));
    } // retranslateUi

};

namespace Ui {
    class QMostraEventi: public Ui_QMostraEventi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QMOSTRAEVENTI_H
