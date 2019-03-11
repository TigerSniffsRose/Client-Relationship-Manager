#include "recdialog.h"
#include "ui_recdialog.h"
#include <QMessageBox>
#include <QDebug>

recDialog::recDialog(Info *pinfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::recDialog),pInfo(pinfo)
{
    ui->setupUi(this);

    setWindowIcon(QIcon("../eye.ico"));
    QString sPath = QCoreApplication::applicationDirPath() + "\\lxl.db";
    setWindowTitle(sPath);
    east << "上海" << "江苏" << "浙江" << "山东" << "安徽";
    south << "广东" << "广西" << "海南" << "福建";
    north << "北京" << "天津" << "河北" << "山西" << "陕西";
    northeast << "吉林" << "辽宁" << "黑龙江";
    northwest << "甘肃" << "宁夏" << "青海";
    southwest << "云南" << "贵州" << "四川" << "重庆";
    selfCtrl << "新疆" << "西藏" << "内蒙古";
    special << "香港" << "澳门" << "台湾";
    middle << "江西" << "湖南" << "湖北" << "河南";

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setRowCount(20);
    for(int i=0;i<20;i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(""));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(""));
    }

    if (pInfo)
    {
        ui->addBtn->setText("修改");

        ui->addrlineEdit->setText(pInfo->address);
        ui->addtionaltextEdit->setText(pInfo->additional);
        ui->areacomboBox->setCurrentText(pInfo->area);
        ui->departmentlineEdit->setText(pInfo->departmentName);
        ui->institutionlineEdit->setText(pInfo->institutionName);
        ui->introductiontextEdit->setText(pInfo->introduction);
        ui->majorlineEdit->setText(pInfo->majorName);
        ui->provincecomboBox->setCurrentText(pInfo->province);
        //ui->TypecomboBox->setCurrentText(pInfo->institutionType);
        ui->weblineEdit_3->setText(pInfo->website);

        QStringList all = pInfo->contacter.split("#");
        for(int i=0;i<all.size();i++)
        {
            QStringList row = all[i].split("*");

            if(row.size() >= 6)
            {
                ui->tableWidget->item(i,0)->setText(row[0]);
                ui->tableWidget->item(i,1)->setText(row[1]);
                ui->tableWidget->item(i,2)->setText(row[2]);
                ui->tableWidget->item(i,3)->setText(row[3]);
                ui->tableWidget->item(i,4)->setText(row[4]);
                ui->tableWidget->item(i,5)->setText(row[5]);
            }
        }
    }
}

recDialog::~recDialog()
{
    delete ui;
}

