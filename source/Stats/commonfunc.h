#ifndef COMMONFUNC_H
#define COMMONFUNC_H

#include <QString>
#include <QSet>
#include "../commonstruct.h"

#include "sample.h"

class NormalRand;
class StudentTRand;
class ChiSquaredRand;


typedef const QSet<FLAG>& Flags;
typedef const QSet<QString>& Orders;


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
    double operator()(size_t size, double p, double p0 = 0.5) const;

};

struct Beta {
public:
    double operator()(double shape1, double shape2, double p) const;
};


    static const StudentT studentT;
    static const Normal normal;
    static const ChiSquared chiSquared;
    static const Binomial binomial;
    static const Beta beta;
}


namespace str_const {
    const QString accept("Принимается");
    const QString cancel("Отвергается");
    const QString hypoth_more_zero("Гипотеза больше нуля");
    const QString hypoth_less_zero("Гипотеза меньше нуля");
    const QString hypoth_noteq_zero("Гипотеза не равно нулю");
    const QString hypoth_more("Гипотеза больше");
    const QString hypoth_less("Гипотеза меньше");
    const QString hypoth_noteq("Гипотеза не равно");
    const QString point_estimation("Точечная оценка");
    const QString interval_estimation("Доверительный интервал");
    const QString approximate("Аппроксимация");

    QString interval(double left, double right);
}

double square(double x);



size_t binomialUpperAlpha(size_t n, double alpha, double p);

size_t binomialLowerAlpha(size_t n, double alpha, double p);

void ranking(const Sample& data, QMap<double, double>& rank, unsigned startRank = 1, bool average = true);

void uniq_group(const Sample& data, QMap<double, size_t>& groups);

void uniq_group(const QVector<double>& data, QMap<double, size_t>& groups);

bool has_same_value(const Sample& data1, const Sample& data2);

double disp_with_corrects(size_t n, size_t m, const QMap<double, size_t>& groups);


inline void sort(QVector<double>& data);


template <class T>
T accumulate(T value, const Sample& data) {
    for(size_t i(0); i < size_t(data.size()); ++i) value += static_cast<T>(data[i]);
    return value;
}

template <class T>
double median(const QVector<T>& data) {
    size_t size_hf = data.size()/2;
    return (size_hf%2 == 1) ? data[size_hf] : ( (data[size_hf] + data[size_hf-1])/2.0 );
}

template <class T>
inline QString to_str(T val) { return QString::number(val); }

template <class T>
inline bool less(T statistic, decltype(statistic) crit) { return statistic <= crit; }

template <class T>
inline bool more(T statistic, decltype(statistic) crit) { return statistic >= crit; }

template <class T>
inline bool in_range(T statistic, T min, T max) { return min <= statistic && statistic <= max; }

#endif // COMMONFUNC_H
