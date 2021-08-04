#include "commonfunc.h"

#include "RandLib.h"

#include <QMap>
#include <unordered_set>

unsigned qHash(FLAG flag) {return (unsigned)flag; }


const NormalRand CriticalValue::Normal::distr = NormalRand(0, 1);

double CriticalValue::Normal::operator()(double p) const {
    return distr.Quantile(p);
}

double CriticalValue::StudentT::operator()(double degree, double p) const {
    return StudentTRand(degree).Quantile(p);
}

double CriticalValue::ChiSquared::operator()(double degree, double p) const {
    return ChiSquaredRand(degree).Quantile(p);
}

double CriticalValue::Binomial::operator()(size_t size, double p, double p0) const {
    return BinomialRand(size, p0).Quantile(p);
}

double CriticalValue::Beta::operator()(double shape1, double shape2, double p) const {
    return BetaRand(shape1, shape2).Quantile(p);
}

QString str_const::interval(double left, double right) {
    return "(" + to_str(left) + ", " + to_str(right) + ")";
}


double square(double x) { return x*x; }



size_t binomialLowerAlpha(size_t n, double alpha, double p) {
    BinomialRand binRand(n, p);

    size_t b = 0;
    double delta_prev = std::numeric_limits<double>::max(), delta;
    for(size_t i = 0; i < n; ++i) {
        delta = abs(alpha - binRand.F(i));
        if(delta > delta_prev) break;

        delta_prev = delta;
        b = i;
    }

    return b;
}

size_t binomialUpperAlpha(size_t n, double alpha, double p) {
    BinomialRand binRand(n, p);

    size_t b = 0;
    double delta_prev = std::numeric_limits<double>::max(), delta;
    for(size_t i = 0; i < n; ++i) {
        delta = abs( alpha - (binRand.S(i) + binRand.P(i)) );
        if(delta > delta_prev) break;

        delta_prev = delta;
        b = i;
    }

    return b;
}


inline void sort(QVector<double>& data) { std::sort(data.begin(), data.end()); }

void ranking(const Sample& data, QMap<double, double>& rank, unsigned startRank, bool average) {
    size_t size = data.size();

    for (size_t i = 0; i < size; ++i) {
        double val = data[i];
        if (rank.find(val) == rank.end()) rank.insert(val, 1);
        else rank[val]++;
    }

    auto average_rank = [](size_t& r, size_t count) {
        double rank = 0.0;
        for (size_t i = 0; i < count; i++, rank += r++);
        return rank / count;
    };

    size_t r = startRank;
    for (auto it = rank.begin(); it != rank.end(); it++) {
        it.value() = average_rank(r, it.value());
    }

}

void uniq_group(const Sample& data, QMap<double, size_t>& groups) {
    for(const double& val: data.data()) {
        if(groups.contains(val)) groups[val]++;
        else groups[val] = 0;
    }
}

void uniq_group(const QVector<double>& data, QMap<double, size_t>& groups) {
    for(const double& val: data) {
        if(groups.contains(val)) groups[val]++;
        else groups[val] = 0;
    }
}

bool has_same_value(const Sample& data1, const Sample& data2) {
    std::unordered_set<double> values;
    for(const auto& val: data1.data()) values.insert(val);
    return std::any_of(data2.cbegin(), data2.cbegin(), [&values](double val) {
        return values.find(val) == values.end();
    });
    //return std::find_first_of(data1.cbegin(), data1.cend(), data2.cbegin(), data2.cend()) != data1.cend();
}

double disp_with_corrects(size_t n, size_t m, const QMap<double, size_t>& groups) {
    double summ(0.0);
    for(auto val: groups.values())
        summ += val*(square(val) - 1.0);
    double temp = summ / ( (n + m)*(n + m - 1.0) );

    return ( n*m/12.0 ) * (n + m + 1.0 - temp);
}
