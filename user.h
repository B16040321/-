#ifndef USER_H
#define USER_H
#include <QWidget>
#include<QCloseEvent>

namespace Ui {
class user;
class QSqlTableModel;
}

class QSqlTableModel;

class user : public QWidget
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    ~user();

private slots:
    void closeEvent(QCloseEvent *);
    void setTableViewTitle(); //输出表

    void on_addBtn_clicked(); //增加记录

    void on_modifyBtn_clicked();//修改记录

    void on_undoBtn_clicked(); //回滚

    void on_deleteBtn_clicked(); //删除记录
    void on_searchByNameBtn_clicked();
    void on_searchByIdBtn_clicked();
    void on_searchByphoneBtn_clicked();
    void on_searchByemailBtn_clicked();

    void on_idSearchLineEdit_textChanged(const QString &str);
    void on_nameSearchLineEdit_textChanged(const QString &str);
    void on_phoneSearchLineEdit_textChanged(const QString &str);
    void on_emailSearchLineEdit_textChanged(const QString &str);

    void on_idSearchLineEdit_returnPressed();
    void on_nameSearchLineEdit_returnPressed();
    void on_phoneSearchLineEdit_returnPressed();
    void on_emailSearchLineEdit_returnPressed();

signals:
    void signal_ret();

private:
    Ui::user *ui;
    QSqlTableModel *model;
    void reload_table(); //重新输出表
};

#endif // USER_H
