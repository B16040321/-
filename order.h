#ifndef ORDER_H
#define ORDER_H
#include <QWidget>
#include<QCloseEvent>

namespace Ui {
class order;
class QSqlTableModel;
}

class QSqlTableModel;
class order : public QWidget
{
    Q_OBJECT

public:
    explicit order(QWidget *parent = nullptr);
    ~order();

private slots:
    void closeEvent(QCloseEvent *);
    void setTableViewTitle(); //输出表

    void on_addBtn_clicked(); //增加记录

    void on_modifyBtn_clicked();//修改记录

    void on_undoBtn_clicked(); //回滚

    void on_deleteBtn_clicked(); //删除记录
    void on_searchByNameBtn_clicked();
    void on_searchByIdBtn_clicked();
    void on_searchByperBtn_clicked();

    void on_idSearchLineEdit_textChanged(const QString &str);
    void on_nameSearchLineEdit_textChanged(const QString &str);
    void on_perSearchLineEdit_textChanged(const QString &str);

    void on_idSearchLineEdit_returnPressed();
    void on_nameSearchLineEdit_returnPressed();
    void on_perSearchLineEdit_returnPressed();

signals:
    void signal_ret();

private:
    Ui::order *ui;
    QSqlTableModel *model;
    void reload_table(); //重新输出表
};

#endif // ORDER_H
