#ifndef COLORS_H
#define COLORS_H

#include <QColor>

struct UIColor {
private:
    QColor q_color;
    QString css_color;

public:
    UIColor(QColor qc, QString cssc) :
        q_color(qc), css_color(cssc) {

    }

    QColor getColor() const {
        return q_color;
    }

    QString getCssColor() const {
        return css_color;
    }

};

static const UIColor main_color({255, 26, 68}, "rgb(255, 26, 68)");
static const UIColor background_color({53, 53, 53}, "rgb(53, 53, 53)");
static const UIColor group_color({255, 255, 255}, "rgb(255, 255, 255)");

#endif // COLORS_H
