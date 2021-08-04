#ifndef TWOSAMPLESHIFTPROBLEM_H
#define TWOSAMPLESHIFTPROBLEM_H

#include <QSet>

class Sample;
struct OutputData;
enum class FLAG;

typedef const QSet<FLAG>& Flags;
typedef const QSet<QString>& Orders;


namespace TwoSampleShiftProblem {

    void medianTestMuda(const Sample& data1, const Sample& data2, double alpha, const QSet<FLAG>& flags, OutputData& result);

    void wilcoxonSummRank(const Sample& data1, const Sample& data2, double alpha, const QSet<FLAG>& flags, OutputData& result);

    void mannWhitneyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, Orders orders, OutputData& result);

    void flynnerPolycelloTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result);

}

#endif // TWOSAMPLESHIFTPROBLEM_H
