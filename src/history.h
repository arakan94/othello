#ifndef HISTORY_INC
#define HISTORY_INC

/** \file			history.h
 *  \brief		Header file containing class THistory
 *  \author		David Novák, xnovak1m
 *  \date			7. 5. 2016
 *
 */

using namespace std;

#include <list>

#include "action.h"

/**
 *	\brief Contains list of actions and enables undo/redo.
 *
 *	Supports << and >> operators (saves/loads whole history of TAction). When new action is added and undo was used, undid part of history is erased
 *	and replaced with new action. Iterator is used to store current position in list.
 */
class THistory {
public:
	void add(TAction *action);
	
	void undo(TOthelier *othelier);
	void redo(TOthelier *othelier);
	
	void load(TOthelier *othelier);

	friend ostream & operator<< (ostream &out, const THistory &h);
	friend istream & operator>> (istream &in, THistory &h);
	
	THistory()
	{
		it = actions.end();
	}

	~THistory() {}

private:
	list<TAction> actions;
	list<TAction>::iterator it;
};

#endif
