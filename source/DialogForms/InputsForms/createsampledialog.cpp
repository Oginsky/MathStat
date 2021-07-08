#include "createsampledialog.h"
#include "ui_createsampledialog.h"

#include <algorithm>
#include <random>

#include "mainwindow.h"

CreateSampleDialog::CreateSampleDialog(const QStringList &listName, const QList<Sample> &sampleList, QWidget *parent):
    QDialog(parent),
    ui(new Ui::CreateSampleDialog),
    sampleList(sampleList),
    listName(listName)
{
    ui->setupUi(this);

    QDialog::setWindowTitle("Создать");

    lable1->setEnabled(false);
    lable2->setEnabled(false);
    lable3->setEnabled(false);
    lable33->setEnabled(false);

    //Вариационный ряд.
    ui->varSeries->setCheckable(true);
    ui->varSeries->setChecked(false);
    nameSample1->setPlaceholderText("Название ряда");
    vbl1->addWidget(nameSample1);
    lable1->setBuddy(sampleL1);
    sampleL1->addItems(listName);
    hbl1->addWidget(lable1);
    hbl1->addWidget(sampleL1);
    vbl1->addLayout(hbl1);
    ui->varSeries->setLayout(vbl1);

    //Повторная выборка
    ui->reSlamping->setCheckable(true);
    ui->reSlamping->setChecked(false);
    nameSample2->setPlaceholderText("Название ряда");
    countSample2->setPlaceholderText("Введите размер создаваемого ряда");
    vbl2->addWidget(nameSample2);
    vbl2->addWidget(countSample2);
    lable2->setBuddy(sampleL2);
    sampleL2->addItems(listName);
    hbl2->addWidget(lable2);
    hbl2->addWidget(sampleL2);
    vbl2->addLayout(hbl2);
    ui->reSlamping->setLayout(vbl2);

    //Объединение
    ui->unioin->setCheckable(true);
    ui->unioin->setChecked(false);
    nameSample3->setPlaceholderText("Название ряда");
    vbl3->addWidget(nameSample3);
    lable3->setBuddy(sampleL3);
    sampleL3->addItems(listName);
    hbl3->addWidget(lable3);
    hbl3->addWidget(sampleL3);
    lable33->setBuddy(sampleL33);
    sampleL33->addItems(listName);
    hbl33->addWidget(lable33);
    hbl33->addWidget(sampleL33);
    vbl3->addLayout(hbl3);
    vbl3->addLayout(hbl33);
    ui->unioin->setLayout(vbl3);
}

CreateSampleDialog::~CreateSampleDialog()
{
    delete ui;
}


void CreateSampleDialog::on_buttonBox_accepted()
{
    if(ui->varSeries->isChecked())
    {
        createVarSeries(nameSample1->text(), sampleL1->currentIndex());
    }
    if(ui->reSlamping->isChecked())
    {
        createReSampling(nameSample2->text(),sampleL2->currentIndex(), countSample2->text().toInt());
    }
    if(ui->unioin->isChecked())
    {
        createUnion(nameSample3->text(),sampleL3->currentIndex(), sampleL33->currentIndex());
    }

    emit send_samples(newSample);
}

void CreateSampleDialog::createVarSeries(QString name, int sampleId)
{
    auto info = sampleList.at(sampleId);
    QVector data = info.getSampleData();

    std::sort(data.begin(),data.end());
    newSample.append(Sample(data,name));
}

void CreateSampleDialog::createReSampling(QString name, int sampleId, int countSample)
{
    auto info = sampleList.at(sampleId);
    QVector data = info.getSampleData();

    auto rng = std::default_random_engine {};
    std::shuffle(data.begin(),data.end(), rng);
    data.resize(countSample);
    newSample.append(Sample(data,name));
}

void CreateSampleDialog::createUnion(QString name, int sampleId1, int sampleId2)
{
    auto info1 = sampleList.at(sampleId1);
    QVector<double> data1 = info1.getSampleData();

    auto info2 = sampleList.at(sampleId2);
    QVector<double> data2 = info2.getSampleData();

    data1 += data2;

    newSample.append(Sample(data1,name));
}
