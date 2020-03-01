#include "qprocesso.h"
#include "ui_qprocesso.h"
#include <QSqlRelationalDelegate>
#include <QDebug>
#include <QInputDialog>
#include <QSqlError>
#include <QMessageBox>
#include "qnuovocompstep.h"
//#include "../srcplc/MainCostanti.h"

//ALTER TABLE `PROCESSO` ADD `ID_MACCHINA` INT NOT NULL DEFAULT '0' AFTER `ID_PROCESSO`;
//ALTER TABLE `COMP_STEP` ADD `ID_MACCHINA` INT NOT NULL DEFAULT '0' AFTER `ID_COMP_STEP`;

QWidget* ColorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    Q_UNUSED(option)

    QTipoComp Tipo=TipoComp.at(index.row());

    double INFO=index.column()-1;
    INFO=INFO/4;
    double RAMPA=index.column();
    RAMPA=RAMPA/4;
    if((index.column()-1)/4==INFO){
        if(Tipo.index==CTipoComp::Sorgente){
            QComboBox *editor=new QComboBox(parent);
            editor->addItem("No");
            editor->addItem("Aperto");
            editor->addItem("Chiuso");
            return editor;
        }else if(Tipo.index==CTipoComp::Bias){
            QComboBox *editor=new QComboBox(parent);
            editor->addItem("Low");
            editor->addItem("Height");
            return editor;
        }else if(Tipo.index==CTipoComp::Gas){
            QSpinBox *editor = new QSpinBox(parent);
            editor->setMinimum(0);
            editor->setMaximum(500);
            return editor;
        }

    }else if((index.column())/4==RAMPA){
        QTimeEdit *editor = new QTimeEdit(parent);
        editor->setDisplayFormat("hh:mm:ss");
        return editor;
    }
    else{
          QSpinBox *editor = new QSpinBox(parent);
          editor->setMinimum(0);
          if(Tipo.index==CTipoComp::Sorgente){
             editor->setMaximum(120);
             editor->setSingleStep(5);
          }else if(Tipo.index==CTipoComp::Bias){
             editor->setMaximum(1200);
             editor->setSingleStep(50);
          }else if(Tipo.index==CTipoComp::Gas){
             editor->setMaximum(600);
             editor->setSingleStep(50);
          }
          return editor;
     }
    //}


    return NULL;

}


void ColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // Get the value via index of the Model
    //qDebug() << index.row();
    QTipoComp Tipo=TipoComp.at(index.row());


    int value = index.model()->data(index, Qt::EditRole).toInt();
    double INFO=index.column()-1;
    INFO=INFO/4;
    double RAMPA=index.column();
    RAMPA=RAMPA/4;
    if((index.column()-1)/4==INFO){
        if(Tipo.index==CTipoComp::Sorgente){
            QComboBox *combox = static_cast<QComboBox*>(editor);
            combox->setCurrentIndex(value);
        }else if(Tipo.index==CTipoComp::Bias){
            QComboBox *combox = static_cast<QComboBox*>(editor);
            combox->setCurrentIndex(value);
        }else if(Tipo.index==CTipoComp::Gas){
            QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
            spinbox->setValue(value);
        }
    }else if((index.column())/4==RAMPA){
        QTimeEdit *timebox = static_cast<QTimeEdit*>(editor);
        QTime tempo(0,0,0);
        tempo=tempo.addSecs(value);
        timebox->setTime(tempo);
    }else{
        QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
        spinbox->setValue(value);
    }

}


void ColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QTipoComp Tipo=TipoComp.at(index.row());

    double INFO=index.column()-1;
    INFO=INFO/4;
    double RAMPA=index.column();
    RAMPA=RAMPA/4;
    if((index.column()-1)/4==INFO){
        if(Tipo.index==CTipoComp::Sorgente){
            QComboBox *combox = static_cast<QComboBox*>(editor);
            int value = combox->currentIndex();
            model->setData(index, value, Qt::EditRole);
        }else if(Tipo.index==CTipoComp::Bias){
            QComboBox *combox = static_cast<QComboBox*>(editor);
            int value = combox->currentIndex();
            model->setData(index, value, Qt::EditRole);
        }else if(Tipo.index==CTipoComp::Gas){
            QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
            spinbox->interpretText();
            int value = spinbox->value();
            model->setData(index, value, Qt::EditRole);
        }
    }else if((index.column())/4==RAMPA){
        QTimeEdit *timebox = static_cast<QTimeEdit*>(editor);
        QTime tempo=timebox->time();
        int value= tempo.second();
        value+=tempo.minute()*60;
        value+=tempo.hour()*3600;
        model->setData(index, value, Qt::EditRole);
    }else{
        QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
        spinbox->interpretText();
        int value = spinbox->value();
        model->setData(index, value, Qt::EditRole);
    }

}


void ColorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void ColorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

  QTipoComp Tipo=TipoComp.at(index.row());

  if(index.column()>1){
    double DA=index.column()+2;
    DA=DA/4;
    double A=index.column()+1;
    A=A/4;
    double RAMPA=index.column();
    RAMPA=RAMPA/4;
    double INFO=index.column()-1;
    INFO=INFO/4;
    if((index.column()+2)/4==DA){
        if(index.model()->data(index).toInt())
          painter->fillRect(option.rect, QColor(Qt::green));
        QItemDelegate::paint(painter, option, index);
    }
    if((index.column()+1)/4==A){
        if(index.model()->data(index).toInt())
          painter->fillRect(option.rect, QColor(Qt::green));
        QItemDelegate::paint(painter, option, index);
    }
    if((index.column())/4==RAMPA){
        painter->fillRect(option.rect, QColor(Qt::red));
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,
                          QDateTime::fromTime_t(index.model()->data(index).toInt()).toUTC().toString("hh:mm:ss"));
       // qDebug() << index.model()->data(index).toInt() << QDateTime::fromTime_t(index.model()->data(index).toInt()).toUTC().toString("hh:mm:ss");
    }
    if((index.column()-1)/4==INFO){
        painter->fillRect(option.rect, QColor(Qt::yellow));
        painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        if(Tipo.index==CTipoComp::Sorgente){
            if(index.model()->data(index).toInt()==0)
                painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,"No");
            else if(index.model()->data(index).toInt()==1)
                painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,"Aperto");
            else if(index.model()->data(index).toInt()==2)
                painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,"Chiuso");
        }else if(Tipo.index==CTipoComp::Bias){ painter->setRenderHint(QPainter::Antialiasing, true);
            if(index.model()->data(index).toInt()==0)
                painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,"Low");
            else
                painter->drawText(option.rect,Qt::AlignCenter | Qt::AlignHCenter,"Height");
        }else
          QItemDelegate::paint(painter, option, index);

    }
   }else
     QItemDelegate::paint(painter, option, index);
}


void ColorDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    painter->fillRect(option.rect, QColor(qrand()%255, qrand()%255, qrand()%255));
}



QProcesso::QProcesso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QProcesso)
{
    ui->setupUi(this);

    ui->ButtonSalva->setToolTip("Salva");
    ui->ButtonSalva->setIcon( QIcon(":/images/salva.png"));
    ui->ButtonSalva->setIconSize(QSize(32,32));
    connect(ui->ButtonSalva, SIGNAL(clicked()), this, SLOT(Salva()));

    ui->ButtonAddStep->setToolTip("Aggiungi Step");
    ui->ButtonAddStep->setIcon( QIcon(":/images/piu.png"));
    ui->ButtonAddStep->setIconSize(QSize(32,32));
    connect(ui->ButtonAddStep, SIGNAL(clicked()), this, SLOT(StepAdd()));

    ui->ButtonDuplicaStep->setToolTip("Duplica Step");
    ui->ButtonDuplicaStep->setIcon( QIcon(":/images/duplica.png"));
    ui->ButtonDuplicaStep->setIconSize(QSize(32,32));
    connect(ui->ButtonDuplicaStep, SIGNAL(clicked()), this, SLOT(StepDuplica()));

    ui->ButtonDelStep->setToolTip("Elimina Step");
    ui->ButtonDelStep->setIcon( QIcon(":/images/cut.png"));
    ui->ButtonDelStep->setIconSize(QSize(32,32));
    connect(ui->ButtonDelStep, SIGNAL(clicked()), this, SLOT(StepElimina()));

    ui->ButtonPrima->setToolTip("Sposta Su");
    ui->ButtonPrima->setIcon( QIcon(":/images/prec.png"));
    ui->ButtonPrima->setIconSize(QSize(32,32));
    connect(ui->ButtonPrima, SIGNAL(clicked()), this, SLOT(StepSpostaSu()));

    ui->ButtonDopo->setToolTip("Sposta Giu");
    ui->ButtonDopo->setIcon( QIcon(":/images/next.png"));
    ui->ButtonDopo->setIconSize(QSize(32,32));
    connect(ui->ButtonDopo, SIGNAL(clicked()), this, SLOT(StepSpostaGiu()));


    ui->ButtonDelCompStep->setToolTip("Elimina CompStep");
    ui->ButtonDelCompStep->setIcon( QIcon(":/images/cut.png"));
    ui->ButtonDelCompStep->setIconSize(QSize(32,32));
    connect(ui->ButtonDelCompStep, SIGNAL(clicked()), this, SLOT(CompStepElimina()));

    ui->ButtonAddTempo->setToolTip("Aggiungi Step Tempo");
    ui->ButtonAddTempo->setIcon( QIcon(":/images/addtempo.png"));
    ui->ButtonAddTempo->setIconSize(QSize(32,32));
    connect(ui->ButtonAddTempo, SIGNAL(clicked()), this, SLOT(CompStepAddTempo()));

    ui->ButtonDelTempo->setToolTip("Elimina Step Tempo");
    ui->ButtonDelTempo->setIcon( QIcon(":/images/deltempo.png"));
    ui->ButtonDelTempo->setIconSize(QSize(32,32));
    connect(ui->ButtonDelTempo, SIGNAL(clicked()), this, SLOT(CompStepDelTempo()));


    ui->ButtonNuovoProcesso->setToolTip("Nuovo Processo");
    ui->ButtonNuovoProcesso->setIcon( QIcon(":/images/new.png"));
    ui->ButtonNuovoProcesso->setIconSize(QSize(32,32));
    connect(ui->ButtonNuovoProcesso, SIGNAL(clicked()), this, SLOT(ProcessoNuovo()));

    ui->ButtonDuplicaProcesso->setToolTip("Duplica Processo");
    ui->ButtonDuplicaProcesso->setIcon( QIcon(":/images/duplica.png"));
    ui->ButtonDuplicaProcesso->setIconSize(QSize(32,32));
    connect(ui->ButtonDuplicaProcesso, SIGNAL(clicked()), this, SLOT(ProcessoDuplica()));

    ui->ButtonEliminaProcesso->setToolTip("Elimina Processo");
    ui->ButtonEliminaProcesso->setIcon( QIcon(":/images/cut.png"));
    ui->ButtonEliminaProcesso->setIconSize(QSize(32,32));
    connect(ui->ButtonEliminaProcesso, SIGNAL(clicked()), this, SLOT(ProcessoElimina()));

    ui->ButtonCaricaProcesso->setToolTip("Carica Processo");
    ui->ButtonCaricaProcesso->setIcon( QIcon(":/images/tutte.png"));
    ui->ButtonCaricaProcesso->setIconSize(QSize(32,32));
    connect(ui->ButtonCaricaProcesso, SIGNAL(clicked()), this, SLOT(ProcessoCaricaSuPlc()));

    ui->ButtonSetTmpProcesso->setToolTip("Carica Processo");
    ui->ButtonSetTmpProcesso->setIcon( QIcon(":/images/edit.png"));
    ui->ButtonSetTmpProcesso->setIconSize(QSize(32,32));
    connect(ui->ButtonSetTmpProcesso, SIGNAL(clicked()), this, SLOT(ProcessoTmp()));


    ui->ButtonSalvaCompCome->setToolTip("Salva Comp Con Nome");
    ui->ButtonSalvaCompCome->setIcon( QIcon(":/images/salva.png"));
    ui->ButtonSalvaCompCome->setIconSize(QSize(32,32));
    connect(ui->ButtonSalvaCompCome, SIGNAL(clicked()), this, SLOT(CompStepSalvaCome()));

    ui->ButtonAddCompStep->setToolTip("Aggingi Componente  ");
    ui->ButtonAddCompStep->setIcon( QIcon(":/images/piu.png"));
    ui->ButtonAddCompStep->setIconSize(QSize(32,32));
    connect(ui->ButtonAddCompStep, SIGNAL(clicked()), this, SLOT(CompStepAdd()));

    ui->ButtonCambiaCompStep->setToolTip("Cambia Componente ");
    ui->ButtonCambiaCompStep->setIcon( QIcon(":/images/cambia.png"));
    ui->ButtonCambiaCompStep->setIconSize(QSize(32,32));
    connect(ui->ButtonCambiaCompStep, SIGNAL(clicked()), this, SLOT(CompStepCambia()));

    ui->TabCompStep->hide();
    ui->WidButtonCompStep->hide();
    ui->ButtonDelTempo->hide();
    ui->ButtonAddTempo->hide();
    Info=false;
    CompStepTempoCount=0;
    ui->GraficoView->setMaxOut(1100,QTipoScala::Lineare);
    ui->ButtonSalva->setEnabled(false);

    ProcessoMod = NULL;
    CompMod = NULL;
    StepMod = NULL;
    CompStepMod = NULL;
    CTipoStepMod = NULL;
    DbProcesso=NULL;
    IsRollBack=true;
    IsModificato=false;


}

