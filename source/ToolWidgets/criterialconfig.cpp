#include "criterialconfig.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QPushButton>

#include <QDebug>

#include "../core.h"
#include "../Styles/stylesheets.h"

using Core::CriterialInfo;

CriterialConfig::CriterialConfig(const CriterialInfo* criterial, const QStringList& sampleNames, QWidget *parent)
        : QWidget(parent), _criterial(criterial) {
    mainLayout = new QFormLayout(this);

    auto getSectionTitle = [this](const QString& title) {
       QHBoxLayout* section = new QHBoxLayout(this);
       section->addWidget(new QLabel(title, this));
       section->setMargin(3);
       section->itemAt(0)->setAlignment(Qt::AlignCenter);
       return section;
    };

    // Размещаем на форме QComboBox для выбора данных
    mainLayout->addRow(getSectionTitle("Данные:"));
    for(const QString& sample_label: criterial->samples) {
        samplesList.append(new QComboBox);
        samplesList.back()->addItems(sampleNames);
        mainLayout->addRow(sample_label + ":", samplesList.back());
    }

    // Размещаем на форме QLibeEdit для ввода параметров
    if(!criterial->parameters.isEmpty()) mainLayout->addRow(getSectionTitle("Параметры:"));
    for(const auto& param: criterial->parameters) {
        parametersLines.append(new QLineEdit);
        if(param.is_real) parametersLines.back()->setValidator(new QDoubleValidator(param.min, param.max, 10));
        else parametersLines.back()->setValidator(new QIntValidator(param.min, param.max));

        mainLayout->addRow(param.name + ":", parametersLines.back());
    }

    // Размещаем на форме QCheckBox для выбора аппроксимаций
    if(!criterial->aproximations.isEmpty()) mainLayout->addRow(getSectionTitle("Аппроксимации:"));
    for(size_t i = 0; i < criterial->aproximations.size(); ++i) {
        checkBoxList.append(new QCheckBox(criterial->aproximations[i].name));

        QHBoxLayout* tmp = new QHBoxLayout();
        tmp->addWidget(checkBoxList.back());
        if(i+1 < criterial->aproximations.size()) {
            checkBoxList.append(new QCheckBox(criterial->aproximations[i+1].name));
            tmp->addWidget(checkBoxList.back());
            i++;
        }

        mainLayout->addRow(tmp);
    }

    // Размещаем на форме QCheckBox для выбора дополнительных сведений
    if(!criterial->corrects.isEmpty()) mainLayout->addRow(getSectionTitle("Дополнительно:"));
    for(size_t i = 0; i < criterial->corrects.size(); ++i) {
        checkBoxList.append(new QCheckBox(criterial->corrects[i].name));

        QHBoxLayout* tmp = new QHBoxLayout();
        tmp->addWidget(checkBoxList.back());
        if(i+1 < criterial->corrects.size()) {
            checkBoxList.append(new QCheckBox(criterial->corrects[i+1].name));
            tmp->addWidget(checkBoxList.back());
            i++;
        }

        mainLayout->addRow(tmp);
    }

    // Кнопка отправки данных
    acept_btn = new QPushButton("Рассчитать");
    acept_btn->setStyleSheet(StyleTable::acceptBtn());
    connect(acept_btn, SIGNAL(clicked()), this, SLOT(on_acept_btn_clicked()));

    mainLayout->addWidget(acept_btn);
    setLayout(mainLayout);
}


void CriterialConfig::updateData(QStringList samplesNames) {
    for(auto& combobox: samplesList)
        combobox->addItems(samplesNames);
}

void CriterialConfig::on_acept_btn_clicked() {
    /* Собираем введенные данные */
    QMap<QString, QVariant> sendingData;

    // Имя критерия для определения функции
    sendingData["name"] = _criterial->name;
    sendingData["id"] = _criterial->id;

    // Собираем выборки
    for(size_t i = 0; i < _criterial->samples.size(); ++i) {
        sendingData[_criterial->samples.at(i)] = samplesList.at(i)->currentIndex();
    }

    // Собираем значения
    for(size_t i = 0; i < _criterial->parameters.size(); ++i) {
        sendingData[_criterial->parameters.at(i).name] = parametersLines.at(i)->text().replace(',', '.').toDouble();
    }

    // Собираем аппроксимации
    size_t index = 0;
    for(size_t i = 0; i < size_t(_criterial->aproximations.size()); ++i, ++index) {
        if(checkBoxList.at(index)->isChecked())
            sendingData[_criterial->aproximations.at(i).name] = true;
    }

    // Собираем дополнительные данные
    for(size_t i = 0; i < size_t(_criterial->corrects.size()); ++i, ++index) {
        if(checkBoxList.at(index)->isChecked())
            sendingData[_criterial->corrects.at(i).name] = true;
    }

    // Отправляем
    emit sendConfigs(_criterial, sendingData);
}
