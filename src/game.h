#ifndef GAME_INC
#define GAME_INC

/** \file			game.h
 *  \brief		Header file containing class TGame
 *  \author		David Novák, xnovak1m
 *  \date			3. 5. 2016
 *
 */

using namespace std;

#include <memory>
#include <map>
#include <vector>

#include "othelier.h"
#include "player.h"
#include "history.h"
#include "lib.h"

/**
 *	\brief Main class of game logic
 *	\note Using singleton pattern
 *
 *	Savegame has following format: 'size ai_1 ai_2 ai_algorithm(string)\\n action\\n action\\n ...'\n
 *	For players, there are two pointers - curr and next (used for actual playing).\n
 *	AI algoritms are stored in map with string as key.
 */
class TGame {
public:
	/// current turn number
	int turn_count;
	
	/// current size of game board
	int size;
	
	bool execute(TCommand cmd);

	vector<string> get_difficulties();
	Owner get_owner(int x, int y);
	Owner get_player_color();
	string get_player_name();
	int get_player_stones();
	int get_player_stones(Owner player);
	int get_turn();
	bool is_ai();
	
	void change_count(int gain1, int gain2);
	void next_player();
	
	/// returns instance of TGame class (singleton)
	static TGame& get_instance()
	{
		static TGame instance{};
		return instance;
	}
	
	/// forbid copy constructor (singleton)
	TGame(TGame const&) = delete;
	
	/// forbid copy assingment (singleton)
	void operator=(TGame const&) = delete;

protected:
	/// constructor has to be protected or private (singleton)
	TGame();

private:
	TPlayer *next;
	TPlayer *curr;
	TPlayer *black_player;
	TPlayer *white_player;
	
	TOthelier *othelier;
	THistory  *history;
	
	TAi *simple_ai;
	TAi *easy_ai;
	
	string curr_ai;
	map<string, TAi*> ai_algols;
	
	void cleanup();
	
	bool next_turn();

	bool save();
	bool load();
	void new_game(int size = 8, bool black_ai = false, bool white_ai = false, string diff = "simple");
};

#endif
