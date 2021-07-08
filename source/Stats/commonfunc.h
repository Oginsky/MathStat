#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#include <QString>
#include <QSet>
#include "../commonstruct.h"

class Sample;

class NormalRand;
class StudentTRand;
class ChiSquaredRand;


namespace CriticalValue {

struct Normal {
    private:
        static const NormalRand distr;
    public:
        double operator()(double p) const;
};

struct StudentT {
public:
    double operator()(double degree, double p) const;
};

struct ChiSquared {
public:
    double operator()(double degree, double p) const;
};

struct Binomial {
public:
    double operator()(size_t size, double p) const;

};


    static const StudentT studentT;
    static const Normal normal;
    static const ChiSquared chiSquared;
    static const Binomial binomial;

}


namespace str_const {
    const QString accept("Принимается");
    const QString cancel("Отвергается");
    const QString hypoth_more_zero("Гипотеза больше нуля");
    const QString hypoth_less_zero("Гипотеза меньше нуля");
    const QString hypoth_noteq_zero("Гипотеза не равно нулю");
    const QString point_estimation("Точечная оценка");
    const QString interval_estimation("Доверительный интервал");
    const QString approximate("Аппроксимация");
}


size_t binomialUpperAlpha(size_t n, double alpha, double p);

size_t binomialLowerAlpha(size_t n, double alpha, double p);

void ranking(const Sample& data, QMap<double, double>& rank, unsigned startRank = 1, bool average = true);

void uniq_group(const Sample& data, QMap<double, size_t>& groups);
void uniq_group(const QVector<double>& data, QMap<double, size_t>& groups);

template <class T>
double median(const QVector<T>& data) {
    size_t size_hf = data.size()/2;
    return (size_hf%2 == 1) ? data[size_hf] : ( (data[size_hf] + data[size_hf-1])/2.0 );
}

inline void sort(QVector<double>& data);

template <class T>
inline QString to_str(T val) { return QString::number(val); }

#endif // COMMONFUNC_H
