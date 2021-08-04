#include "criterialresult.h"

#include "../commonstruct.h"
#include "../Styles/stylesheets.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>

#include <QHeaderView>      // Для того чтобы убрать header

CriterialResult::CriterialResult(const OutputData& output, QWidget *parent) : QWidget(parent) {
    QVBoxLayout* main_layout = new QVBoxLayout(this);

    if(!output.hypoth.isEmpty()) {

        for(const QString& key: output.hypoth.keys()) {

            main_layout->addWidget(new QLabel(key));

            int index = 0;
            QTableWidget* hypoths_table = new QTableWidget(this);
            hypoths_table->verticalHeader()->setVisible(false);
            hypoths_table->horizontalHeader()->setVisible(false);
            hypoths_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            hypoths_table->setRowCount(2);
            hypoths_table->setColumnCount(output.hypoth[key].hypoth.size());
            hypoths_table->setFixedHeight(hypoths_table->rowCount() * 55);

            for(const QString& hyp: output.hypoth[key].hypoth.keys()) {
                hypoths_table->setItem(0, index, new QTableWidgetItem(hyp));
                hypoths_table->setItem(1, index++, new QTableWidgetItem(output.hypoth[key].hypoth[hyp]));
            }
            hypoths_table->resizeRowsToContents();
            hypoths_table->setStyleSheet(StyleTable::hypothTable());
            hypoths_table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
            hypoths_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            hypoths_table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

            hypoths_table->setEditTriggers(QAbstractItemView::EditTriggers::enum_type::NoEditTriggers);
            main_layout->addWidget(hypoths_table);
        }
    }

    QFormLayout* values_layout = new QFormLayout(this);
    for(QString name_value : output.values.keys())
        values_layout->addRow(name_value + ":", new QLabel(output.values[name_value], this));

    QFormLayout* extra_layout = new QFormLayout(this);

    for(QString name_value : output.extra.keys())
        extra_layout->addRow(name_value + ":", new QLabel(output.extra[name_value], this));

    main_layout->addLayout(values_layout);
    main_layout->addLayout(extra_layout);
    setLayout(main_layout);
}
