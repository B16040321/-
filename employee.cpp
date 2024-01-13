#include "employee.h"
#include "ui_employee.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

//构造函数
employee::employee(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::employee)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("employee_table");
    model->select();

    // 设置编辑策略,必须点提交修改才能提交到数据库
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    // 设置主界面
    setTableViewTitle();
    setWindowTitle(tr("宠物店管理系统"));

    //text隐式消息提示
    ui->idSearchLineEdit->setPlaceholderText("请输入员工编号");
    ui->nameSearchLineEdit->setPlaceholderText("请输入员工姓名");
    ui->perSearchLineEdit->setPlaceholderText("请输入员工权限(0 or 1");
    // 设置主界面按钮可用性
    ui->searchByIdBtn->setEnabled(false);
    ui->searchByNameBtn->setEnabled(false);
    ui->searchByperBtn->setEnabled(false);

}

employee::~employee()
{
    delete ui;
}

//该界面关闭时，上一级界面出现
void employee::closeEvent(QCloseEvent *){
    emit signal_ret();
    this->close();
}

//设置表名
void employee::setTableViewTitle()
{
    model->setHeaderData(0, Qt::Horizontal, tr("员工编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("员工姓名"));
    model->setHeaderData(2, Qt::Horizontal, tr("员工密码"));
    model->setHeaderData(3, Qt::Horizontal, tr("员工权限(0为管理员)"));
}

//添加记录
void employee::on_addBtn_clicked()
{
    // 获得表的行数
    int rowNum = model->rowCount();
    // 添加一行
    model->insertRow(rowNum);
    model->setData(model->index(rowNum, 0), rowNum);
    //reload_table();
}
//提交修改
void employee::on_modifyBtn_clicked()
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
        QMessageBox::information(this, tr("提示"), tr("修改失败，修改的内容不匹配数据类型！"), QMessageBox::Ok);
    }

    reload_table();
}

//未提交修改时，撤销修改过的内容
void employee::on_undoBtn_clicked()
{
   model->revertAll();
   QMessageBox::information(this, tr("提示"), tr("未提交的修改内容已复原！"), QMessageBox::Ok);
   reload_table();
}

//删除选中行”
void employee::on_deleteBtn_clicked()
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
            QMessageBox::information(this, tr("提示"), tr("删除失败，无法访问数据库！"), QMessageBox::Ok);
        }
    }
    reload_table();
}
//显示全表
void employee::reload_table()
{
    model->setTable("employee_table");
    model->select();
    setTableViewTitle();
}
//按编号查询
void employee::on_searchByIdBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString id = ui->idSearchLineEdit->text().trimmed();
    int idd = id.toInt();
    model->setFilter(QString("employeeid = '%1'").arg(idd));
    model->select();
}
//姓名查询
void employee::on_searchByNameBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString name = ui->nameSearchLineEdit->text().trimmed();
    model->setFilter(QString("employeename = '%1'").arg(name));
    model->select();
}

//权限查询
void employee::on_searchByperBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString per = ui->perSearchLineEdit->text().trimmed();
    int permission = per.toInt();
    qDebug()<<permission<<"~~~~~~\n";
    model->setFilter(QString("permission  = '%1'").arg(permission));
    model->select();
}

//按学号查询并显示
void employee::on_idSearchLineEdit_returnPressed()
{
    employee::on_searchByIdBtn_clicked();
}

//按姓名查询并显示
void employee::on_nameSearchLineEdit_returnPressed()
{
    employee::on_searchByNameBtn_clicked();
}

//按permission查询并显示
void employee::on_perSearchLineEdit_returnPressed()
{
    employee::on_searchByperBtn_clicked();
}

//没有输入学号时，无法点击按学号查询按钮
void employee::on_idSearchLineEdit_textChanged(const QString &str)
{

    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByperBtn->setEnabled(false);
    }
    else
    {
        ui->searchByIdBtn->setEnabled(true);
    }
}

//功能：没有输入姓名时，无法点击按姓名查询按钮
void employee::on_nameSearchLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByperBtn->setEnabled(false);
    }
    else
    {
        ui->searchByNameBtn->setEnabled(true);
    }
}
//功能：没有输入permission时，无法点击按姓名查询按钮
void employee::on_perSearchLineEdit_textChanged(const QString &str)
{
    if (str == "")
    {
        ui->searchByIdBtn->setEnabled(false);
        ui->searchByNameBtn->setEnabled(false);
        ui->searchByperBtn->setEnabled(false);
    }
    else
    {
        ui->searchByperBtn->setEnabled(true);
    }
}
