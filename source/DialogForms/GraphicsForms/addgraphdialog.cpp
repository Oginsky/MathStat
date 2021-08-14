#include "addgraphdialog.h"
#include "ui_addgraphdialog.h"


AddGraphDialog::AddGraphDialog(const QStringList& listname, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddGraphDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Добавить график");

    ui->samplesNameBox->addItems(listname);
}

AddGraphDialog::~AddGraphDialog()
{
    delete ui;
}

void AddGraphDialog::on_buttonBox_accepted() {
    QList<QString>* plotObjects = new QList<QString>();

    if(ui->bars->isChecked()) plotObjects->push_back("BarsGraph");
    if(ui->empfunc->isChecked()) plotObjects->push_back("EmpericDistribFunction");


    emit send_graph(*plotObjects, ui->samplesNameBox->currentText(), ui->name_line->text());
}
