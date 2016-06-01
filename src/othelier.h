#ifndef OTHELIER_INC
#define OTHELIER_INC

/** \file			othelier.h
 *  \brief		Header file containing class TOthelier
 *  \author		David Novák, xnovak1m
 *  \date			25. 4. 2016
 *
 */

using namespace std;

#include <iostream>
#include <list>

#include "field.h"
#include "lib.h"

/**
 *	\brief Defines game board (array of TField)
 *	
 *	Size can be 6-12 (only even numbers), default is 8.
 */
class TOthelier {
public:
	int place(int x, int y, Owner owner);
	void flip(list<int> *flip = nullptr);
	void remove(int x, int y);

	int get_size();
	void get_flipped(list<int> *flipped, int x, int y, Owner owner);
	Owner get(int x, int y, Owner owner);

	/**
	 *	\brief Possible sizes are 6, 8, 10 and 12
	 *	\note If wrong size is set, default 8 is used
	 *
	 *	Initial stones are placed. Data are stored as array of TField.
	 */	
	TOthelier(int desired_size = 8)
	{
		if (desired_size < 6 || desired_size > 12 || desired_size % 2 != 0)
			size = 8;
		else
			size = desired_size;	
			
		board = new TField[size*size]();
		
		// initialization of board
		place(size/2-1, size/2-1, white);
		place(size/2-1, size/2, black);
		place(size/2, size/2-1, black);
		place(size/2, size/2, white);
	}

	~TOthelier()
	{
		delete[] board;
	}

private:
	int size;

	TField *board;
	
	bool is_valid(int x, int y, Owner next);
	bool check_line(int x, int y, int dx, int dy, Owner next, list<int> *flip = nullptr);
	inline int get_index(int x, int y)
	{
		return x*size + y;
	}
};

#endif