QProcesso::~QProcesso()
{
    Aggiorna();
    delete ui;
}

void QProcesso::ProcessoIsOn(){

    if(Processa->pData->ProcessoIsOn){
      ui->TabProcesso->setEnabled(false);
    }else{
      ui->TabProcesso->setEnabled(true);
    }
    ProcessoApri(Processa->pData->File.ProcessoId);
    ui->Macchina->setCurrentIndex(Processa->IdCiclo);

}



//#define WAIT_TIME 25200000

//SET GLOBAL wait_timeout=28800
//SET GLOBAL wait_timeout=2
//SHOW VARIABLES LIKE "%wait%"

void QProcesso::setDbProcesso(QSqlDatabase *NewDbProcesso, int newIdMacchina, int newIdProcesso){

    DbProcesso=NewDbProcesso;
    if(!DbProcesso->isOpen())
        return;

    IdMacchina=newIdMacchina;
    QSqlQuery query(*DbProcesso);
    if(query.exec("SHOW VARIABLES LIKE 'wait_timeout%'")){
       query.next();
       //qDebug() << query.record();
       WAIT_TIME=(query.record().value("Value").toInt()-10)*1000;
       if(WAIT_TIME==0){
          WAIT_TIME =25200000 ;
       }
    }
    this->setEnabled(true);

    ProcessoMod = new QSqlQueryModel();
    CompMod = new QSqlQueryModel();
    StepMod = new QSqlRelationalTableModel(this,*DbProcesso);
    CompStepMod = new QSqlRelationalTableModel(this,*DbProcesso);
    CTipoStepMod = new QSqlQueryModel();

    CTipoStepMod->setQuery("SELECT * FROM TIPO_STEP ",*DbProcesso);
    getProcessi();

    CompMod->setQuery("SELECT TIPO_COMP, ID_TIPO_COMP, COD_COMP FROM TIPO_COMP ",*DbProcesso);

    QSqlQuery sql(*DbProcesso);    
    sql.exec("SELECT MAX(ID_STEP)  AS ID FROM STEP"),
    sql.next();
    sql.record().value("ID").toInt();
    StepMaxId= sql.record().value("ID").toInt();

    DbTimer = new QTimer(this);
    QObject::connect(DbTimer, SIGNAL(timeout()),
                     this, SLOT(onTimeOut()));
    DbTimer->start(WAIT_TIME);

    ProcessoApri(newIdProcesso);

    ui->Macchina->setCurrentIndex(IdMacchina);

}

void QProcesso::getProcessi(){

    QString str="SELECT PROCESSO, ID_PROCESSO, PROCESSO.ID_COATING ," ;
    for(int i=0;i< Processa->SorgenteCount;i++)
        str+=" TARGET_" + QString::number(i) + ", ";
    str+= " COATING  FROM PROCESSO LEFT JOIN COATING ON(COATING.ID_COATING=PROCESSO.ID_COATING) ";
    str+=" WHERE ID_MACCHINA = '" + QString::number(IdMacchina) + "' ";
    ProcessoMod->setQuery(str,*DbProcesso);

    ui->TabProcesso->setModel(ProcessoMod);

    QSqlQuery query(*DbProcesso);

    for(int i=0; i< Processa->CamereCount;i++){
        str="CREATE TEMPORARY TABLE IF NOT EXISTS COMP_STEP_" + QString::number(i)
                + " AS SELECT * FROM COMP_STEP WHERE ID_MACCHINA = '" + QString::number(i) + "'";

         if(!query.exec(str)){
             qDebug()  << str;
         }
    }

    ui->TabProcesso->setCurrentIndex(0);
    for(int i=0; i< ProcessoMod->rowCount(); i++){
        if(Processa->pData[IdMacchina].File.ProcessoId==ProcessoMod->record(i).value("ID_PROCESSO").toInt()){
            ui->TabProcesso->setCurrentIndex(i);
            ProcessoApri(Processa->pData[IdMacchina].File.ProcessoId);
            break;
        }
    }

}

void QProcesso::onTimeOut(){

    DbProcesso->rollback();
    DbTimer->stop();
    IsModificato=false;


    ui->TabCompStep->hide();
    ui->WidButtonCompStep->hide();
    ui->ButtonDelTempo->hide();
    ui->ButtonAddTempo->hide();
    CompStepTempoCount=0;
    this->setEnabled(false);

    /*QMessageBox msg;
    msg.setWindowTitle("Connetti DB");
    msg.setIcon(QMessageBox::Critical);
    msg.setText("TimeOut Riconnettersi al Database? ");
    msg.exec();
    this->setCursor(QCursor(Qt::WaitCursor));*/

    emit getNuovaConnesione();
}



QString QProcesso::getProcesso(){

    return ui->TabProcesso->currentText();
}

QString QProcesso::CTipoStep(int index){

    if(CTipoStepMod==NULL)
        return "";

    if(index==-1)
       return "";

    for(int i=0; i< CTipoStepMod->rowCount();i++){
        if(index==CTipoStepMod->record(i).value("ID_TIPO_STEP").toInt())
            return CTipoStepMod->record(i).value("TIPO_STEP").toString();
    }

    return "";
}

void QProcesso::nuovoProcesso(QString Id, int macchina){

    QSqlQuery query(*DbProcesso);
    QString str="SELECT * FROM `PROCESSO`  WHERE ID_PROCESSO = " + Id;
    if(query.exec(str)){
       query.next();
       //qDebug() << query.record().value("PROCESSO").toString();
       memset(Processa->pData[macchina].File.ProcessoNome,0,UTENTE_SIZE);
       memcpy(Processa->pData[macchina].File.ProcessoNome,
        query.record().value("PROCESSO").toString().toUtf8(),
              __min(query.record().value("PROCESSO").toString().length(),UTENTE_SIZE));
       Processa->pData[macchina].File.ProcessoId=Id.toInt();

    }
}


void QProcesso::StepAggiorna(){

    if(DbProcesso==NULL)
        return;

    if(IsRollBack)
       DbProcesso->transaction();
    int row = ui->TabProcesso->currentIndex();
    QSqlRecord record = ProcessoMod->record(row);
    ProcessoId=record.value("ID_PROCESSO").toInt();
    ProcessoNome=record.value("PROCESSO").toString();
    //qDebug() << record;

    for(int i=0; i< Processa->SorgenteCount;i++){
        int TipoTarget=record.value("TARGET_" + QString::number(i)).toInt();
        Processa->pTarget[i].TipoTarget=TipoTarget;
    }

    StepMod = new QSqlRelationalTableModel(this,*DbProcesso);
    StepMod->setTable("STEP");
    StepMod->setFilter("ID_PROCESSO = " + QString::number(ProcessoId) );

    StepMod->setSort(2,Qt::AscendingOrder);
    StepMod->setEditStrategy(QSqlTableModel::OnFieldChange);

    int Index = StepMod->fieldIndex("ID_TIPO_STEP");
    StepMod->setRelation(Index, QSqlRelation("TIPO_STEP", "ID_TIPO_STEP", "TIPO_STEP"));

    Index = StepMod->fieldIndex("ID_COMP_STEP");
    StepMod->setRelation(Index, QSqlRelation("COMP_STEP_" + QString::number(IdMacchina), "ID_COMP_STEP", "COMP_STEP"));

    StepMod->select();


    ui->TabStep->setModel(StepMod);
    ui->TabStep->setItemDelegate(new QSqlRelationalDelegate(StepMod));

    ui->TabStep->hideColumn(0);
    ui->TabStep->hideColumn(1);
    ui->TabStep->hideColumn(10);
    ui->TabStep->hideColumn(11);
    ui->TabStep->setAlternatingRowColors(true);

    ProcessoApri();

    connect(ui->TabStep->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &,const QItemSelection &)),
            this, SLOT(CompStepAggiorna()));

    connect(ui->TabStep->selectionModel(), SIGNAL(currentColumnChanged ( QModelIndex ,  QModelIndex )),
            this, SLOT(StepColumnChanged(QModelIndex , QModelIndex )));

    connect(ui->TabStep->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(StepRowChange(QModelIndex)));

    connect(StepMod, SIGNAL(beforeUpdate(int,QSqlRecord&)),
            this, SLOT(StepModBeforeUpdate(int,QSqlRecord&)));


}


