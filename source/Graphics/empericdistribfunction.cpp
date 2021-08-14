#include "empericdistribfunction.h"

#include <QMap>

EmpericDistribFunction::EmpericDistribFunction() {

}

EmpericDistribFunction::EmpericDistribFunction(QString name) {
    _name = name;
}

EmpericDistribFunction::EmpericDistribFunction(QCPAxis* xAxis, QCPAxis* yAxis) :
    PlotObject(xAxis, yAxis) {

    empericdistrfunc = new QCPCurve(xAxis, yAxis);

    // Настройка внешнего вида графика
    empericdistrfunc->setPen(main_color.getColor());

    plottable = empericdistrfunc;

}

EmpericDistribFunction::~EmpericDistribFunction() {

}

void EmpericDistribFunction::setPlot(QCustomPlot* plot) {
    xAxis = plot->xAxis;
    yAxis = plot->yAxis;

    empericdistrfunc = new QCPCurve(xAxis, yAxis);

    // Настройка внешнего вида графика
    empericdistrfunc->setPen(main_color.getColor());
    empericdistrfunc->setName(_name);

    plottable = empericdistrfunc;
}

void EmpericDistribFunction::build(const Sample& sample) {
    QMap<double, size_t> counts;
    size_t size = sample.size();

    for(size_t i = 0; i < size; ++i) {
        if(!counts.contains(sample[i])) counts[sample[i]] = 1;
        else counts[sample[i]]++;
    }

    size_t graphic_size = counts.size()*2 + 1;
    double prev_val = counts.begin().key();

    double possibility = 0.0;
    for(auto val: counts.keys()) {
        xAxisData.push_back(prev_val), yAxisData.push_back(possibility);
        xAxisData.push_back(val), yAxisData.push_back(possibility);

        possibility += (double)counts[val] / (double)size;
        xAxisData.push_back(val); yAxisData.push_back(possibility);

        prev_val = val;

    }
    xAxisData.pop_back(), yAxisData.pop_back();

    empericdistrfunc->setData(xAxisData, yAxisData);
    xAxis->setRange(xAxisData.front()-1, xAxisData.back()+1);
    yAxis->setRange(0.0, 1.1);
}

QString EmpericDistribFunction::type_name() const {
    return "Функция распределения";
}
