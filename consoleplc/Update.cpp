#include "Update.h"
#include "DataFile.h"
#include <QSqlError>



QUpdate::QUpdate(QSqlDatabase *newDataBase,QString strSql)
{
    DataBase=newDataBase;
    strUpdate=strSql;
}



void QUpdate::run(){

    if(DataBase==NULL){
        MostraErrore("DatataBase == NULL");
        emit isUpdate(false);
        return;
    }

    QSqlQuery query(*DataBase);
    if (query.exec(strUpdate)){
        emit isUpdate(true);
    }else{
        std::string str;
        str.append(strUpdate.toUtf8());
        MostraErrore(str);
        str.clear();
        str.append(query.lastError().text().toUtf8());
        MostraErrore(str);
        emit isUpdate(false);
    }

    return;
}


