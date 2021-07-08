#ifndef CRITERIALCONFIG_H
#define CRITERIALCONFIG_H

#include <QWidget>
#include <QMap>
#include <QVariant>

class QFormLayout;
class QComboBox;
class QLineEdit;
class QCheckBox;
class QPushButton;

namespace Core {
   struct CriterialInfo;
}

class CriterialConfig : public QWidget {

    Q_OBJECT
public:
    explicit CriterialConfig(const Core::CriterialInfo* criterial, const QStringList& sampleNames, QWidget *parent = nullptr);
    //explicit CriterialConfig(QWidget *parent = nullptr);

signals:
    void sendConfigs(const Core::CriterialInfo*, QMap<QString, QVariant>);      // Отправляет введенные значения

private slots:
    void updateData(QStringList);                   // Добавляет новые выборки
    void on_acept_btn_clicked();                    // Собирает данные и посылает сигнал sendConfigs


private:
    QFormLayout* mainLayout;
    QList<QComboBox*> samplesList;
    QList<QLineEdit*> parametersLines;
    QList<QCheckBox*> checkBoxList;

    QPushButton* acept_btn;

    const Core::CriterialInfo* _criterial;

};

#endif // CRITERIALCONFIG_H
