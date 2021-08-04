#ifndef V_H
#define V_H

#include <QVector>
#include <QString>

using std::size_t;



class SampleCharacteristic {
public:
    double _expectation;
    double _variance;
    double _correct_var;
    double _median;

    double _min, _max;
    double _span;

    double expectation() const;
    double variance()    const;
    double correct_var() const;
    double median()      const;
    double span()        const;
    double min()         const;
    double max()         const;
};


class Sample {

public:

    Sample();
    Sample(QString name);
    Sample(QVector<double>& data, QString name = "");

    void setData(const QVector<double>& data);
    void setName(const QString& name);
    QVector<double>& getSampleData();

    size_t size() const;
    QString name() const;
    const QVector<double>& data() const;


    SampleCharacteristic characters() const;
    SampleCharacteristic& characters();
    void countCharacteristic();

    double& operator[](size_t index);
    double operator[](size_t index) const;
    const Sample operator+(const Sample& right) const;

    QVector<double>::const_iterator cbegin() const;
    QVector<double>::const_iterator cend() const;


    void resize(size_t size);
    void reserve(size_t size);
    void push_back(double value);
    void makeSort();

private:
    size_t _size;
    QVector<double> sampleData;
    QString _name;
    size_t id;
    SampleCharacteristic _characters;

    static size_t totalID;

    /* Методы */
};

#endif // V_H
