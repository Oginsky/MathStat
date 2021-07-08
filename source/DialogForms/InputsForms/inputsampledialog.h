#ifndef INPUTSAMPLEDIALOG_H
#define INPUTSAMPLEDIALOG_H

#include <QDialog>

#include "Stats/sample.h"

namespace Ui {
class InputSampleDialog;
}

class InputSampleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputSampleDialog(QWidget *parent = nullptr);
    ~InputSampleDialog();

    QString getSeparator(const QString& filepath);

signals:
    void send_samples(QList<Sample>);

private slots:
    void on_choosefile_btn_clicked();
    void on_buttonBox_accepted();

private:
    Ui::InputSampleDialog *ui;

    QString filepath;
    QList<Sample> samples;
};

#endif // INPUTSAMPLEDIALOG_H
