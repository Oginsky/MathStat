#include "shiftproblem.h"

#include <algorithm>
#include <cmath>

#include <QVector>
#include <QMap>

//#include "RandLib.h"
#include "commonfunc.h"

void averagesWilsha(const QVector<double>& zi, QVector<double>& wi) {
    size_t n = zi.size(), m = (n*(n+1)) / 2;
    wi.resize(m);

    size_t index = 0;
    for(size_t i = 0; i < n; ++i)
        for(size_t j = i; j < n; ++j)
            wi[index++] = (zi[i] + zi[j]) / 2;

    std::sort(wi.begin(), wi.end());
}


void ImansStatistic(double aprox_s, double norm_crit, double level, size_t size, OutputData& result) {
    double imans_aprox = (aprox_s / 2.0) * (1.0 + sqrt( (size - 1.0) / (size - aprox_s*aprox_s) ) );
    double student_crit = CriticalValue::studentT(size-1, level);
    double imans_crit = (norm_crit + student_crit) / 2.0;

    // Выводы по аппроксимации Имана
    OutputData::CriterialHypoth hypoths("Аппроксимация Имана");
    hypoths.addHypoth(str_const::hypoth_more_zero, (imans_aprox >= imans_crit) ? str_const::accept : str_const::cancel);
    hypoths.addHypoth(str_const::hypoth_less_zero, (imans_aprox <= -imans_crit) ? str_const::accept : str_const::cancel);
    double imans_crit2 = (CriticalValue::normal(level/2) + CriticalValue::studentT(size-1, level/2) ) / 2.0;
    hypoths.addHypoth(str_const::hypoth_noteq_zero, (std::abs(aprox_s) >+ imans_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoths.name, hypoths);
}

void ShiftProblem::wilcoxonSignRank(const Sample& before, const Sample& after, double level, QSet<FLAG> flags, OutputData& result) {
    size_t size = before.size();

    // Ранги для расчета статистики
    QVector<double> Z(size);
    for(size_t i = 0; i < size; ++i) Z[i] = std::abs(after[i] - before[i]);
    QMap<double, double> rank;
    ranking(Z, rank);

    // Расчет статистики
    double statistic = 0;
    for(double val: Z) statistic += (val > 0) ? rank[val] : -rank[val];

    bool was_avg = rank.size() != size; // Значит есть повторяющиеся значение, то есть связи

    // Расчет апроксимации нормальным распределением
    double mat_s     = size*(size + 1.0) / 4.0;
    double disp_s    = (size*(size + 1.0)*(2.0*size + 1.0)) / 24.0;
    if(was_avg) {
        // Коррекция дисперсии на связи
        QMap<double, size_t> groups;
        uniq_group(Z, groups);
        int sum = 0;
        for(auto val: groups) {
           int _count = groups[val];
           sum += _count * (_count + 1) * (_count - 1);
        }
        disp_s -= sum / 48.0;
    }
    // Аппроксимированная статистика критерия и критическое значение аппроксимации
    double aprox_s   = (statistic - mat_s) / sqrt(disp_s);
    double norm_crit = CriticalValue::normal(level);

    // Выводы о гипотезе
    OutputData::CriterialHypoth hypoths("Критерий знаковых рангов Уилкоксона");
    hypoths.addHypoth(str_const::hypoth_more_zero, (aprox_s >= norm_crit) ? str_const::accept : str_const::cancel);
    hypoths.addHypoth(str_const::hypoth_less_zero, (aprox_s <= -norm_crit) ? str_const::accept : str_const::cancel);
    double norm_crit2 = CriticalValue::normal(level/2);
    hypoths.addHypoth(str_const::hypoth_noteq_zero, (std::abs(aprox_s) >+ norm_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoths.name, hypoths);

    // Аппроксимация Имана, на основе лин комб нормального и стьюдент распределения
    if(flags.contains(FLAG::ImansAproximate)) ImansStatistic(aprox_s, norm_crit, level, size, result);

}

void ShiftProblem::wilcoxonSignRankEstimation(const Sample& before, const Sample& after,  OutputData& result) {
    size_t size = after.size();
    QVector<double> zi(size);

    for(size_t i = 0; i < size; ++i) zi[i] = after[i] - before[i];

    QVector<double> wi;
    averagesWilsha(zi, wi);
    size_t m = wi.size();

    double estimation = (m % 2 == 1) ? wi[m/2] : ((wi[m/2] + wi[m/2 + 1])/2);
    result.addValue(str_const::point_estimation, to_str(estimation));
}

void ShiftProblem::wilcoxonSignRankInterval(const Sample& before, const Sample& after, double level, OutputData& result) {
    size_t size = after.size();
    QVector<double> zi(size);
    for(size_t i = 0; i < size; ++i) zi[i] = after[i] - before[i];

    // Построение средних Уолша
    QVector<double> wi;
    averagesWilsha(zi, wi);

    // Индексы в вариационном ряде Уолша
    size_t sizem = (size*(size+1)) / 2;
    double norm_crit = CriticalValue::normal(level/2.0);
    size_t left_index = size*(size+1.0) / 4.0  - norm_crit*sqrt( (size*(size+1.0)*(2.0*size + 1.0)) / 24.0 );
    size_t right_index = sizem + 1 - left_index;

    // Границы доверительного интервала
    double left_range = wi[left_index];
    double right_range = wi[right_index];

    result.addExtra(str_const::approximate, ": N(0,1), z_alpha = " + to_str(norm_crit));
    result.addValue(str_const::interval_estimation, "(" + to_str(left_range) + ", " + to_str(right_range) + ")");
}


void SignRank::signRank(const Sample& before, const Sample& after, double level, QSet<FLAG> flags, OutputData& result) {
    size_t statistic = 0, size = after.size();

    for(size_t i = 0; i < size; ++i)
        if(after[i] - before[i] > 0)
            statistic++;

    bool aproximate = flags.contains(FLAG::NormalAproximate);
    size_t binom_crit = (!aproximate) ? binomialLowerAlpha(size, level, 0.5) : CriticalValue::normal(level);
    if(aproximate) statistic = (statistic - size/2) / (sqrt(size/4));

    OutputData::CriterialHypoth hypoth( (aproximate) ? "Аппроксимация нормальным распределением" : "Точное решение с биномиальным распределением");
    hypoth.addHypoth(str_const::hypoth_less_zero, (statistic <= size - binom_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_more_zero, (statistic >= binom_crit) ? str_const::accept : str_const::cancel);

    double binom_crit2 = (!aproximate) ? binomialLowerAlpha(size, level/2.0, 0.5) : CriticalValue::normal(level/2.0);
    QString eq_or_not;
    if(aproximate) eq_or_not = (statistic > binom_crit2) ? str_const::cancel : str_const::accept;
    else eq_or_not =(statistic >= binom_crit2 || statistic <= size - binom_crit2) ? str_const::cancel : str_const::accept;
    hypoth.addHypoth(str_const::hypoth_noteq_zero, eq_or_not);

    result.addHypoth(hypoth.name, hypoth);
}

void SignRank::signRankEstimation(const Sample& before, const Sample& after, OutputData& result) {
    // Разности для расчета оценки
    QVector<double> Z(before.size());
    for(size_t i = 0; i < size_t(before.size()); ++i) Z[i] = std::abs(after[i] - before[i]);
    std::sort(Z.begin(), Z.end());

    double estimation = median(Z);
    result.addValue(str_const::point_estimation, to_str(estimation));
}

void SignRank::signRankInterval(const Sample& before, const Sample& after, double level, QSet<FLAG>& flags, OutputData& result) {
    size_t size = before.size();

    // Разности для расчета оценки
    QVector<double> Z(before.size());
    for(size_t i = 0; i < size_t(before.size()); ++i) Z[i] = std::abs(after[i] - before[i]);
    std::sort(Z.begin(), Z.end());

    size_t left_index = size + 1 - binomialUpperAlpha(size, level/2.0, 0.5);
    size_t right_index = size + 1 - left_index;

    double left_range = Z[left_index], right_range = Z[right_index];

    result.addValue(str_const::interval_estimation, "(" + to_str(left_range) + ", " + to_str(right_range) + ")");

    if(flags.contains(FLAG::NormalAproximate)) {
        left_index = (size / 2.0) - CriticalValue::normal(level/2.0) * std::sqrt(size / 4.0);
        right_index = size + 1 - left_index;
        left_range = Z[left_index];
        right_range = Z[right_index];

        result.addValue(str_const::interval_estimation, "(" + to_str(left_range) + ", " + to_str(right_range) + ")");
    }
}
