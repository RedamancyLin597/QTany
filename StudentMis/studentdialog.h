#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QtDebug>
#include <QMessageBox>
#include <QTableView>
#include<QSqlTableModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class StudentDialog;
}
QT_END_NAMESPACE

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    StudentDialog(QWidget *parent = nullptr);
    ~StudentDialog();

private slots:
    void on_pushButtonSort_clicked();

    void on_showUserDb(QString select_sql);

    void on_pushButton_INSERT_clicked();

    void on_pushButton_DELETE_clicked();

    void on_pushButton_UPDATE_clicked();

    void on_pushButton_SEARCH_clicked();

private:
    void CreateDatabaseFunc();  // 创建SQLite数据库
    void CreateTableFunc();     // 创建SQLite数据表
    void QueryTableFunc();      // 执行查询操作

    QSqlDatabase sqldb;         // 创建Qt和数据库链接
    QSqlQueryModel sqlmodel;    // 存储结果集

private:
    Ui::StudentDialog *ui;
};
#endif // STUDENTDIALOG_H
