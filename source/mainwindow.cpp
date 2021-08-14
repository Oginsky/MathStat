#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "Styles/colors.h"
#include "Styles/stylesheets.h"

#include "DialogForms/InputsForms/inputsampledialog.h"
#include "DialogForms/InputsForms/generatesampledialog.h"
#include "DialogForms/InputsForms/createsampledialog.h"
#include "DialogForms/GraphicsForms/addgraphdialog.h"

#include "Graphics/barsgraph.h"
#include "Graphics/empericdistribfunction.h"

#include "qdebug.h"

#include "RandLib.h"

#include "ToolWidgets/criterialconfig.h"
#include "ToolWidgets/criterialresult.h"

#include "core.h"
#include "taskmanager.h"
#include "commonstruct.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, &MainWindow::show_characteristic, &MainWindow::show_samples_characteristics);

    // Инициализация компонентов
    graphicsManager = new GraphicsManager(ui->plot, &graphicsList, this);
    graphicsManager->setTriggeredAction(ui->graphic_title_change, true);
    graphicsManager->setTriggeredAction(ui->graphic_legend_change);

    // Первоначальные настройки
    setElementConfig();             // Настройки виджетов
    createCharacteristicsTable();   // Заполнение строк таблицы характеристик данных
    createStatisticsMenu();         // Создание меню для выбора непараметрических критериев

    // Внешний вид приложения
    this->setElementStyle();        // Настройки стиля отдельных элементов
    this->setPlotStyle();           // Установка внешнего вида холста для графиков

    // Коннекты
    connect(ui->graphic_legend_change, &QAction::triggered, graphicsManager, &GraphicsManager::changeLegendVisible);
    connect(ui->graphic_title_change, &QAction::triggered, graphicsManager, &GraphicsManager::changeTitleVisible);
    connect(graphicsManager, SIGNAL(add_graphic()), ui->add_graphics, SIGNAL(triggered()));


    //ui->scenarios_layout->setAlignment(Qt::Alignment::enum_type::AlignTop);

   //BinomialRand binRand(8, 0.4);
   //qDebug() << binRand.F(3); // <=
   //qDebug() << binRand.S(3); // >
   //qDebug() << binRand.P(3); // =

    // Проверка виджета настройки критериев
    //CollapsibleSection* testconfig = new CollapsibleSection("Конфигурация", 100, this);
    //CriterialConfig* tmp = new CriterialConfig(&Core::Criterial::WilcoxonSignRank, samplesName, this);
    //testconfig->setContentLayout(*tmp->layout());
    //content_widget->layout()->addWidget(testconfig);

}


MainWindow::~MainWindow()
{
    delete ui;
}

/*
 *  Слоты добавления новых выборок
*/
void MainWindow::on_add_sample_triggered() {
    InputSampleDialog* isd = new InputSampleDialog(this);
    connect(isd, SIGNAL(send_samples(QList<Sample>)), this, SLOT(add_samplesList(QList<Sample>)));
    isd->show();
}
void MainWindow::on_generate_sample_triggered() {
    GenerateSampleDialog* gsd = new GenerateSampleDialog(this);
    gsd->show();
}
void MainWindow::on_create_sample_triggered() {
    CreateSampleDialog* csd = new CreateSampleDialog(samplesName, samplesList);
    connect(csd, SIGNAL(send_samples(QList<Sample>)), this, SLOT(add_samplesList(QList<Sample>)));
    csd->show();
}

void MainWindow::add_samplesList(QList<Sample> samples) {
    for(auto& sample: samples) {
        samplesList.append(sample);
        QString new_name = (!sample.name().isEmpty()) ? sample.name() : "X" + QString::number(samplesName.size());
        samplesName << new_name;

        if(sample.name().isEmpty()) samplesList.back().setName(new_name);

        ui->sample_table->setColumnCount(ui->sample_table->horizontalHeader()->count() + 1);
        if(sample.size() > ui->sample_table->rowCount()) ui->sample_table->setRowCount(sample.size());

        size_t coln = ui->sample_table->horizontalHeader()->count() - 1;
        for(size_t i = 0; i < sample.size(); ++i) {
            ui->sample_table->setItem(i, coln, new QTableWidgetItem(QString::number(sample[i])));
        }
    }

    ui->sample_table->setHorizontalHeaderLabels(samplesName);
    ui->sample_table->resizeColumnsToContents();

    emit show_characteristic();

    QStringList samples_names;
    for(auto& sample: samples) samples_names << sample.name();
    emit new_samples(samples_names);
}


