#include "qnuovocompstep.h"
#include "ui_qnuovocompstep.h"
#include <QSqlQuery>
#include <QSqlRecord>

QNuovoCompStep::QNuovoCompStep(QWidget *parent,QSqlDatabase NewDbProcesso,QSqlRelationalTableModel *newCompStepMod) :
    QDialog(parent),
    ui(new Ui::QNuovoCompStep)
{
    ui->setupUi(this);

    DbProcesso=NewDbProcesso;

    CompStepMod=newCompStepMod;

    QSqlQuery sql(DbProcesso);
    sql.exec("SELECT * FROM TIPO_COMP ORDER BY ID_TIPO_COMP");

    bool trovato=false;
    while(sql.next()){
        QSqlRecord record=sql.record() ;
        trovato=false;
        for(int j=0; j <CompStepMod->rowCount();j++ ){
            if(record.value("TIPO_COMP").toString()==CompStepMod->record(j).value("TIPO_COMP").toString() ){
                trovato=true;
                break;
            }
        }
        if(!trovato)
            ui->ListaCompStep->addItem(record.value("TIPO_COMP").toString());
    }

}

QNuovoCompStep::~QNuovoCompStep()
{
    delete ui;
}

void QNuovoCompStep::on_ButtonOk_clicked()
{
    ok=true;
    close();
}

void QNuovoCompStep::on_ButtonAnnulla_clicked()
{
    ok=false;
    close();
}

int QNuovoCompStep::TipoComp(){

    if(ok){
      QSqlQuery sql(DbProcesso);
      sql.exec("SELECT * FROM TIPO_COMP WHERE TIPO_COMP = '" + ui->ListaCompStep->currentText() + "'");
      sql.next();
      return sql.record().value("ID_TIPO_COMP").toInt();
    }
    else
      return 0;
}





