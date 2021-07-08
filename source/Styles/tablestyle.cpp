#include "tablestyle.h"

#include "colors.h"

#include <QStyleOptionViewItemV4>

myStyle::myStyle() :
    QProxyStyle()
{
}

void myStyle::drawControl(QStyle::ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
	switch( element )
	{
		case CE_ShapedFrame:
		{
			QLinearGradient gradient(0, 0, 0, opt->rect.height());
			gradient.setColorAt(0, QColor(255,255,255));
			gradient.setColorAt(0.5, QColor(225,225,225));
			gradient.setColorAt(1, QColor(255,255,255));
			QPen pen;
			pen.setBrush( gradient );
			p->setPen(pen);
			p->drawRect( 0,0,opt->rect.width()-1, opt->rect.height()-1);
		}
		break;

		case CE_ItemViewItem:
		{
			drawPrimitive(PE_PanelItemViewItem, opt, p, w);
			const QStyleOptionViewItemV4 * option = qstyleoption_cast<const QStyleOptionViewItemV4 *>(opt);
			if( !option )
			{
                //QStyle::drawControl(element, opt, p, w);
                QProxyStyle::drawControl(element, opt, p, w);
				return;
			}

			if (option->state & QStyle::State_HasFocus) {
                //QPen pen(QColor( 170,170,170 ));
                QPen pen(main_color.getColor());
				p->save();
				p->setPen(pen);
				p->drawRect(opt->rect.x(), opt->rect.y(), opt->rect.width()-1, opt->rect.height()-1);
				p->restore();
			}

			QRect textRect = subElementRect(SE_ItemViewItemText, option, w);
			textRect.setX( textRect.x() + 5 );
			textRect.setY( textRect.y() + 5 );
			if( !option->text.isEmpty() )
			{
				p->drawText(textRect, option->text);
			}
		}

		break;

		case CE_Header:
            QProxyStyle::drawControl(element, opt, p, w);
		break;

		case CE_HeaderEmptyArea:
			p->fillRect( opt->rect, QBrush( QColor( 233, 233, 233 ) ) );
		break;
		case CE_HeaderSection:
		{
			if( opt->state & State_Sunken || opt->state & State_On )
			{
				p->fillRect(opt->rect, QBrush( QColor(255,170,80) ));
				p->save();
                //p->setPen( QPen( QColor( 170,170,170) ) );
                p->setPen(QPen(main_color.getColor()));
				p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
				p->restore();
			}
			else
			{
				QLinearGradient gradient(0, 0, 0, opt->rect.height());
				gradient.setSpread(QGradient::PadSpread);
				gradient.setColorAt(0, QColor(255,255,255));
				gradient.setColorAt(1, QColor(220,220,220));
				p->fillRect(opt->rect, QBrush( gradient ));

				gradient.setColorAt(0, QColor(230,230,230));
				gradient.setColorAt(0.5, QColor(175,175,175));
				gradient.setColorAt(1, QColor(230,230,230));

				QPen pen;
				pen.setStyle(Qt::SolidLine);
				pen.setBrush(gradient);
				p->setPen(pen);
				p->drawLine( opt->rect.width()  + opt->rect.x() - 1,
							 opt->rect.y() + 3,
							 opt->rect.width()  + opt->rect.x() - 1,
							 opt->rect.height() + opt->rect.y() - 3 );
			}
		}
		break;
		default:
            QProxyStyle::drawControl(element, opt, p, w);
		break;
	}
}

void myStyle::drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const
{
	painter->setPen( QPen( QColor( 30, 30, 30 )));
	painter->setFont(QFont("Consolas"));
	painter->drawText(rect, text);
}

void myStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
	switch( pe )
	{
		case PE_PanelItemViewItem:
			if (const QStyleOptionViewItemV4 *option = qstyleoption_cast<const QStyleOptionViewItemV4 *>(opt))
				if ((option->state & QStyle::State_Selected))
					p->fillRect(option->rect, QBrush( QColor( 220,220,220,100 )));

		break;
		default:
            QProxyStyle::drawPrimitive( pe, opt, p, w);
		break;
	}
}