/*
 *  Слоты по обновлению таблицы характеристик
*/
void MainWindow::show_samples_characteristics() {
    size_t index = ui->charact_table->columnCount() - 1;
    ui->charact_table->setColumnCount(samplesList.size() + 1);  // + стобец названий
    for(index; index < samplesList.size(); ++index) {
        samplesList[index].countCharacteristic();
        SampleCharacteristic ch = samplesList[index].characters();

        size_t column_idx = index + 1;
        ui->charact_table->setItem(0, column_idx, new QTableWidgetItem(samplesList[index].name()));
        ui->charact_table->setItem(1, column_idx, new QTableWidgetItem(QString::number(ch.expectation())));
        ui->charact_table->setItem(2, column_idx, new QTableWidgetItem(QString::number(ch.variance())));
        ui->charact_table->setItem(3, column_idx, new QTableWidgetItem(QString::number(ch.correct_var())));
        ui->charact_table->setItem(4, column_idx, new QTableWidgetItem(QString::number(ch.median())));
        ui->charact_table->setItem(5, column_idx, new QTableWidgetItem(QString::number(ch.min())));
        ui->charact_table->setItem(6, column_idx, new QTableWidgetItem(QString::number(ch.max())));
        ui->charact_table->setItem(7, column_idx, new QTableWidgetItem(QString::number(ch.span())));
    }

    ui->charact_table->resizeColumnsToContents();
    //ui->charact_table->setTextElideMode(Qt::TextElideMode::ElideMiddle);
}


/*
 *  Слоты добавления и редактирования графиков
*/
void MainWindow::on_add_graphics_triggered() {
    AddGraphDialog* agd = new AddGraphDialog(samplesName, this);
    connect(agd, SIGNAL(send_graph(QList<QString>, QString, QString)), this, SLOT(add_graphic_object(QList<QString>, QString, QString)));
    agd->show();
}
void MainWindow::add_graphic_object(QList<QString> plotObjects, QString sample_name, QString graphics_name) {
    for(QString graphic: plotObjects) {
        size_t sample_index = samplesName.indexOf(sample_name);

        PlotObject* newGraphic;
        if(graphic == "BarsGraph") newGraphic = new BarsGraph(graphics_name);
        else if(graphic == "EmpericDistribFunction") newGraphic = new EmpericDistribFunction(graphics_name);

        graphicsManager->registredNewGraphic(newGraphic, samplesList[sample_index], sample_name);
    }

    ui->plot->replot();
}


/*
 *  Слоты критериев
*/
void MainWindow::add_criterial_config() {
    QAction* action = qobject_cast<QAction*>(sender());
    QString criterial_name = action->data().toString();

    // Размещаем форму критерия
    // Получить тип критерия, создать форму, и установить форме критерий
    const Core::CriterialInfo* criterial = Core::Criterial::getCriterialByName(criterial_name);
    CollapsibleSection* criterial_section = new CollapsibleSection(criterial->name, 100, this);
    criterialsList.append(criterial_section);
    CriterialConfig* critConfig = new CriterialConfig(criterial, samplesName, criterial_section);
    criterial_section->setContentLayout(*critConfig->layout());

    // Размещаем на виджете сценариев (временно)
    content_widget->layout()->addWidget(criterial_section);

    // Добавляем связи (обновление выборок и обработка введенных настроек)
    connect(this, SIGNAL(new_samples(QStringList)), critConfig, SLOT(updateData(QStringList)));
    connect(critConfig, SIGNAL(sendConfigs(const Core::CriterialInfo*, QMap<QString, QVariant>)), this, SLOT(handleCiterialConfig(const Core::CriterialInfo*, QMap<QString, QVariant>)));
}
void MainWindow::on_add_criterial_btn_clicked() {
    QString criterial_name = ui->criterial_name_line->text();
    if(criterial_name.isEmpty()) {
        ui->criterial_name_line->setText("Укажите название критерия!");
        return;
    }

    // Проверка корректного названия
    if(!Core::Criterial::CriterialsNames.contains(criterial_name)) {
        ui->criterial_name_line->setText("Введите название существующего критерия!");
        return;
    }

    // Вызываем добавление критерия через сигнал
    QAction action;
    action.setData(criterial_name);
    connect(&action, &QAction::triggered, this, &MainWindow::add_criterial_config);
    action.trigger();
}
void MainWindow::handleCiterialConfig(const Core::CriterialInfo* criterial, QMap<QString, QVariant> configs) {
    // Данные отправляем в taskmanager, которыей их распределяет по функциям Stats
    // И возвращает результаты. Затем их отображаем (пока что временно на месте сценариев)
    OutputData result;
    TaskManager::handle(criterial, configs, samplesList, result);

    CollapsibleSection* new_section = new CollapsibleSection(criterial->name + ": Результат", 100, this);
    criterialsList.append(new_section);
    CriterialResult* resultForm = new CriterialResult(result, new_section);
    new_section->setContentLayout(*resultForm->layout());

    content_widget->layout()->addWidget(new_section);
}


