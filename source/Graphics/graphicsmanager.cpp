#include "graphicsmanager.h"

#include "barsgraph.h"
#include "empericdistribfunction.h"

#include "Styles/colors.h"


GraphicsManager::GraphicsManager(QCustomPlot* _plot, QList<PlotObject*>* _graphics, QObject *parent)
    : QObject(parent), plot(_plot), graphics(_graphics) {

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                          QCP::iSelectLegend | QCP::iSelectPlottables);

    createContextMenu();

    // Легенда и Заголовок
    plot->legend->setVisible(false);
    plot->legend->setSelectableParts(QCPLegend::spItems); // Можно выбирать только графики

    title = new QCPPlotTitle(plot, "Заголовок");
    title->setTextColor(Qt::white);
    title->setVisible(false);
    plot->plotLayout()->insertRow(0);
    plot->plotLayout()->addElement(0, 0, title);


    // Цвета выделенных объектов
    QPen selectedPen = plot->xAxis->selectedBasePen();
    selectedPen.setColor(Qt::green);
    plot->xAxis->setSelectedBasePen(selectedPen);
    plot->xAxis->setSelectedSubTickPen(selectedPen);
    plot->xAxis->setSelectedTickPen(selectedPen);
    plot->yAxis->setSelectedBasePen(selectedPen);
    plot->yAxis->setSelectedSubTickPen(selectedPen);
    plot->yAxis->setSelectedTickPen(selectedPen);

    plot->xAxis->setSelectedTickLabelColor(Qt::green);
    plot->xAxis->setSelectedLabelColor(Qt::green);
    plot->yAxis->setSelectedLabelColor(Qt::green);
    plot->yAxis->setSelectedTickLabelColor(Qt::green);

    plot->legend->setSelectedTextColor(main_color.getColor());
    plot->legend->setSelectedBorderPen(QPen(main_color.getColor()));


    // Масштабирование плота
    connect(plot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(plot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(handleZoom()));

    // Контекстное меню
    plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(plot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

    // Показать/Скрыть график
    connect(plot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)),
            this, SLOT(itemWasDClicked(QCPLegend*,QCPAbstractLegendItem*, QMouseEvent*)));

    // Установка заголовка
    connect(plot, SIGNAL(titleDoubleClick(QMouseEvent*,QCPPlotTitle*)),
            this, SLOT(changeTitle(QMouseEvent*,QCPPlotTitle*)));

    // Выбор элемента на графике
    connect(plot, SIGNAL(itemClick(QCPAbstractItem*, QMouseEvent*)),
            this, SLOT(graphicClick(QCPAbstractItem*, QMouseEvent*)));

    // Выбор элемента на легенде
    connect(plot, SIGNAL(legendClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)),
            this, SLOT(legendItemClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)));
}

GraphicsManager::~GraphicsManager() {

}

void GraphicsManager::registredNewGraphic(PlotObject* graphic, const Sample& sample, QString name) {
    graphic->setPlot(plot);
    graphic->build(sample);
    graphics->push_back(graphic);

    graphic->getPlottable()->setSelectedPen(QPen(Qt::white));
    connect(graphic->getPlottable(), SIGNAL(selectionChanged(bool)), this, SLOT(itemClick(bool)));

    plot->legend->addElement(plot->legend->rowCount(), 0, new QCPPlottableLegendItem(plot->legend, graphic->getPlottable()));

    plot->replot();
}



/*
 *   Слоты
*/
void GraphicsManager::itemWasDClicked(QCPLegend *legend,  QCPAbstractLegendItem *item, QMouseEvent *event) {
    Q_UNUSED(event);

    if(!item) return;

    static const QColor active(Qt::black);
    static const QColor hiden(Qt::gray);

    bool setHiden = item->textColor() == active;
    size_t graphic_index = graphicIndex(item);

    graphics->at(graphic_index)->getPlottable()->setVisible(!setHiden);
    item->setTextColor( setHiden ? hiden : active );
    item->setSelected(!setHiden);

    plot->replot();
}

void GraphicsManager::selectionChanged() {
    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        plot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
      plot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      plot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }

    if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
        plot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || plot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
    {
      plot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
      plot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    }
}

