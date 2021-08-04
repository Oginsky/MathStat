#include "twosampleshiftproblem.h"

#include "commonfunc.h"

#include <cmath>


/* Дополнительные заказы */

void medianTestRanksForm(size_t size1, size_t size2, size_t m1, double alpha, OutputData& result);

void pointEstimate(const Sample& data1, const Sample& data2, OutputData& result);

void confidenceInterval(const Sample& data1, const Sample& data2, double alpha, OutputData& result);

void imansStatistic(double normal_stat, size_t size1, size_t size2, double alpha, OutputData& result);

void continuityCorrection(double statistic, double M, double D, OutputData& result);

void effectSize(double statistic, double aprox, const Sample& data1, const Sample& data2, Orders orders, OutputData& result);


/* Критерии */

void TwoSampleShiftProblem::medianTestMuda(const Sample& data1, const Sample& data2, double alpha, const QSet<FLAG>& flags, OutputData& result) {
    size_t size1(data1.size()), size2(data2.size());
    Sample common = data1 + data2;
    common.makeSort();

    double median_value = median( common.data() );

    auto less_median = [median_value](double val) { return val <= median_value; };
    auto more_median = [median_value](double val) { return val > median_value;  };

    size_t l1 = std::count_if(data1.cbegin(), data1.cend(), less_median),   // Количество меньших медианы в первой выборке
           l2 = std::count_if(data2.cbegin(), data2.cend(), less_median),   // Количество меньших мединаны во второй выборке
           m1 = size1 - l1,                                                 // Количество больших медианы в первой выборке
           m2 = size2 - l2;                                                 // Количество больших медианы в первой выборке
    size_t L(l1 + l2), M(m1 + m2), N(size1 + size2);                        // Суммарные величины

    // Таблица сопряженности
    QVector<QVector<size_t>> table = {
        {m1, l1, size1},
        {m2, l2, size2},
        {M,  L,  N}
    };

    double statistic(0);
    for(size_t i = 0; i < 2; ++i)
        for(size_t j = 0; j < 2; ++j)
            statistic += square( (double)table[i][j] - (double)table[i][2]*table[2][j] / (double)N ) / ((double)table[i][2]*table[2][j]);
    statistic *= N;

    double chi_crit = CriticalValue::chiSquared(1, 1.0 - alpha);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Медианный критерий (хи-квадрат статистика)");
    hypoth.addHypoth(str_const::hypoth_noteq_zero + " (Параметр сдвига)", more(statistic, chi_crit) ? str_const::accept : str_const::cancel);


    if(flags.contains(FLAG::Ranks)) medianTestRanksForm(size1, size2, m1, alpha, result);
    if(flags.contains(FLAG::ChiSquareCorrection)) {
        double stat(0);
        for(size_t i = 0; i < 2; ++i) {
            for(size_t j = 0; j < 2; ++j) {
                double val = std::abs((double)table[i][j] - (double)table[i][2]*table[2][j] / (double)N) - 0.5;
                statistic += square( val ) / ((double)table[i][2]*table[2][j]);
            }
        }
        stat *= N;
        hypoth.addHypoth(str_const::hypoth_noteq_zero + " (Поправка Йейтса)", more(stat, chi_crit) ? str_const::accept : str_const::cancel);
    }

    result.addHypoth(hypoth.name, hypoth);
}

