#ifndef BARGRAPHS_H
#define BARGRAPHS_H

#include "plotobject.h"

class BarsGraph : public PlotObject {

public:
   BarsGraph(QCPAxis* xAxis, QCPAxis* yAxis);
   BarsGraph();
   BarsGraph(QString name);
   ~BarsGraph();

   QCPBars* getBarsGraph();

   void setPlot(QCustomPlot* plot) override;
   void build(const Sample& sample) override;

   QString type_name() const override;

   bool operator==(const BarsGraph &right);

private:
   QCPBars* barsgraph;
   QString _type_name = "Гистограмма";



};

#endif // BARGRAPHS_H
