#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "recdialog.h"
#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QTreeWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initDatabase();
    void initModelView();

private slots:
    void on_delBtn_clicked();

    void on_addBtn_clicked();

    void slotAdd(Info *pInfo);
    void slotDC(const QString &filter);
    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    QSqlTableModel *model;
    QTableView *view;
};

#endif // MAINWINDOW_H
