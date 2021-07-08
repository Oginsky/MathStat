#include "anaysisdichotomousdata.h"
#include "commonfunc.h"
#include "RandLib.h"

template <class T>
inline bool less(T statistic, decltype(statistic) crit) { return statistic <= crit; }

template <class T>
inline bool more(T statistic, decltype(statistic) crit) { return statistic >= crit; }

void dixanalyz::binimCritThetaEqual(size_t B, size_t n, double p0, double alpha, QStringList& result) {

    bool aproximate = false, ispr = false;

    double B_approximate = (B - n*p0) / (sqrt( (p0*(1.0 - p0))/n ));
    B_approximate = (ispr) ? (B + 0.5 - n*p0) / (sqrt( (p0*(1.0 - p0))/n )) : B_approximate ;

    size_t upper_b = (aproximate) ? 1.96 : binomialUpperAlpha(n, alpha, p0);
    size_t lower_b = (aproximate) ? -1.96 : binomialLowerAlpha(n, alpha, p0);

    QString p0_str = QString::number(p0), more_hypot, less_hypot;
    if(aproximate) {
        more_hypot = ( (more(B_approximate, upper_b)) ? "Принимается" : "Отвергается" ) ;
        less_hypot = ( (less(B_approximate, lower_b)) ? "Принимается" : "Отвергается" ) ;
    }
    else {
        more_hypot = ( (B >= upper_b) ? "Принимается" : "Отвергается" ) ;
        less_hypot = ( (B <= lower_b) ? "Принимается" : "Отвергается" ) ;
    }

    result << "Верхняя alpha-% точка = " + to_str(upper_b)
           << "Нижняя alpha-% точка = " + to_str(lower_b);

    if(aproximate) result << "Аппроксимация B: " + to_str(B_approximate);

    result << "Гипотеза p > " + p0_str + more_hypot
           << "Гипотеза p > " + p0_str + less_hypot;
}

void dixanalyz::simpletest(const Sample& sample, OutputData& result) {
    // something
    result.addValue("Simple value", to_str(999));
}
