#ifndef PLAYER_INC
#define PLAYER_INC

/** \file			player.h
 *  \brief		Header file containing class TPlayer
 *  \author		David Novák, xnovak1m
 *  \date			1. 5. 2016
 *
 */

using namespace std;

#include <string>

#include "ai.h"
#include "lib.h"
#include "command.h"
#include "othelier.h"
#include "history.h"

/**
 *	\brief Defining player logic, processes turns
 *	\note For both human and ai players
 *
 *	Uses TOthelier and THistory from TGame and creates TAction objects from received TCommand objects.
 */
class TPlayer {
public:
	/// name of player (just flavor)
	string name;

	void make_turn(TCommand cmd);
	void change_count(int change);
	
	bool is_ai() const;
	int get_count() const;
	Owner get_color() const;
	
	TPlayer(string name_, TOthelier *othelier_, THistory *history_, Owner owner_, bool b_ai, TAi *ai);

	~TPlayer() {}

private:
	TOthelier *othelier;
	THistory  *history;
	
	Owner color;
	int stone_count;
	bool b_ai;
	
	/// pointer to base class TAi, actually pointing to TAiEasy or TAiSimple -> polymorphism
	TAi *ai;
};

#endif