void recDialog::on_addBtn_clicked()
{
    Info *p = new Info;
    if (ui->institutionlineEdit->text().isEmpty())
    {
        QMessageBox::critical(this, "^*^", "校名不能为空!");
        return;
    }
    p->institutionName     = ui->institutionlineEdit->text();
    p->departmentName      = ui->departmentlineEdit->text();
    p->majorName           = ui->majorlineEdit->text();
    p->website             = ui->weblineEdit_3->text();
    p->area                = ui->areacomboBox->currentText();
    p->province            = ui->provincecomboBox->currentText();
    p->address             = ui->addrlineEdit->text();
    p->institutionType     = ui->TypecomboBox->currentText();
    p->introduction        = ui->introductiontextEdit->toPlainText();
    p->additional          = ui->addtionaltextEdit->toPlainText();

    QString sAll = "";
    QString s = "";
    int rows = ui->tableWidget->rowCount();
    int columns = ui->tableWidget->columnCount();
    for(int i=0;i<rows;i++)
    {
        if(ui->tableWidget->item(i,0)->text().isEmpty())
        {
            continue;
        }
        for(int j=0;j<columns;j++)
        {
            //#和*是用来分隔的标记，禁止用户输入这2个符号，如果不小心输了，程序也替换成空格
            s = ui->tableWidget->item(i,j)->text();
            s.replace("#"," ");
            s.replace("*"," ");
            sAll += s;
            if(j == columns-1)
            {
                sAll += "#";
            }
            else
            {
                sAll += "*";
            }
        }
    }
    p->contacter = sAll;

    if(pInfo)
    {
        //修改
        pInfo->institutionName = p->institutionName;
        pInfo->departmentName = p->departmentName;
        pInfo->majorName = p->majorName;
        pInfo->website = p->website;
        pInfo->area = p->area;
        pInfo->province = p->province;
        pInfo->address = p->address;
        pInfo->institutionType = p->institutionType;
        pInfo->introduction = p->introduction;
        pInfo->additional = p->additional;
        pInfo->contacter = p->contacter;
        accept();
    }
    else
    {
        //新增
        emit sigAdd(p);

        ui->addrlineEdit->setText("");
        ui->addtionaltextEdit->setText("");
        ui->areacomboBox->setCurrentText("");
        ui->departmentlineEdit->setText("");
        ui->institutionlineEdit->setText("");
        ui->introductiontextEdit->setText("");
        ui->majorlineEdit->setText("");
        ui->provincecomboBox->setCurrentText("");
        ui->TypecomboBox->setCurrentText("");
        ui->weblineEdit_3->setText("");

        for(int i=0;i<20;i++)
        {
            ui->tableWidget->item(i,0)->setText("");
            ui->tableWidget->item(i,1)->setText("");
            ui->tableWidget->item(i,2)->setText("");
            ui->tableWidget->item(i,3)->setText("");
            ui->tableWidget->item(i,4)->setText("");
            ui->tableWidget->item(i,5)->setText("");
        }
        QMessageBox::information(this, "^*^", "添加成功!");
    }
}

void recDialog::on_cancelBtn_clicked()
{
    reject();
}

void recDialog::on_institutionlineEdit_editingFinished()
{
    if(ui->institutionlineEdit->text().contains("理工"))
    {
        ui->TypecomboBox->setCurrentText("理工");
    }
    else if(ui->institutionlineEdit->text().contains("工业"))
    {
        ui->TypecomboBox->setCurrentText("工业");
    }
    else if(ui->institutionlineEdit->text().contains("财经"))
    {
        ui->TypecomboBox->setCurrentText("财经");
    }
    else if(ui->institutionlineEdit->text().contains("农业"))
    {
        ui->TypecomboBox->setCurrentText("农业");
    }
    else if(ui->institutionlineEdit->text().contains("科技"))
    {
        ui->TypecomboBox->setCurrentText("科技");
    }
    else if(ui->institutionlineEdit->text().contains("师范"))
    {
        ui->TypecomboBox->setCurrentText("师范");
    }
    else if(ui->institutionlineEdit->text().contains("航空"))
    {
        ui->TypecomboBox->setCurrentText("航空");
    }
    else if(ui->institutionlineEdit->text().contains("交通"))
    {
        ui->TypecomboBox->setCurrentText("交通");
    }
    else if(ui->institutionlineEdit->text().contains("邮电"))
    {
        ui->TypecomboBox->setCurrentText("邮电");
    }
    else
    {
        ui->TypecomboBox->setCurrentText("综合");
    }
}

void recDialog::on_areacomboBox_currentIndexChanged(const QString &arg1)
{
    if ("区域" == arg1)
    {
        ui->provincecomboBox->setCurrentText("请选择区域");
    }
    else if ("华东" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(east);
    }
    else if ("华南" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(south);
    }
    else if ("华北" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(north);
    }
    else if ("东北" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(northeast);
    }
    else if ("西北" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(northwest);
    }
    else if ("西南" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(southwest);
    }
    else if ("自治区" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(selfCtrl);
    }
    else if ("华中" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(middle);
    }
    else if ("行政特区" == arg1)
    {
        ui->provincecomboBox->clear();
        ui->provincecomboBox->addItems(special);
    }
}












