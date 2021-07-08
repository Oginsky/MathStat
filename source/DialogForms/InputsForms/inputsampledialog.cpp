#include "inputsampledialog.h"
#include "ui_inputsampledialog.h"

#include <QStyleFactory>


#include <QFileDialog>
#include <QFile>
#include <QDebug>


InputSampleDialog::InputSampleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputSampleDialog)
{
    ui->setupUi(this);
    QDialog::setWindowTitle("Загрузить данные");
}

InputSampleDialog::~InputSampleDialog()
{
    delete ui;
}


QString InputSampleDialog::getSeparator(const QString& filepath) {
    QString default_sep = " ";
    static const QMap<QString, QChar> separators = {
        {"csv", ';'},
        {"xlsx", ';'},
        {"txt", '\t'}
    };

   QString filetype = filepath.split(".").last();


   return (separators.contains(filetype) ? separators[filetype] : default_sep);
}


void InputSampleDialog::on_choosefile_btn_clicked() {
    filepath = QFileDialog::getOpenFileName(0, "Выберите данные ", "C:\\Users\\Ogins\\Documents\\QtProject\\MathStats\\Data", "");
    if(filepath.isEmpty()) {
        ui->showfilename_line->setText("Файл не выбран");
        return;
    }

    ui->showfilename_line->setText(filepath);
}

void InputSampleDialog::on_buttonBox_accepted() {

    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream fin(&file);
    QString filetype = filepath.split(".").last();
    bool headers = ui->headers_check->isChecked();

    QString sep = getSeparator(filepath);

    if(headers) {
        QStringList headersList = fin.readLine().split(";");
        for(auto& name: headersList) {
            samples.append(Sample(name));
        }
    }
    else {
        // Нужно узнать количество выборок
        QStringList valuesList = fin.readLine().split(";");
        for(auto& val: valuesList) {
            double value = val.replace(',', '.').toDouble();
            samples.append(Sample());
            samples.back().push_back(value);
        }
    }

    while(!fin.atEnd()) {
        QStringList valuesList = fin.readLine().split(";");
        int sample_index = 0;

        for(auto& val: valuesList) {
            if(val.isEmpty()) {
                // Пустой столбец будет выглядить как ;;;;
                sample_index++;
                continue;
            }

            double value = val.replace(',', '.').toDouble();
            if(sample_index >= samples.size()) break; // выходит за пределы обозначенных столбцов
            samples[sample_index++].push_back(value);
        }
    }

    file.close();


    emit send_samples(samples);
}
