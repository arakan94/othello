/** \file			hra2016.cc
 *  \brief		Main file of doxygen example.
 *  \author		David Novák, xnovak1m
 *  \date			9. 4. 2016
 *  \note			Something interesting about this file.
 *  \warning	Be careful with xyz
 *
 *	More detailed description
 *	of this file...
 *
 */

using namespace std;

#include <iostream>

#include <QApplication>
#include "ui_gui.h"

/**
 *	\brief This is main function of program.
 *
 *	It doesn't do much - it's just an example of doxygen use.
 */
int main(int argc, char *argv[])
{
	QApplication app{argc, argv};
	TUI &ui = TUI::get_instance();
	ui.show();
	ui.menu();

	return app.exec();
}
