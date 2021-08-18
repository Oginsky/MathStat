#include "core.h"

#include <QVariant>
#include <vector>

#include <Stats/sample.h>
#include "RandLib.h"


using namespace Core;
using namespace Core::Criterial;

using std::vector;



Core::ParametersInfo::ParametersInfo(QString n, bool r) :
    name(n), is_real(r) {
}

Core::ParametersInfo::ParametersInfo(QString n, decltype(min) _min,
                    decltype(max) _max, bool r) :
    name(n), is_real(r), min(_min), max(_max) {

}
Core::ParametersInfo::ParametersInfo(QString n, decltype(min) _min, bool r) :
    name(n), is_real(r), min(_min) {

}


Core::DistributionInfo::DistributionInfo(DistributionType t, QString n, QVector<ParametersInfo>&& p) :
    type(t), name(n), parameters(p) {
    _parameters_count = parameters.size();
}

size_t Core::DistributionInfo::parameters_count() const {
    return _parameters_count;
}


Core::CriterialInfo::CriterialInfo(CriterialSection type, QString&& name, QVector<QString>&& sample,
              QVector<ParametersInfo>&& parameters, QVector<Pair>&& aproximation, QVector<Pair>&& corrects) {
    section_type = type;
    this->name = name;
    this->parameters = parameters;
    this->samples = sample;
    this->aproximations = aproximation;
    this-> corrects = corrects;

    //id = ++TOTAL_ID;
}


QString Core::CriterialInfo::fullName() {
    return CriterialSectionNameByType[section_type] + " : " + name;
}


QString Core::Criterial::criterialName(CriterialSection type, QString name) {
    return CriterialSectionNameByType[type] + " : " + name;
}


void Core::Distribution::generateSample(const DistributionInfo* distrInfo, QVariant parameter, size_t size, Sample& sample) {
    vector<int> data_int;
    vector<double> data_double;

    switch (distrInfo->type) {
        case DistributionType::Bernoulli:
            data_int.resize(size);
            BernoulliRand(parameter.toDouble()).Sample(data_int);
            break;

        case DistributionType::Geometric:
            data_int.resize(size);
            GeometricRand(parameter.toDouble()).Sample(data_int);
            break;

        case DistributionType::Chi:
            data_double.resize(size);
            ChiRand(parameter.toUInt()).Sample(data_double);
            break;

        case DistributionType::Exponential:
            data_double.resize(size);
            ExponentialRand(parameter.toDouble()).Sample(data_double);
            break;

        case DistributionType::Zeta:
            data_int.resize(size);
            ZetaRand(parameter.toDouble()).Sample(data_int);
            break;
    }

    QVector<double> data;
    data.reserve(size);
    if(!data_int.empty())
        for(auto val: data_int) data.push_back(val);
    else data = QVector<double>::fromStdVector(data_double);


    sample.setData(data);
}

void Core::Distribution::generateSample(const DistributionInfo* distrInfo, QVariant pr1, QVariant pr2, size_t size, Sample& sample) {
    vector<int> data_int;
    vector<double> data_double;

    switch (distrInfo->type) {
        case DistributionType::Beta:
            data_double.resize(size);
            BetaRand(pr1.toDouble(), pr2.toDouble()).Sample(data_double);
            break;

        case DistributionType::Bimomial:
            data_int.resize(size);
            BinomialRand(pr1.toUInt(), pr2.toDouble()).Sample(data_int);
            break;

        case DistributionType::FisherF:
            data_double.resize(size);
            FisherFRand(pr1.toUInt(), pr2.toUInt()).Sample(data_double);
            break;

        case DistributionType::Gamma:
            data_double.resize(size);
            GammaRand(pr1.toDouble(), pr2.toDouble()).Sample(data_double);
            break;

        case DistributionType::Laplace:
            data_double.resize(size);
            LaplaceRand(pr1.toDouble(), pr2.toDouble()).Sample(data_double);
            break;

        case DistributionType::Normal:
            data_double.resize(size);
            NormalRand(pr1.toDouble(), pr2.toDouble()).Sample(data_double);
            break;

        case DistributionType::StudentT:
            data_double.resize(size);
            StudentTRand(pr1.toDouble(), 0.0, pr2.toDouble()).Sample(data_double);
            break;
    }

    QVector<double> data;
    data.reserve(size);
    if(!data_int.empty())
        for(auto val: data_int) data.push_back(val);
    else data = QVector<double>::fromStdVector(data_double);

    sample.setData(data);
}

void Core::Distribution::generateSample(const DistributionInfo* distrInfo, QVariant pr1, QVariant pr2, QVariant pr3, size_t size, Sample& sample) {
    vector<int> data_int;
    vector<double> data_double;

    switch (distrInfo->type) {
        case DistributionType::Arcsine:
            data_double.resize(size);
            ArcsineRand(pr1.toDouble(), pr2.toDouble(), pr3.toDouble()).Sample(data_double);
            break;
    }

    QVector<double> data;
    data.reserve(size);
    if(!data_int.empty())
        for(auto val: data_int) data.push_back(val);
    else data = QVector<double>::fromStdVector(data_double);

    sample.setData(data);
}


const CriterialInfo* Core::Criterial::getCriterialByName(const QString& name) {
    return criterials[CriterialsNames.indexOf(name)];
}


QStringList Core::Criterial::getCriterialsBySection(CriterialSection type) {
    QStringList result;
    for(const auto& test: criterials)
        if(test->section_type == type)
            result.append(test->name);

    return result;
}
