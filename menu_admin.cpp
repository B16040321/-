#include "menu_admin.h"
#include "ui_menu_admin.h"
#include <QDebug>
menu_admin::menu_admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu_admin)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose,1); //窗口设置为关闭时,会回收资源

    connect(ui->btn_employee, SIGNAL(clicked(bool)), this, SLOT(into_employee()));
    connect(ui->btn_goods, SIGNAL(clicked(bool)), this, SLOT(into_goods()));
    connect(ui->btn_order, SIGNAL(clicked(bool)), this, SLOT(into_order()));
    connect(ui->btn_user, SIGNAL(clicked(bool)), this, SLOT(into_user()));
    connect(ui->btn_return_2, SIGNAL(clicked(bool)), this, SLOT(slot_ret()));
}

menu_admin::~menu_admin()
{
    delete ui;
}

void menu_admin::slot_ret() {
    emit signal_ret();
    this->close();
}

void menu_admin::get_name(QString str) {
    user_name = str;
    QString string = tr("欢迎") +user_name + tr("进入宠物店管理系统");
    ui->lb_welcome_2->setText(string);
}

void menu_admin::into_employee() {    //跳转至employee界面
    this->hide();
    employee *em = new employee;
    connect(em, SIGNAL(signal_ret()), this, SLOT(reshow()));
    em->show();
}

void menu_admin::into_goods() {  //跳转至goods
    this->hide();
    goods *g = new goods;
    connect(g, SIGNAL(signal_ret()), this, SLOT(reshow()));
    g->show();
}

void menu_admin::into_order() { //跳转至order
    this->hide();
    order *o = new order;
    connect(o, SIGNAL(signal_ret()), this, SLOT(reshow()));
    o->show();
}

void menu_admin::into_user() {  //跳转至user
    this->hide();
    user *u = new user;
    connect(u, SIGNAL(signal_ret()), this, SLOT(reshow()));
    u->show();
}

void menu_admin::reshow() {

    this->show();
}


