#include "stylesheets.h"

#include "colors.h"

StyleTable::StyleTable()
{

}

QString StyleTable::QTabWidgetStyle() {
    return "QTabWidget::pane {"
            "border: 1px solid white;"
            "background: rgb(53, 53, 53);"          // Использовать UIColor background_color
           "}"
           "QTabWidget::tab-bar:top {"
            "top: 1px;"
           "}"
           "QTabWidget::tab-bar:bottom {"
            "bottom: 1px;"
           "}"
           "QTabWidget::tab-bar:left {"
            "right: 1px;"
           "}"
           "QTabWidget::tab-bar:right {"
            "left: 1px;"
           "}"
           "QTabBar::tab {"
            "border: 1px solid white;"
           "}"
           "QTabBar::tab:selected {"
            "background: rgb(53, 53, 53);"          // Использовать UIColor background_color
           "}"
           "QTabBar::tab:!selected {"
           "background: silver;"
           "}"
            "QTabBar::tab:top, QTabBar::tab:bottom {"
            "min-width: 8ex;"
            "margin-right: -1px;"
            "padding: 5px 10px 5px 10px;"
           "}"
           "QTabBar::tab:top:selected { border-bottom-color: none; }"
           "QTabBar::tab:bottom:selected { border-top-color: none; }"
           "QTabBar::tab:top:last, QTabBar::tab:bottom:last, QTabBar::tab:top:only-one, QTabBar::tab:bottom:only-one {"
            "margin-right: 0;"
           "}"
           "QTabBar::tab:left:!selected { margin-right: 3px; }"
           "QTabBar::tab:right:!selected { margin-left: 3px; }"
           "QTabBar::tab:left, QTabBar::tab:right {"
            "min-height: 8ex;"
            "margin-bottom: -1px;"
            "padding: 10px 5px 10px 5px;"
           "}"
           "QTabBar::tab:left:selected { border-left-color: none; }"
           "QTabBar::tab:right:selected { border-right-color: none; }"
           "QTabBar::tab:left:last, QTabBar::tab:right:last, QTabBar::tab:left:only-one, QTabBar::tab:right:only-one {"
            "margin-bottom: 0;"
           "}";
}

QString StyleTable::QGroupBoxStyle() {
    return "color" + group_color.getCssColor() + ";"
           "qproperty-alignment: AlignCenter;";
}

QString StyleTable::QFrameStyle()
{
    return "QFrame { border: none; }";
}

QString StyleTable::acceptBtn()
{
    return "float: right;"
           "margin-right:5px;"
           "border-radius: 5px;"
           "-moz-border-radius: 5px;"
           "-webkit-border-radius: 5px;"
           "padding: 5px 9px;"
           "font-size: 1.2em;"
           "color: rgb(255, 255, 255);"
           "background-color: rgb(207, 26, 68);"
           "text-shadow: #454545 0 0 2px;"
           "border-botton: 4px solid rgba(217, 91, 72, 1);";
           //"QPushButton:hover { color: rgb(207, 26, 68);"
           //"background-color: white; }";
}

QString StyleTable::hypothTable() {
    return "background: rgb(47, 49, 54);"
           "border: none;"
           "color: white;";
}
