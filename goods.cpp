#include "goods.h"
#include "ui_goods.h"
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

goods::goods(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::goods)
{
    ui->setupUi(this);

    model = new QSqlTableModel(this);
    model->setTable("goods_table");
    model->select();

    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);

    // 设置主界面
    setTableViewTitle();
    setWindowTitle(tr("宠物店管理系统"));
    //text隐式消息提示
    ui->idSearchLineEdit->setPlaceholderText("请输入商品编号");
    ui->nameSearchLineEdit->setPlaceholderText("请输入宠物品种");
    ui->perSearchLineEdit->setPlaceholderText("请输入商品单价");
    // 设置主界面按钮可用性
    ui->searchByIdBtn->setEnabled(false); //编号
    ui->searchByNameBtn->setEnabled(false); //品种
    ui->searchByperBtn->setEnabled(false); //单价
}

goods::~goods()
{
    delete ui;
}
//该界面关闭时，上一级界面出现
void goods::closeEvent(QCloseEvent *){
    emit signal_ret();
    this->close();
}

//设置表名
void goods::setTableViewTitle()
{
    model->setHeaderData(0, Qt::Horizontal, tr("商品编号"));
    model->setHeaderData(1, Qt::Horizontal, tr("宠物品种"));
    model->setHeaderData(2, Qt::Horizontal, tr("单价"));
    model->setHeaderData(3, Qt::Horizontal, tr("数量)"));
}

//添加记录
void goods::on_addBtn_clicked()
{
    // 获得表的行数
    int rowNum = model->rowCount();
    // 添加一行
    model->insertRow(rowNum);
    model->setData(model->index(rowNum, 0), rowNum);
    //reload_table();
}
//提交修改
void goods::on_modifyBtn_clicked()
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
void goods::on_undoBtn_clicked()
{
   model->revertAll();
   QMessageBox::information(this, tr("提示"), tr("未提交的修改内容已复原！"), QMessageBox::Ok);
   reload_table();
}

//删除选中行”
void goods::on_deleteBtn_clicked()
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
            QMessageBox::information(this, tr("提示"), tr("删除失败，请检查数据库是否存在问题！"), QMessageBox::Ok);
        }
    }
    reload_table();
}
//显示全表
void goods::reload_table()
{
    model->setTable("goods_table");
    model->select();
    setTableViewTitle();
}
//按编号查询
void goods::on_searchByIdBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString id = ui->idSearchLineEdit->text().trimmed();
    model->setFilter(QString("goodsid = '%1'").arg(id));
    model->select();
}
//品种查询
void goods::on_searchByNameBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString name = ui->nameSearchLineEdit->text().trimmed();
    model->setFilter(QString("goodstype = '%1'").arg(name));
    model->select();
}

//单价查询
void goods::on_searchByperBtn_clicked()
{
    // trimmed() 用于去除输入字符串前后的空白字符
    QString per = ui->perSearchLineEdit->text().trimmed();
    int value = per.toInt();
    model->setFilter(QString("goodsvalue = '%1'").arg(value));
    model->select();
}

//按编号查询并显示
void goods::on_idSearchLineEdit_returnPressed()
{
    goods::on_searchByIdBtn_clicked();
}

//按品种查询并显示
void goods::on_nameSearchLineEdit_returnPressed()
{
    goods::on_searchByNameBtn_clicked();
}

//按单价查询并显示
void goods::on_perSearchLineEdit_returnPressed()
{
    goods::on_searchByperBtn_clicked();
}

//没有输入编号时，无法点击查询按钮
void goods::on_idSearchLineEdit_textChanged(const QString &str)
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

//没有输入品种时，无法点击查询按钮
void goods::on_nameSearchLineEdit_textChanged(const QString &str)
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
//没有输入单价时，无法点击查询按钮
void goods::on_perSearchLineEdit_textChanged(const QString &str)
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

