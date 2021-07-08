#include "graphicsmanager.h"

#include "barsgraph.h"
#include "empericdistribfunction.h"

GraphicsManager::GraphicsManager(QCustomPlot* _plot, QList<PlotObject*>* _graphics, QObject *parent)
    : QObject(parent), plot(_plot), graphics(_graphics) {

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                        QCP::iSelectLegend | QCP::iSelectPlottables);
    plot->legend->setSelectableParts(QCPLegend::spItems); // Можно выбирать только графики

    connect(plot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)),
            this, SLOT(itemWasDClicked(QCPLegend*,QCPAbstractLegendItem*, QMouseEvent*)));

}
GraphicsManager::~GraphicsManager() {
    for(auto& graphic: *graphics) delete graphic;
    delete graphics;
}

void GraphicsManager::registredNewGraphic(PlotObject* graphic, const Sample& sample, QString name) {
    graphic->setPlot(plot);
    graphic->setName(graphic->type_name() + " - " + name);
    graphic->build(sample);
    graphics->push_back(graphic);

    plot->legend->setVisible(true);
}



/*
 *   Слоты
*/

void GraphicsManager::itemWasDClicked(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event) {
    if(item && item->visible()) {
        item->setVisible(false);
        QFont font = item->font();
        font.setBold(false);
        item->setFont(font);
        return;
    }

    if(item && !item->visible()) {
        item->setVisible(true);
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        return;
    }
}
