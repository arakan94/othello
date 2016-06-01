/** \file			othelier.cc
 *  \brief		Definition of class TOthelier methods
 *  \author		David Novák, xnovak1m
 *  \date			30. 4. 2016
 *
 */

using namespace std;

#include <iostream>

#include "othelier.h"

/**
 *	\brief This function puts a stone on coordinates.
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\param[in] owner a player that will be owner of placed stone 
 *
 *	It calls TField::place() method of particular field 
 */
int TOthelier::place(int x, int y, Owner owner)
{
	if (!(x < size && y < size))
	{
		cerr << "chybne koordinaty: [" << x << ", " << y << "]" << endl;
		return 0;	
	}
	
	if (!board[get_index(x,y)].place(owner))
		return 0;
	
	return 1;
}

/**
 *	\brief Fills list with stones to be flipped
 *	\param[inout] flip pointer to store list of ints (coordinates) to be flipped
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\param[in] owner find valid fields for this player
 *	
 */
void TOthelier::get_flipped(list<int> *flipped, int x, int y, Owner owner)
{
	check_line(x, y,  0,  1, owner, flipped);
	check_line(x, y,  0, -1, owner, flipped);
	check_line(x, y,  1,  0, owner, flipped);
	check_line(x, y, -1,  0, owner, flipped);
	check_line(x, y,  1,  1, owner, flipped);
	check_line(x, y,  1, -1, owner, flipped);
	check_line(x, y, -1,  1, owner, flipped);
	check_line(x, y, -1, -1, owner, flipped);
}

/**
 *	\brief This method flips stones according to list of coordinates
 *	\param[in] flip pointer to list of ints (coordinates) to be flipped
 */
void TOthelier::flip(list<int> *flip)
{
	for (auto i = flip->begin(); i != flip->end(); i++)
	{
		board[*i].flip();
	}
}

/**
 *	\brief This method removes a stone from coordinates.
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *
 *	It is used for backtracking in game history.
 */
void TOthelier::remove(int x, int y)
{
	if (x < size && y < size)
		board[get_index(x,y)].remove();
	else
		cout << "chybne koordinaty: [" << x << ", " << y << "]" << endl;
}

/**
 *	\brief Checks single line for validity
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\param[in] dx x difference
 *	\param[in] dy y difference
 *	\param[in] owner player asking
 *	\param[inout] flip list of stones to flip
 *	\return is valid?
 *
 *	If the field is valid, adds stones to be flipped to flip. However, method can be used without this feature (flip == nullptr) to only check validity.
 */	
bool TOthelier::check_line(int x, int y, int dx, int dy, Owner owner, list<int> *flip)
{
	bool valid = false;
	Owner this_field;
	
	list<int> tmp;	
	
	do {
		x += dx;
		y += dy;
	
		// check board bounds
		if (x < 0 || y < 0 || x >= size || y >= size)
		{
			valid = false;    // we reached border of board and didn't find this player's stone
			break;
		}
	
		this_field = board[get_index(x,y)].get();
		
		if (this_field == owner)  // stone is owned by this player
		{
			break;
		}
		else if (this_field != none)  // stone is owned by other player
		{
			valid = true;
			if (flip != nullptr)
				tmp.push_back(get_index(x,y));
			continue;
		}
		else                  // stone is empty -> not valid
		{
			valid = false;
		}
	} while(valid);
	
	if (flip != nullptr && valid)
	{
		tmp.sort();
		flip->sort();
		flip->merge(tmp);
	}
	
	return valid;
}

/**
 *	\brief This method looks trough lines. If there is one valid, field is valid
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\param[in] player find valid fields for this player
 *	\return is valid?
 *	
 *	| x-1 y-1 | x-1 y | x-1 y+1 |
 *	|  x y-1  |  x y  |  x y+1  |
 *	| x+1 y-1 | x+1 y | x+1 y+1 | 
 *
 */
bool TOthelier::is_valid(int x, int y, Owner player)
{
	if (check_line(x, y, 0, 1, player))
		return true;
	if (check_line(x, y, 0, -1, player))
		return true;
	if (check_line(x, y, 1, 0, player))
		return true;
	if (check_line(x, y, -1, 0, player))
		return true;
	if (check_line(x, y, 1, 1, player))
		return true;
	if (check_line(x, y, 1, -1, player))
		return true;
	if (check_line(x, y, -1, 1, player))
		return true;
	if (check_line(x, y, -1, -1, player))
		return true;
	
	return false;
}

/**
 *	\brief This method returns state of specific field.
 *	\param[in] x x-axis coordinate of stone
 *	\param[in] y y-axis coordinate of stone
 *	\param[in] owner player asking
 *	\return state of othelier (including validity check)
 *
 *	If the field is empty, method checks (based on asking player) whether it is valid for him to place stone on.
 *	This is used both in UI (highlightning valid places) and in AI.
 */	
Owner TOthelier::get(int x, int y, Owner owner)
{
	Owner this_field = board[get_index(x,y)].get();

	if (this_field == none)
	{
		if (is_valid(x, y, owner))
			return valid;
		else
			return none;
	}
	else
		return this_field;
}

/**
 *	\brief This method returns size of othelier.
 *	\return size of othelier
 */	
int TOthelier::get_size()
{
	return size;
}

