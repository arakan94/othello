#ifndef GUI_FIELD_H
#define GUI_FIELD_H
/** \file			gui_field.h
 *  \brief		Header file containing class Tgui_field
 *  \author		Roman Ondraszek, xondra43
 *  \date			29. 4. 2016
 */

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QRgb>

#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QGraphicsRectItem>
#include <QObject>
#include <QMouseEvent>
#include <QTimer>
#include "command.h"
#include "game.h"

/**
 *	\brief Class represents field
 *
 *	Class made to represents ingame fields and actions on them.
 */
class Tgui_field : public QGraphicsRectItem
{
public:
	/// Set x and y coordinates
	void set_coords(int y_, int x_);

	/// public event method
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
	int x;
	int y;
	bool valid;
};

#endif