void QProcesso::on_TabProcesso_activated(int index)
{
    Q_UNUSED(index);
    ui->TabCompStep->hide();
    ui->WidButtonCompStep->hide();
    ui->ButtonDelTempo->hide();
    ui->ButtonAddTempo->hide();
    Aggiorna();
    IsModificato=false;
    StepAggiorna();
    DbTimer->start(WAIT_TIME);


    for(int i=0;i< ListaProcessiCaricati.count();i++){
        if(ListaProcessiCaricati.at(i).toInt()==Processa->pData->File.ProcessoId)
            break;
        return;
    }

    QMessageBox msg;
    msg.setWindowTitle("QProcesso");
    msg.setIcon(QMessageBox::Critical);
    msg.setText("Il Processo non è caricato nel PLC");
    msg.exec();

}

void QProcesso::Aggiorna(){

    if(DbProcesso==NULL)
        return;

    if(IsRollBack & IsModificato){
        QMessageBox MsgBox;
        MsgBox.setWindowTitle("Informazione");
        MsgBox.setText( "Salva Le Modifiche ");
        MsgBox.setIcon(QMessageBox::Information);
        MsgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        MsgBox.setDefaultButton(QMessageBox::No);
        if(MsgBox.exec() == QMessageBox::Yes)
           DbProcesso->commit();
        else
           DbProcesso->rollback();
    }
    IsModificato=false;
    ui->ButtonSalva->setEnabled(false);
}

void QProcesso::Salva(){

    DbProcesso->commit();
    ui->ButtonSalva->setEnabled(false);
    IsModificato=false;
    DbTimer->start(WAIT_TIME);

}

void QProcesso::processiDisponibili(int count,QString str){

    for(int i=0; i< count; i++){
        int pos=str.indexOf(";");
        if(pos>0){
            QString nome=str.left(pos);
            ListaProcessiCaricati.append(nome);
            str=str.mid(pos+1,str.length());
        }

    }

}

void QProcesso::StepModBeforeUpdate(int index, QSqlRecord &record){

    Q_UNUSED(index)
    Q_UNUSED(record)

    ui->ButtonSalva->setEnabled(true);
    IsModificato=true;
    DbTimer->start(WAIT_TIME);

}

void QProcesso::CompStepModBeforeUpdate(int index, QSqlRecord &record){

    Q_UNUSED(index)
    Q_UNUSED(record)

    ui->ButtonSalva->setEnabled(true);
    IsModificato=true;
    DbTimer->start(WAIT_TIME);

}

bool QProcesso::ProcessoApri(int id){

    if(ProcessoMod==NULL)
        return false;

    for(int i=0; i< ProcessoMod->rowCount(); i++){
        if(id==ProcessoMod->record(i).value("ID_PROCESSO").toInt() ){
            ui->TabProcesso->setCurrentText(ProcessoMod->record(i).value("PROCESSO").toString() );
            CoatingId=ProcessoMod->record(i).value("ID_COATING").toInt();
            Coating=ProcessoMod->record(i).value("COATING").toString();
            StepAggiorna();
            return true;
        }
    }
    return false;
}

int QProcesso::getPercent(int CompStep){

    int i;
    double percent;
    for( i=0; i< pListaSegnali.count();i++){
        if(pListaSegnali.at(i)->TipoComp==CompStep){
           percent=i+1;
           break;
        }
    }

    percent/=pListaSegnali.count();
    percent*=100;
    //qDebug() << CompStep<< i+1 << pListaSegnali.count()<< percent;
    return (int)percent;

}

int intCTipoStep(QString str){

   if(str=="Vuoto")
        return CTipoStep::Vuoto;
    else if(str=="Riscaldo")
        return CTipoStep::Riscaldo;
    else if(str=="Etching")
        return CTipoStep::Etching;
    else if(str=="Coating")
        return CTipoStep::Coating;
   else if(str=="Venting")
       return CTipoStep::Venting;

   return -1;
}
//#define DEBUG_PRO_SET


void QProcesso::ProcessoCaricaDaPlc(){

    QString str="";
    for(int i=0;i<Processa->Pc_StepCount;i++ ){
        //qDebug() << Processa->Pc_StepCount << Processa->IdPro;
        //StepType a=Processa->Pc_Step[i];
        //StepType b=Processa->Step[Processa->IdPro][i];
        if(Processa->Pc_Step[i].Tempo!=Processa->Step[Processa->IdPro][i].Tempo ){
            str+="Pos " + QString::number(i) + " " + Processa->Step[Processa->IdPro][i].Descrizione + "\t" +
                 " Tempo \t" + QDateTime::fromTime_t(Processa->Step[Processa->IdPro][i].Tempo/1000).toUTC().toString("hh:mm:ss") +
                 " su " + QDateTime::fromTime_t(Processa->Pc_Step[i].Tempo/1000).toUTC().toString("hh:mm:ss") + "\n";
        }
        if(Processa->Pc_Step[i].Pressione!=Processa->Step[Processa->IdPro][i].Pressione ){
            str+="Pos " + QString::number(i) + " " + Processa->Step[Processa->IdPro][i].Descrizione + "\t" +
                 " Pressione \t" + QString::number(Processa->Step[Processa->IdPro][i].Pressione) +
                 " su " + QString::number(Processa->Pc_Step[i].Pressione)+ "\n";
        }
        if(Processa->Pc_Step[i].Temperatura!=Processa->Step[Processa->IdPro][i].Temperatura ){
            str+="Pos " + QString::number(i) + " " + Processa->Step[Processa->IdPro][i].Descrizione + "\t" +
                 " Temperatura \t" + QString::number(Processa->Step[Processa->IdPro][i].Temperatura) +
                 " su " + QString::number(Processa->Pc_Step[i].Temperatura)+ "\n";
        }
        if(Processa->Pc_Step[i].Potenza!=Processa->Step[Processa->IdPro][i].Potenza ){
            str+="Pos " + QString::number(i) + " " + Processa->Step[Processa->IdPro][i].Descrizione + "\t" +
                 " Potenza \t" + QString::number(Processa->Step[Processa->IdPro][i].Potenza) +
                 " su " + QString::number(Processa->Pc_Step[i].Potenza)+ "\n";
        }
        if(Processa->Pc_Step[i].RepitCount!=Processa->Step[Processa->IdPro][i].RepitCount ){
            str+="Pos " + QString::number(i) + " " + Processa->Step[Processa->IdPro][i].Descrizione + "\t" +
                 " Repit \t" + QString::number(Processa->Step[Processa->IdPro][i].RepitCount) +
                 " su " + QString::number(Processa->Pc_Step[i].RepitCount)+ "\n";
        }
    }

    /*
    for(int i=0;i<pListaSegnali.count();i++ ){
        CIOSegnale *pPc_Segnale=pListaSegnali.at(i);
        CIOSegnale *pSegnale;
        for(int j=0;j<Processa->SorgenteCount;j++ ){
            if(pPc_Segnale->TipoComp==Processa->Bias[IdMacchina].Sorgente[j].SetI->TipoComp){
                pSegnale=Processa->Bias[IdMacchina].Sorgente[j].SetI;
                str+=ControllaStep(pPc_Segnale,pSegnale);
            }
        }
        for(int j=0;j<Processa->GasCount;j++ ){
            if(pPc_Segnale->TipoComp==Processa->GGas[j].SetG->TipoComp){
                pSegnale=Processa->GGas[j].SetG;
                str+=ControllaStep(pPc_Segnale,pSegnale);
            }
        }
        if(pPc_Segnale->TipoComp==Processa->Bias[IdMacchina].SetV->TipoComp){
            pSegnale=Processa->Bias[IdMacchina].SetV;
            str+=ControllaStep(pPc_Segnale,pSegnale);
        }

    }
  */
    if(str.length()!=0){
        int questione=QMessageBox::question(this," Differenze Processo Plc & PC Ricarico Processo?" + ProcessoNome,str);
        if(questione==QMessageBox::Yes)
            ProcessoCaricaSuPlc();
    }


}

QString QProcesso::ControllaStep(CIOSegnale *pPc_Segnale,CIOSegnale *pSegnale){

    bool IsDiverso=false;
    QString str=pPc_Segnale->Descrizione();

    for(int i=0;i<Processa->Pc_StepCount;i++ ){
       if(i==5)
           i=5;
       for(int j=0;j<pPc_Segnale->m_Step[i].StepCount;j++ ){
         if(pPc_Segnale->m_Step[i].Step[j].Inf!=pSegnale->m_Step[i].Step[j].Inf){
              IsDiverso=true;
              str+=" Step  " + QString::number(i) + " Inf " + QString::number(pPc_Segnale->m_Step[i].Step[j].Inf)
                   + " su " + QString::number(pSegnale->m_Step[i].Step[j].Inf) + "\n";
         }
         if(pPc_Segnale->m_Step[i].Step[j].Start!=pSegnale->m_Step[i].Step[j].Start){
               IsDiverso=true;
               str+=" Step  " + QString::number(i) + " Start " + QString::number(pPc_Segnale->m_Step[i].Step[j].Start*pSegnale->Fattore(),'f',0)
                       + " su " +  QString::number(pSegnale->m_Step[i].Step[j].Start*pSegnale->Fattore(),'f',0)+ "\n";
         }
         if(pPc_Segnale->m_Step[i].Step[j].Stop!=pSegnale->m_Step[i].Step[j].Stop){
               IsDiverso=true;
               str+=" Step  " + QString::number(i) + " Stop " + QString::number(pPc_Segnale->m_Step[i].Step[j].Stop*pSegnale->Fattore(),'f',0)
                     + " Su " +  QString::number(pSegnale->m_Step[i].Step[j].Stop*pSegnale->Fattore(),'f',0)+ "\n";
         }
         if(pPc_Segnale->m_Step[i].Step[j].Rampa!=pSegnale->m_Step[i].Step[j].Rampa){
               IsDiverso=true;
               str+=" Step  " + QString::number(i) + " Rampa " + QDateTime::fromTime_t(pPc_Segnale->m_Step[i].Step[j].Rampa/1000).toUTC().toString("hh:mm:ss")
                   + " Su " +  QDateTime::fromTime_t(pSegnale->m_Step[i].Step[j].Rampa/1000).toUTC().toString("hh:mm:ss") + "\n";
         }
       }
    }

    //QDateTime::fromTime_t(pSegnale->m_Step[i].Step[j].Rampa/1000).toUTC().toString("hh:mm:ss")

    if(IsDiverso){
        return str;
    }

    return "";
}

