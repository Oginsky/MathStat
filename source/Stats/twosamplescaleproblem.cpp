#include "twosamplescaleproblem.h"

#include "commonfunc.h"

#include <cmath>


/* Вспомогательное */

struct PosPair {
    double value;
    bool pos;

    PosPair(double value = 0, bool pos = false) {
        this->value = value;
        this->pos = pos;
    }

};

bool comparator(const PosPair& left, const PosPair& right) {
    return left.value < right.value;
}

void special_ranks(const Sample& data1, const Sample& data2, QVector<PosPair>& ranks) {
    size_t size1 = data1.size(), size2 = data2.size(), size = size1 + size2;
    ranks.resize(size);

    size_t index = 0;
    for(size_t i = 0; i < size1; ++i, ++index) ranks[i] = PosPair(data1[i], false);
    for(size_t i = 0; i < size2; ++i, ++index) ranks[i] = PosPair(data2[i], true);
    std::stable_sort(ranks.begin(), ranks.end(), comparator);

    index = size / 2;
    for(size_t i = 0; i < index; ++i) ranks[i].value = i+1;
    for(size_t i = size-1, r = 1; i >= index; --i, ++r) ranks[i].value = r;

    if(size % 2 == 1) {
        ranks[index].pos = (size - 1.0) / 2.0;
        ranks[index+1].pos = (size + 1.0) / 2.0;
        ranks[index+2].pos = (size - 1.0) / 2.0;
    }
}

void sigel_ranks(const Sample& data1, const Sample& data2, QVector<PosPair>& ranks) {
    size_t size1 = data1.size(), size2 = data2.size(), size = size1 + size2;
    ranks.resize(size);

    size_t index = 0;
    for(size_t i = 0; i < size1; ++i, ++index) ranks[i] = PosPair(data1[i], false);
    for(size_t i = 0; i < size2; ++i, ++index) ranks[i] = PosPair(data2[i], true);
    std::stable_sort(ranks.begin(), ranks.end(), comparator);

    size_t i = 1;
    for(i; i <= size_t(size/2); ++i) ranks[i-1] = (i%2) ? 2*i - 1 : 2*i;
    for(i; i <= size; ++i) ranks[i-1] = (i%2) ? (2*(size-i+1)) : (2*(size-i) + 1);

}


/* Дополнительные заказы */

void continuityCorrection(double statistic, double M, double D, double crit, double crit2, OutputData& result);


/* Критерии */

