#ifndef CREATESAMPLEDIALOG_H
#define CREATESAMPLEDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

#include "Stats/sample.h"

namespace Ui {
class CreateSampleDialog;
}

class CreateSampleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSampleDialog(const QStringList &listname, const QList<Sample> &sampleList, QWidget *parent = nullptr);
    ~CreateSampleDialog();
    signals:
    void send_samples(QList<Sample>);
private slots:
    void createVarSeries(QString name, int sampleId);
    void createUnion(QString name, int sampleId, int sampleId2);
    void createReSampling(QString name, int sampleId1, int countSample);
    void on_buttonBox_accepted();
private:
    Ui::CreateSampleDialog *ui;
    QList<Sample> sampleList;
    QStringList listName;
    QList<Sample> newSample;

    // He-he
    QVBoxLayout* vbl1 = new QVBoxLayout();
    QVBoxLayout* vbl2= new QVBoxLayout();
    QVBoxLayout* vbl3= new QVBoxLayout();
    QHBoxLayout* hbl1= new QHBoxLayout();
    QHBoxLayout* hbl2= new QHBoxLayout();;
    QHBoxLayout* hbl3= new QHBoxLayout();;
    QHBoxLayout* hbl33= new QHBoxLayout();;
    QLabel* lable1 = new QLabel("Выберите ряд");
    QLabel* lable2 = new QLabel("Выберите ряд");
    QLabel* lable3 = new QLabel("Выберите ряд");
    QLabel* lable33 = new QLabel("Выберите ряд");
    QLineEdit* nameSample1 = new QLineEdit();
    QLineEdit* nameSample2 = new QLineEdit();
    QLineEdit* countSample2 = new QLineEdit();
    QLineEdit* nameSample3 = new QLineEdit();
    QComboBox* sampleL1 = new QComboBox();
    QComboBox* sampleL2 = new QComboBox();
    QComboBox* sampleL3 = new QComboBox();
    QComboBox* sampleL33 = new QComboBox();


};

#endif // CREATESAMPLEDIALOG_H
