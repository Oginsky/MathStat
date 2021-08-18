#ifndef GENERATESAMPLEDIALOG_H
#define GENERATESAMPLEDIALOG_H

#include <QDialog>

#include "Stats/sample.h"

class QLineEdit;
class QLabel;
class QHBoxLayout;

namespace Ui {
class GenerateSampleDialog;
}

class GenerateSampleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GenerateSampleDialog(QWidget *parent = nullptr);
    ~GenerateSampleDialog();

signals:
    void send_samples(QList<Sample>);

private slots:

    void on_distr_box_activated(int index);
    void on_buttonBox_accepted();

private:
    Ui::GenerateSampleDialog *ui;

    QList<QLineEdit*> editLinesList;
    QList<QLabel*> labelsList;
    QList<QHBoxLayout*> hlayoutsList;

};

#endif // GENERATESAMPLEDIALOG_H
