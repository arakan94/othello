/** \file			ui_cli.cc
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
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>

#include "ui_cli.h"
#include "game.h"

/**
 *	\brief Function strips all white spaces from the left side of string
 *	\param[in] s string to be stripped
 *	\return stripped string
 */
static inline void lstrip(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
}

/**
 *	\brief Function checks if given string is number
 *	\param[in] input string to check
 *	
 */
inline bool is_numeric(const std::string& input) {
	return std::all_of(input.begin(), input.end(), ::isdigit);
}

/**
 *	\brief Sets size to given size
 *	\param[in] size_ int to be set
 *	
 */
void TUI::set_size(int size_)
{
	cout << "Size set to: " << size_ << endl;
	size = size_;
	start = true;
	undo_called = 0;
}

/// returns game size
inline int TUI::get_size()
{
	return size;
}

/**
 *	\brief Splits first word from the rest of string and retunrs it
 *	\param[in] s string to be splited
 *	\return first word
 */
string TUI::get_str(string s)
{
	string tmp("");
	string tmp2("");

	tmp = s.substr(0,1);
	while(tmp != "" && tmp != " " && tmp != "\t" && tmp != "\n" && tmp != "\r")
	{
		tmp2 += tmp;
		s = s.substr(1,s.length() - 1);
		tmp = s.substr(0,1);
	}
	return tmp2;
}

/// print out all avalible commands
void TUI::possible_commands()
{
	TGame &game = TGame::get_instance();
	
	cout << "(";
	
	if (game.is_ai())
		cout << "AI TURN confirm by hitting ENTER, ";

	if (undo_called > 0)
	{
		cout << "redo, ";
	}
	if (!start)
	{
		cout << "undo, save, ";
	}
	if (valid_stones == 0)
	{
		cout << "pass or exit)" << endl;
	}
	else
		cout << "coordinates or exit)" << endl;
}

/// method handeling user imput
bool TUI::input()
{
	TGame &game = TGame::get_instance();

	bool end = false;
	string cmd;
	string c1("");
	string c2("");
	string pom("");

	// print avalible commands
	possible_commands();

	while(1){
	
		// check if game ended
		if ((valid_stones == 0 && valid_stones_prev == 0) || ((game.get_player_stones(white) + game.get_player_stones(black)) == (size * size)) || game.get_player_stones(white) == 0 || game.get_player_stones(black) == 0)
		{
			cout << "End of game!" << endl << endl << get_winner() << endl;
			cout << "Press enter to continue" << endl;
			string tmp;
			getline(cin, tmp);
			game.execute(TCommand{exit_game});
			end = true;
			break;
		}
		else if (valid_stones == 0)
			cout << "You cannot place a stone! Type 'pass' to pass turn to your enemy: " << endl;
		else	
			cout << "Command: ";
	
		// user give command 
		getline(cin, cmd);
		
		// handling user imput and executing given command
		if (cmd == "" && game.is_ai()){
			game.execute(TCommand{coords});
			break;
		}
		else if (cmd.substr(0,4) == "exit" || end) 
		{
			game.execute(TCommand{exit_game});
			end = true;
			break;
		}
		if (cmd.substr(0,4) == "pass")
		{
			game.execute(TCommand{empty});
			break;
		}
		else if (cmd.substr(0,4) == "redo" && undo_called > 0) 
		{
			undo_called--;
			game.execute(TCommand{redo});
			if (game.is_ai())
			{
				undo_called--;
				game.execute(TCommand{redo});
			}
			start = false;
			break;
		}
		else if (cmd.substr(0,4) == "undo" && !start)
		{
			undo_called++;
			game.execute(TCommand{undo});
			if (game.is_ai())
			{
				undo_called++;
				game.execute(TCommand{undo});
			}
			if (game.get_turn() == 0)
				start = true;           // don't display undo
			break;
		}
		else if (cmd.substr(0,4) == "save" && !start)
		{
			// asking for file path
			string file_path;
			update();
			cout << "Save command. Specify path." << endl << "Path: ";
			getline(cin, file_path);
			if (file_path.size() < 4 || file_path.substr(file_path.size()-4,file_path.size()) != ".sav")
					file_path += ".sav";
			
			ofstream file;
			file.open(file_path);
			if (file.is_open())	// tests file
			{
				file.close();
				game.execute(TCommand{save, file_path});
				break;
			}
		}
		else if (valid_stones != 0)
		{
			lstrip(cmd);
			// coordinates handeling

			if (cmd != "" && cmd.length() > 1 && cmd.length() <= 3)
			{
				// parsing input
				c1 = get_str(cmd);
				cmd = cmd.substr(c1.length(), cmd.length() - c1.length());
				lstrip(cmd);
				c2 = get_str(cmd);

				if (c1.length() <= 3)
				{
					if (c1.length() != 1)
					{
						if ((c1.substr(0,1) >= "A" && c1.substr(0,1) <= "Z") || (c1.substr(0,1) >= "a" && c1.substr(0,1) <= "z"))
						{
							c2 = c1.substr(1,2);
							c1 = c1.substr(0,1);
						}
						else
						{
							if ((c1.substr(1,1) >= "A" && c1.substr(1,1) <= "Z") || (c1.substr(1,1) >= "a" && c1.substr(1,1) <= "z"))
							{
								c2 = c1.substr(0,1);
								c1 = c1.substr(1,1);
							}
							else
							{
								c2 = c1.substr(0,2);
								c1 = c1.substr(2,1); 
							}
						}
					}

					if ((c2 >= "A" && c2 <= "Z") || (c2 >= "a" && c2 <= "z"))
					{
						pom = c2;
						c2 = c1;
						c1 = pom;
					}
					
					if (is_numeric(c2))
					{
						int y = stoi(c2) - 1;
						int x;

						if ((c1[0u] - 'a') < 0)
							x = c1[0u] - 'A';
						else
							x = c1[0u] - 'a';

						// checking validity
						if ((x >= 0 && x < size) && (y >= 0 && y < size))
						{
							if (game.get_owner(x, y) == valid)
							{	// executing
								start = false;
								undo_called = 0;
								game.execute(TCommand{coords, x, y});
								break;
							}
							else
							{
								cout << "On " << x << " & " << y << "stone can't be placed. ";
							}
						}
					}
				}
			}			
		}
		// updating
		update();	

		cout << "Command Error! ";
		possible_commands();

	}
	// updating
	if (!end)
		update();
	return end;
}