void QProcesso::ProcessoApri(){

    if(Processa==NULL | DbProcesso==NULL)
        return;

    TempoProcesso=0;
    pListaSegnali.clear();
    Processa->Pc_StepCount=0;
    Processa->ProcessoId=StepMod->record(0).value("ID_PROCESSO").toInt();
    memset(Processa->ProcessoNome,0,UTENTE_SIZE);
    memcpy(Processa->ProcessoNome,ProcessoNome.toUtf8(),__min(ProcessoNome.length(),UTENTE_SIZE));
    for(int i=0;i<StepMod->rowCount();i++){
       TempoProcesso+=StepMod->record(i).value("TEMPO").toInt()*StepMod->record(i).value("REPIT").toInt();
       for(int j=0;j<6;j++){
           double maxi=StepMod->record(j).value("`" + QString::number(j+1) + "`").toInt();
           double fattore=Processa->Bias[IdMacchina].Sorgente[j].SetI->Fattore();
           if(fattore){
              maxi/=fattore;
              Processa->Bias[IdMacchina].Sorgente[j].Catodo->setMaxI((WORD)maxi);
           }
       }
       memset(&Processa->Pc_Step[Processa->Pc_StepCount],0,sizeof(StepType));
       TempoProcesso+=StepMod->record(i).value("TEMPO").toInt()*StepMod->record(i).value("REPIT").toInt();
       Processa->Pc_Step[Processa->Pc_StepCount].Step=Processa->Pc_StepCount;
       Processa->Pc_Step[Processa->Pc_StepCount].ProStep=Processa->Pc_StepCount;
       Processa->Pc_Step[Processa->Pc_StepCount].CTipoStep=intCTipoStep(StepMod->record(i).value("TIPO_STEP").toString());
       int repit=StepMod->record(i).value("REPIT").toInt();
       if(repit==0)
           Processa->Pc_Step[Processa->Pc_StepCount].RepitCount=1;
       else if(repit>255)
           Processa->Pc_Step[Processa->Pc_StepCount].RepitCount=255;
       else
           Processa->Pc_Step[Processa->Pc_StepCount].RepitCount=repit;
       Processa->Pc_Step[Processa->Pc_StepCount].Repit=1;
       Processa->Pc_Step[Processa->Pc_StepCount].Tempo=StepMod->record(i).value("TEMPO").toInt()*1000;
       Processa->Pc_Step[Processa->Pc_StepCount].Temperatura=StepMod->record(i).value("TEMPERATURA").toInt();
       if((StepMod->record(i).value("BIAS_OFF_TIME").toInt()>100) | (StepMod->record(i).value("BIAS_OFF_TIME").toInt()<0)){
           QMessageBox msg;
           msg.setWindowTitle("Errore Processo");
           msg.setIcon(QMessageBox::Critical);
           msg.setText("La percentuale di off Bias deve essere max 100");
           msg.exec();
           ProcessoNonValido=true;
           return;
       }
       Processa->Pc_Step[Processa->Pc_StepCount].PerBiasOnTime=StepMod->record(i).value("BIAS_OFF_TIME").toInt();
       if(StepMod->record(i).value("PRESSIONE").toDouble()){
          double val=StepMod->record(i).value("PRESSIONE").toDouble()/1000;
          double pressione;
          pressione= log(val);
          pressione/=log(10);
          pressione+= Processa->Vuoto[IdMacchina].Penning->Segnale()->SecondoFattore();
          if(Processa->Vuoto[IdMacchina].Penning->Segnale()->Fattore())
             pressione/= Processa->Vuoto[IdMacchina].Penning->Segnale()->Fattore() ;
          pressione /= 10;
          pressione *= Processa->Vuoto[IdMacchina].Penning->Segnale()->FondoScala();
          Processa->Pc_Step[Processa->Pc_StepCount].Pressione=pressione;
       }else{
          if(Processa->Pc_Step[Processa->Pc_StepCount].CTipoStep==CTipoStep::Riscaldo){
              QMessageBox msg;
              msg.setWindowTitle("Errore Processo");
              msg.setIcon(QMessageBox::Critical);
              msg.setText("La pressione nel Riscaldo non puo essere nulla");
              msg.exec();
              ProcessoNonValido=true;
              return;
          }else
             Processa->Pc_Step[Processa->Pc_StepCount].Pressione=0;
       }
       Processa->Pc_Step[Processa->Pc_StepCount].Potenza=StepMod->record(i).value("POTENZA").toInt();
       int temp= StepMod->record(i).value("TEMP").toInt();
       if(Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->Fattore())
         temp/=Processa->Vuoto[IdMacchina].Resistenza->Temp_Resistenza->Fattore();
       Processa->Pc_Step[Processa->Pc_StepCount].Temperatura=temp;
       Processa->Pc_Step[Processa->Pc_StepCount].Tavola=50;
       BYTE mSorgentiOn=0; //
       memset(Processa->Pc_Step[Processa->Pc_StepCount].Descrizione,0,UTENTE_SIZE);
       memcpy(Processa->Pc_Step[Processa->Pc_StepCount].Descrizione,StepMod->record(i).value("COMP_STEP").toString().toUtf8(),
               __min(StepMod->record(i).value("COMP_STEP").toString().length(),UTENTE_SIZE));
       if(StepMod->record(i).value("COMP_STEP").toString()!="NESSUNO"){
          QSqlQuery sql(*DbProcesso);
          QString str="SELECT * FROM `" + StepMod->record(i).value("COMP_STEP").toString() + "` ";
          str+=" ORDER BY ID_TIPO_COMP";
          if(!sql.exec(str) ){
              QMessageBox msgbox;
              msgbox.setIcon(QMessageBox::Critical);
              msgbox.setWindowTitle(StepMod->record(i).value("COMP_STEP").toString());
              msgbox.setText(sql.lastError().text());
              msgbox.exec();
              qDebug() << str;
              ProcessoNonValido=true;
              break;
          }
          if(sql.numRowsAffected()==0){
              QMessageBox msgbox;
              msgbox.setIcon(QMessageBox::Critical);
              msgbox.setWindowTitle(StepMod->record(i).value("COMP_STEP").toString());
              msgbox.setText("Nessun Record");
              msgbox.exec();
              qDebug() << str;
              ProcessoNonValido=true;
              break;
          }

          short start,stop,info;
          int rampa;
          int MaxCompSteptempo=0;
          while (sql.next()) {
              str=sql.record().value("ID_TIPO_COMP").toString();
              int comp=str.left(1).toInt() ;
              int index=str.right(1).toInt() ;
              for(int j=0; j<(sql.record().count()-1)/4; j++){
                  Processa->Pc_Step[Processa->Pc_StepCount].CompStepCount++;
                  //int indexenabled=str.right(1).toInt() ;
                  info=sql.record().value("INFO" + QString::number(j+1) ).toInt();
                  start=sql.record().value("DA" + QString::number(j+1)).toInt();
                  stop=sql.record().value("A" + QString::number(j+1) ).toInt();
                  MaxCompSteptempo=__max(MaxCompSteptempo,sql.record().value("TR" + QString::number(j+1)).toInt());
                  if(j==0){
                    rampa=sql.record().value("TR" + QString::number(j+1)).toInt();
                  }else{
                    rampa=sql.record().value("TR" + QString::number(j+1)).toInt();
                    rampa-=sql.record().value("TR" + QString::number(j)).toInt();
                  }
                  if(rampa<=0){
                      QMessageBox msg;
                      msg.setWindowTitle("Processo ");
                      msg.setIcon(QMessageBox::Critical);
                      msg.setText("Steep Processo num " + QString::number(Processa->Pc_StepCount) +
                                  "\nRampa Negativa o Nulla " + QString::number(rampa) +
                                  " di " + sql.record().value("ID_TIPO_COMP").toString());
                      msg.exec();
                      ProcessoNonValido=true;
                      break;
                  }
                  CIOSegnale *pSegnale=NULL;
                  if(comp==CTipoComp::Gas){
                     pSegnale=&Processa->Pc_Gas[index];
                  }else if(comp==CTipoComp::Sorgente){
                     pSegnale=&Processa->Pc_Sorgenti[index];
                     mSorgentiOn=SetBit(mSorgentiOn, index, true);
                  }else if(comp==CTipoComp::Bias){
                     pSegnale=Processa->Pc_Bias;
                  }else if(comp==CTipoComp::SchermoTurbo){
                      pSegnale=&Processa->Pc_SchermoTurbo[IdMacchina];
                   }
                  addSegnale(str,pSegnale);
                  if(j>D_COMP_STEP){
                      QMessageBox msg;
                      msg.setWindowTitle("Processo ");
                      msg.setIcon(QMessageBox::Critical);
                      msg.setText("Steep Processo num " + QString::number(Processa->Pc_StepCount) + "\nSuperato il Numero di CompStep " + QString::number(j) +
                                  " di " + sql.record().value("ID_TIPO_COMP").toString());
                      msg.exec();
                      ProcessoNonValido=true;
                      break;
                  }
                  if(!LoadSegnale(pSegnale, true,info,i,j, start, stop, rampa)){
                      QMessageBox msg;
                      msg.setWindowTitle("Processo ");
                      msg.setIcon(QMessageBox::Critical);
                      msg.setText("Steep Processo num " + QString::number(Processa->Pc_StepCount) + "\nSuperato il Numero di CompStep " + QString::number(j) +
                                  " di " + sql.record().value("ID_TIPO_COMP").toString());
                      msg.exec();
                      ProcessoNonValido=true;
                      break;
                  }
#ifdef DEBUG_PRO_SET
                  qDebug() << i << j << sql.record().value("ID_TIPO_COMP").toString()
                           << pSegnale->m_Step[i].Step[j].Start << pSegnale->m_Step[i].Step[j].Stop
                           << pSegnale->m_Step[i].Step[j].Rampa;
#endif
              }
              if(MaxCompSteptempo*1000!=(int)Processa->Pc_Step[Processa->Pc_StepCount].Tempo){
                  //mostraEvento(record.value("COMP_STEP").toString() + " Cambio Step Tempo da " + QString::number(Processa->Pc_Step[Processa->Pc_StepCount].Tempo) + " a " + QString::number(MaxCompSteptempo) );
                  Processa->Pc_Step[Processa->Pc_StepCount].Tempo=MaxCompSteptempo*1000;
                  QSqlRecord record=StepMod->record(Processa->Pc_StepCount);
                  record.setValue("TEMPO" ,MaxCompSteptempo);
                  record.setGenerated("TEMPO" ,true);
                  StepMod->setRecord(Processa->Pc_StepCount,record);
              }
          }
       }
       Processa->Pc_Step[Processa->Pc_StepCount].SorgentiOn=mSorgentiOn;
       mSorgentiOn=0;
       Processa->Pc_StepCount++;
       if(Processa->Pc_StepCount>D_STEP){
           QMessageBox msg;
           msg.setWindowTitle("Processo ");
           msg.setIcon(QMessageBox::Critical);
           msg.setText("Superato in Numero di Step " + QString::number(Processa->Pc_StepCount));
           msg.exec();
           ProcessoNonValido=true;
           break;
       }
    }

    ui->labelTempoProcesso->setText(QDateTime::fromTime_t(TempoProcesso).toUTC().toString("hh:mm:ss"));
    ui->GraficoView->setSegnali(pListaSegnali);
    int scala=4000; //720
    qreal zoom=scala;
    if(TempoProcesso)
      zoom/=TempoProcesso;
    ui->GraficoView->Grafico->setTempo(scala,zoom);
    Processa->pListaSegnali=&pListaSegnali;
    ProcessoNonValido=false;


}


