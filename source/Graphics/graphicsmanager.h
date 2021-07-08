#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <QObject>

class QCustomPlot;
class QCPLegend;
class QCPAbstractLegendItem;
class QMouseEvent;

class PlotObject;
class Sample;

class GraphicsManager : public QObject {
    Q_OBJECT

public:
    explicit GraphicsManager(QCustomPlot* plot, QList<PlotObject*>* graphics, QObject *parent = nullptr);
    ~GraphicsManager();

    void registredNewGraphic(PlotObject* graphic, const Sample& sample, QString name = "");

private:
    QCustomPlot* const plot;
    QList<PlotObject*>* const graphics;


signals:

private slots:
    void itemWasDClicked(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event);


};

#endif // GRAPHICSMANAGER_H
