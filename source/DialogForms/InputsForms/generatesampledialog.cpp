#include "generatesampledialog.h"
#include "ui_generatesampledialog.h"

#include <QLineEdit>
#include <QLabel>
#include <QCompleter>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QVariant>

#include "../../core.h"


GenerateSampleDialog::GenerateSampleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerateSampleDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Сгенерировать выборки");

    QCompleter* distr_compliter = new QCompleter(Core::Distribution::names);
    distr_compliter->setCompletionMode(QCompleter::CompletionMode::InlineCompletion);
    ui->distr_box->addItems(Core::Distribution::names);
    ui->distr_box->setCompleter(distr_compliter);

    ui->parameters_layout->setAlignment(Qt::AlignTop);

}

GenerateSampleDialog::~GenerateSampleDialog() {
    delete ui;
}


void GenerateSampleDialog::on_distr_box_activated(int index) {
    Core::DistributionInfo distr = *Core::Distribution::distributions[index];

    // Удаляем предыдущие поля параметров
    for(size_t i = 0; i < size_t(ui->parameters_layout->rowCount()); ++i) {
        ui->parameters_layout->removeRow(0);
    }
    if(ui->parameters_layout->rowCount() != 0) ui->parameters_layout->removeRow(0);    // Почему-то последняя строка всегда остается, ее удаляю отдельно

    editLinesList.clear();

    // Создаем новые
    for(auto param: distr.parameters) {
        QLineEdit* parameterLine = new QLineEdit();

        QValidator* validator;
        if(param.is_real) validator = new QDoubleValidator(param.min, param.max, 10, parameterLine);
        else validator = new QIntValidator(static_cast<int>(param.min), static_cast<int>(param.max > INT_MAX ? INT_MAX : param.max), parameterLine);
        parameterLine->setValidator(validator);

        ui->parameters_layout->addRow(param.name, parameterLine);
        editLinesList.push_back(parameterLine);
    }
}

void GenerateSampleDialog::on_buttonBox_accepted() {
    const Core::DistributionInfo* distrInfo = Core::Distribution::distributions[ui->distr_box->currentIndex()];
    Sample sample;
    size_t sample_size = ui->sampl_size_spin->value();
    sample.setName(ui->name_line->text());

    switch (distrInfo->parameters_count()) {
        case 1: {
            QVariant value = editLinesList.front()->text().replace(',', '.');
            Core::Distribution::generateSample(distrInfo, value, sample_size, sample);
            break;
        }

        case 2: {
                QVariant value1 = editLinesList.front()->text().replace(',', '.');
                QVariant value2 = editLinesList.at(1)->text().replace(',', '.');
                Core::Distribution::generateSample(distrInfo, value1, value2, sample_size, sample);
                break;
        }

        case 3: {
                QVariant value1 = editLinesList.front()->text().replace(',', '.');
                QVariant value2 = editLinesList.at(1)->text().replace(',', '.');
                QVariant value3 = editLinesList.at(2)->text().replace(',', '.');
                Core::Distribution::generateSample(distrInfo, value1, value2, value3, sample_size, sample);
                break;
        }
    }

    emit send_samples(QList<Sample>{sample});
}
