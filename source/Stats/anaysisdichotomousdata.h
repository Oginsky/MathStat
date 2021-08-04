#ifndef DIXOTOMIXANALYZ_H
#define DIXOTOMIXANALYZ_H

#include <QSet>

class Sample;
class OutputData;
enum class FLAG;


namespace Dichotomical {

    void binomialCriterial(const Sample& data, double p0, double alpha, const QSet<FLAG>& flags, OutputData& result);

    void esvaluationProbability(const Sample& data, double alpha, const QSet<QString>& orders, const QSet<FLAG>& flags, OutputData& result);

    void compareProbability(const Sample& data1, const Sample& data2, double alpha, const QSet<QString>& orders, const QSet<FLAG>& flags, OutputData& result);

}

#endif // DIXOTOMIXANALYZ_H
