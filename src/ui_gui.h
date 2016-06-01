#ifndef UI_GUI_INC
#define UI_GUI_INC

/** \file			ui_gui.h
 *  \brief		Header file containing class TUI
 *  \author		Roman Ondraszek, xondra43
 *  \date			19. 4. 2016
 *
 */

using namespace std;

#include <iostream>
#include "lib.h"
#include "command.h"
#include "gui_field.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QColor>
#include <QRgb>
#include <QComboBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsItem>
#include <QPointF>
#include <QObject>


class TGame;
/**
 *	\brief Main class of game graphic user interface
 *	\note Using singleton pattern
 *
 *	This class uses extensivly Qt graphics. All buttons are connected to slots.
 *	This class contains main game loop
 */
class TUI: public QGraphicsView {
	Q_OBJECT
public:

	/// returns instance of TUI class (singleton)
	static TUI& get_instance()
	{
		static TUI instance{};
		return instance;
	}

	void update();
	void run_game();
	void menu();
	void set_game();

	void set_size(int size_)
	{
		cout << "Size set to: " << size_ << endl;
		size = size_;
	}

	/// forbid copy constructor (singleton)
	TUI(TUI const&) = delete;

	/// forbid copy assingment (singleton)
	void operator=(TUI const&) = delete;

protected:
	
	/// constructor has to be protected or private (singleton)
	TUI(int size = 8): size{size}
	{	
		// set fixed size and turn off scrollbars
		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		width = (box_size * 16) + 40 + control_panel_size;
		height = (box_size * 16) + 40;
		setFixedSize(width, height);

		// set up the scene
		if (scene == NULL)
			scene = new QGraphicsScene{};

		scene->setSceneRect(0, 0, width, height);
		setScene(scene);
		scene->clear();
	}

	QGraphicsScene* scene = NULL;

	int box_size = 40;
	int control_panel_size = 150;

public slots:
	void undo_event();
	void redo_event();
	void save_event();
	void pass_event();
	void play_event();
	void exit_event();
	void load_event();
	void game_event();
	void back_menu();
	void size1_set();
	void size2_set();
	void size3_set();
	void size4_set();
	void hvai1_set();
	void hvai2_set();
	void diff1_set();
	void diff2_set();

private:

	int size;
	bool visual = false;
	bool start = false;
	bool ai_1 = false;
	bool ai_2 = false;
	bool size1_t = false;
	bool size2_t = true;
	bool size3_t = false;
	bool size4_t = false;
	bool hvai1_t = true;
	bool hvai2_t = false;
	bool diff1_t = true;
	bool diff2_t = false;
	int undo_called = 0;
	int valid_stones;
	int valid_stones_prev;
	Tgui_field **field;
	QGraphicsPolygonItem **stone;
	QGraphicsTextItem *stats;
	QGraphicsTextItem* endText;
	int height;
	int width;
	int offset_view;
	int diffs_index = 0;

	void stats_text();
	void octagonal(int x, int y, Owner owns);
	static Owner value_owner(int x, int y);
};

#endif
