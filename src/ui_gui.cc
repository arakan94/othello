/** \file			ui_gui.cc
 *  \brief		Definition of class TUI methods
 *  \author		Roman Ondraszek, xondra43
 *  \date			19. 4. 2016
 *  \note			Without actual implementation so far
 *
 */

using namespace std;
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include <QGraphicsPolygonItem>
#include <QPointF>
#include <QLineF>
#include <QString>
#include <QFileDialog>

#include "ui_gui.h"
#include "game.h"
#include "button.h"

/**
 *	\brief This method setups game GUI.
 *	\warning Before calling this method it is requied to make new game instance by calling function run_game
 *	
 *	Create GUI of game which contains game field, stats and buttons for game controls.
 *	
 */
void TUI::set_game(){
	// set up the screen
	QGraphicsRectItem *frame = new QGraphicsRectItem{};
	field = (Tgui_field **) malloc(sizeof(Tgui_field) * size * size);
	stone = (QGraphicsPolygonItem **) malloc(sizeof(QGraphicsPolygonItem) * size * size);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	brush.setColor(QColor(139, 69, 19, 255));

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	width = (box_size * 16) + 40 + control_panel_size;
	height = (box_size * 16) + 40;
	setFixedSize(width, height);

	// centers game filed
	offset_view = (height / 2) - ((size * box_size + 40) / 2);

	// if scene is somehow null create new instance
	if (scene == NULL)
		scene = new QGraphicsScene{};

	scene->setSceneRect(0, 0, width, height);
	setScene(scene);
	scene->clear();
	frame->setRect(offset_view + 10, offset_view + 10, (box_size * size) + 20, (box_size * size) + 20);
	frame->setBrush(brush);
	scene->addItem(frame);		// adding component ro scene
	brush.setColor(QColor(44, 176, 55, 255));

	// Fills game filed with instances of Tgui_field
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
		{
			field[i * size + j] = new Tgui_field{};
			field[i * size + j]->set_coords(i, j);
			field[i * size + j]->setRect(offset_view + 20 + box_size * i, offset_view + 20 + box_size * j, box_size, box_size);
			field[i * size + j]->setBrush(brush);
			scene->addItem(field[i * size + j]);
		}

	// create buttons
	TButton* saveButton = new TButton(QString("Save"));
	int bxPos = width - control_panel_size + 25;
	int byPos = height - 40 - 120;
	saveButton->setPos(bxPos,byPos);
	connect(saveButton,SIGNAL(clicked()),this,SLOT(save_event()));		// connect slot to button
	scene->addItem(saveButton);

	TButton* undoButton = new TButton(QString("Undo"));
	byPos = height - 40 - 90;
	undoButton->setPos(bxPos,byPos);
	connect(undoButton,SIGNAL(clicked()),this,SLOT(undo_event()));
	scene->addItem(undoButton);

		TButton* redoButton = new TButton(QString("Redo"));
	byPos = height - 40 - 60;
	redoButton->setPos(bxPos,byPos);
	connect(redoButton,SIGNAL(clicked()),this,SLOT(redo_event()));
	scene->addItem(redoButton);

	TButton* passButton = new TButton(QString("Pass"));
	byPos = height - 40 - 30;
	passButton->setPos(bxPos,byPos);
	connect(passButton,SIGNAL(clicked()),this,SLOT(pass_event()));
	scene->addItem(passButton);

	TButton* exitButton = new TButton(QString("Back"));
	byPos = height - 40;
	exitButton->setPos(bxPos,byPos);
	connect(exitButton,SIGNAL(clicked()),this,SLOT(back_menu()));
	scene->addItem(exitButton);

	show();	// show scene to user
	stats_text();	// update game stats

	// placeing first stones on board
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			octagonal(i, j, value_owner(i,j));
}

/// Button event call starts game
void TUI::game_event()
{
	run_game();
}

/// Starts new game
void TUI::run_game()
{
	start = true;
	TGame &game = TGame::get_instance();

	vector<string> diffs = game.get_difficulties();
	game.execute(TCommand{new_game, ai_1, ai_2, size, diffs[diffs_index]});
	undo_called = 0;
	set_game();
}

/**
 *	\brief This method setups menu GUI.
 *	
 *	Create welcomming scene with brief menu.
 *	
 */
