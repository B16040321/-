#include "user.h"
#include "ui_user.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


user::user(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::user)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("user_table");
    model->select();

    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    // 设置主界面
    setTableViewTitle();
    setWindowTitle(tr("宠物店管理系统"));
    //text隐式消息提示
    ui->idSearchLineEdit->setPlaceholderText("请输入客户编号");
    ui->nameSearchLineEdit->setPlaceholderText("请输入客户姓名");
    ui->phoneSearchLineEdit->setPlaceholderText("请输入客户电话");
    ui->emailSearchLineEdit->setPlaceholderText("请输入客户邮箱");
    // 设置主界面按钮可用性,客户四种可查询的按钮
    ui->searchByIdBtn->setEnabled(false);
    ui->searchByNameBtn->setEnabled(false);
    ui->searchByphoneBtn->setEnabled(false);
    ui->searchByemailBtn->setEnabled(false);
}

user::~user()
{
    delete ui;
}

//该界面关闭时，上一级界面出现
void user::closeEvent(QCloseEvent *){
    emit signal_ret();
    this->close();
}
//设置表名
void user::setTableViewTitle()
{   //客户编号，客户姓名，电话，邮箱,新增表的属性，可在这新增一行以输出
    model->setHeaderData(0, Qt::Horizontal, tr("客户编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("客户姓名"));
    model->setHeaderData(2, Qt::Horizontal, tr("电话"));
    model->setHeaderData(3, Qt::Horizontal, tr("邮箱"));
}

//添加记录
void user::on_addBtn_clicked()
{
    // 获得表的行数
    int rowNum = model->rowCount();
    // 添加一行
    model->insertRow(rowNum);
    model->setData(model->index(rowNum, 0), rowNum);
    //reload_table();
}
//提交修改
void user::on_modifyBtn_clicked()
{
    // 开始事务操作
    model->database().transaction();
    if (model->submitAll())
    {
        // 提交
        model->database().commit();
        QMessageBox::information(this, tr("提示"), tr("修改成功！"), QMessageBox::Ok);

    }
    else
    {
        // 回滚
        model->database().rollback();
        QMessageBox::information(this, tr("提示"), tr("修改失败，修改的内容不匹配数据类型！！"), QMessageBox::Ok);
    }

    qDebug() << "counted";
    reload_table();
}

//未提交修改时，撤销修改过的内容
void user::on_undoBtn_clicked()
{
   model->revertAll();
   QMessageBox::information(this, tr("提示"), tr("未提交的修改内容已复原！"), QMessageBox::Ok);
   reload_table();
}

//删除选中行”
void user::on_deleteBtn_clicked()
{
    // 获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    // 删除该行
    model->removeRow(curRow);
    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                    tr("你确定删除当前行吗？"), QMessageBox::Yes, QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        // 如果不删除，则撤销
        model->revertAll();
    }
    else
    {
        // 否则提交，在数据库中删除该行
        if(model->submitAll())
        {
            // 提交
            model->database().commit();
            QMessageBox::information(this, tr("提示"), tr("删除成功！"), QMessageBox::Ok);
        }
        else
        {
            // 回滚
            model->database().rollback();
            QMessageBox::information(this, tr("提示"), tr("删除失败，访问数据库出错！"), QMessageBox::Ok);
        }
    }
    reload_table();
}
//显示全表，可选
void user::reload_table()
{
    model->setTable("user_table");
    model->select();
    setTableViewTitle();
}
//按编号查询
void user::on_searchByIdBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString id = ui->idSearchLineEdit->text().trimmed();
    model->setFilter(QString("userid = '%1'").arg(id));
    model->select();
}
//姓名查询
void user::on_searchByNameBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString name = ui->nameSearchLineEdit->text().trimmed();
    model->setFilter(QString("username = '%1'").arg(name));
    model->select();
}
//电话查询
void user::on_searchByphoneBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString phone = ui->phoneSearchLineEdit->text().trimmed();
    model->setFilter(QString("phone = '%1'").arg(phone));
    model->select();
}
//邮箱查询
void user::on_searchByemailBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString email = ui->emailSearchLineEdit->text().trimmed();
    model->setFilter(QString("email = '%1'").arg(email));
    model->select();
}

//查询并显示
void user::on_idSearchLineEdit_returnPressed()
{
    user::on_searchByIdBtn_clicked();
}

//查询并显示
void user::on_nameSearchLineEdit_returnPressed()
{
    user::on_searchByNameBtn_clicked();
}

//查询并显示
void user::on_phoneSearchLineEdit_returnPressed()
{
    user::on_searchByphoneBtn_clicked();
}
//查询并显示
void user::on_emailSearchLineEdit_returnPressed()
{
    user::on_searchByemailBtn_clicked();
}

//没有输入时，无法点击查询按钮
void user::on_idSearchLineEdit_textChanged(const QString &str)
{

    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByphoneBtn->setEnabled(false);
        ui->searchByemailBtn->setEnabled(false);
    }
    else
    {
        ui->searchByIdBtn->setEnabled(true);
    }
}

//没有输入时，无法点击查询按钮
void user::on_nameSearchLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByphoneBtn->setEnabled(false);
        ui->searchByemailBtn->setEnabled(false);
    }
    else
    {
        ui->searchByNameBtn->setEnabled(true);
    }
}
//没有输入时，无法点击查询按钮
void user::on_phoneSearchLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByphoneBtn->setEnabled(false);
        ui->searchByemailBtn->setEnabled(false);
    }
    else
    {
        ui->searchByphoneBtn->setEnabled(true);
    }
}

//功能：没有输入时，无法点击查询按钮
void user::on_emailSearchLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByphoneBtn->setEnabled(false);
        ui->searchByemailBtn->setEnabled(false);
    }
    else
    {
        ui->searchByemailBtn->setEnabled(true);
    }
}
