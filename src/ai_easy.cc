/** \file			ai_easy.cc
 *  \brief		Definition of class TAiEasy methods
 *  \author		David Novák, xnovak1m
 *  \date			8. 5. 2016
 *
 */

using namespace std;

#include "ai_easy.h"

#include <iostream>
#include <vector>

/**
 *	\brief Picks one valid field to place a stone on
 *	\param[out] x x-axis coordinate of placed stone
 *	\param[out] y y-axis coordinate of placed stone
 *	\param[in] color player making the turn
 *	\param[in] othelier pointer to game board, AI will look for valid fields there
 *	\return is this turn pass or not
 *	\note method overrides TAi::pick_field
 *
 *	Firstly, vector of valid places is created, then field with biggest number of flipped stones is found.
 *	If there is no valid field, turn is pass (return true)
 */
bool TAiEasy::pick_field(int &x, int &y, Owner color, TOthelier *othelier)
{
	typedef struct {
		int x;
		int y;
	} Coords;
	
	Coords c;
	
	vector<Coords> tmp;
	
	for (int i = 0; i < othelier->get_size(); i++)
	{
		for (int j = 0; j < othelier->get_size(); j++)
		{
			if (othelier->get(i, j, color) == valid)
			{
				c.x = i;
				c.y = j;
				tmp.push_back(c);
			}
		}
	}
	
	if (tmp.size() == 0)
		return true;	
	
	unsigned int max_i = 0;
	unsigned int max = 0;
	
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		list<int> flipped;
		othelier->get_flipped(&flipped, x, y, color);
		
		if (flipped.size() > max)
		{
			max = flipped.size();
			max_i = i;
		}
		flipped.clear();
	}
	
	c = tmp[max_i];
	x = c.x;
	y = c.y;
		
	return false;
}
