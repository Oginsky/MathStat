#ifndef ADDGRAPHDIALOG_H
#define ADDGRAPHDIALOG_H

#include <QDialog>

#include <QList>



namespace Ui {
class AddGraphDialog;
}

class AddGraphDialog : public QDialog
{
    Q_OBJECT

signals:
    void send_graph(QList<QString>, QString);

public:
    explicit AddGraphDialog(const QStringList& listname, QWidget *parent = nullptr);
    ~AddGraphDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AddGraphDialog *ui;
};

#endif // ADDGRAPHDIALOG_H
