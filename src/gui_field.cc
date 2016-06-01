#include "gui_field.h"
#include "game.h"
#include "ui_gui.h"
#include <iostream>
#include <QTime>
#include <QCoreApplication>

using namespace std;

/// Function which make short delay to represent time of ai reaction
void delay()
{
	QTime dieTime= QTime::currentTime().addSecs(1);
	while (QTime::currentTime() < dieTime)
	QCoreApplication::processEvents(QEventLoop::AllEvents, 25);
}

void Tgui_field::set_coords(int y_, int x_)
{
	x = x_;
	y = y_;
}

void Tgui_field::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	TGame &game = TGame::get_instance();
	cout << "clicked:" << x << ":" << y << endl;
	cout <<  game.get_owner(y, x) << endl;

	if ((event->button() == Qt::LeftButton) && (game.get_owner(y, x) == 3)) // react only on left mouse button and field must be valid
	{
		TUI &ui = TUI::get_instance();
		game.execute(TCommand{coords,y,x});
		ui.update();

		if (game.is_ai())
		{	// when game mode is single player call ai step
			delay();
			TGame::get_instance().execute(TCommand{coords});
			TUI::get_instance().update();
		}
	}
}
