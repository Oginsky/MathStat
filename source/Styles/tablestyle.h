#ifndef MYSTYLE_H
#define MYSTYLE_H

//#include <QCommonStyle>
#include <QProxyStyle>
#include <QtGui>

class myStyle : public QProxyStyle
{
	Q_OBJECT
public:
	explicit myStyle();

	void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
	void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const;
	void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;

signals:

public slots:

};

#endif // MYSTYLE_H
