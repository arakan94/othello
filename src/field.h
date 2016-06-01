#ifndef FIELD_INC
#define FIELD_INC

/** \file			field.h
 *  \brief		Header file containing class TField
 *  \author		David Novák, xnovak1m
 *  \date			19. 4. 2016
 *
 */

using namespace std;

#include "lib.h"

/**
 *	\brief TOthelier consists of objects of this class.
 *	
 *	Each field has it's owner (or is empty) and can be flipped by using flip().
 */
class TField {
public:
	int place(Owner new_owner);
	void remove();
	bool flip();

	Owner get() const;

	TField(Owner desired_owner = none): owner{desired_owner}
	{}

	~TField() {}

private:
	Owner owner;
};

#endif
