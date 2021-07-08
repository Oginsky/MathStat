#ifndef DIXOTOMIXANALYZ_H
#define DIXOTOMIXANALYZ_H

#include <QStringList>
#include <QSet>

#include "commonfunc.h"

class Sample;



namespace dixanalyz {

    void binimCritThetaEqual(size_t B, size_t n, double p0, double alpha, QStringList& result);

    void simpletest(const Sample& sample, OutputData& result);

}

#endif // DIXOTOMIXANALYZ_H
