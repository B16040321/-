#include "logindialog.h"
#include "ui_logindialog.h"
#include "menu_admin.h"
#include "menu_em.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>
#include <QString>
#include <string>
#include <iostream>
#include <QtDebug>

//“登录界面” 类——构造函数
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 设置登录界面标题
    setWindowTitle(tr("欢迎进入登录界面"));

    // 设置登录界面按钮图标大小
    ui->loginBtn->setIconSize(QSize(40, 40));
    ui->quitBtn->setIconSize(QSize(40, 40));

    ui->userlineEdit->setPlaceholderText("请输入员工编号");

    // 设置登录界面显示时间
    ui->timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd"));

    // 设置登录界面按钮可用性
    ui->pwdLineEdit->setFocus();
    ui->loginBtn->setEnabled(false);

    ui->loginBtn->setDefault(true);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//“密码输入框” 文本编辑——槽函数
void LoginDialog::on_pwdLineEdit_returnPressed()
{
    if (ui->pwdLineEdit->text().isEmpty())
    {
        QMessageBox::information(this, tr("请输入密码"),
                                    tr("请先输入密码再登录！"), QMessageBox::Ok);
        ui->pwdLineEdit->setFocus();
    }
    else
    {
        LoginDialog::on_loginBtn_clicked();
    }
}

//没有输入密码时，无法点击登录按钮
void LoginDialog::on_pwdLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->loginBtn->setEnabled(false);
    }
    else
    {
        ui->loginBtn->setEnabled(true);
    }
}

//登录
void LoginDialog::on_loginBtn_clicked()
{
    // 输入密码后，从数据库中获取密码
    QSqlQuery query;
    int id = (ui->userlineEdit->text()).toInt();//员工编码
    query.exec(QString("select password,permission,empolyeename from employee_table where employeeid='%1'").arg(id));
    if(query.next()){
        // 如果输入的密码与库中密码相同，判断是否为管理员，进入menu_admin或者menu_employee
        if (query.value(0).toString() == ui->pwdLineEdit->text())
        {
            if(query.value(1) == 0){//权限,管理员
                qDebug() << "~~~~~~~~~here~~~~~~~~~~\n";
                this->hide();
                m_a = new menu_admin;
                connect(m_a, SIGNAL(signal_ret()), this, SLOT(reshow()));
                QString user_name = query.value(2).toString();
                connect(this, SIGNAL(send_name(QString)), m_a, SLOT(get_name(QString))); //传递用户名给子窗口
                emit send_name(user_name);
                m_a->show();
            }
            else{ //普通员工
                qDebug() << "~~~~~~~~~there~~~~~~~~~~\n";
                this->hide();
                m_e = new menu_em;
                QString user_name = query.value(2).toString();
                connect(m_e, SIGNAL(signal_ret()), this, SLOT(reshow()));
                connect(this, SIGNAL(send_name(QString)), m_e, SLOT(get_name(QString))); //传递用户名给子窗口
                emit send_name(user_name);
                m_e->show();
            }
        }
        // 登录失败后，清空内容并定位光标到用户名输入框中
        else
        {
            QMessageBox::warning(this, tr("密码错误"),
                                    tr("请输入正确的密码再登录！"), QMessageBox::Ok);
            ui->pwdLineEdit->clear();
            ui->pwdLineEdit->setFocus();
        }
    }
    else{
        QMessageBox::warning(this, tr("没有这个员工"),
                                tr("请输入正确的员工编码再登录！"), QMessageBox::Ok);
        ui->pwdLineEdit->clear();
        ui->pwdLineEdit->setFocus();

    }
}

//退出
void LoginDialog::on_quitBtn_clicked()
{
    QDialog::reject();
}

//显示密码按钮
void LoginDialog::on_showPwdBtn_clicked()
{
    if(ui->showPwdBtn->isChecked())
    {
        ui->pwdLineEdit->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    }

}

void LoginDialog::reshow() {
    clearUI();
    this->show();
}

void LoginDialog::clearUI() {
    ui->pwdLineEdit->clear();
    ui->userlineEdit->clear();
}
