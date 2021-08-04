#ifndef TWOSAMPLESCALEPROBLEM_H
#define TWOSAMPLESCALEPROBLEM_H

#include <QSet>

class Sample;
struct OutputData;
enum class FLAG;

typedef const QSet<FLAG>& Flags;
typedef const QSet<QString>& Orders;


namespace TwoSampleScaleProblem {

    /* Медианы известны или равны */

    void ansariBradleyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result);

    void sigelTukeyTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result);

    /* Медианы неизвестны и неравны */

    void mosesTest(const Sample& data1, const Sample& data2, double alpha, Flags flags, OutputData& result);

}

#endif // TWOSAMPLESCALEPROBLEM_H
