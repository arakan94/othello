#ifndef BUTTON_H
#define BUTTON_H
/** \file			button.h
 *  \brief		Header file containing class TButton
 *  \author		Roman Ondraszek, xondra43
 *  \date			29. 4. 2016
 */


#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

/**
 *	\brief Custom button class
 *
 *	Class made to suits needs of GUI. Allows to toggle buttons.
 */
class TButton:public QObject, public QGraphicsRectItem{
	Q_OBJECT
public:
	/// constructor
	TButton(QString name, bool toggle_ = false);

	/// public methods (events)
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	/// switch state to oposite of current
	void toggled();

signals:
	/// signal handeling
	void clicked();

private:
	QGraphicsTextItem* text;
	bool toggle;
};

#endif
