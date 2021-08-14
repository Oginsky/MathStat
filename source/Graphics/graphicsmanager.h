#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <QObject>

#include <QPoint>

class QCustomPlot;
class QCPLegend;
class QCPPlotTitle;
class QCPAbstractLegendItem;
class QCPAbstractItem;

class QMouseEvent;
class QMenu;
class QAction;

class PlotObject;
class Sample;

class GraphicsManager : public QObject {
    Q_OBJECT

public:
    explicit GraphicsManager(QCustomPlot* plot, QList<PlotObject*>* graphics, QObject *parent = nullptr);
    ~GraphicsManager();

    void registredNewGraphic(PlotObject* graphic, const Sample& sample, QString name = "");


    /* Методы */
    void setTriggeredAction(QAction* action, bool with_separetor = false);

private:
    QCustomPlot* const plot;
    QCPPlotTitle* title;
    QList<PlotObject*>* const graphics;

    QMenu* contextMenu;
    QMenu* contextMenuWithSelected;


    /* Методы */
    bool has_selected();
    size_t graphicIndex(QCPAbstractLegendItem* item);
    size_t legendIndex(QCPAbstractItem* item);
    void createContextMenu();


signals:
    void add_graphic();


public slots:
    /* Элементы плота */
    void changeTitle(QMouseEvent* mouseEvent, QCPPlotTitle* title);
    void changeTitleVisible();
    void changeLegendVisible();

    /* Интерактив */
    void selectionChanged();
    void handleZoom();

private slots:
    void itemWasDClicked(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event);

    /* Скрыть/Удалить графики */
    void contextMenuRequest(QPoint click_point);
    void hideSelectedGraph();
    void removeSelectedGraph();
    void removeAllGraphs();

    /* Выбор графика и элемента легенды */
    void itemClick(bool selected);
    void graphicClick(QCPAbstractItem *item, QMouseEvent *event);
    void legendItemClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event);


};

#endif // GRAPHICSMANAGER_H