void TUI::menu()
{
	scene->clear();

	// create game title
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Othello"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	int txPos = width/2 - titleText->boundingRect().width()/2;
	int tyPos = 150;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);

	// create the play button
	TButton* playButton = new TButton(QString("Play"));
	int xPos = width/2 - playButton->boundingRect().width()/2;
	int byPos = 275;
	playButton->setPos(xPos,byPos);
	connect(playButton,SIGNAL(clicked()),this,SLOT(play_event()));
	scene->addItem(playButton);

	// create the load button
	TButton* loadButton = new TButton(QString("Load"));
	int lyPos = 350;
	loadButton->setPos(xPos,lyPos);
	connect(loadButton,SIGNAL(clicked()),this,SLOT(load_event()));
	scene->addItem(loadButton);

	// create the quit button
	TButton* quitButton = new TButton(QString("Quit"));
	int qyPos = 425;
	quitButton->setPos(xPos,qyPos);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(exit_event()));
	scene->addItem(quitButton);
}

/// for owner on given coordinates
Owner TUI::value_owner(int x, int y)
{
	TGame &game = TGame::get_instance();
	return game.get_owner(x, y);
}

/// Undo action event
void TUI::undo_event()
{
	TGame &game = TGame::get_instance();
	if ((game.get_player_stones(black) + game.get_player_stones(white)) > 4 )
	{
		undo_called++;
		game.execute(TCommand{undo});
		update();

		if (game.is_ai())
		{
			undo_called++;
			game.execute(TCommand{undo});
			update();
		}
	}
}

/// Redo action event
void TUI::redo_event()
{
	if (undo_called > 0)
	{
		undo_called--;
		TGame &game = TGame::get_instance();
		game.execute(TCommand{redo});
		update();

		if (game.is_ai())
		{
			undo_called--;
			game.execute(TCommand{redo});
			update();
		}
	}
}

/// save action opens file dialog
void TUI::save_event()
{
	QString file_string = QFileDialog::getSaveFileName(this, "Save Game", "./", "Save Files (*.sav)");
	if (file_string.toStdString() != "")
	{
		TGame &game = TGame::get_instance();
		string file_path = file_string.toStdString();
		if (file_path.substr(file_path.size()-4,file_path.size()) != ".sav")
			file_path += ".sav";
		game.execute(TCommand{save, file_path});
		update();
	}
}

/// load action opens file dialog
void TUI::load_event()
{
	QString file_string = QFileDialog::getOpenFileName(this, "Load Game", "./", "Save Files (*.sav)");
	if (file_string.toStdString() != "")
	{
		TGame &game = TGame::get_instance();
		game.execute(TCommand{load, file_string.toStdString()});
		size = game.size;
		undo_called = 0;
		set_game();
	}
}

/// pass action event
void TUI::pass_event()
{
	if (valid_stones == 0)
	{
		TGame &game = TGame::get_instance();
		game.execute(TCommand{empty});
		update();
		if (game.is_ai())
		{
			game.execute(TCommand{coords});
			update();
		}
	}
}


/**
 *	\brief This method setups settings GUI.
 *	
 *	Create scene with settings menu.
 *	
 */
