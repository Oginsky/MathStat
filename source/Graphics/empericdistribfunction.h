#ifndef EMPERICDISTRIBFUNCTION_H
#define EMPERICDISTRIBFUNCTION_H

#include "plotobject.h"


class EmpericDistribFunction : public PlotObject {
public:
    EmpericDistribFunction();
    EmpericDistribFunction(QCPAxis* xAxis, QCPAxis* yAxis);

    void setPlot(QCustomPlot* plot) override;
    void build(const Sample& sample) override;

    QString type_name() const override;

private:
    QCPCurve* empericdistrfunc;
    QString _type_name = "Эмпирическая функция распределения";
};

#endif // EMPERICDISTRIBFUNCTION_H