bool QProcesso::CompStepBiasIsOn(){

   bool BiasIsOn=true;

   for(int i=0; i<Processa->pData->ProStepCount;i++ ){
       if((Processa->Pc_Step[i].CTipoStep==CTipoStep::Coating) | (Processa->Pc_Step[i].CTipoStep==CTipoStep::Etching)){
           int TempoOn=0;
           for(int j=0;j<Processa->SorgenteCount; j++){
               TempoOn=__max(TempoOn, (int)Processa->Bias[IdMacchina].Sorgente[j].SetI->OnTime(i));
           }
           if(TempoOn>Processa->Bias[IdMacchina].SetV->OnTime(i)){
               QMessageBox msg;
               msg.setWindowTitle("Processo ");
               msg.setIcon(QMessageBox::Critical);
               msg.setText("BiasIsOFF al step  " + QString::number(i) +
                           " di "  + QString::number(Processa->Bias[IdMacchina].Sorgente[i].SetI->TipoComp));
               msg.exec();
               ProcessoNonValido=true;
               BiasIsOn=false;
               return BiasIsOn;


         }
       }
   }

   return BiasIsOn;

}

void QProcesso::addSegnale(QString id,CIOSegnale *Segnale){

    CIOSegnale *pSegnale=Segnale;

    if(!pSegnale)
        return;

    for(int i=0;i<pListaSegnali.count();i++){
        pSegnale=pListaSegnali.at(i);
        if(QString::number( pSegnale->TipoComp)==id)
            return;
    }

    Segnale->UnLoad(0);
    pListaSegnali.append(Segnale);

}


void QProcesso::StepAdd(){

    //QSqlRecord selrecord=StepMod->record(ui->TabStep->currentIndex().row());
    int row = StepMod->rowCount();
    int pos;
    QSqlRecord record;
    if(row>0){
        record=StepMod->record(row-1);
        pos=record.value("POS").toInt();
    }else
        pos=0;


    //qDebug() << record;
    record=StepMod->record(row);
    if(StepMod->insertRow(row)){
        record.setValue("ID_STEP",StepMaxId+1);
        record.setValue("ID_PROCESSO",ProcessoId);
        record.setValue("POS",pos+1);
        record.setValue("TIPO_STEP",1);
        record.setValue("TEMPO",0);
        record.setValue("POTENZA",0);
        record.setValue("TEMP",0);
        record.setValue("REPIT",1);
        record.setValue("PRESSIONE",0.3);
        record.setValue("COMP_STEP",1);
    }

    //qDebug() << record;
    StepMod->setRecord(row,record);
    StepMaxId++;

}

void QProcesso::StepDuplica(){

    int row = ui->TabStep->currentIndex().row()  ;
    if(row==-1 )
        return;

    QSqlRecord record=StepMod->record(row) ;
    QString id=record.value("ID_STEP").toString();
    int newpos=record.value("POS").toInt();

    if(row+1<StepMod->rowCount()){
        for(int i=row+1; i<StepMod->rowCount();i++){
            record=StepMod->record(i) ;
            int pos=record.value("POS").toInt()+1;
            record.setValue("POS",pos);
            StepMod->setRecord(i,record);
        }
    }


    QSqlQuery sql(*DbProcesso);
    sql.exec("CREATE TEMPORARY TABLE TMP AS SELECT * FROM STEP WHERE ID_STEP ='" + id + "'");
    sql.exec("UPDATE TMP SET ID_STEP = NULL , POS ='" + QString::number(newpos+1) + "'");
    sql.exec("INSERT INTO STEP SELECT * FROM TMP");
    sql.exec("DROP TABLE TMP");


    StepAggiorna();


}

void QProcesso::StepSpostaSu(){

    int row = ui->TabStep->currentIndex().row()  ;

    if(row==-1 || row==0)
        return;

    QSqlRecord record=StepMod->record(row) ;
    int pos=record.value("POS").toInt();
    pos--;
    if(pos<0)
        return;
    record.setValue("POS",pos);
    StepMod->setRecord(row,record);
    if(row-1>=0){
      record=StepMod->record(row-1) ;
      int pos=record.value("POS").toInt();
      pos++;
      record.setValue("POS",pos);
      StepMod->setRecord(row-1,record);
    }


    StepAggiorna();
    if(row-1>=0)
      ui->TabStep->selectRow(row-1);

}

void QProcesso::StepSpostaGiu(){

    int row = ui->TabStep->currentIndex().row()  ;

    if(row==-1 || row==StepMod->rowCount())
        return;

    QSqlRecord record=StepMod->record(row) ;
    int pos=record.value("POS").toInt();
    pos++;
    if(pos>StepMod->rowCount())
        return;
    record.setValue("POS",pos);
    StepMod->setRecord(row,record);
    if(row+1<=StepMod->rowCount()){
      record=StepMod->record(row+1) ;
      int pos=record.value("POS").toInt();
      pos--;
      record.setValue("POS",pos);
      StepMod->setRecord(row+1,record);
    }


    StepAggiorna();
    if(row+1<=StepMod->rowCount())
      ui->TabStep->selectRow(row+1);

}


void QProcesso::StepElimina(){

    int row = ui->TabStep->currentIndex().row()  ;
    if(row==-1 )
        return;

    QSqlRecord record=StepMod->record(row) ;
    int id=record.value("ID_STEP").toInt();
    if(row+1<StepMod->rowCount()){
        for(int i=row+1; i<StepMod->rowCount();i++){
            record=StepMod->record(i) ;
            int pos=record.value("POS").toInt()-1;
            record.setValue("POS",pos);
            StepMod->setRecord(i,record);
        }
    }

    QSqlQuery sql(*DbProcesso);
    sql.exec("DELETE FROM STEP WHERE ID_STEP = " + QString::number(id));
    StepAggiorna();
}

void QProcesso::Edit(){

    if(Processa->pData->ProcessoIsOn){
       if(Processa->pData->StepIndex>ui->TabStep->currentIndex().row()){
           ui->TabCompStep->hide();
           ui->WidButtonCompStep->hide();
           ui->ButtonDelTempo->hide();
           ui->ButtonAddTempo->hide();
           CompStepTempoCount=0;
       }
    }



}


void QProcesso::CompStepAggiorna(){

    int row = ui->TabStep->currentIndex().row();
    QSqlRecord record = StepMod->record(row);
    CompStepNome=record.value("COMP_STEP").toString();

    if(CompStepNome=="NESSUNO"){
        ui->TabCompStep->hide();
        ui->WidButtonCompStep->hide();
        ui->ButtonDelTempo->hide();
        ui->ButtonAddTempo->hide();
        CompStepTempoCount=0;
        return;
    }

    if(Processa->pData->ProcessoIsOn){
       if(Processa->pData->StepIndex>row){
           ui->TabCompStep->hide();
           ui->WidButtonCompStep->hide();
           ui->ButtonDelTempo->hide();
           ui->ButtonAddTempo->hide();
           CompStepTempoCount=0;
           return;
       }
    }

    ui->TabCompStep->show();
    ui->WidButtonCompStep->show();
    ui->ButtonDelTempo->show();
    ui->ButtonAddTempo->show();
    CompStepMod->setTable( "`" + CompStepNome + "`");
    CompStepMod->sort(1 ,Qt::AscendingOrder);
    CompStepMod->setEditStrategy(QSqlTableModel::OnFieldChange);
    CompStepMod->select();

    int Index = CompStepMod->fieldIndex("ID_TIPO_COMP");
    CompStepMod->setRelation(Index, QSqlRelation("TIPO_COMP", "ID_TIPO_COMP", "TIPO_COMP"));
    CompStepMod->select();

    QList <QTipoComp> TipoComp;
    for(int i=0 ; i< CompStepMod->rowCount(); i++){
        QString str = getTipoComp(CompStepMod->record(i).value("TIPO_COMP").toString());
        QTipoComp in;
        in.index=str.left(1).toInt();
        in.Descrizione=CompStepMod->record(i).value("TIPO_COMP").toString();
        TipoComp.append(in);
        //qDebug() << TipoComp;
    }

    ui->TabCompStep->setModel(CompStepMod);
    ui->TabCompStep->setItemDelegate(new ColorDelegate(ui->TabCompStep,TipoComp));
    ui->TabCompStep->setAlternatingRowColors(true);

    //ui->TabCompStep->hideRow(1);

    ui->TabCompStep->hideColumn(0);


    CompStepTempoCount=(CompStepMod->record(0).count()-1)/4;

    CompStepMaxId=CompStepMod->record(CompStepMod->rowCount()-1).value("ID").toInt();

    connect(ui->TabCompStep->selectionModel(), SIGNAL(currentColumnChanged ( QModelIndex ,  QModelIndex )),
            this, SLOT(CompStepColumnChanged(QModelIndex , QModelIndex )));

    connect(ui->TabCompStep->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(CompStepRowChange(QModelIndex)));

    connect(CompStepMod, SIGNAL(beforeUpdate(int,QSqlRecord&)),
            this, SLOT(CompStepModBeforeUpdate(int,QSqlRecord&)));

    update();

}

void QProcesso::paintEvent(QPaintEvent *event){

    Q_UNUSED(event);
    this->setCursor(QCursor(Qt::WaitCursor));
    ProcessoDisegna();
    this->setCursor(QCursor(Qt::ArrowCursor));

    if(CompStepMod==NULL)
        return;

    for(int i=2;i<CompStepMod->columnCount() ;i++){
        if(CompStepMod->record().fieldName(i).left(2)=="TR")
          ui->TabCompStep->setColumnWidth(i,90);
        else
         ui->TabCompStep->setColumnWidth(i,50);
    }

    //ui->TabStep->setColumnWidth(0,80);
    ui->TabStep->setColumnWidth(2,40);
    ui->TabStep->setColumnWidth(4,60);
    ui->TabStep->setColumnWidth(5,50);
    ui->TabStep->setColumnWidth(6,90);
    ui->TabStep->setColumnWidth(7,80);
    ui->TabStep->setColumnWidth(8,50);
    ui->TabStep->setColumnWidth(9,120);
    ui->TabStep->setColumnWidth(12,200);

}

void QProcesso::ProcessoDisegna(){

    if(StepMod==NULL)
        return;

    qreal tempo=0,dt;
    for(int i=0 ; i<StepMod->rowCount();i++){
        if(StepMod->record(i).value("COMP_STEP").toString()!="NESSUNO"){
            for(int j=0; j<StepMod->record(i).value("REPIT").toInt();j++){
               //ui->GraficoView->Grafico->drawCompStep(tempo,i);
               dt=StepMod->record(i).value("TEMPO").toInt();
               tempo+=dt;
            }
        }
    }

    ui->GraficoView->Grafico->setTempo(tempo);
    tempo=0;
    for(int i=0 ; i<StepMod->rowCount();i++){
        if(StepMod->record(i).value("COMP_STEP").toString()!="NESSUNO"){
            for(int j=0; j<StepMod->record(i).value("REPIT").toInt();j++){
               ui->GraficoView->Grafico->drawCompStep(tempo,i);
               dt=StepMod->record(i).value("TEMPO").toInt();
               tempo+=dt;
            }
        }
    }


}

