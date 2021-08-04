#ifndef GENERATESAMPLEDIALOG_H
#define GENERATESAMPLEDIALOG_H

#include <QDialog>

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

private slots:

    void on_distr_box_activated(int index);

private:
    Ui::GenerateSampleDialog *ui;

    QList<QLineEdit*> editLinesList;
    QList<QLabel*> labelsList;
    QList<QHBoxLayout*> hlayoutsList;

};

#endif // GENERATESAMPLEDIALOG_H
