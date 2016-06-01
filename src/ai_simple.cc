/** \file			ai_simple.cc
 *  \brief		Definition of class TAiSimple methods
 *  \author		David Novák, xnovak1m
 *  \date			8. 5. 2016
 *
 */

using namespace std;

#include "ai_simple.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

/**
 *	\brief Picks one valid field to place a stone on
 *	\param[out] x x-axis coordinate of placed stone
 *	\param[out] y y-axis coordinate of placed stone
 *	\param[in] color player making the turn
 *	\param[in] othelier pointer to game board, AI will look for valid fields there
 *	\return is this turn pass or not
 *	\note method overrides TAi::pick_field
 *
 *	Firstly, vector of valid places is created, then random field picked.
 */
bool TAiSimple::pick_field(int &x, int &y, Owner color, TOthelier *othelier)
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
	
	srand(time(NULL));
	int i = rand() % tmp.size();
	
	c = tmp[i];
	x = c.x;
	y = c.y;
		
	return false;
}
