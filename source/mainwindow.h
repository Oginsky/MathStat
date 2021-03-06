#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ToolWidgets/collapsibleSection.h"

#include "Graphics/graphicsmanager.h"

#include "Stats/sample.h"

#include <QMap>
#include <QString>
#include <QVariant>

class PlotObject;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Core { struct CriterialInfo; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void show_characteristic();
    void new_samples(QStringList);

private slots:
    /* Обработка добавления выборок */
    void add_samplesList(QList<Sample> samples);    // Регистрирует новые выборки и отправляет сигнал для обновления
    void show_samples_characteristics();            // Выводит характеристика новых выборок
    void on_add_sample_triggered();                 // Вызов формы считывания выборок
    void on_create_sample_triggered();              // Вызов формы повторных выборок
    void on_generate_sample_triggered();            // Вызов формы генерациии выборок

    /* Добавление и редактирование графиков */
    void add_graphic_object(QList<QString> plotObjects, QString sample_name, QString graphics_name);
    void on_add_graphics_triggered();               // Вызов формы добавления графиков

    /* Обработка критериев */
    void add_criterial_config();
    void on_add_criterial_btn_clicked();        // Добавляем форму настроек критериев
    void handleCiterialConfig(const Core::CriterialInfo* criterial, QMap<QString, QVariant> configs); // Обрабатываем введенные насройки критерия
    void remove_criterial();                    // Ощичает раздел с критериями


private:
    Ui::MainWindow *ui;

    /* Для хранения выборок */
    QList<Sample> samplesList;
    QStringList samplesName;

    /* Для хранения и управления графиками */
    GraphicsManager* graphicsManager;
    QList<PlotObject*> graphicsList;

    /* Отображение форм критериев и сценариев */
    QList<CollapsibleSection*> criterialsList;
    QWidget* content_widget;


    /*  Методы  */

    /* Начальные настройки */
    void createCharacteristicsTable();
    void setElementConfig();
    void createStatisticsMenu();

    /* Изменение внешнего вида приложения */
    void setElementStyle();
    void setPlotStyle();

};
#endif // MAINWINDOW_H

/* TODO-list:
 * Удаление выборок (Не забыть очистить с графиков)
 * Удаление виджетов с критериями
 * Экспорт выборок (Форматы: txt, csv, json)
 * Копирование выборок и создание новых "налету" (Посмотреть QBuffer)
 * График: Сглаженная эмпирическая плотность распределения
 * График: Отображение данных на графике (группировка)
 * Как-то связывать объекты (выборки с их графиками)
 * В TaskManager проверять удалось ли преобразовать в ожидаймый тип, иначе исключение (система исключений)
 * Система исключений?
 * Оптимизация:
 *      * Перейти на модели для хранения данных
 *      * Статические окна с умными указателями на них
 *      * Вынести TaskManager в отдельный поток
*/