void QProcesso::CompStepCambia(){

    int row = ui->TabCompStep->currentIndex().row() ;
    if(row==-1 )
        return;

    QSqlRecord record=CompStepMod->record(row) ;
    QString TipoComp;
    for(int i=0;i< CompMod->rowCount();i++){
        if(record.value("TIPO_COMP").toString()==CompMod->record(i).value("TIPO_COMP").toString()){
           TipoComp=CompMod->record(i).value("ID_TIPO_COMP").toString();
           break;
        }
    }

    int index;
    CIOSegnale *pOldSegnale;
    for(int i=0;i< pListaSegnali.count() ;i++){
        pOldSegnale=pListaSegnali.at(i);
        if(QString::number( pOldSegnale->TipoComp)==TipoComp){
            index=i;
            break;
        }
    }


    QNuovoCompStep dialog(this,*DbProcesso,CompStepMod);

    dialog.exec();

    if(dialog.TipoComp()==0)
       return;

    QString str=QString::number(dialog.TipoComp());
    CIOSegnale *pNewSegnale;
    if(str.left(1).toInt()==1){
       pNewSegnale=Processa->GGas[str.right(1).toInt()].SetG;
    }else if(str.left(1).toInt()==2){
       pNewSegnale=Processa->Bias[IdMacchina].Sorgente[str.right(1).toInt()].SetI;
    }else if(str.left(1).toInt()==3){
       pNewSegnale=Processa->Bias[IdMacchina].SetV;
    }

    pNewSegnale->m_Step=pOldSegnale->m_Step;
    pListaSegnali.replace(index,pNewSegnale);
    pOldSegnale->m_Step=NULL;

    record.setValue("TIPO_COMP",pNewSegnale->TipoComp);
    CompStepMod->setRecord(row,record);

    ui->GraficoView->setSegnali(pListaSegnali);
    update();

}

void QProcesso::CompStepAdd(){

    if(DbProcesso==NULL)
        return;

    QNuovoCompStep dialog(this,*DbProcesso,CompStepMod);

    dialog.exec();

    if(dialog.TipoComp()==0)
       return;


    QSqlRecord record, lastrecord;


    if(CompStepTempoCount>=D_COMP_STEP){
        QMessageBox msg;
        msg.setWindowTitle("Errore Processo");
        msg.setIcon(QMessageBox::Critical);
        msg.setText("Massimo numero di CompStep = " + QString::number(D_COMP_STEP) );
        msg.exec();
        return;
    }

    int row = CompStepMod->rowCount();
    int id=0;
    QString lastid;

    record=CompStepMod->record(row);
    if(row>0){
       for(int i=0; i< row;i++){
           if(id<CompStepMod->record(i).value("ID").toInt())
               id=CompStepMod->record(i).value("ID").toInt();
       }
       lastrecord=CompStepMod->record(row-1);
       lastid=lastrecord.value("ID").toString();
    }

    QSqlQuery sql(*DbProcesso);
    if(!sql.exec("CREATE TEMPORARY TABLE TMP AS SELECT * FROM " + CompStepNome + " WHERE ID ='" + lastid + "'")){
        QMessageBox msg;
        msg.setWindowTitle("Errore Processo");
        msg.setIcon(QMessageBox::Critical);
        msg.setText(sql.lastError().text() );
        msg.exec();
    }
    if(!sql.exec("UPDATE TMP SET ID = NULL , ID_TIPO_COMP ='"  + QString::number( dialog.TipoComp()) + "'")){
        QMessageBox msg;
        msg.setWindowTitle("Errore Processo");
        msg.setIcon(QMessageBox::Critical);
        msg.setText(sql.lastError().text() );
        msg.exec();
    }
    sql.exec("INSERT INTO " + CompStepNome + " SELECT * FROM TMP");
    sql.exec("DROP TABLE TMP");

    CompStepAggiorna();




}



void QProcesso::CompStepElimina(){

    if(DbProcesso==NULL)
        return;

    int row = ui->TabCompStep->currentIndex().row()  ;
    if(row==-1 )
        return;

    QSqlRecord record=CompStepMod->record(row) ;  
    QString TipoComp;
    for(int i=0;i< CompMod->rowCount();i++){
        if(record.value("TIPO_COMP").toString()==CompMod->record(i).value("TIPO_COMP").toString()){
           TipoComp=CompMod->record(i).value("ID_TIPO_COMP").toString();
           break;
        }
    }

    QSqlQuery sql(*DbProcesso);
    sql.exec("DELETE FROM " + CompStepNome +  " WHERE ID = " + record.value("ID").toString());

    CIOSegnale *pSegnale;
    for(int i=0;i< pListaSegnali.count() ;i++){
        pSegnale=pListaSegnali.at(i);
        if(QString::number( pSegnale->TipoComp)==TipoComp){
            pListaSegnali.removeAt(i);
            break;
        }
    }

    ui->GraficoView->setSegnali(pListaSegnali);
    update();
    CompStepAggiorna();
}

void QProcesso::CompStepAddTempo(){

    if(DbProcesso==NULL)
        return;

    if(CompStepTempoCount==0)
            return;

    this->setCursor(QCursor(Qt::WaitCursor));
    QSqlQuery sql(*DbProcesso);
    sql.exec("ALTER TABLE " + CompStepNome + " ADD DA" + QString::number(CompStepTempoCount+1) + " INTEGER");
    sql.exec("UPDATE " + CompStepNome +  " SET DA" + QString::number(CompStepTempoCount+1) +
              "= DA"  + QString::number(CompStepTempoCount) );
    sql.exec("ALTER TABLE " + CompStepNome + " ADD A" + QString::number(CompStepTempoCount+1) + " INTEGER");
    sql.exec("UPDATE " + CompStepNome +  " SET A" + QString::number(CompStepTempoCount+1) +
              "= A"  + QString::number(CompStepTempoCount) );
    sql.exec("ALTER TABLE " + CompStepNome + " ADD TR" + QString::number(CompStepTempoCount+1) + " INTEGER");
    sql.exec("UPDATE " + CompStepNome +  " SET TR" + QString::number(CompStepTempoCount+1) +
              "= TR"  + QString::number(CompStepTempoCount) );
    sql.exec("ALTER TABLE " + CompStepNome + " ADD INFO" + QString::number(CompStepTempoCount+1) + " TEXT");
    sql.exec("UPDATE " + CompStepNome +  " SET INFO" + QString::number(CompStepTempoCount+1) +
              "= INFO"  + QString::number(CompStepTempoCount) );

    CIOSegnale *pSegnale;
    for(int i=0; i< pListaSegnali.count();i++){
        pSegnale=pListaSegnali.at(i);
        pSegnale->append(ui->TabStep->currentIndex().row());
    }

    CompStepTempoCount++;
    CompStepAggiorna();
    ProcessoApri();
    this->setCursor(QCursor(Qt::ArrowCursor));

}

void QProcesso::CompStepDelTempo(){

    if(DbProcesso==NULL)
        return;

    QString strTab;
    QSqlQuery sql(*DbProcesso);
    int i;

    if(CompStepTempoCount<2)
        return;

    this->setCursor(QCursor(Qt::WaitCursor));

    strTab="CREATE TABLE `TMP` (`ID` int(11)  NOT NULL AUTO_INCREMENT ,`ID_TIPO_COMP` int(11),";

    for(i=0;i<CompStepTempoCount-1;i++){
        strTab+="`DA" + QString::number(i+1) + "` int(11)  ,";
        strTab+="`A" + QString::number(i+1) + "` int(11)  ,";
        strTab+="`TR" + QString::number(i+1) + "` int(11)  ,";
        if(i==CompStepTempoCount-2){
          strTab+="`INFO" + QString::number(i+1) + "` char(4)  , ";
          strTab+="PRIMARY KEY  (`ID`)  )";
        }
        else
          strTab+="`INFO" + QString::number(i+1) + "` char(4)  ,";
    }
    sql.exec(strTab);

    strTab="INSERT INTO TMP (ID_TIPO_COMP ,";
    for(i=0;i<CompStepTempoCount-1;i++){
        strTab+="DA" + QString::number(i+1) + " ,";
        strTab+="A" + QString::number(i+1) + "   ,";
        strTab+="TR" + QString::number(i+1) +  " ,";
        if(i==CompStepTempoCount-2){
          strTab+="INFO" + QString::number(i+1) + "  )";
        }
        else
          strTab+="INFO" + QString::number(i+1) + "  ,";
    }

    strTab+=" SELECT ID_TIPO_COMP ,";
    for(i=0;i<CompStepTempoCount-1;i++){
        strTab+="DA" + QString::number(i+1) + " ,";
        strTab+="A" + QString::number(i+1) + "   ,";
        strTab+="TR" + QString::number(i+1) +  " ,";
        if(i==CompStepTempoCount-2)
          strTab+="INFO" + QString::number(i+1) + " ";
        else
          strTab+="INFO" + QString::number(i+1) + "  ,";
    }
    strTab+=" FROM " + CompStepNome ;
    qDebug() << strTab;
    sql.exec(strTab);
    sql.exec("DROP TABLE " + CompStepNome);
    sql.exec("ALTER TABLE TMP RENAME TO " + CompStepNome);

    CompStepTempoCount--;
    CompStepAggiorna();
    ProcessoApri();
    this->setCursor(QCursor(Qt::ArrowCursor));

}