void TwoSampleScaleProblem::ansariBradleyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result) {
    size_t m = data1.size(), n = data2.size(), N = m + n;
    QVector<PosPair> ranks;
    special_ranks(data1, data2, ranks);

    double statistic(0);
    for(auto& val: ranks)
        if(!val.pos) statistic += val.value;    // Сумма рангов относящихся к первой выборке

    double summ(0.0);
    bool corrects = has_same_value(data1, data2);
    if(corrects) {
        QMap<double, size_t> groups;
        QMap<double, double> rank;
        Sample common = data1 + data2;
        ranking(common, rank);
        uniq_group(data1 + data2, groups);
        for(auto it = groups.begin(); it != QMap<double, size_t>::iterator(groups.end()); ++it)
            summ += it.value()*square(rank[it.key()]);
    }

    double M(0.0), D(0.0);
    if(N % 2 == 0) {
        M = ( m*(m + n + 2.0) ) / 4.0;
        if(corrects) D = (m*n*(16.0*summ - (m+n)*square(m+n+2.0))) / (16.0*square(m+n)*(m+n-1.0));
        else D = ( m*n*(m+n+2.0)*(m+n-2.0) ) / (48.0*(m+n-1.0));
    }
    else {
        M = ( m*square(m + N + 1.0) ) / (4.0*(m+n));
        if(corrects) D = ( m*n*(16.0*(m+n)*summ - square(square(m+n+1.0))) ) / ( 16.0*square(m+n)*(m+n-1.0) );
        else D = ( m*n*(m+n+1.0)*(3.0 + square(m+n)) ) / (48*square(m+n));
    }

    double aprox_stat = (statistic - M) / std::sqrt(D);
    double normal_crit = CriticalValue::normal(1.0 - alpha);
    double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий Ансари-Брэдли: асимптотика");
    hypoth.addHypoth(str_const::hypoth_more + " 1", more(aprox_stat, normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less + " 1", less(aprox_stat, -normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq + " 1", more(std::abs(aprox_stat),normal_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);
}

void TwoSampleScaleProblem::sigelTukeyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result) {
    size_t m = data1.size(), n = data2.size(), N = m + n;
    QVector<PosPair> ranks;
    sigel_ranks(data1, data2, ranks);

    size_t statistic(0);
    for(auto val: ranks)
        if(val.pos) statistic + val.value;

    double M = n*m/2.0;
    double D = m*n*(m + n + 1.0) / 12.0;
    if(has_same_value(data1, data2)) {
        QMap<double, size_t> groups;
        uniq_group(data1 + data2, groups);
        double summ(0);
        for(auto val: groups.values()) summ += val*(square(val));
        D -= (m*n*summ) / (12.0*(m + n)*(m + n - 1.0));
    }

    double aprox_stat = (statistic - M) / std::sqrt(D);
    double normal_crit = CriticalValue::normal(1.0 - alpha);
    double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий Ансари-Брэдли: асимптотика");
    hypoth.addHypoth(str_const::hypoth_more + " 1", more(aprox_stat, normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less + " 1", less(aprox_stat, -normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq + " 1", more(std::abs(aprox_stat),normal_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);

    // Дополнительные заказы
    if(flags.contains(FLAG::ContinuityCorrection)) continuityCorrection(aprox_stat, M, D, normal_crit, normal_crit2, result);
}

void TwoSampleScaleProblem::mosesTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result) {
    size_t k = 4;               // Количество групп
    size_t size1 = data1.size() / k, size2 = data2.size() / k;
    QVector<double> C(k, 0.0), D(k, 0.0), MX(k, 0.0), MY(k, 0.0);

    // Средние значения
    for(size_t i = 0 ; i < k; ++i) {
        for(size_t j = 0; j < size1; ++j) {
            size_t idx = i*size1 + j;
            MX[i] += data1[idx];
        }

        for(size_t j = 0; j < size2; ++j) {
            size_t idx = i*size2 + j;
            MY[i] += data2[idx];
        }
    }
    for(size_t i = 0; i < k; ++i)
        MX[i] /= size1, MY[i] /= size2;

    // Ранги
    for(size_t i = 0 ; i < k; ++i) {
        for(size_t j = 0; j < size1; ++j) {
            size_t idx = i*size1 + j;
            C[i] += square(data1[idx] - MX[i]);
        }

        for(size_t j = 0; j < size2; ++j) {
            size_t idx = i*size2 + j;
            D[i] += square(data2[idx] - MY[i]);
        }
    }

    // Общие ранги
    C += D;
    QMap<double, double> ranks;
    ranking(C, ranks);

    double statistic(0.0);
    for(auto val: D) statistic += ranks[val];

    double m = k*(2*k + 1.0) / 2.0;
    double disp = (k*k* (2*k + 1.0)) / 12.0;
    if(ranks.size() != 2*k) {
        QMap<double, size_t> groups;
        uniq_group(C, groups);
        disp = disp_with_corrects(k, k, groups);
    }

    double aprox_stat = (statistic - m) / std::sqrt(disp);
    double normal_crit = CriticalValue::normal(1.0 - alpha);
    double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий Мозеса: асимптотика");
    hypoth.addHypoth("ln(σ1/σ2) > 0", more(aprox_stat, normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth("ln(σ1/σ2) < 0", less(aprox_stat, -normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth("ln(σ1/σ2) ≠ 0", more(std::abs(aprox_stat),normal_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);


}


void continuityCorrection(double statistic, double M, double D, double crit, double crit2, OutputData& result) {
    double correct_stat = ( std::abs(statistic - M) - 0.5 ) / std::sqrt(D);
    if(statistic < M) correct_stat = -correct_stat;
    result.addValue("Нормировка с коррекцией", correct_stat);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий Ансари-Брэдли: асимптотика");
    hypoth.addHypoth(str_const::hypoth_more + " 1", more(correct_stat, crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less + " 1", less(correct_stat, -crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq + " 1", more(std::abs(correct_stat),crit) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);
}
