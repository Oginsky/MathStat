/*
    Elypson/qt-collapsible-CollapsibleSection
    (c) 2016 Michael A. Voelkel - michael.alexander.voelkel@gmail.com

    This file is part of Elypson/qt-collapsible CollapsibleSection.

    Elypson/qt-collapsible-CollapsibleSection is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Elypson/qt-collapsible-CollapsibleSection is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Elypson/qt-collapsible-CollapsibleSection. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CollapsibleSection_H
#define CollapsibleSection_H

#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>

class CollapsibleSection : public QWidget {
    Q_OBJECT
private:

    QGridLayout* mainLayout;
    QToolButton* toggleButton;
    QFrame* headerLine;
    QParallelAnimationGroup* toggleAnimation;
    QScrollArea* contentArea;
    int animationDuration;


public slots:

    void toggle(bool collapsed);


public:
    explicit CollapsibleSection(const QString & title = "", const int animationDuration = 100, QWidget* parent = 0);

    void setContentLayout(QLayout & contentLayout);
};

#endif // CollapsibleSection_H