void TwoSampleShiftProblem::wilcoxonSummRank(const Sample &data1, const Sample &data2, double alpha, const QSet<FLAG> &flags, OutputData &result) {
    size_t size1(data1.size()), size2(data2.size());
    Sample common(data1 + data2);
    QMap<double, double> ranks;         // Ранги по объедененной выборке
    ranking(data1 + data2, ranks);

    double statistic(0);
    for(size_t i = 0; i < size2; ++i) statistic += ranks[data2[i]];     // Сумма рангов относящихся к data2 в общей выборке

    // Критические точки нормального распределения
    double normal_crit = CriticalValue::normal(1.0 - alpha);
    double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    double M = size1 * (size1 + size2 + 1.0) / 2.0;
    double D = size1*size2 * (size1 + size2 + 1.0) / 12.0;

    if(ranks.size() < size1) {
        // Значит есть связи и нужно сделать коррекцию на них
        QMap<double, size_t> groups;
        uniq_group(common, groups);
        size_t summ(0);
        for(size_t val: groups.values()) summ += val*(square(val) - 1);

        D += size1*size2*summ / ( 12.0 * (size1 + size2)*(size1 + size2 - 1.0) );
    }

    double aprox_stat = (statistic - M) / std::sqrt(D);         // Нормированная статистика

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий ранговых сумм Уилкоксона: апроксимация");
    hypoth.addHypoth(str_const::hypoth_more_zero, more(aprox_stat, normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less_zero, less(aprox_stat, -normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq_zero, more(std::abs(aprox_stat),normal_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);

    // Дополнительные заказы
    if(flags.contains(FLAG::ImansAproximate))       imansStatistic(aprox_stat, size1, size2, alpha, result);
    if(flags.contains(FLAG::PointEstimate))         pointEstimate(data1, data2, result);
    if(flags.contains(FLAG::ConfidenceInterval))    confidenceInterval(data1, data2, alpha, result);

}

void TwoSampleShiftProblem::mannWhitneyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, Orders orders, OutputData& result) {
     size_t size1(data1.size()), size2(data2.size());
     double statistic(0);

     for(size_t i = 0; i < size1; ++i)
         for(size_t j = 0; j < size2; ++j)
             statistic += (data1[i] < data2[j]) ? 1
                                                : ( data1[i] == data2[j] ? 1 : 0 );

     double M = size1*size2 / 2.0;
     double D = (size1*size2)*(size1 + size2 + 1.0) / 12.0;
     if(has_same_value(data1, data2)) {
         // Коррекция на связи
         QMap<double, size_t> groups;
         uniq_group(data1.data() + data2.data(), groups);
         size_t summ(0);
         for(auto val: groups.values()) summ += val*(square(val) - 1);
         D += ( (size1 * size2) * summ ) / (12.0 * (size1*size2)*(size1 + size2 + 1.0));
     }

     double aprox_stat = (statistic - M) / std::sqrt(D);
     double normal_crit = CriticalValue::normal(1.0 - alpha);
     double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

     // Выводы по гипотезе
     OutputData::CriterialHypoth hypoth("Критерий ранговых сумм Уилкоксона: апроксимация");
     hypoth.addHypoth(str_const::hypoth_more_zero, more(aprox_stat, normal_crit) ? str_const::accept : str_const::cancel);
     hypoth.addHypoth(str_const::hypoth_less_zero, less(aprox_stat, -normal_crit) ? str_const::accept : str_const::cancel);
     hypoth.addHypoth(str_const::hypoth_noteq_zero, more(std::abs(aprox_stat),normal_crit2) ? str_const::accept : str_const::cancel);
     result.addHypoth(hypoth.name, hypoth);

     // Дополнительные заказы
     if(flags.contains(FLAG::ContinuityCorrection)) continuityCorrection(statistic, M, D, result);
     if(flags.contains(FLAG::EffectSize)) effectSize(statistic, aprox_stat, data1, data2, orders, result);
}

void TwoSampleShiftProblem::flynnerPolycelloTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result) {
    if(data1.size() <= 12 || data2.size() <= 12) {
        result.addValue("Размеры выборок должны быть больше 12", "");
        return;
    }

    size_t size1(data1.size()), size2(data2.size());
     auto N = [](const Sample& data, double value, bool reverse = false) {
         double result;
         for(size_t i = 0; i < size_t(data.size()); ++i)
             if(!reverse) result += (data[i] < value) ? 1.0 : ( data[i] == value ? 0.5 : 0.0);
             else result += (value < data[i]) ? 1.0 : ( data[i] == value ? 0.5 : 0.0);
         return result;
     };


     double mx(0.0), my(0.0), sx(0.0), sy(0.0);
     for(auto x: data1.data()) mx += N(data2, x);
     for(auto y: data2.data()) my += N(data1, y, true);
     mx /= (double)size1;
     my /= (double)size2;

     for(auto x: data1.data()) sx += square(N(data2, x) - mx);
     for(auto y: data2.data()) sx += square(N(data1, y, true) - my);

     double val = (size1*mx + size2*my) / (size1 * size2);
     double val2 = (4.0 * (sx + sy + mx * my)) / square(size1*size2);
     double stat = val / std::sqrt(val2);
     double normal_crit = CriticalValue::normal(1.0 - alpha);
     double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

     // Выводы по гипотезе
     OutputData::CriterialHypoth hypoth("Стохастическое равенство");
     hypoth.addHypoth(str_const::hypoth_more_zero, more(stat, normal_crit) ? str_const::accept : str_const::cancel);
     hypoth.addHypoth(str_const::hypoth_less_zero, less(stat, -normal_crit) ? str_const::accept : str_const::cancel);
     hypoth.addHypoth(str_const::hypoth_noteq_zero, more(std::abs(stat),normal_crit2) ? str_const::accept : str_const::cancel);
     result.addHypoth(hypoth.name, hypoth);
}


void medianTestRanksForm(size_t size1, size_t size2, size_t m1, double alpha, OutputData& result) {
    double M = size1 / 2.0;
    double D = (size1 + size2 % 2 == 0) ? (size1 * size2) / (4.0 * (size1 + size2 - 1.0))
                                        : (size1 * size2) / (4.0 * (size1 + size2));
    double statistic = (m1 - M) / D;
    double norm_crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Медианный критерия (Ранговая форма)");
    hypoth.addHypoth(str_const::hypoth_noteq_zero + " (Параметр сдвига)", more(std::abs(statistic), norm_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);
}

void pointEstimate(const Sample& data1, const Sample& data2, OutputData& result) {
    size_t size1(data1.size()), size2(data2.size());
    QVector<double> arr;
    arr.reserve(size1*size2);

    for(size_t i = 0; i < size2; ++i)
        for(size_t j = 0; j < size1; ++j)
            arr.push_back( data2[i] - data1[j] );

    std::sort(arr.begin(), arr.end());
    double point = median(arr);

    result.addValue("Точечная оценка", point);
}

void confidenceInterval(const Sample& data1, const Sample& data2, double alpha, OutputData& result)  {
    size_t size1(data1.size()), size2(data2.size());
    QVector<double> arr;
    arr.reserve(size1*size2);

    for(size_t i = 0; i < size2; ++i)
        for(size_t j = 0; j < size1; ++j)
            arr.push_back( data2[i] - data1[j] );

    std::sort(arr.begin(), arr.end());

    double val = (size1*size2*(size1 + size2 + 1.0)) / 12.0;
    double left_index = std::round( size1*size2 / 2.0 - CriticalValue::normal(1.0 - alpha) * std::sqrt(val) ) ;
    double right_index = size1*size2 + 1 - left_index;
    double left_range = arr[left_index], right_range = arr[right_index];

    result.addValue(str_const::interval_estimation + " Норм. распр.", str_const::interval(left_range, right_range));
}

void imansStatistic(double normal_stat, size_t size1, size_t size2, double alpha, OutputData& result) {

    double val = (size1 + size2 - 2.0) / (size1 + size2 - 1.0 - square(normal_stat));
    double imans_stat = (normal_stat/2.0) * ( 1.0 + std::sqrt(val) );

    double norm_crit = CriticalValue::normal(1.0 - alpha);
    double norm_crit2 = CriticalValue::normal(1.0 - alpha/2.0);
    double stud_crit = CriticalValue::studentT(size1 + size2 - 2, 1.0 - alpha);
    double stud_crit2 = CriticalValue::studentT(size1 + size2 - 2, 1.0 - alpha/2.0);

    double imans_crit = (norm_crit + stud_crit) / 2.0;
    double imans_crit2 = (norm_crit2 + stud_crit2) / 2.0;

    // Выводы по гипотезе
    OutputData::CriterialHypoth hypoth("Критерий ранговых сумм Уилкоксона: апроксимация Имана");
    hypoth.addHypoth(str_const::hypoth_more_zero, more(imans_stat, imans_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less_zero, less(imans_stat, -imans_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq_zero, more(std::abs(imans_stat), imans_crit) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);
}

void continuityCorrection(double statistic, double M, double D, OutputData& result) {
    double aprox_correct_plus = ( std::abs(statistic - M) - 0.5 ) / std::sqrt(D);
    result.addValue("Нормировка с коррекцией", aprox_correct_plus);
}

void effectSize(double statistic, double aprox, const Sample& data1, const Sample& data2, Orders orders, OutputData& result) {
    size_t size1(data1.size()), size2(data2.size());
    if(orders.contains(Order::r_Ratio)) {
        double r_ratio = square(aprox) / (size1 + size2);
        result.addValue(Order::r_Ratio, r_ratio);
    }
    if(orders.contains(Order::KliffDelta)) {
        double kliff_delta = (2.0 * statistic) / (size1*size2) - 1.0;
        result.addValue(Order::KliffDelta, kliff_delta);
    }
    if(orders.contains(Order::DominatedProbability)) {
        size_t count_more(0);
        for(size_t i = 0; i < size2; ++i)
            for(size_t j = 0; j < size1; j++)
                if(data2[i] > data1[j]) count_more++;
        double ps = (double)count_more / (size1*size2);
        result.addValue(Order::DominatedProbability, ps);
    }
    if(orders.contains(Order::A_ValueWardDelani)) {
        double A = statistic / (size1 * size2);
        result.addValue(Order::A_ValueWardDelani, A);
    }
}