/// updates game CLI
void TUI::update()
{
	valid_stones_prev = valid_stones;
	valid_stones = 0;

	for (int i = 0; i < 50; ++i)
		cout << endl;

	top_line_print();
	content_line_print(0);

	for (int i = 1; i < size; ++i)
	{
		middle_line_print();
		content_line_print(i);
	}

	bottom_line_print();
}

/// Top layer of game board
void TUI::top_line_print()
{
	string line_str("");
	line_str += top_start;
	line_str += top_line + top_line + top_line;

	for (int i = 1; i < size; i++)
	{
		line_str += top_cross + top_line + top_line + top_line;
	}
	
	line_str += top_end;

	for (char aplhabet = 'A'; aplhabet - 'A' < size; aplhabet++)
	{
		cout << "  " << aplhabet << " ";
	}

	cout << endl << line_str << endl;
}

/// Middle layer of game board
void TUI::middle_line_print()
{	
	string line_str("");
	line_str += middle_start;
	line_str += middle_line + middle_line + middle_line;

	for (int i = 1; i < size; i++)
		line_str += middle_cross + middle_line + middle_line + middle_line;
	
	line_str += middle_end;

	cout << line_str << endl;
}

/// Bottom layer of game board
void TUI::bottom_line_print()
{
	string line_str("");
	line_str += bottom_start;
	line_str += bottom_line + bottom_line + bottom_line;

	for (int i = 1; i < size; i++)
		line_str += bottom_cross + bottom_line + bottom_line + bottom_line;
	
	line_str += bottom_end;

	cout << line_str << endl;
}

/// function which returns winner
string TUI::get_winner()
{
	TGame &game = TGame::get_instance();

	int p1 = game.get_player_stones(black);
	int p2 = game.get_player_stones(white);

	if (p1 == p2)
		return "It's a DRAW!";
	if (p1 > p2)
		return "Black WINS!";
	else
		return "White WINS!";
}