void QProcesso::CompStepSalvaCome(){

    if(DbProcesso==NULL)
        return;

     bool ok;
     QString text = QInputDialog::getText(this, tr("Salva Con Nome"),
                                            tr("Nuovo Comp Step"), QLineEdit::Normal,
                                            CompStepNome, &ok);
     if (ok && !text.isEmpty()){
         if(text!=CompStepNome){
             QString strTab;
             QSqlQuery sql(*DbProcesso);
             int i;
             text=text.toUpper();

             this->setCursor(QCursor(Qt::WaitCursor));


             strTab="CREATE TABLE `" + text + "` (`ID` int(11)  NOT NULL AUTO_INCREMENT ,`ID_TIPO_COMP` int(11),";

             for(i=0;i<CompStepTempoCount;i++){
                 strTab+="`DA" + QString::number(i+1) + "` int(11)  ,";
                 strTab+="`A" + QString::number(i+1) + "` int(11)  ,";
                 strTab+="`TR" + QString::number(i+1) + "` int(11)  ,";
                 if(i==CompStepTempoCount-1){
                   strTab+="`INFO" + QString::number(i+1) + "` char(4)  , ";
                   strTab+="PRIMARY KEY  (`ID`)  )";
                 }
                 else
                   strTab+="`INFO" + QString::number(i+1) + "` char(4)  ,";
             }
             sql.exec(strTab);

             strTab="INSERT INTO " + text + " (ID_TIPO_COMP ,";
             for(i=0;i<CompStepTempoCount;i++){
                 strTab+="DA" + QString::number(i+1) + " ,";
                 strTab+="A" + QString::number(i+1) + "   ,";
                 strTab+="TR" + QString::number(i+1) +  " ,";
                 if(i==CompStepTempoCount-1)
                   strTab+="INFO" + QString::number(i+1) + "  )";
                 else
                   strTab+="INFO" + QString::number(i+1) + "  ,";
             }

             strTab+=" SELECT ID_TIPO_COMP ,";
             for(i=0;i<CompStepTempoCount;i++){
                 strTab+="DA" + QString::number(i+1) + " ,";
                 strTab+="A" + QString::number(i+1) + "   ,";
                 strTab+="TR" + QString::number(i+1) +  " ,";
                 if(i==CompStepTempoCount-1)
                   strTab+="INFO" + QString::number(i+1) + " ";
                 else
                   strTab+="INFO" + QString::number(i+1) + "  ,";
             }
             strTab+=" FROM " + CompStepNome ;
             sql.exec(strTab);
             sql.prepare("INSERT INTO COMP_STEP(COMP_STEP) "
                           "VALUES (:COMP_STEP)");
             sql.bindValue(0,text);
             sql.exec() ;
             StepAggiorna();
             this->setCursor(QCursor(Qt::ArrowCursor));

         }
     }

}

void QProcesso::StepRowChange(QModelIndex index){

    int row=index.row();
    StepRecord = StepMod->record(row);
    ui->label->setText(QString::number(row) +
                       " TIPO STEP: " + StepRecord.value("TIPO_STEP").toString() +
                       " COMP STEP: " + StepRecord.value("COMP_STEP").toString());

}

void QProcesso::CompStepRowChange(QModelIndex index){

    int row=index.row();
    CompStepRecord = CompStepMod->record(row);

}

void QProcesso::StepColumnChanged(QModelIndex rowindex, QModelIndex previous){

    int row=previous.row();
    QSqlRecord record= StepMod->record(previous.row());
    QString m_Campo =record.fieldName(previous.column());
    CIOSegnale *pSegnale;

    if(Processa->pData->ProcessoIsOn ){
       if(record==StepRecord)
           return;
       if(Processa->pData->StepIndex> row){
          // QMessageBox::information(this, tr("Informa"),
          //                          "Non puo essere Modificato " );
           record.setValue(m_Campo ,StepRecord.value(m_Campo).toInt());
           record.setGenerated(m_Campo ,true);
           StepMod->setRecord(row,record);
           IsModificato=true;
           return;

       }else if(Processa->pData->StepIndex== rowindex.row()){
          if(m_Campo=="TEMPO"){
             if(record.value(m_Campo).toInt()<Processa->StepTime){
                 record.setValue(m_Campo ,Processa->StepTimeStop);
                 record.setGenerated(m_Campo ,true);
                 StepMod->setRecord(row,record);
                 StepRecord=record;
                 IsModificato=true;
                 return;
             }
          }
       }
    }

    if(record.value("COMP_STEP").toString()=="NESSUNO")
        return;

    if(StepRecord.value(m_Campo).toString()==record.value(m_Campo).toString())
        return;

    StepRecord=record;

    if(rowindex.row()!=previous.row())
        return;

    if(m_Campo!="TEMPO" )
        return;



    int tabsteprow=ui->TabStep->currentIndex().row();
    int rampa,newtempo;

    newtempo=StepMod->record(row).value("TEMPO").toInt();
    if(CompStepTempoCount>0){
       if(newtempo<CompStepMod->record(0).value("TR" + QString::number(CompStepTempoCount-1)).toInt()){
          newtempo=CompStepMod->record(0).value("TR" + QString::number(CompStepTempoCount-1)).toInt()+10;
          QSqlRecord record=StepMod->record(row);
          record.setValue("TEMPO" ,newtempo);
          record.setGenerated("TEMPO" ,true);
          StepMod->setRecord(row,record);
          IsModificato=true;
       }
    }
    if(CompStepTempoCount==1){
        rampa=CompStepMod->record(0).value("TR" + QString::number(CompStepTempoCount)).toInt();
     }else{
        rampa=CompStepMod->record(0).value("TR" + QString::number(CompStepTempoCount)).toInt();
        rampa-=CompStepMod->record(0).value("TR" + QString::number(CompStepTempoCount-1)).toInt();
     }
     for(int j=0;j<CompStepMod->rowCount();j++){
         pSegnale=getSegnale(CompStepMod->record(j).value("TIPO_COMP").toString());
         pSegnale->reLoad(tabsteprow,CompStepTempoCount-1, rampa);
         QSqlRecord record=CompStepMod->record(j);
         record.setValue("TR" + QString::number(CompStepTempoCount),newtempo);
         record.setGenerated("TR" + QString::number(CompStepTempoCount),true);
         CompStepMod->setRecord(j,record);
         IsModificato=true;
      }
      update();




}

void QProcesso::CompStepColumnChanged(QModelIndex rowindex, QModelIndex previous){

    QSqlRecord record= CompStepMod->record(previous.row());
    QString m_Campo =record.fieldName(previous.column());
    CIOSegnale *pSegnale;

    if(CompStepRecord.value(m_Campo).toString()==record.value(m_Campo).toString())
        return;

    //CompStepRecord=record;

    if(rowindex.row()!=previous.row())
        return;

    if(m_Campo=="" ||  m_Campo.left(1)=="I")
        return;

    int tabsteprow=ui->TabStep->currentIndex().row();
    int compsteprow=rowindex.row();

    if(m_Campo=="TIPO_COMP"){

    }

    int index=m_Campo.right(1).toInt();
    short start,stop,info;
    int rampa,newtempo;
    if(m_Campo.left(2)=="TR" ){
        int OldTempo=CompStepRecord.value("TR" + QString::number(index)).toInt();
        newtempo=CompStepMod->record(compsteprow).value("TR" + QString::number(index)).toInt();
        int addTempo=newtempo-OldTempo;
        for(int y=index; y< CompStepTempoCount+1; y++){
            for(int j=0;j<CompStepMod->rowCount();j++){
                pSegnale=getSegnale(CompStepMod->record(j).value("TIPO_COMP").toString());
                int Nuovotempo;
                if(y==index){
                    Nuovotempo=newtempo;
                }else{
                    Nuovotempo=CompStepMod->record(j).value("TR" + QString::number(y)).toInt();
                    Nuovotempo+=addTempo;
                    qDebug() << j << "TR" + QString::number(y) << CompStepMod->record(j).value("TR" + QString::number(y)).toInt() << Nuovotempo;
                }
                if(pSegnale){
                    rampa=pSegnale->Rampa(tabsteprow,y-1);
                    rampa+=addTempo;
                    pSegnale->reLoad(tabsteprow,y-1, rampa);
                    QSqlRecord record=CompStepMod->record(j);
                    record.setValue("TR" + QString::number(y),Nuovotempo);
                    record.setGenerated("TR" + QString::number(y),true);
                    CompStepMod->setRecord(j,record);
                }

            }

        }
        CompStepRecord=record;
        newtempo=CompStepMod->record(compsteprow).value("TR" + QString::number(CompStepTempoCount)).toInt();
        record=StepMod->record(tabsteprow);
        record.setValue("TEMPO" ,newtempo);
        record.setGenerated("TEMPO" ,true);
        StepMod->setRecord(tabsteprow,record);
        update();

        return;

        if(index<CompStepTempoCount){
           if(newtempo>CompStepMod->record(compsteprow).value("TR" + QString::number(index+1)).toInt())
              newtempo=CompStepMod->record(compsteprow).value("TR" + QString::number(index+1)).toInt()-10;
           if(index>0){
              if(newtempo<CompStepMod->record(compsteprow).value("TR" + QString::number(index-1)).toInt())
                 newtempo=CompStepMod->record(compsteprow).value("TR" + QString::number(index-1)).toInt()+10;
           }
           QSqlRecord record=CompStepMod->record(compsteprow);
           record.setValue("TR" + QString::number(index),newtempo);
           record.setGenerated("TR" + QString::number(index),true);
           CompStepMod->setRecord(compsteprow,record);
        }
        if(index==1){
          rampa=CompStepMod->record(compsteprow).value("TR" + QString::number(index)).toInt();
        }else{
          rampa=CompStepMod->record(compsteprow).value("TR" + QString::number(index)).toInt();
          rampa-=CompStepMod->record(compsteprow).value("TR" + QString::number(index-1)).toInt();
        }
        for(int j=0;j<CompStepMod->rowCount();j++){
            pSegnale=getSegnale(CompStepMod->record(j).value("TIPO_COMP").toString());
            if(pSegnale){
                pSegnale->reLoad(tabsteprow,index-1, rampa);
                QSqlRecord record=CompStepMod->record(j);
                record.setValue("TR" + QString::number(index),newtempo);
                record.setGenerated("TR" + QString::number(index),true);
                CompStepMod->setRecord(j,record);
            }

        }
        newtempo=CompStepMod->record(compsteprow).value("TR" + QString::number(CompStepTempoCount)).toInt();
        QSqlRecord record=StepMod->record(tabsteprow);
        record.setValue("TEMPO" ,newtempo);
        record.setGenerated("TEMPO" ,true);
        StepMod->setRecord(tabsteprow,record);
        update();
        return;
    }

    info=CompStepMod->record(compsteprow).value("INFO" + QString::number(index) ).toInt();
    start=CompStepMod->record(compsteprow).value("DA" + QString::number(index)).toInt();
    stop=CompStepMod->record(compsteprow).value("A" + QString::number(index) ).toInt();
    if(index==1){
      rampa=CompStepMod->record(compsteprow).value("TR" + QString::number(index)).toInt();
    }else{
      rampa=CompStepMod->record(compsteprow).value("TR" + QString::number(index)).toInt();
      rampa-=CompStepMod->record(compsteprow).value("TR" + QString::number(index-1)).toInt();
    }
    pSegnale=getSegnale(CompStepMod->record(compsteprow).value("TIPO_COMP").toString());
    LoadSegnale(pSegnale,false,info,tabsteprow,index-1, start, stop, rampa);

    update();

}

