#include "empirical.h"

#include <vector>

#include <cmath>
#include <algorithm>

void buildDistributionBarsGraph(const Sample& sample, BarsGraph& plotobject) {
    int intervals_count = ceil(1 + log(sample.size()));
    double step = sample.characters().span() / (double)intervals_count;

    QVector<double> bounds(intervals_count+1);
    bounds[0] = sample.characters()._min;
    for(int i(1); i <= intervals_count; ++i)
        bounds[i] = bounds[i-1] + step;

    QVector<int> counts(intervals_count, 0);


    auto getIntervalNumber = [&bounds](double value) {
      int prev = 0;
      while(value > bounds[prev] && prev < bounds.size()) prev++;
      return prev;
    };

    for(const auto value: sample.data()) {
        int interval_number = getIntervalNumber(value);
        if(interval_number >= counts.size()) interval_number = counts.size()-1;
        counts[interval_number]++;
    }

    auto height = [&step](int count, int size) { return count/(step*size); };
    for(int i(0); i < counts.size(); ++i)
        plotobject.getBarsGraph()->addData(bounds[i]+step/2.0, height(counts[i], sample.size()));

}