/// Content layer of game board, informative layer
void TUI::content_line_print(int y)
{
	TGame &game = TGame::get_instance();

	string line_str("");
	string content_str("");

	for (int i = 0; i < size; i++)
	{
		// choose right representation of owner
		switch(game.get_owner(i, y))
		{
			case none: 
				content_str = " ";
				break;

			case black: 
				content_str = "\u25CF";
				break;

			case white: 
				content_str = "\u25CB";
				break;

			case valid:
				content_str = "\u25CC";
				valid_stones++;
				break;

			default:
				content_str = " ";
		}

		line_str += content_cross + " " + content_str + " ";
	}

	// game stats
	line_str += content_cross;
	if (y == 1)
		cout << line_str << " " << y + 1 << "\t" << "  TAH: " << game.get_turn() << endl;
	else if (y == (size / 2 - 1))
	{
		if ((game.get_player_color() == black))
			cout << line_str << " " << y + 1 << "\t" << "> BLACK: " << game.get_player_stones(black) << endl;
		else
			cout << line_str << " " << y + 1 << "\t" << "  BLACK: " << game.get_player_stones(black) << endl;
	}
	else if (y == (size / 2))
	{
		if ((game.get_player_color() == white))
			cout << line_str << " " << y + 1 << "\t" << "> WHITE: " << game.get_player_stones(white) << endl;
		else
			cout << line_str << " " << y + 1 << "\t" << "  WHITE: " << game.get_player_stones(white) << endl;
	}
	else
		cout << line_str << " " << y + 1 << endl;
}	

/// Game menu
void TUI::menu()
{
	TGame &game = TGame::get_instance();
	bool bad = false;
	bool ai_2 = true;
	int sizer = 8;
	string load_file_path;
	string cmd;
	string pom;
	vector<string> diffs = game.get_difficulties();
	vector<string>::size_type diffs_size = diffs.size();
	int diffs_index = 0;

	// main game loop
	while(1){
		// menu loop
		while(1){
			for (int i = 0; i < 50; ++i)
				cout << endl;

			cout << "\t\t\tOthello - MENU" << endl << endl;
			cout << "\t\tCurrent game settings:" << endl;
			cout << "\t\tGame style: " << ((ai_2)? "Single-player":"Multi-player") << endl;
			cout << "\t\tDifficuly: " << diffs[diffs_index] << endl;
			cout << "\t\tBoard Size: " << sizer << endl << endl;
			cout << "\tYou can load your saved game by typing 'load' and then valid filepath." << endl;
			cout << "\tValible commands: load, size, style (to switch to other one)," << endl;
			cout << "\t                  start (to start game), exit (to quit game)," << endl;
			cout << "\t                  diffs (to change difficulty of AI opponent)" << endl;
			if (!bad)
				cout << endl << endl << endl << endl << "\t\t\tCommand: ";
			else
				cout << endl << endl << endl << "\t\t\tCommand Error on: " << cmd << endl << "\t\t\tCommand: ";

			bad = false;
			getline(cin, cmd);

			if (cmd.substr(0,4) == "load")
			{
				string file_path;
				cout << "Load command. Specify path." << endl << "Path: ";
				getline(cin, file_path);
				if (file_path.substr(0, 1) != "/")
					file_path = "./" + file_path;
				if (file_path.size() < 4 || file_path.substr(file_path.size()-4,file_path.size()) != ".sav")
					file_path += ".sav";
				ifstream file;

				file.open(file_path);
				if (file.is_open())
				{
					file.close();
					game.execute(TCommand{load, file_path});
					size = game.size;
					start = false;
					undo_called = 0;
					break;
				}
				else
					bad = true;
			}
			else if (cmd.substr(0,4) == "size")
			{
				cout << "Set size command (6 / 8 / 10 / 12)." << endl << "Set to: ";
				getline(cin, pom);
				if (is_numeric(pom))
				{
					if (stoi(pom) >= 6 && stoi(pom) <= 12 && (stoi(pom) % 2) == 0 )
						sizer = stoi(pom);
					else
						bad = true;
				}
				else
					bad = true;
			}
			else if (cmd.substr(0,5) == "style")
			{
				ai_2 = !ai_2;
			}
			else if (cmd.substr(0,5) == "diffs")
			{
				for (int i = 0; i < 50; ++i)
					cout << endl;

				cout << "Choose one by entering number of difficulty." << endl;

				for (unsigned int i = 0; i < diffs_size; i++)
				{
					cout << i + 1 << ": " << diffs[i] << endl;
				}

				cout << "Set to: ";
				getline(cin, pom);
				if (is_numeric(pom))
				{
					if (stoi(pom) > 0 && (unsigned) stoi(pom) <= diffs_size)
						diffs_index = stoi(pom) - 1;
					else
						bad = true;
				}
				else
					bad = true;
			}
			else if (cmd.substr(0,5) == "start" && cmd.length() < 6) 
			{
				game.execute(TCommand{new_game, false, ai_2, sizer, diffs[diffs_index]});
				size = game.size;
				start = true;
				undo_called = 0;
				break;
			}
			else if (cmd.substr(0,4) == "exit") 
			{
				exit(0);
			}
			else
				bad = true;
		}
		update();

		// game loop
		while(!input());
	}
}
