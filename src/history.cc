/** \file			history.cc
 *  \brief		Definition of class THistory methods
 *  \author		David Novák, xnovak1m
 *  \date			8. 5. 2016
 *
 */

using namespace std;

#include "history.h"

/**
 *	\brief Adds action to history
 *	\param[in] action pointer to TAction that should be added
 *
 *	Move semantic is used to add actions to list. Iterator is set to list.end()
 */
void THistory::add(TAction *action)
{
	if (!actions.empty() && it != actions.end())
		actions.erase(it, actions.end());
		
	actions.push_back(move(*action));
	it = actions.end();
}

/**
 *	\brief Undoes current action
 *	\param[in] othelier pointer to game board where action should be executed
 *
 */
void THistory::undo(TOthelier *othelier)
{
	if (it == actions.begin())
		return;
	it--;
	it->undo(othelier);
}

/**
 *	\brief Redoes current action
 *	\param[in] othelier pointer to game board where action should be executed
 *
 */
void THistory::redo(TOthelier *othelier)
{
	if (it == actions.end())
		return;
	it->execute(othelier);
	it++;
}

/**
 *	\brief Executes all actions after load from save
 *	\param[in] othelier pointer to game board where actions should be executed
 *
 */
void THistory::load(TOthelier *othelier)
{
	for (auto tmp_it = actions.begin(); tmp_it != actions.end(); tmp_it++)
	{
		tmp_it->execute(othelier);
		
	}
}

/// outputs all actions using their << operator
ostream & operator<< (ostream &out, const THistory &h)
{
	for (auto it = h.actions.begin(); it != h.actions.end(); it++)
	{
		out << *it;
	}

	return out;
}

/// reads actions from file until EOF
istream & operator>> (istream &in, THistory &h)
{
	bool end = false;
	
	while (!end)
	{
		TAction *action = new TAction{0, 0, none};
		if (in >> (*action))
			h.add(action);
		else
			end = true;
		
		delete action;
	}
	return in;
}
