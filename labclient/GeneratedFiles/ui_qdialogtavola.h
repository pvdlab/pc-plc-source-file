/********************************************************************************
** Form generated from reading UI file 'qdialogtavola.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDIALOGTAVOLA_H
#define UI_QDIALOGTAVOLA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QDialogTavola
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *TavolaLabel;
    QTableView *Tavola;
    QHBoxLayout *horizontalLayout;
    QPushButton *OkButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *ButtonChiudi;

    void setupUi(QDialog *QDialogTavola)
    {
        if (QDialogTavola->objectName().isEmpty())
            QDialogTavola->setObjectName(QStringLiteral("QDialogTavola"));
        QDialogTavola->resize(576, 326);
        verticalLayout = new QVBoxLayout(QDialogTavola);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        TavolaLabel = new QLabel(QDialogTavola);
        TavolaLabel->setObjectName(QStringLiteral("TavolaLabel"));

        verticalLayout->addWidget(TavolaLabel);

        Tavola = new QTableView(QDialogTavola);
        Tavola->setObjectName(QStringLiteral("Tavola"));

        verticalLayout->addWidget(Tavola);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        OkButton = new QPushButton(QDialogTavola);
        OkButton->setObjectName(QStringLiteral("OkButton"));

        horizontalLayout->addWidget(OkButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        ButtonChiudi = new QPushButton(QDialogTavola);
        ButtonChiudi->setObjectName(QStringLiteral("ButtonChiudi"));

        horizontalLayout->addWidget(ButtonChiudi);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QDialogTavola);

        QMetaObject::connectSlotsByName(QDialogTavola);
    } // setupUi

    void retranslateUi(QDialog *QDialogTavola)
    {
        QDialogTavola->setWindowTitle(QApplication::translate("QDialogTavola", "Dialog", 0));
        TavolaLabel->setText(QApplication::translate("QDialogTavola", "TextLabel", 0));
        OkButton->setText(QApplication::translate("QDialogTavola", "oK", 0));
        ButtonChiudi->setText(QApplication::translate("QDialogTavola", "Chiudi", 0));
    } // retranslateUi

};

namespace Ui {
    class QDialogTavola: public Ui_QDialogTavola {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDIALOGTAVOLA_H
