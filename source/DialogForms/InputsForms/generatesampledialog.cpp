#include "generatesampledialog.h"
#include "ui_generatesampledialog.h"

#include <QLineEdit>
#include <QLabel>
#include <QCompleter>
#include <QDoubleValidator>
#include <QIntValidator>

#include "../../core.h"

GenerateSampleDialog::GenerateSampleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateSampleDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Сгенерировать выборки");

    QCompleter* distr_compliter = new QCompleter(Core::Distribution::names);
    ui->distr_box->addItems(Core::Distribution::names);
    ui->distr_box->setCompleter(distr_compliter);

    ui->verticalLayout->setAlignment(Qt::AlignTop);

}

GenerateSampleDialog::~GenerateSampleDialog()
{
    delete ui;
}

void GenerateSampleDialog::on_distr_box_activated(int index)
{
    Core::DistributionInfo distr = *Core::Distribution::distributions[index];

    // Удаляем предыдущие поля параметров
    for(int i = 0; i < hlayoutsList.size(); ++i) {

        ui->verticalLayout->removeItem(ui->verticalLayout->itemAt(1));

        delete editLinesList.front();
        delete labelsList.front();
        delete hlayoutsList.front();

        hlayoutsList.erase(hlayoutsList.begin());
        editLinesList.erase(editLinesList.begin());
        labelsList.erase(labelsList.begin());
    }

    // Создаем новые
    for(auto param: distr.parameters) {
        labelsList.push_back(new QLabel(param.name));
        editLinesList.push_back(new QLineEdit);
        hlayoutsList.push_back(new QHBoxLayout);

        QValidator* validator;
        if(param.is_real) validator = new QDoubleValidator(param.min, param.max, 10, editLinesList.back());
        else validator = new QIntValidator(param.min, param.max, editLinesList.back());
        editLinesList.back()->setValidator(validator);

        hlayoutsList.back()->addWidget(labelsList.back());
        hlayoutsList.back()->addWidget(editLinesList.back());

        ui->verticalLayout->addLayout(hlayoutsList.back());
    }


}
