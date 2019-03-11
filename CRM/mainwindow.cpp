#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QCompleter>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("./eye.ico"));
    QString sPath = QCoreApplication::applicationDirPath() + "\\lxl.db";
    setWindowTitle(sPath);

    initDatabase();
    initModelView();
}

void MainWindow::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("lxl.db");

    if (!db.open())
    {
        qDebug() << "Err01: " << QSqlDatabase().lastError();
    }
    QSqlQuery query;
    QString create = "create table if not exists Tab010 ("
                     "institutionName varchar(64),"
                     "departmentName varchar(64),"
                     "majorName varchar(64),"
                     "website varchar(64),"
                     "area varchar(16),"
                     "province varchar(32),"
                     "address varchar(128),"
                     "institutionType varchar(16),"
                     "introduction varchar(1024),"
                     "additional varchar(1024),"
                     "contacter varchar(1024)"
                     ")";//"id int"
    query.prepare(create);
    if (!query.exec(create))
    {
        qDebug() << "Err02: " << QSqlDatabase().lastError();
    }
}

void MainWindow::initModelView()
{
    model = new QSqlTableModel;

    model->setTable("Tab010");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setHeaderData(0, Qt::Horizontal, QVariant("校名"));
    model->setHeaderData(1, Qt::Horizontal, QVariant("院系"));
    model->setHeaderData(2, Qt::Horizontal, QVariant("专业"));
    model->setHeaderData(3, Qt::Horizontal, QVariant("网址"));
    model->setHeaderData(4, Qt::Horizontal, QVariant("区域"));
    model->setHeaderData(5, Qt::Horizontal, QVariant("省份"));
    model->setHeaderData(6, Qt::Horizontal, QVariant("地址"));
    model->setHeaderData(7, Qt::Horizontal, QVariant("类型"));
    model->setHeaderData(8, Qt::Horizontal, QVariant("简介"));
    model->setHeaderData(9, Qt::Horizontal, QVariant("备注"));
    model->select();

    view = ui->tableView;
    view->setModel(model);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->hideColumn(10);
    view->horizontalHeader()->setStretchLastSection(true);
    //view->hideColumn(1);

    //connect(model, &QSqlTableModel::dataChanged,this, &Widget::slotDC);
    connect(ui->lineEdit, &QLineEdit::textChanged,this, &MainWindow::slotDC);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_delBtn_clicked()
{
    QItemSelectionModel *selected = view->selectionModel();
    QModelIndexList list = selected->selectedIndexes();
    for (int i = 0; i != list.size(); ++i)
    {
        if ( !model->removeRow(list.at(i).row()) )
            qDebug() << "Err03: " << QSqlDatabase().lastError();
    }
    int i = QMessageBox::question(this, "^*^", "确定删除?",
                                  QMessageBox::Yes|
                                  QMessageBox::No);
    switch (i) {
    case QMessageBox::Yes:
        model->submitAll();
        break;
    case QMessageBox::No:
        model->revertAll();
        break;
    default:
        break;
    }
}

void MainWindow::on_addBtn_clicked()
{
    recDialog dlg(Q_NULLPTR);
    connect(&dlg, &recDialog::sigAdd, this, &MainWindow::slotAdd);
    dlg.exec();
}

void MainWindow::slotAdd(Info *pInfo)
{
    QSqlRecord rec = model->record();

    QString institutionName = pInfo->institutionName;
    QString departmentName = pInfo->departmentName;
    QString majorName = pInfo->majorName;
    QString website = pInfo->website;
    QString area = pInfo->area;
    QString province = pInfo->province;
    QString address = pInfo->address;
    QString institutionType = pInfo->institutionType;
    QString introduction = pInfo->introduction;
    QString additional = pInfo->additional;
    QString contacter = pInfo->contacter;

    rec.setValue("institutionName", QVariant(institutionName));
    rec.setValue("departmentName", QVariant(departmentName));
    rec.setValue("majorName", QVariant(majorName));
    rec.setValue("website", QVariant(website));
    rec.setValue("area", QVariant(area));
    rec.setValue("province", QVariant(province));
    rec.setValue("address", QVariant(address));
    rec.setValue("institutionType", QVariant(institutionType));
    rec.setValue("introduction", QVariant(introduction));
    rec.setValue("additional", QVariant(additional));
    rec.setValue("contacter", QVariant(contacter));
    model->insertRecord(-1, rec);
    model->submitAll();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QSqlRecord rec = model->record(row);
    Info *p = new Info;

    p->institutionName = rec.value("institutionName").toString();
    p->departmentName = rec.value("departmentName").toString();
    p->majorName = rec.value("majorName").toString();
    p->website = rec.value("website").toString();
    p->area = rec.value("area").toString();
    p->province = rec.value("province").toString();
    p->address = rec.value("address").toString();
    p->institutionType = rec.value("institutionType").toString();
    p->introduction = rec.value("introduction").toString();
    p->additional = rec.value("additional").toString();
    p->contacter = rec.value("contacter").toString();

    if (p)
    {
        recDialog dlg(p);
        if (recDialog::Accepted == dlg.exec())
        {
            rec.setValue("institutionName", QVariant(p->institutionName));
            rec.setValue("departmentName", QVariant(p->departmentName));
            rec.setValue("majorName", QVariant(p->majorName));
            rec.setValue("website", QVariant(p->website));
            rec.setValue("area", QVariant(p->area));
            rec.setValue("province", QVariant(p->province));
            rec.setValue("address", QVariant(p->address));
            rec.setValue("institutionType", QVariant(p->institutionType));
            rec.setValue("introduction", QVariant(p->introduction));
            rec.setValue("additional", QVariant(p->additional));
            rec.setValue("contacter", QVariant(p->contacter));

            model->setRecord(row, rec);
            model->submitAll();
        }
    }
}

void MainWindow::slotDC(const QString &filter)
{
    QStringList strList;

    for (int i = 0; i != model->rowCount(); ++i)
    {
        QSqlRecord rec = model->record(i);
        for (int j = 0; j != rec.count(); ++j)
        {
            QString var = rec.value(j).toString();
            if (var.isEmpty()) continue;
            strList << var;
        }
    }
    qDebug() << "strList: " << strList;
    QCompleter *completer = new QCompleter(strList);
    //completer->setCompletionMode(QCompleter::InlineCompletion);////QCompleter::UnfilteredPopupCompletion
    ui->lineEdit->setCompleter(completer);

    if (filter.isEmpty())
    {
        model->setFilter("");
        model->select();
        return;
    }
    QString subflt;
    //select * form tab where (name like %""% or Gender like %""% or Age like %""%)

    QSqlRecord rec = model->record();
    for (int i = 0; i != rec.count(); ++i)
    {
        if (i != 0)
        {
            subflt += " or ";
        }
        QString fildname = rec.fieldName(i);
        QString flt = QString("%1 like '%%2%'").arg(fildname).arg(filter);
        subflt += flt;
    }
    qDebug() << "subflt: " << subflt;
    model->setFilter(subflt);
    model->select();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    QString str = current->text(0);
    QString filter;
    qDebug() << "str: " << str;
    if ("区域" == str)
    {
        model->setFilter("");
        model->select();
        return;
    }
    filter = QString("area = '%1' or province = '%2'").arg(str).arg(str);
    model->setFilter(filter);
    model->select();
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    QString filter;
    if ("全部" == arg1)
    {
        model->setFilter("");
        model->select();
        return;
    }
    filter = QString("institutionType = '%1'").arg(arg1);
    model->setFilter(filter);
    model->select();
}












