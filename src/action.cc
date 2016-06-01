/** \file			action.cc
 *  \brief		Definition of class TAction methods
 *  \author		David Novák, xnovak1m
 *  \date			5. 5. 2016
 *
 */

using namespace std;

#include <sstream>

#include "action.h"
#include "game.h"

TGame &game = TGame::get_instance();

/**
 *	\brief Execute action (place stone and flip according to rules)
 *	\param[in] othelier pointer to game board where it should be executed
 *
 *	Pass only increase turn count and passes turn.
 */
void TAction::execute(TOthelier *othelier)
{
	if (!pass)
	{
		othelier->place(x, y, owner);
		othelier->flip(flipped);
		game.change_count(1 + flipped->size(), -flipped->size());
	}

	game.turn_count++;
	game.next_player();	
}  

/**
 *	\brief Undo action (remove stone and flip previously flipped stones)
 *	\param[in] othelier pointer to game board where it should be executed
 *
 *	Pass only decrease turn count and passes turn.
 */	
void TAction::undo(TOthelier *othelier)
{
	if (!pass)
	{
		othelier->remove(x, y);
		othelier->flip(flipped);
		game.change_count(flipped->size(), -flipped->size() - 1);
	}
	
	game.turn_count--;
	game.next_player();
}

/**
 *	\brief Serializes data for export to save in TGame
 *
 *	All data are ints (plus one enum), so it's easy to serialize. Individual values are separated by spaces.
 */	
ostream & operator<< (ostream &out, const TAction &a)
{
	out << a.x << " " << a.y << " " << a.owner << " " << a.pass;
	
	for (auto it = a.flipped->begin(); it != a.flipped->end(); it++)
	{
		out << " " << *it;
	}
	out << endl;
	
	return out;
}

/**
 *	\brief Reads data from input and fills TAction object
 *
 *	First three values are coordinates and owner (static_cast needed). Rest of line are values of list<int> flipped.
 */	
istream & operator>> (istream &in, TAction &a)
{
	int tmp;
	string list;
	
	in >> a.x >> a.y >> tmp >> a.pass;
	
	a.owner = static_cast<Owner>(tmp);
	
	getline(in, list);
	istringstream iss{list};
	
	while (iss >> tmp)
		a.flipped->push_back(tmp);
		
	return in;
}
