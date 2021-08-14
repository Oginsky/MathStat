#ifndef PLOTOBJECT_H
#define PLOTOBJECT_H

#include <QVector>

class QString;


#include "qcustomplot/qcustomplot.h"
#include "Styles/colors.h"
#include "../Stats/sample.h"

class PlotObject {

public:
    PlotObject(QCPAxis* _xAxis, QCPAxis* _yAxis);
    PlotObject();

    virtual ~PlotObject();

    QVector<double>& getXAxisData();
    QVector<double>& getYAxisData();
    QCPAbstractPlottable* getPlottable() const;


    virtual void build(const Sample& sample) = 0;
    virtual void setPlot(QCustomPlot* plot);
    virtual QString type_name() const = 0;


    void setName(QString name);
    QString name() const;
    size_t id() const;

private:
    static size_t total_id;

protected:
    QCPAxis* xAxis, *yAxis;
    QCPAbstractPlottable* plottable;

    QString _name;
    QString _type_name;
    size_t _id;

    QVector<double> xAxisData, yAxisData;
};

#endif // PLOTOBJECT_H
