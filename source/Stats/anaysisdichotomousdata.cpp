#include "anaysisdichotomousdata.h"

#include <cmath>

#include "RandLib.h"

#include "commonfunc.h"


double count_evaluation(const Sample& data);


// Аппроксимация нормальным распределением
void binomialCriterialApprox(size_t size, double p0, size_t statistic, double alpha, OutputData& result) {
    // Мат. ожидание и дисперсия биномиального распределения для аппроксимации
    double M = size * p0;
    double D = M * (1.0 - p0);

    // Аппроксимированная статистика и критические точки
    double approx_stat = (statistic - M) / std::sqrt(D);
    double normal_crit = CriticalValue::normal(1.0 - alpha);
    double normal_crit2 = CriticalValue::normal(1.0 - alpha / 2.0);

    // Выводы по гипотезе
    QString p0_str(" " + to_str(p0));
    OutputData::CriterialHypoth hypoth("Биномиальный критерий: аппроксимация");
    hypoth.addHypoth(str_const::hypoth_more + p0_str, more(approx_stat, normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less + p0_str, less(approx_stat, -normal_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq + p0_str, more(std::abs(approx_stat),normal_crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);
}

void binimialCriterialAddInfo(double p0, size_t size, size_t lower_crit, size_t upper_crit, OutputData& result) {
    BinomialRand binomial(size, p0);

    double beta1 = binomial.S(upper_crit) + binomial.P(upper_crit),
           beta2 = binomial.F(lower_crit),
           beta3 = beta1 + beta2;

    result.addExtra("№1 Мощность и вер. ошибки 2 рода: ",
                    to_str(beta1) + ", " + to_str(1.0 - beta1));
    result.addExtra("№2 Мощность и вер. ошибки 2 рода: ",
                    to_str(beta2) + ", " + to_str(1.0 - beta2));
    result.addExtra("№3 Мощность и вер. ошибки 2 рода: ",
                    to_str(beta2) + ", " + to_str(1.0 - beta2));
}

void Dichotomical::binomialCriterial(const Sample& data, double p0, double alpha, const QSet<FLAG>& flags, OutputData& result) {
    size_t statistics(0), size(data.size());
    for(size_t i = 0; i < size; ++i) statistics += data[i];

    // Критические точки
    double level = 1.0 - alpha;
    size_t upper_crit = CriticalValue::binomial(size, level, p0);
    size_t lower_crit = binomialLowerAlpha(size, level, p0);

    // Выводы по гипотезе
    QString p0_str(" " + to_str(p0));
    OutputData::CriterialHypoth hypoth("Биномиальный критерий: точное решение");
    hypoth.addHypoth(str_const::hypoth_more + p0_str, more(statistics, upper_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_less + p0_str, less(statistics, lower_crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(str_const::hypoth_noteq + p0_str, in_range(statistics, lower_crit, upper_crit) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);

    // Дополнительные заказы
    if(flags.contains(FLAG::NormalAproximate)) binomialCriterialApprox(size, p0, statistics, level, result);
    if(flags.contains(FLAG::StatisticsInfo)) binimialCriterialAddInfo(p0, size, lower_crit, upper_crit, result);
}


void confidenceInterval(size_t statistic, size_t size, double p, double alpha, const QSet<QString>& orders, OutputData& result) {
    if(orders.contains(Order::CopperPearsonInterval)) {
        if(statistic <= 0) statistic = 1;
        double left_range = CriticalValue::beta(statistic, size - statistic + 1, 1.0 - alpha);
        double right_range = CriticalValue::beta(statistic, size - statistic + 1, 1.0 - alpha/2.0);

        result.addValue(str_const::interval_estimation + " Коппера Пирсона: ", str_const::interval(left_range, right_range));
    }

    if(orders.contains(Order::WaldApproximation)) {
        double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);
        double val = std::sqrt( (p * (1.0 - p)) / size );
        double left_range = p - normal_crit2 * val;
        double right_range = p + normal_crit2 * val;

        result.addValue(str_const::interval_estimation + " Вальда: ", str_const::interval(left_range, right_range));
    }

    if(orders.contains(Order::WilsonInterval)) {
        double crit = CriticalValue::normal(1.0 - alpha/2.0);
        double critsq = square(crit);

        // Решение уравнения доверительного интервала Вилсона
        double l = 1.0 / 2.0 * size;
        double a = 1 + critsq / (double)size,
               b = 2.0*l*critsq - critsq / (double)size,
               c = 0.0;
        double d = square(b) - 4.0 * a * c;
        if(d >= 0) {
            double p = (-b + std::sqrt(d)) / (2.0 * a);
            result.addValue("Значение Вилсона", "+/- " + to_str(p));
        }
    }
}


void Dichotomical::esvaluationProbability(const Sample& data, double alpha,
                                          const QSet<QString>& orders, const QSet<FLAG>& flags, OutputData& result)
{

    size_t size(data.size());
    double sqrt_size(std::sqrt(size));
    double level = 1.0 - alpha;
    size_t statistic = std::count_if(data.cbegin(), data.cend(), [](double val) { return val != 0.0; });

    // Частотная оценка
    double count_p(0);
    count_p = (double)statistic / (double)size;
    result.addValue("Частотная оценка", to_str(count_p));


    // Заказы
    // Оценка Ходжеса и Лемана
    if(orders.contains(Order::HodgesLehmanEstimate)) {
         double xod_p = (count_p * sqrt_size + 0.5) / (1 + sqrt_size);
         result.addValue(Order::HodgesLehmanEstimate, to_str(xod_p));
    }

    // Заказ на доверительный интервал
    if(flags.contains(FLAG::ConfidenceInterval)) confidenceInterval(statistic, size, count_p, alpha, orders, result);
}


void diffInterval(const Sample& data1, const Sample& data2, double p1, double p2, double alpha, OutputData& result) {
    size_t size1(data1.size()), size2(data2.size());
    size_t b1 = std::count_if(data1.cbegin(), data1.cbegin(), [](const double val) { return val != 0.0; });
    size_t b2 = std::count_if(data2.cbegin(), data2.cbegin(), [](const double val) { return val != 0.0; });

    double p = ( (double)(p1 + p2) ) / ( (double)(data1.size() + data2.size()) );

    double statistic = (p1 - p2)*(p1 - p2) / ( p * (1.0 - p) * (1.0/size1 + 1.0/size2) );
    double chi2_crit = CriticalValue::chiSquared(1, 1.0 - alpha);

    // Выводы по гипотезе
    QString noteq_h("p1 != p2");
    OutputData::CriterialHypoth hypoth("Гипотеза о равенстве вероятностей");
    hypoth.addHypoth(noteq_h, more(statistic, chi2_crit) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);

    // Интервал
    double norm_crit2 = CriticalValue::normal(1.0 - alpha/2.0);
    double val = norm_crit2 * std::sqrt( p1*(1.0 - p1)/size1 + (p2*(1.0 - p2)/size2) );
    double left_range = p1 - p2 - val;
    double right_range = p1 - p2 + val;

    result.addValue(str_const::interval_estimation + " для разности: ", str_const::interval(left_range, right_range));
}

void probabilityRatioInterval(double p1, double p2, size_t size1, size_t size2, double alpha, OutputData& result) {
    double ratio = p1 / p2;
    double normal_crit2 = CriticalValue::normal(1.0 - alpha/2.0);
    double val = (1.0 - p1) / (p1 * size1) + (1.0 - p2) / (p2 * size2);

    double left_range = ratio * std::exp( -normal_crit2 * std::sqrt(val) );
    double right_range = ratio * std::exp( normal_crit2 * std::sqrt(val) );

    result.addValue(str_const::interval_estimation + " для отношения: ", str_const::interval(left_range, right_range));

}

void Dichotomical::compareProbability(const Sample& data1, const Sample& data2, double alpha,
                                      const QSet<QString>& orders, const QSet<FLAG>& flags, OutputData& result)
{
    size_t size1(data1.size()), size2(data2.size());
    double p1(count_evaluation(data1)), p2(count_evaluation(data2));

    double val1 = (p1*(1.0 - p2)) / size1;
    double val2 = (p2*(1.0 - p2)) / size2;

    double stat = (p1 - p2) / std::sqrt( val1 + val2);
    double crit = CriticalValue::normal(1.0 - alpha);
    double crit2 = CriticalValue::normal(1.0 - alpha/2.0);

    // Выводы по гипотезе
    QString more_h("p1 > p2"), less_h("p1 < p2"), noteq_h("p1 ≠ p2");
    OutputData::CriterialHypoth hypoth("Сравнение на основе N(0, 1)");
    hypoth.addHypoth(more_h, more(stat, crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(less_h, less(stat, -crit) ? str_const::accept : str_const::cancel);
    hypoth.addHypoth(noteq_h, more(std::abs(stat), crit2) ? str_const::accept : str_const::cancel);
    result.addHypoth(hypoth.name, hypoth);

    // Дополнительные заказы
    if(orders.contains(Order::AsymptoticDifferenceInterval)) diffInterval(data1, data2, p1, p2, alpha, result);
    if(orders.contains(Order::ProbabilityRatioEstimate)) result.addValue("Отношение вероятностей", to_str( p1 / p2 ));
    if(orders.contains(Order::ProbabilityRatioConfidenceInterval)) probabilityRatioInterval(p1, p2, size1, size2, alpha, result);
}



double count_evaluation(const Sample& data) {
    return (double)std::count_if(data.cbegin(), data.cend(), [](double val){ return val != 0.0; }) / (double)data.size();
}

