#include "button.h"
#include <QGraphicsTextItem>
#include <QBrush>

TButton::TButton(QString name, bool toggle_)
{
	// draw the rect
	setRect(0,0,100,20);
	toggle = toggle_;
	toggled();

	// draw the text
	text = new QGraphicsTextItem(name,this);
	int xPos = rect().width()/2 - text->boundingRect().width()/2;
	int yPos = rect().height()/2 - text->boundingRect().height()/2;
	text->setPos(xPos,yPos);

	// allow responding to hover events
	setAcceptHoverEvents(true);
}

void TButton::mousePressEvent(QGraphicsSceneMouseEvent *event){
	emit clicked();
}

void TButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
	// change color to gray
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(Qt::lightGray);
	setBrush(brush);
}

void TButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
	// change color to dark gray
	toggled();
}

void TButton::toggled()
{	
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);

	if (toggle)
		brush.setColor(Qt::lightGray);
	else
		brush.setColor(Qt::gray);

	setBrush(brush);
}
