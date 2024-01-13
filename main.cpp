#include "mainwindow.h"
#include <QSqlQuery>
#include <QSql>

#include <QtWidgets/QMessageBox>
#include "logindialog.h"
#include <QApplication>
#include <QProcess>
#include <iostream>
#include <QDebug>
#include <QSqlError>
#include <QTextCodec>
#include <string.h>

int main(int argc,char *argv[]) {

    QApplication q(argc, argv);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");  //连接本地主机
    db.setPort(3306);
    db.setDatabaseName("petshop_manager");
    db.setUserName("root");
    db.setPassword("123456");
    bool ok = db.open();
    QWidget *a = new QWidget;
    if (ok){
        QMessageBox::information(a, "infor", "link success");
    }
    else {
        QMessageBox::information(a, "infor", "link failed");
        qDebug()<<"error open database because"<<db.lastError().text();
    }
    a->show();
    LoginDialog dlg;
    dlg.show();

    return q.exec();

}
