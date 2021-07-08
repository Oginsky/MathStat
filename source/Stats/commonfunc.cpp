#include "commonfunc.h"

#include "RandLib.h"

#include <QMap>

#include "sample.h"


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

double CriticalValue::Binomial::operator()(size_t size, double p) const {
    return BinomialRand(size, p).Quantile(p);
}


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

    for (size_t i = 0; i < size_t(data.size()); ++i) {
        double val = data[i];
        if (rank.find(val) == rank.end()) rank.insert(val, 1);
        else rank[val]++;
    }

    auto average_rank = [](size_t& r, size_t count) {
        double rank = 0.0;
        for (size_t i = 0; i < count; i++, rank += r++);
        return rank / count;
    };

    size_t r = 1;
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


