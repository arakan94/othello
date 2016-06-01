#ifndef UI_CLI_INC
#define UI_CLI_INC

/** \file			ui_cli.h
 *  \brief		Header file containing class TUI
 *  \author		Roman Ondraszek, xondra43
 *  \date			19. 4. 2016
 *
 */

using namespace std;

#include <iostream>
#include "lib.h"
#include "command.h"

class TGame;

/**
 *	\brief Main class of game command line interface
 *	\note Using singleton pattern
 *
 *	This class contains main game loop
 */
class TUI{
public:
	
	/// returns instance of TUI class (singleton)
	static TUI& get_instance()
	{
		static TUI instance{};
		return instance;
	}

	void menu();
	
	void set_size(int size_);

	/// forbid copy constructor (singleton)
	TUI(TUI const&) = delete;

	/// forbid copy assingment (singleton)
	void operator=(TUI const&) = delete;

protected:
	
	/// constructor has to be protected or private (singleton)
	TUI()
	{
		size = 8;
		valid_stones = 0;
	}

private:
	void update();
	bool input();

	const string top_start{"\u250F"};
	const string top_line{"\u2501"};
	const string top_cross{"\u2533"};
	const string top_end{"\u2513"};

	const string middle_start{"\u2523"};
	const string middle_line{"\u2501"};
	const string middle_cross{"\u254B"};
	const string middle_end{"\u252B"};

	const string bottom_start{"\u2517"};
	const string bottom_line{"\u2501"};
	const string bottom_cross{"\u253B"};
	const string bottom_end{"\u251B"};

	const string content_cross{"\u2503"};

	int size;
	int valid_stones;
	int valid_stones_prev;
	bool visual = false;
	bool start = true;
	int undo_called = 0;

	void top_line_print();
	void middle_line_print();
	void bottom_line_print();
	void content_line_print(int y);
	void possible_commands();
	string get_winner();

	string get_str(string s);
	int get_size();
};

#endif