void GraphicsManager::handleZoom() {
    if (plot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
      plot->axisRect()->setRangeZoom(plot->xAxis->orientation());
    else if (plot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
      plot->axisRect()->setRangeZoom(plot->yAxis->orientation());
    else
      plot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}


/*
 * Методы
*/
bool GraphicsManager::has_selected() {
    for(const auto gr: *graphics)
        if(gr->getPlottable()->selected())
            return true;

    return false;
}

size_t GraphicsManager::graphicIndex(QCPAbstractLegendItem* item) {
    if(!item) return graphics->size();

    for(size_t index(0); index < size_t(plot->legend->rowCount()); ++index)
        if(item == (QCPAbstractLegendItem*)plot->legend->elementAt(index))
            return index;

    return graphics->size();
}

size_t GraphicsManager::legendIndex(QCPAbstractItem* item) {
     if(!item) return plot->legend->rowCount();

     for(size_t index(0); index < size_t(graphics->size()); ++index)
             if(item == (QCPAbstractItem*)graphics->at(index)->getPlottable())
                 return index;

     return plot->legend->rowCount();
}

void GraphicsManager::createContextMenu() {
    contextMenu = new QMenu();
    contextMenu->addAction("Добавить", this, SIGNAL(add_graphic()));
    contextMenu->addAction("Удалить все", this, SLOT(removeAllGraphs()));

    contextMenuWithSelected = new QMenu();
    contextMenuWithSelected->addAction("Скрыть выбранный график", this, SLOT(hideSelectedGraph()));
    contextMenuWithSelected->addAction("Удалить выбранный график", this, SLOT(removeSelectedGraph()));
    contextMenuWithSelected->addSeparator();
    contextMenuWithSelected->addAction("Добавить", this, SIGNAL(add_graphic()));
    contextMenuWithSelected->addAction("Удалить все", this, SLOT(removeAllGraphs()));
}

void GraphicsManager::setTriggeredAction(QAction* action, bool with_separetor) {
    if(!contextMenu || !contextMenuWithSelected)
        createContextMenu();

    if(with_separetor) {
        contextMenu->addSeparator();
        contextMenuWithSelected->addSeparator();
    }

    contextMenu->addAction(action);
    contextMenuWithSelected->addAction(action);
}


/*
 * Слоты удаления графиков
*/
void GraphicsManager::contextMenuRequest(QPoint click_point) {
    if(has_selected()) contextMenuWithSelected->popup(plot->mapToGlobal(click_point));
    else  contextMenu->popup(plot->mapToGlobal(click_point));
}

void GraphicsManager::hideSelectedGraph() {
    auto it = graphics->begin();
    auto end = graphics->end();
    size_t legend_index(0);

    for(it; it != end; ++it, ++legend_index) {
        if((*it)->getPlottable()->selected()) {
            (*it)->getPlottable()->setVisible(false);

            QCPAbstractLegendItem* item = plot->legend->item(legend_index);
            item->setTextColor(Qt::gray);

            break;
        }
    }

    plot->replot();
}

void GraphicsManager::removeSelectedGraph() {
    auto it = graphics->begin();
    auto end = graphics->end();

    for(it; it != end; ++it) {
        if((*it)->getPlottable()->selected()) {
            (*it)->getPlottable()->removeFromLegend();
            PlotObject* obj = *it;

            graphics->erase(it);
            delete obj;

            break;
        }
    }

    plot->replot();
}

void GraphicsManager::removeAllGraphs() {
   auto it = graphics->begin();
   auto end = graphics->end();

   for(it; it != end; ++it) {
       (*it)->getPlottable()->removeFromLegend();
       PlotObject* obj = *it;

       graphics->erase(it);
       delete obj;
   }

   plot->replot();
}


/*
 *  Слоты редактирования элементов плота
*/
void GraphicsManager::changeTitle(QMouseEvent* mouseEvent, QCPPlotTitle* title) {
    QWidget* temp = new QWidget();
    temp->setAttribute(Qt::WA_DeleteOnClose);

    QString newTitle = QInputDialog::getText(temp, "MathStat", "Новый заголовок");
    newTitle = (newTitle.isEmpty()) ? "Заголовок" : newTitle;

    title->setText(newTitle);

    plot->replot();
}

void GraphicsManager::changeTitleVisible() {
     title->setVisible(!title->visible());
     plot->replot();
}

void GraphicsManager::changeLegendVisible() {
    plot->legend->setVisible(!plot->legend->visible());
    plot->replot();
}


/*
 * Слоты выбора графика и элемента легенды
*/
void GraphicsManager::itemClick(bool selected) {
    QCPAbstractItem* item = static_cast<QCPAbstractItem*>(sender());
    size_t legend_item_idx = legendIndex(item);
    QCPAbstractLegendItem* legendItem =  (QCPAbstractLegendItem*)plot->legend->elementAt(legend_item_idx);

    legendItem->setSelected(selected);
}

void GraphicsManager::graphicClick(QCPAbstractItem *item, QMouseEvent *event) {
    if(!item) return;

    size_t legen_item_idx = legendIndex(item);
    QCPAbstractLegendItem* legendItem =  (QCPAbstractLegendItem*)plot->legend->elementAt(legen_item_idx);
    legendItem->setSelected(true);
}

void GraphicsManager::legendItemClick(QCPLegend *legend, QCPAbstractLegendItem *item, QMouseEvent *event) {
    if(!item) return;

    size_t graphic_item_idx = graphicIndex(item);
    graphics->at(graphic_item_idx)->getPlottable()->setSelected(true);
}