bool QProcesso::LoadSegnale(CIOSegnale *pSegnale, bool load,int info, int ProStep,int Step, int start, int stop,int rampa){

    if(pSegnale==NULL)
        return true;

    if((pSegnale->getNoZeroStepCount()>D_COMP_STEP*D_COMP_STEP) | (ProStep>D_STEP)){
        return false;
    }

    QString TipoComp=QString::number( pSegnale->TipoComp);
    if(TipoComp.left(1).toInt()==CTipoComp::Gas){
        if(info){
          double Pressione=info;
          Pressione/=10000;
          if(Processa->Vuoto[IdMacchina].Baratron.Segnale->Fattore())
             Pressione/=Processa->Vuoto[IdMacchina].Baratron.Segnale->Fattore();
          info=Pressione;
        }
    }else if(TipoComp.left(1).toInt()==CTipoComp::Sorgente){
       BYTE newinfo=0;
       if(info==1)
           newinfo=SetBit(newinfo, 0, true);
       if(info==2)
           newinfo=SetBit(newinfo, 1, true);
       if(info==3){
           newinfo=SetBit(newinfo, 0, true);
           newinfo=SetBit(newinfo, 1, true);
       }
       info=newinfo;
    }else if(TipoComp.left(1).toInt()==CTipoComp::Bias){

    }

    double fattore=pSegnale->Fattore();
    if(TipoComp.left(1).toInt()==CTipoComp::Bias){
        if(info==0){
           fattore*=pSegnale->SecondoFattore();
        }
    }

    double dStart=start;
    if(fattore)
      dStart/=fattore;
    start=dStart;

    double dStop=stop;
    if(fattore)
      dStop/=fattore;
    stop=dStop;

    if(load)
      pSegnale->Load(info,ProStep,Step, start, stop, rampa);
    else
      pSegnale->reLoad(info,ProStep,Step, start, stop, rampa);

    return true;

}




QString QProcesso::getTipoComp(QString str){

    for(int i=0;i< CompMod->rowCount();i++){
        if(str==CompMod->record(i).value("TIPO_COMP").toString()){
            return CompMod->record(i).value("ID_TIPO_COMP").toString();
        }
    }

    return "";
}

CIOSegnale *QProcesso::getSegnale(QString str){

    CIOSegnale *pSegnale;
    QString TipoComp;

    for(int i=0;i< CompMod->rowCount();i++){
        if(str==CompMod->record(i).value("TIPO_COMP").toString()){
           TipoComp=CompMod->record(i).value("ID_TIPO_COMP").toString();
           break;
        }
    }

    for(int i=0;i< pListaSegnali.count();i++){
         pSegnale=pListaSegnali.at(i);
         if(QString::number( pSegnale->TipoComp)==TipoComp){
             return pSegnale;
          }
     }

    return NULL;

}




void QProcesso::ProcessoNuovo()
{

    if(DbProcesso==NULL)
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("Nuovo Processo"),
                                           tr("Nuovo Processo"), QLineEdit::Normal,
                                           ui->TabProcesso->currentText(), &ok);
    if (ok && !text.isEmpty()){
        this->setCursor(QCursor(Qt::WaitCursor));
        if(text==ui->TabProcesso->currentText())
            return;
        text=text.toUpper();
        QSqlQuery sql(*DbProcesso);
        sql.prepare("INSERT INTO PROCESSO(PROCESSO) "
                      "VALUES (:PROCESSO)");
        sql.bindValue(0,text);
        sql.exec() ;

        sql.exec("SELECT MAX(ID_PROCESSO) AS ID FROM PROCESSO");
        sql.next();
        QString id=sql.record().value("ID").toString();

        sql.prepare("INSERT INTO STEP(ID_PROCESSO,ID_TIPO_STEP,POS,TEMPO,REPIT,PRESSIONE,POTENZA,TEMP) "
                      "VALUES (:ID_PROCESSO,:ID_TIPO_STEP,:POS,:TEMPO,:REPIT,:PRESSIONE,:POTENZA,:TEMP)");
        sql.bindValue(0,id);
        sql.bindValue(1,1);
        sql.bindValue(2,1);
        sql.bindValue(3,60);
        sql.bindValue(4,1);
        sql.bindValue(5,0.03);
        sql.bindValue(6,0);
        sql.bindValue(7,0);
        sql.exec() ;
        sql.prepare("INSERT INTO STEP(ID_PROCESSO,ID_TIPO_STEP,POS,TEMPO,REPIT,PRESSIONE,POTENZA,TEMP) "
                      "VALUES (:ID_PROCESSO,:ID_TIPO_STEP,:POS,:TEMPO,:REPIT,:PRESSIONE,:POTENZA,:TEMP)");
        sql.bindValue(0,id);
        sql.bindValue(1,2);
        sql.bindValue(2,2);
        sql.bindValue(3,60);
        sql.bindValue(4,1);
        sql.bindValue(5,0.03);
        sql.bindValue(6,0);
        sql.bindValue(7,0);
        sql.exec() ;
        sql.prepare("INSERT INTO STEP(ID_PROCESSO,ID_TIPO_STEP,POS,TEMPO,REPIT,PRESSIONE,POTENZA,TEMP) "
                      "VALUES (:ID_PROCESSO,:ID_TIPO_STEP,:POS,:TEMPO,:REPIT,:PRESSIONE,:POTENZA,:TEMP)");
        sql.bindValue(0,id);
        sql.bindValue(1,3);
        sql.bindValue(2,3);
        sql.bindValue(3,60);
        sql.bindValue(4,1);
        sql.bindValue(5,0.03);
        sql.bindValue(6,0);
        sql.bindValue(7,0);
        sql.exec() ;
        sql.prepare("INSERT INTO STEP(ID_PROCESSO,ID_TIPO_STEP,POS,TEMPO,REPIT,PRESSIONE,POTENZA,TEMP) "
                      "VALUES (:ID_PROCESSO,:ID_TIPO_STEP,:POS,:TEMPO,:REPIT,:PRESSIONE,:POTENZA,:TEMP)");
        sql.bindValue(0,id);
        sql.bindValue(1,4);
        sql.bindValue(2,4);
        sql.bindValue(3,60);
        sql.bindValue(4,1);
        sql.bindValue(5,0.03);
        sql.bindValue(6,0);
        sql.bindValue(7,0);
        sql.exec() ;
        ProcessoMod->setQuery("SELECT PROCESSO, ID_PROCESSO FROM PROCESSO ",*DbProcesso);
        ui->TabProcesso->setModel(ProcessoMod);

        ui->TabProcesso->setCurrentText(text);

        this->setCursor(QCursor(Qt::ArrowCursor));

    }

}

void QProcesso::ProcessoDuplica()
{
    if(DbProcesso==NULL)
        return;

    bool ok;
    QString text = QInputDialog::getText(this, tr("Salva Con Nome Processo"),
                                           tr("Salva Con Nome Processo"), QLineEdit::Normal,
                                           ui->TabProcesso->currentText(), &ok);
    if (ok && !text.isEmpty()){
        if(text==ui->TabProcesso->currentText())
            return;
        QString lastid=ProcessoMod->record(ui->TabProcesso->currentIndex()).value("ID_PROCESSO").toString();

        this->setCursor(QCursor(Qt::WaitCursor));
        text=text.toUpper();

        QSqlQuery sql(*DbProcesso);
        sql.prepare("INSERT INTO PROCESSO(PROCESSO) "
                      "VALUES (:PROCESSO)");
        sql.bindValue(0,text);
        sql.exec() ;

        sql.exec("SELECT MAX(ID_PROCESSO) AS ID FROM PROCESSO");
        sql.next();

        QString id=sql.record().value("ID").toString();

        sql.exec("CREATE TEMPORARY TABLE TMP AS SELECT * FROM STEP WHERE ID_PROCESSO ='" + lastid + "'");
        sql.exec("UPDATE TMP SET ID_STEP = NULL , ID_PROCESSO ='" + id + "'");
        sql.exec("INSERT INTO STEP SELECT * FROM TMP");
        sql.exec("DROP TABLE TMP");

        ProcessoMod->setQuery("SELECT PROCESSO, ID_PROCESSO FROM PROCESSO ",*DbProcesso);
        ui->TabProcesso->setModel(ProcessoMod);

        ui->TabProcesso->setCurrentText(text);

        this->setCursor(QCursor(Qt::ArrowCursor));

    }

}

void QProcesso::ProcessoElimina()
{
    if(DbProcesso==NULL)
        return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "?", "Elimina " + ui->TabProcesso->currentText(),
                                   QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No)
       return;

    this->setCursor(QCursor(Qt::WaitCursor));

    QString id=ProcessoMod->record(ui->TabProcesso->currentIndex()).value("ID_PROCESSO").toString();

    QSqlQuery sql(*DbProcesso);
    sql.exec("DELETE FROM STEP WHERE ID_PROCESSO= " + id);
    sql.exec("DELETE FROM PROCESSO WHERE ID_PROCESSO= " + id);

    ProcessoMod->setQuery("SELECT PROCESSO, ID_PROCESSO FROM PROCESSO ",*DbProcesso);
    ui->TabProcesso->setModel(ProcessoMod);

    ui->TabProcesso->setCurrentIndex(0);

    this->setCursor(QCursor(Qt::ArrowCursor));
}

void QProcesso::ProcessoTmp()
{

}

void QProcesso::ProcessoCaricaSuPlc()
{

    if(DbProcesso==NULL)
        return;

    ProcessoApri();
    for(int i=0; i<StepMod->rowCount();i++){
       QSqlRecord record=StepMod->record(i);
       int pos=record.value("POS").toInt();
       if(pos!=i+1){
          QMessageBox::critical(this, tr("ERRORE"),"CONTROLLA CAMPO POS");
          return;
       }
    }

    QSqlQuery query(*DbProcesso);
    QString str="SELECT * FROM STEP INNER JOIN COMP_STEP ON (COMP_STEP.ID_COMP_STEP=STEP.ID_COMP_STEP) ";
    str +=" INNER JOIN TIPO_STEP ON (TIPO_STEP.ID_TIPO_STEP=STEP.ID_TIPO_STEP) ";
    str +=" INNER JOIN PROCESSO ON (PROCESSO.ID_PROCESSO=STEP.ID_PROCESSO) ";
    str+=" WHERE STEP.ID_PROCESSO = " + QString::number(ProcessoId);
    str+=" ORDER BY POS ";
    query.exec(str);

    QString Processo;
    QString coating;
    str="";
    while (query.next()) {
        QSqlRecord record=query.record();
        Processo=record.value("PROCESSO").toString();
        coating=record.value("ID_COATING").toString();
        str+=QString::number(record.value("POS").toInt()) + "\t"
             + record.value("TIPO_STEP").toString() + "\t"
             + QDateTime::fromTime_t(record.value("TEMPO").toInt()).toUTC().toString("hh:mm:ss") + "\t"
             +  record.value("COMP_STEP").toString()  + "\n";
    }
    int questione=QMessageBox::question(this,"Spedisco " + Processo,str);
    if(questione==QMessageBox::No)
        return;

    Processa->pData->ComStepLoad=0;
    Processa->AddMsg(CTipoMessaggio::CaricaProcesso,IdMacchina,CTipoComando::CaricaProcesso,ProcessoId);

}


void QProcesso::on_Macchina_activated(int index)
{

    if(index!= IdMacchina){
        IdMacchina=index;
        ui->TabStep->setModel(NULL);
        ui->TabCompStep->setModel(NULL);
        getProcessi();
    }
}


