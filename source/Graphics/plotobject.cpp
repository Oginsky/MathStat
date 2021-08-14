#include "plotobject.h"

size_t PlotObject::total_id(0);

PlotObject::PlotObject() {
    _id = total_id++;
}

PlotObject::PlotObject(QCPAxis* _xAxis, QCPAxis* yAxis) :
    xAxis(_xAxis), yAxis(yAxis) {
    _id = total_id++;
}

PlotObject::~PlotObject() {
    xAxisData.clear();
    yAxisData.clear();
    delete plottable;
}

void PlotObject::setPlot(QCustomPlot* plot) {
    xAxis = plot->xAxis;
    yAxis = plot->yAxis;
}
void PlotObject::setName(QString name) {
    _name = name;
}

QString PlotObject::name()  const { return _name; }
size_t PlotObject::id()     const { return _id;   }

QVector<double>& PlotObject::getXAxisData() { return xAxisData; }
QVector<double>& PlotObject::getYAxisData() { return yAxisData; }
QCPAbstractPlottable* PlotObject::getPlottable() const { return plottable; }
