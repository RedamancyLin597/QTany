#include "studentdialog.h"
#include "ui_studentdialog.h"

QString selAll = "select * from student";

StudentDialog::StudentDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StudentDialog)
{
    ui->setupUi(this);

    CreateDatabaseFunc();
    CreateTableFunc();
    on_showUserDb("select * from student");
}

StudentDialog::~StudentDialog()
{
    delete ui;

}

void StudentDialog::CreateDatabaseFunc(){// 创建SQLite数据库 打开连接
    // 1：添加数据库驱动
    sqldb=QSqlDatabase::addDatabase("QSQLITE");

    // 2：设置数据库名称
    sqldb.setDatabaseName("studentmis.db");

    // 3：打开此数据库是否成功
    if(sqldb.open()==true){
        QMessageBox::information(0,"正确","恭喜你，数据库打开成功！",QMessageBox::Ok);
    }else{
        QMessageBox::critical(0,"错误","数据库打开失败，请重新检测！",QMessageBox::Ok);
    }
}
void StudentDialog::CreateTableFunc(){ // 创建SQLite数据表
    QSqlQuery createquery;//执行查询sql语句的类对象

    // 创建SQL语句
    QString strsql=QString("create table student(id int primary key not null , name text not null , score real not null)");

    // 执行SQL语句
    // if(createquery.exec(strsql)==false){
    //     QMessageBox::critical(0,"失败","数据表创建失败，请重新检查！",QMessageBox::No);
    // }else{
    //     QMessageBox::information(0,"成功","恭喜你，数据表创建成功！",QMessageBox::Ok);
    // }
}
void StudentDialog::QueryTableFunc(){// 执行查询操作

}

void StudentDialog::on_showUserDb(QString sql)
{
    //QString str = "select * from student";
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView_stu);
    model->setQuery(sql);

    model->setHeaderData(0, Qt::Horizontal, tr("学号"));
    model->setHeaderData(1, Qt::Horizontal, tr("姓名"));
    model->setHeaderData(2, Qt::Horizontal, tr("成绩"));

    ui->tableView_stu->setModel(model);//数据放置进去
    ui->tableView_stu->setColumnWidth(0,225);
    ui->tableView_stu->setColumnWidth(1,225);
    ui->tableView_stu->setColumnWidth(2,225);
    ui->tableView_stu->verticalHeader()->hide();//不显示序号
}

void StudentDialog::on_pushButtonSort_clicked()
{
    QString sort = ui->comboBoxVal->currentText();//排序的字段
    QString order = ui->comboBoxCond->currentText();//排序的方式

    QString select_sql = "select * from student";//sql
    if(sort=="按成绩排序"){
        select_sql += " order by score";
    }else if(sort=="按学号排序"){
        select_sql += " order by id";
    }else{
        QMessageBox::critical(this,"错误","提示：请您选择排序内容！",QMessageBox::Ok);
        return;
    }
    if(order=="升序"){
        select_sql += " asc";
    }else if(order=="降序"){
        select_sql += " desc";
    }else{
        QMessageBox::critical(this,"错误","提示：请您选择排序方式！",QMessageBox::Ok);
        return;
    }

    on_showUserDb(select_sql);
}

void StudentDialog::on_pushButton_INSERT_clicked()
{
    QSqlQuery sqlquery;
    int id = ui->lineEdit_ID -> text().toInt();
    if(id<=0){
        QMessageBox::critical(this,"失败","提示：输入错误？学号有误？",QMessageBox::Ok);
        return ;
    }

    QString name=ui->lineEdit_NAME ->text();
    if(name==""){
        QMessageBox::critical(this,"失败","提示：输入错误？姓名不能为空？",QMessageBox::Ok);
        return ;
    }

    double score=ui->lineEdit_SCORE->text().toDouble();
    if(score<0||score>100){
        QMessageBox::critical(this,"失败","提示：分数范围有误!",QMessageBox::Ok);
        return ;
    }
    // %1  做占位符
    QString strs=QString("insert into student values(%1,'%2','%3')").arg(id).arg(name).arg(score);

    if(sqlquery.exec(strs)==false){
        QMessageBox::critical(0,"失败","向数据表插入记录失败，请重新检查！",QMessageBox::Ok);
    }else{
        QMessageBox::information(0,"成功","恭喜你，向数据表插入记录成功！",QMessageBox::Ok);
    }
    on_showUserDb(selAll);
}


void StudentDialog::on_pushButton_DELETE_clicked()
{
    QSqlQuery sqlquery;
    int id = ui->lineEdit_ID -> text().toInt();
    if(id<=0){
        QMessageBox::critical(this,"失败","提示：输入错误？学号有误？",QMessageBox::Ok);
        return ;
    }
    QString strs=QString("delete from student where id = %1").arg(id);

    if(sqlquery.exec(strs)==false){
        QMessageBox::critical(0,"失败","删除失败，请您联系管理员！",QMessageBox::Ok);
    }else{
        QMessageBox::information(0,"成功","恭喜你，删除成功！",QMessageBox::Ok);
    }
    on_showUserDb(selAll);
}


void StudentDialog::on_pushButton_UPDATE_clicked()
{
    QSqlQuery sqlquery;
    int id = ui->lineEdit_ID -> text().toInt();
    if(id<=0){
        QMessageBox::critical(this,"失败","提示：输入错误？学号有误？",QMessageBox::Ok);
        return ;
    }

    QString name=ui->lineEdit_NAME ->text();
    if(name==""){
        QMessageBox::critical(this,"失败","提示：输入错误？姓名不能为空？",QMessageBox::Ok);
        return ;
    }

    double score=ui->lineEdit_SCORE->text().toDouble();
    if(score<0||score>100){
        QMessageBox::critical(this,"失败","提示：分数范围有误!",QMessageBox::Ok);
        return ;
    }
    // %1  做占位符
    QString strs=QString("update student set id = %1,name = '%2',score = %3 where id = %1").arg(id).arg(name).arg(score);

    if(sqlquery.exec(strs)==false){
        QMessageBox::critical(0,"失败","修改失败，请您联系管理员！",QMessageBox::Ok);
    }else{
        QMessageBox::information(0,"成功","恭喜你，修改成功！",QMessageBox::Ok);
    }

    on_showUserDb(selAll);
}


void StudentDialog::on_pushButton_SEARCH_clicked()
{
    bool hadKey = false;
    QString sql = "select * from student Where 1=1 ";
    int id = ui->lineEdit_ID -> text().toInt();
    if(id>0){
        hadKey = true;
        sql += " And id = " + QString::number(id);
    }
    QString name=ui->lineEdit_NAME ->text();
    if(name!=""){
        hadKey = true;
        sql += " And name like '%"+name+"%' ";
    }
    QString score=ui->lineEdit_SCORE->text();
    if(score!=""){
        hadKey = true;
        sql += " And score = '"+score+"' ";
    }
    if(!hadKey){
        on_showUserDb(selAll);
        return ;
    }

    on_showUserDb(sql);
}