void TUI::play_event()
{
	scene->clear();

	// create the game title
	QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("Othello\nSettings"));
	QFont titleFont("comic sans",50);
	titleText->setFont(titleFont);
	int txPos = width/2 - titleText->boundingRect().width()/2;
	int tyPos = 75;
	titleText->setPos(txPos,tyPos);
	scene->addItem(titleText);

	// create the play button
	TButton* playButton = new TButton(QString("Play"));
	int xPos = ((width / 4) * 3) - playButton->boundingRect().width()/8 + 40;
	int byPos = 300;
	playButton->setPos(xPos,byPos);
	connect(playButton,SIGNAL(clicked()),this,SLOT(game_event()));
	scene->addItem(playButton);

	// create the back button
	TButton* quitButton = new TButton(QString("Back"));
	int qyPos = 450;
	quitButton->setPos(xPos,qyPos);
	connect(quitButton,SIGNAL(clicked()),this,SLOT(back_menu()));
	scene->addItem(quitButton);

	// create button which set size
	TButton* size1Button = new TButton(QString("Set size to 6"), size1_t);
	int sxPos = width/4 - size1Button->boundingRect().width();
	int syPos = 275;
	size1Button->setPos(sxPos,syPos);
	connect(size1Button,SIGNAL(clicked()),this,SLOT(size1_set()));
	scene->addItem(size1Button);
	// create button which set size

	TButton* size2Button = new TButton(QString("Set size to 8"), size2_t);
	sxPos = width/4 - size1Button->boundingRect().width() + size1Button->boundingRect().width() + 20;
	size2Button->setPos(sxPos,syPos);
	connect(size2Button,SIGNAL(clicked()),this,SLOT(size2_set()));
	scene->addItem(size2Button);
	// create button which set size

	TButton* size3Button = new TButton(QString("Set size to 10"), size3_t);
	sxPos = width/4 - size1Button->boundingRect().width() + (size1Button->boundingRect().width() + 20)*2;
	size3Button->setPos(sxPos,syPos);
	connect(size3Button,SIGNAL(clicked()),this,SLOT(size3_set()));
	scene->addItem(size3Button);
	// create button which set size

	TButton* size4Button = new TButton(QString("Set size to 12"), size4_t);
	sxPos = width/4 - size1Button->boundingRect().width() + (size1Button->boundingRect().width() + 20)*3;
	size4Button->setPos(sxPos,syPos);
	connect(size4Button,SIGNAL(clicked()),this,SLOT(size4_set()));
	scene->addItem(size4Button);
	// create button which set size

	TButton* hvai1Button = new TButton(QString("Multi-Player"), hvai1_t);
	sxPos = width/4 - size1Button->boundingRect().width() + size1Button->boundingRect().width() + 20;
	int hyPos = 350;
	hvai1Button->setPos(sxPos,hyPos);
	connect(hvai1Button,SIGNAL(clicked()),this,SLOT(hvai1_set()));
	scene->addItem(hvai1Button);
	// create multi-player mode button

	TButton* hvai2Button = new TButton(QString("Single-Player"), hvai2_t);
	sxPos = width/4 - size1Button->boundingRect().width() + (size1Button->boundingRect().width() + 20)*2;
	hvai2Button->setPos(sxPos,hyPos);
	connect(hvai2Button,SIGNAL(clicked()),this,SLOT(hvai2_set()));
	scene->addItem(hvai2Button);
	// create single-player mode button

	if (ai_2)	// when single player is choosen allow to choose dificulty
	{
		QGraphicsTextItem* setText = new QGraphicsTextItem(QString("Set dificulty"));
		QFont titleFont("comic sans",14);
		setText->setFont(titleFont);
		int txPos = width/2 - titleText->boundingRect().width()/2;
		int tyPos = 380;
		setText->setPos(txPos,tyPos);
		scene->addItem(setText);

		TGame &game = TGame::get_instance();
		vector<string> diffs = game.get_difficulties();

		// create label containing name of current difficulty
		QGraphicsTextItem* difficulty = new QGraphicsTextItem(QString::fromStdString(diffs[diffs_index]));
		difficulty->setFont(titleFont);
		txPos = width/2 - titleText->boundingRect().width()/2;
		tyPos = 400;
		difficulty->setPos(txPos,tyPos);
		scene->addItem(difficulty);

		TButton* diff1Button = new TButton(QString("<"));
		txPos = txPos - diff1Button->boundingRect().width();
		int hyPos = 403;
		diff1Button->setPos(txPos,hyPos);
		connect(diff1Button,SIGNAL(clicked()),this,SLOT(diff1_set()));
		scene->addItem(diff1Button);
		// create previous button

		TButton* diff2Button = new TButton(QString(">"));
		txPos = width/2;
		diff2Button->setPos(txPos,hyPos);
		connect(diff2Button,SIGNAL(clicked()),this,SLOT(diff2_set()));
		scene->addItem(diff2Button);
		// create next button
	}
}

/// method which returns to menu
void TUI::back_menu()
{
	TGame &game = TGame::get_instance();

	if (start)
		game.execute(TCommand{exit_game});

	start = false;
	menu();
}

/// exit game event (close game entairly)
void TUI::exit_event()
{
	TGame &game = TGame::get_instance();

	if (start)
		game.execute(TCommand{exit_game});

	close();
}

/// method updating game stats
void TUI::stats_text()
{
	TGame &game = TGame::get_instance();
	string pom{"     Othello\n\nBlack: "};
	string pom2{to_string(game.get_player_stones(black))};
	string pom3{"\nWhite: "};
	string pom4{to_string(game.get_player_stones(white))};
	string pom5{"\nTurn: "};
	string pom6{to_string(game.get_turn())};
	string pom_final{""};

	if (game.get_player_color() == black)
		pom_final = pom + pom2 + pom3 + pom4 + pom5 + pom6 + "\n\nBlack";
	else
		pom_final = pom + pom2 + pom3 + pom4 + pom5 + pom6 + "\n\nWhite";

	stats = new QGraphicsTextItem(QString(pom_final.c_str()));
	QFont titleFont("comic sans",14);
	stats->setFont(titleFont);
	stats->setPos((box_size * 16) + 40 + 10 , 10);
	scene->addItem(stats);
}

/// set size button slot
void TUI::size1_set()
{
	size1_t = true;
	size2_t = false;
	size3_t = false;
	size4_t = false;
	size = 6;
	play_event();
}

/// set size button slot
void TUI::size2_set()
{
	size1_t = false;
	size2_t = true;
	size3_t = false;
	size4_t = false;
	size = 8;
	play_event();
}

/// set size button slot
void TUI::size3_set()
{
	size1_t = false;
	size2_t = false;
	size3_t = true;
	size4_t = false;
	size = 10;
	play_event();
}

