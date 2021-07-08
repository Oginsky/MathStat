#include "barsgraph.h"

BarsGraph::BarsGraph(QCPAxis* xAxis, QCPAxis* yAxis) :
    PlotObject(xAxis, yAxis)
{
    barsgraph = new QCPBars(xAxis, yAxis);

    //barsgraph->setPen(Qt::NoPen);
    //barsgraph->setBrush(main_color.getColor());

    barsgraph->setPen(main_color.getColor());
    barsgraph->setBrush(QBrush(Qt::BrushStyle::NoBrush));


    //barsgraph->setBrush(QColor(10, 140, 70, 160));
}
BarsGraph::BarsGraph() {

}

void BarsGraph::setPlot(QCustomPlot* plot) {
    xAxis = plot->xAxis;
    yAxis = plot->yAxis;

    barsgraph = new QCPBars(xAxis, yAxis);

    barsgraph->setPen(main_color.getColor());
    barsgraph->setBrush(QBrush(Qt::BrushStyle::NoBrush));
}

void BarsGraph::build(const Sample& sample) {
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
      return (prev == 0) ? prev : prev-1;
    };

    for(const auto value: sample.data()) {
        int interval_number = getIntervalNumber(value);
        if(interval_number >= counts.size()) interval_number = counts.size()-1;
        counts[interval_number]++;
    }

    auto height = [&step](int count, int size) { return count/(step*size); };

    xAxisData.reserve(counts.size()), yAxisData.reserve(counts.size());
    for(int i(0); i < counts.size(); ++i) {
        xAxisData.push_back(bounds[i]+step/2.0);
        yAxisData.push_back(height(counts[i], sample.size()));
        //barsgraph->addData(bounds[i]+step/2.0, height(counts[i], sample.size()));
    }

    barsgraph->setWidth(step);
    barsgraph->setData(xAxisData, yAxisData);

    xAxis->setRange(xAxisData.front(), xAxisData.back());
    yAxis->setRange(0, 1);


}

QCPBars *BarsGraph::getBarsGraph() { return barsgraph; }

QString BarsGraph::type_name() const {
    return "Bars";
}

bool BarsGraph::operator==(const BarsGraph &right) { return this->_id == right._id; }
