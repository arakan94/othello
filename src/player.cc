/** \file			player.cc
 *  \brief		Definition of class TPlayer methods
 *  \author		David Novák, xnovak1m
 *  \date			8. 5. 2016
 *
 */

using namespace std;

#include "player.h"
#include "action.h"

#include <iostream>

/**
 *	\brief Takes cmd and acts accordingly
 *	\param[in] cmd command to be processed
 *
 *	Takes coords, empty (pass), undo and redo and creates action or calls THistory.
 */
void TPlayer::make_turn(TCommand cmd)
{
	if (!b_ai)  // if player is not ai
	{		
		if (cmd.type == coords)
		{
			TAction *action = new TAction{cmd.x, cmd.y, color, false};    // create action
			othelier->get_flipped(action->flipped, cmd.x, cmd.y, color);  // fill it with list of flipped stones
			action->execute(othelier);   // execute it
			history->add(action);        // add it to history (move semantics)
			delete action;               // delete empty object
		}
		if (cmd.type == empty)  // create pass action
		{
			TAction *action = new TAction{0, 0, color, true};
			action->execute(othelier);
			history->add(action);
			delete action;
		}
		else if (cmd.type == undo)
		{
			history->undo(othelier);
		}
		else if (cmd.type == redo)
		{
			history->redo(othelier);
		}
	}
	else
	{
		if (cmd.type == coords)
		{
			int x = 0, y = 0;
			bool pass;
			
			pass = ai->pick_field(x, y, color, othelier);  // let ai pick field to place stone
			TAction *action = new TAction{x, y, color, pass};
			
			if (!pass)  // if there was no possible, pass turn
				othelier->get_flipped(action->flipped, x, y, color);
				
			action->execute(othelier);
			history->add(action);
			delete action;
		}
		if (cmd.type == empty)
		{
			TAction *action = new TAction{cmd.x, cmd.y, color, true};
			action->execute(othelier);
			history->add(action);
			delete action;
		}
		else if (cmd.type == undo)
		{
			history->undo(othelier);
		}
		else if (cmd.type == redo)
		{
			history->redo(othelier);
		}
	}
}

TPlayer::TPlayer(string name_, TOthelier *othelier_, THistory *history_, Owner owner_, bool b_ai, TAi *ai): name{name_}, othelier{othelier_}, history{history_}, color{owner_}, stone_count{2}, b_ai{b_ai}, ai{ai}
{
}

/// returns count of owned stones
int TPlayer::get_count() const
{
	return stone_count;
}

/// returns whether is ai or not
bool TPlayer::is_ai() const
{
	return b_ai;
}

/// returns color
Owner TPlayer::get_color() const
{
	return color;
}

/// allows to change count of stones
void TPlayer::change_count(int change)
{
	stone_count += change;
}
