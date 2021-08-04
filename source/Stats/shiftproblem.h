#ifndef SHIFTPROBLEM_H
#define SHIFTPROBLEM_H

#include <QSet>


class Sample;
struct OutputData;
enum class FLAG;


namespace ShiftProblem {

    void wilcoxonSignRank(const Sample& before, const Sample& after, double level, QSet<FLAG> flags, OutputData& result);

    void wilcoxonSignRankEstimation(const Sample& before, const Sample& after, OutputData& result);

    void wilcoxonSignRankInterval(const Sample& before, const Sample& after, double level, OutputData& result);

}

namespace SignRank {

    void signRank(const Sample& before, const Sample& after, double level, QSet<FLAG> flags, OutputData& result);

    void signRankEstimation(const Sample& before, const Sample& after, OutputData& result);

    void signRankInterval(const Sample& before, const Sample& after, double level, QSet<FLAG>& flags, OutputData& result);

}




#endif // SHIFTPROBLEM_H
