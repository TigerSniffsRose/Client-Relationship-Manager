#ifndef RECDIALOG_H
#define RECDIALOG_H

#include <QDialog>

namespace Ui {
class recDialog;
}

typedef struct information
{
    QString institutionName;
    QString departmentName;
    QString majorName;
    QString website;
    QString area;
    QString province;
    QString address;
    QString institutionType;
    QString introduction;
    QString additional;
    QString contacter;
}Info;

class recDialog : public QDialog
{
    Q_OBJECT

public:
    explicit recDialog(Info *pinfo, QWidget *parent = 0);
    ~recDialog();
signals:
    void sigAdd(Info *pinfo);
private slots:
    void on_addBtn_clicked();

    void on_cancelBtn_clicked();

    void on_institutionlineEdit_editingFinished();

    void on_areacomboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::recDialog *ui;

    Info *pInfo;

    QStringList east;
    QStringList south;
    QStringList north;
    QStringList northeast;
    QStringList northwest;
    QStringList southwest;
    QStringList selfCtrl;
    QStringList middle;
    QStringList special;
};

#endif // RECDIALOG_H