/// set size button slot
void TUI::size4_set()
{
	size1_t = false;
	size2_t = false;
	size3_t = false;
	size4_t = true;
	size = 12;
	play_event();
}

/// hvai sets game mode (slot)
void TUI::hvai1_set()
{
	hvai1_t = true;
	hvai2_t = false;
	ai_2 = false;
	play_event();
}

/// hvai sets game mode (slot)
void TUI::hvai2_set()
{
	hvai1_t = false;
	hvai2_t = true;
	ai_2 = true;
	play_event();
}

/// previous button slot
void TUI::diff1_set()
{

	TGame &game = TGame::get_instance();

	vector<string> diffs = game.get_difficulties();
	vector<string>::size_type diffs_size = diffs.size();
	diffs_index--;

	if (diffs_index < 0)
		diffs_index = diffs_size - 1;

	play_event();
}

/// next button slot
void TUI::diff2_set()
{

	TGame &game = TGame::get_instance();

	vector<string> diffs = game.get_difficulties();
	vector<string>::size_type diffs_size = diffs.size();
	diffs_index++;
	
	if ((unsigned) diffs_index + 1 > diffs_size)
		diffs_index = 0;

	play_event();
}

/**
 *	\brief This method updates whole game GUI.
 *	
 *	Calls all needed functions to succesfully update game GUI.
 *	Check if someone wins game	
 */
void TUI::update()
{
	TGame &game = TGame::get_instance();
	valid_stones_prev = valid_stones;
	valid_stones = 0;

	// scene clearing
	scene->removeItem(stats);
	scene->removeItem(endText);
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			if (stone[i * size + j] != NULL)
				scene->removeItem(stone[i * size + j]);

	stats_text();

	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			octagonal(i, j, value_owner(i,j));

	// end game check
	if ( (valid_stones == 0 && valid_stones_prev == 0)
	|| ( (game.get_player_stones(white) + game.get_player_stones(black)) >= (size * size) )
	|| ( (game.get_player_stones(white) == 0 || game.get_player_stones(black) == 0) ) )
	{
		if (game.get_player_stones(white) < game.get_player_stones(black))
			endText = new QGraphicsTextItem(QString("Othello wins Black!"));
		else if (game.get_player_stones(white) > game.get_player_stones(black))
			endText = new QGraphicsTextItem(QString("Othello wins White!"));
		else
			endText = new QGraphicsTextItem(QString("It's Draw!"));

		QFont titleFont("comic sans",50);
		endText->setFont(titleFont);
		endText->setDefaultTextColor(QColor(255, 176, 55, 255));
		int txPos = (width - 150)/2 - endText->boundingRect().width()/2;
		int tyPos = height/2 - endText->boundingRect().height()/2;
		endText->setPos(txPos,tyPos);
		scene->addItem(endText);
	}
}

/**
 *	\brief This method create octogonal.
 *	
 *	Create ocogonal shape with qt library, an paint them to correct color.
 *	
 */
void TUI::octagonal(int x, int y, Owner owns)
{
	QVector<QPointF> points;
	points << QPoint(1,0) << QPoint(2,0) << QPoint(3,1) << QPoint(3,2) << QPoint(2,3)
			   << QPoint(1,3) << QPoint(0,2) << QPoint(0,1);

	// scale points
	int SCALE_FACTOR = 10;
	for (size_t i = 0, n = points.size(); i < n; i++){
		points[i] *= SCALE_FACTOR;
	}

	// create the QGraphicsPolygonItem
	stone[x * size + y] = new QGraphicsPolygonItem{QPolygonF(points)};

	// move the polygon
	QPointF poly_center;
	poly_center = mapToScene(1.5 * SCALE_FACTOR, 1.5 * SCALE_FACTOR);
	QPointF field_center(box_size/2, box_size/2);
	QLineF ln(poly_center,field_center);
	stone[x * size + y]->setPos(offset_view + 20 + box_size * x + box_size / 8, offset_view + 20 + box_size * y + box_size / 8);

	QBrush brush;
	brush.setStyle(Qt::SolidPattern);

	// choose correct color
	switch(owns)
	{
		case black:
			brush.setColor(QColor(0, 0, 0, 255));
			stone[x * size + y]->setBrush(brush);
			scene->addItem(stone[x * size + y]);
		break;

		case white:
			brush.setColor(QColor(255, 255, 255, 255));
			stone[x * size + y]->setBrush(brush);
			scene->addItem(stone[x * size + y]);
		break;

		case valid:
			brush.setColor(QColor(255, 176, 55, 100));
			stone[x * size + y]->setBrush(brush);
			scene->addItem(stone[x * size + y]);
			valid_stones++;
		break;

		default:
		break;
	}
}
