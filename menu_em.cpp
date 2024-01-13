#include "menu_em.h"
#include "ui_menu_em.h"
#include "mainwindow.h"
menu_em::menu_em(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu_em)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,1); //窗口设置为关闭时,会回收资源
    connect(ui->btn_goods, SIGNAL(clicked(bool)), this, SLOT(into_goods()));
    connect(ui->btn_order, SIGNAL(clicked(bool)), this, SLOT(into_order()));
    connect(ui->btn_user, SIGNAL(clicked(bool)), this, SLOT(into_user()));
    connect(ui->btn_return_2, SIGNAL(clicked(bool)), this, SLOT(slot_ret()));
}

menu_em::~menu_em()
{
    delete ui;
}

void menu_em::slot_ret() {
    emit signal_ret();
    this->close();
}

void menu_em::get_name(QString str) {
    user_name = str;
    QString string = tr("欢迎") +user_name + tr("进入宠物店管理系统");
    ui->lb_welcome->setText(string);
}

void menu_em::into_goods() {  //跳转至goods
    this->hide();
    goods *g = new goods;
    connect(g, SIGNAL(signal_ret()), this, SLOT(reshow()));
    g->show();
}

void menu_em::into_order() { //跳转至order
    this->hide();
    order *o = new order;
    connect(o, SIGNAL(signal_ret()), this, SLOT(reshow()));
    o->show();
}

void menu_em::into_user() {  //跳转至user
    this->hide();
    user *u = new user;
    connect(u, SIGNAL(signal_ret()), this, SLOT(reshow()));
    u->show();
}

void menu_em::reshow() {
    this->show();
}
