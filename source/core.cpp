#include "core.h"

using namespace Core;
using namespace Core::Criterial;



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
    parameters_count = parameters.size();
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
