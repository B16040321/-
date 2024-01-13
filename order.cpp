#include "order.h"
#include "ui_order.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>


order::order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::order)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model->setTable("order_table");
    model->select();

    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    // 设置主界面
    setTableViewTitle();
    setWindowTitle(tr("宠物店管理系统"));
    //text隐式消息提示
    ui->idSearchLineEdit->setPlaceholderText("请输入订单编号");
    ui->nameSearchLineEdit->setPlaceholderText("请输入宠物品种");
    ui->perSearchLineEdit->setPlaceholderText("请输入客户编号");
    // 设置主界面按钮可用性，查询按钮、输入文本控件的名字没改，跟employee.ui控件名字一致
    ui->searchByIdBtn->setEnabled(false);  //订单编号
    ui->searchByNameBtn->setEnabled(false);  //宠物品种
    ui->searchByperBtn->setEnabled(false);   //客户编号
}

order::~order()
{
    delete ui;
}

//该界面关闭时，上一级界面出现
void order::closeEvent(QCloseEvent *){
    emit signal_ret();
    this->close();
}

//设置表名
void order::setTableViewTitle()
{
    //订单编号，客户编号，商品编号，数量，品种，订单总额
    model->setHeaderData(0, Qt::Horizontal, tr("订单编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("客户编号"));
    model->setHeaderData(2, Qt::Horizontal, tr("商品编号"));
    model->setHeaderData(3, Qt::Horizontal, tr("品种"));
    model->setHeaderData(4, Qt::Horizontal, tr("数量"));
    model->setHeaderData(5, Qt::Horizontal, tr("订单总额"));
}

//添加记录
void order::on_addBtn_clicked()
{
    // 获得表的行数
    int rowNum = model->rowCount();
    // 添加一行
    model->insertRow(rowNum);
    model->setData(model->index(rowNum, 0), rowNum);
    //reload_table();
}
//提交修改
void order::on_modifyBtn_clicked()
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
void order::on_undoBtn_clicked()
{
   model->revertAll();
   QMessageBox::information(this, tr("提示"), tr("未提交的修改内容已复原！"), QMessageBox::Ok);
   reload_table();
}

//删除选中行”
void order::on_deleteBtn_clicked()
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
//显示全表
void order::reload_table()
{
    model->setTable("order_table");
    model->select();
    setTableViewTitle();
}
//按订单编号查询
void order::on_searchByIdBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString id = ui->idSearchLineEdit->text().trimmed();
    model->setFilter(QString("orderid = '%1'").arg(id));
    model->select();
}
//客户编号查询
void order::on_searchByNameBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString name = ui->nameSearchLineEdit->text().trimmed();
    model->setFilter(QString("userid = '%1'").arg(name));
    model->select();
}

//品种查询
void order::on_searchByperBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString per = ui->perSearchLineEdit->text().trimmed();
    model->setFilter(QString(" goodstype = '%1'").arg(per));
    model->select();
}

//按订单编号查询并显示
void order::on_idSearchLineEdit_returnPressed()
{
    order::on_searchByIdBtn_clicked();
}

//按品种查询并显示
void order::on_nameSearchLineEdit_returnPressed()
{
    order::on_searchByperBtn_clicked();
}

//按编号查询并显示
void order::on_perSearchLineEdit_returnPressed()
{
    order::on_searchByNameBtn_clicked();
}

//没有输入时，无法点击查询按钮
void order::on_idSearchLineEdit_textChanged(const QString &str)
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

//没有品种输入时，无法点击查询按钮
void order::on_nameSearchLineEdit_textChanged(const QString &str)
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
//没有编号输入时，无法点击查询按钮
void order::on_perSearchLineEdit_textChanged(const QString &str)
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