/*
 * Начальные настройки
*/
void MainWindow::createCharacteristicsTable() {
    ui->charact_table->setColumnCount(1);
    ui->charact_table->setRowCount(8);

    ui->charact_table->setItem(0, 0, new QTableWidgetItem("Выборка"));
    ui->charact_table->setItem(0, 1, new QTableWidgetItem("Математическое \nожидание"));
    ui->charact_table->setItem(0, 2, new QTableWidgetItem("Дисперсия"));
    ui->charact_table->setItem(0, 3, new QTableWidgetItem("Исправленная \nдисперсия"));
    ui->charact_table->setItem(0, 4, new QTableWidgetItem("Медиана"));
    ui->charact_table->setItem(0, 5, new QTableWidgetItem("Минимум"));
    ui->charact_table->setItem(0, 6, new QTableWidgetItem("Максимум"));
    ui->charact_table->setItem(0, 7, new QTableWidgetItem("Размах"));

    ui->charact_table->resizeRowsToContents();
    ui->charact_table->resizeColumnToContents(0);
}
void MainWindow::setElementConfig() {
    // Создаем раздел в правом баре под критерии и сценарии
    content_widget = new QWidget(this);
    QVBoxLayout* content_layout = new QVBoxLayout(content_widget);
    content_layout->setAlignment(Qt::AlignTop);
    content_widget->setLayout(content_layout);

    // Довбляем раздел для критериев и сценариев в правый бар
    ui->scrollArea_rightbar->setWidget(content_widget);

    // Установка автодополнения в поиске по названиям критериев
    QCompleter* criterialsNamesCompliter = new QCompleter(Core::Criterial::CriterialsNames);
    criterialsNamesCompliter->setCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
    //criterialsNamesCompliter->setCompletionMode(QCompleter::InlineCompletion);
    criterialsNamesCompliter->setCompletionMode(QCompleter::PopupCompletion);
    ui->criterial_name_line->setCompleter(criterialsNamesCompliter);

    // Добавление форм по нажатию Enter у поля для ввода критериев
    connect(ui->criterial_name_line, SIGNAL(returnPressed()), this, SLOT(on_add_criterial_btn_clicked()));
    ui->criterial_name_line->setPlaceholderText("Начните вводить имя критерия (Например: Критерий знаков : точечная оценка)");

    // Запрет на редактирование таблицы данных и таблицы характеристик
    ui->sample_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->charact_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Добавление возможность перемещать и масштабировать графики
    ui->plot->setInteraction(QCP::Interaction::iRangeDrag);
    ui->plot->setInteraction(QCP::Interaction::iRangeZoom);

    // Убираем статус бар
    ui->statusbar->setEnabled(false);
    ui->statusbar->setVisible(false);
}
void MainWindow::createStatisticsMenu() {
    using namespace Core::Criterial;

    auto makeMenu = [this](Core::CriterialSection section) {
        QStringList criterials = getCriterialsBySection(section);
        QMenu* subMenu = new QMenu(CriterialSectionNameByType[section]);
        for(auto& name: criterials) {
            QAction* addCriterial = new QAction(name);
            addCriterial->setData(QVariant(criterialName(section, name)));
            connect(addCriterial, &QAction::triggered, this, &MainWindow::add_criterial_config);
            subMenu->addAction(addCriterial);
        }

        return subMenu;
    };

    QMenu* mainMenu = ui->nonparam_stats_menu;
    mainMenu->addMenu(makeMenu(Core::CriterialSection::DichotomicData));

    QMenu* shiftParamMenu = new QMenu("Параметр сдвига");
    shiftParamMenu->addMenu(makeMenu(Core::CriterialSection::ShiftProblem));
    shiftParamMenu->addMenu(makeMenu(Core::CriterialSection::SignRank));
    mainMenu->addMenu(shiftParamMenu);

    mainMenu->addMenu(makeMenu(Core::CriterialSection::TwoSampleShiftProblem));
    mainMenu->addMenu(makeMenu(Core::CriterialSection::TwoSampleScaleProblem));

}


/*
 *  Методы изменения внешнего вида приложения
*/
void MainWindow::setElementStyle() {
    //ui->samplegroup->setStyleSheet(StyleTable::QGroupBoxStyle());
    //ui->charactgroup->setStyleSheet(StyleTable::QGroupBoxStyle());
    //ui->leftbar->setStyleSheet(StyleTable::QFrameStyle());
    //ui->rightbar->setStyleSheet(StyleTable::QFrameStyle());
    ui->add_criterial_btn->setStyleSheet(StyleTable::acceptBtn());
}
void MainWindow::setPlotStyle() {
    ui->plot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->yAxis->setTickLabelColor(Qt::white);

    ui->plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));

    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    ui->plot->setBackground(QColor(67, 67, 67));
    ui->plot->axisRect()->setBackground(QColor(30, 30, 30));

}
