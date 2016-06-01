/** \file			game.cc
 *  \brief		Definition of class TGame methods
 *  \author		David Novák, xnovak1m
 *  \date			7. 5. 2016
 *
 */

using namespace std;

#include "game.h"
#include "command.h"
#include "ai_simple.h"
#include "ai_easy.h"

/**
 *	\brief Main method of TGame - interface receiving commands from UI
 *	\param[in] cmd command to be executed
 *	\warning Before doing anything with TGame, new_game command must be executed.
 *	\note To avoid memory leaks, execute exit_game command when ending game and going back to menu.
 *
 *	
 */
bool TGame::execute(TCommand cmd)
{
	fstream file;

	switch (cmd.type)
	{
		// these commands are sent to current player to handle
		case Command::coords:
		case Command::undo:
		case Command::redo:
		case Command::empty:
			curr->make_turn(cmd);
		break;
		
		case Command::load:
			file.open(cmd.file_path, ios_base::in);
			
			if (!file.is_open())
			{
				cerr << cmd.file_path << " cannot be opened!" << endl;
				return false;
			}
			else
			{
				int size;
				bool ai_1, ai_2;
				string diff;
				
				file >> size >> ai_1 >> ai_2 >> diff;  // load gamestate
				
				new_game(size, ai_1, ai_2, diff);   // create game environment
				
				file >> (*history);      // load history
				history->load(othelier);  // execute loaded actions
				file.close();
			}
		break;
		
		case Command::save:
			file.open(cmd.file_path, ios_base::out);
			if (!file.is_open())
			{
				cerr << cmd.file_path << " cannot be opened!" << endl;
				return false;
			}
			else
			{
				file << size << " "  << black_player->is_ai() << " "  << white_player->is_ai() << " " << curr_ai << endl;  // save gamestate
				file << (*history);   // save history
				file.close();
			}
		break;
		
		case Command::new_game:
			new_game(cmd.size, cmd.ai_1, cmd.ai_2, cmd.diffs);
		break;
		
		case Command::exit_game:
			cleanup();  // destroy TGame and clean everything
		break;
		
		// should never occur
		case Command::error:
			cerr << "GUI poslal error!" << endl;
			cleanup();
			return false;
		break;
		
		// should never occur
		default:
			cerr << "input() vratil neocekavany prikaz!" << endl;
			cleanup();
			return false;
		break;
	}
	
	return true;
}

/**
 *	\brief switches players
 */
void TGame::next_player()
{
	TPlayer *tmp = curr;
	curr = next;
	next = tmp;
}

/**
 *	\brief Creates new game environment
 *	\param[in] size_ requested size of game board
 *	\param[in] black_ai_ is black player AI?
 *	\param[in] white_ai_ is white player AI?
 *	\param[in] diff what AI should be used
 */
void TGame::new_game(int size_, bool black_ai, bool white_ai, string diff)
{	
	if (!ai_algols.count(diff))
	{
		cerr << "Non-existing AI algoritm!!" << endl;
		exit(100);
	}

	othelier     = new TOthelier{size_};
	history      = new THistory{};
	black_player = new TPlayer{"Player 1", othelier, history, black, black_ai, ai_algols[diff]};
	white_player = new TPlayer{"Player 2", othelier, history, white, white_ai, ai_algols[diff]};

	curr_ai = diff;
	turn_count = 0;
	size = size_;
	curr = black_player;
	next = white_player;
}

/**
 *	\brief cleans game environment
 */
void TGame::cleanup()
{
	delete black_player;
	delete white_player;
	delete history;
	delete othelier;
		
	curr = nullptr;
	next = nullptr;
}

/// here can be added another AI algoritm implementation
TGame::TGame(): turn_count{0}
{
	simple_ai = new TAiSimple{};
	easy_ai = new TAiEasy{};
	
	ai_algols.insert( std::pair<string, TAi*>("simple", simple_ai) );
	ai_algols.insert( std::pair<string, TAi*>("easy", easy_ai) );
}

/**
 *	\brief get all AI alogrithms
 *	\return vector of algoritm's names
 */
vector<string> TGame::get_difficulties()
{
	vector<string> tmp;
	
	for (auto it = ai_algols.begin(); it != ai_algols.end(); it++)
		tmp.push_back(it->first);
	
	return tmp;
}

/**
 *	\brief return state (owned by player, none, valid) of stone
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\return state of specific stone
 */
Owner TGame::get_owner(int x, int y)
{
	return othelier->get(x, y, curr->get_color());
}

/**
 *	\brief which player is on turn?
 *	\return color of current player
 */
Owner TGame::get_player_color()
{
	return curr->get_color();
}

/**
 *	\brief what is current player's name?
 *	\return name of current player
 */
string TGame::get_player_name()
{
	return curr->name;
}

/**
 *	\brief how many stones current player has?
 *	\return number of current player's stones
 */
int TGame::get_player_stones()
{
	return curr->get_count();
}

/**
 *	\brief how many stones a player has?
 *	\param[in] player get this player's number of stones
 *	\return number of player's stones
 */
int TGame::get_player_stones(Owner player)
{
	if (player == black)
		return black_player->get_count();
	else
		return white_player->get_count();
}

/**
 *	\brief get current turn number
 */
int TGame::get_turn()
{
	return turn_count;
}

/**
 *	\brief change number of stones
 *	\param[in] gain1 change for current player
 *	\param[in] gain2 change for next player
 */
void TGame::change_count(int gain1, int gain2)
{
	curr->change_count(gain1);
	next->change_count(gain2);
}

/**
 *	\brief is current player an ai?
 *	\return true if current player is ai
 */
bool TGame::is_ai()
{
	return curr->is_ai();
}

