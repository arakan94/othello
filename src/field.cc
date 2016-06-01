/** \file			field.cc
 *  \brief		Definition of class TField methods
 *  \author		David Novák, xnovak1m
 *  \date			19. 4. 2016
 *
 */

using namespace std;

#include "field.h"

/**
 *	\brief This method puts a stone to the field
 *	\param[in] new_owner a player that will be owner of placed stone
 *	\return number of placed stones
 *
 */
int TField::place(Owner new_owner)
{
	if (owner == white || owner == black)
		return 0;
	else
		owner = new_owner;

	return 1;
}

/**
 *	\brief This method removes stone.
 */
void TField::remove()
{
	owner = none;
}

/**
 *	\brief This function flips a stone.
 *	\return false if field was empty
 *
 */
bool TField::flip()
{
	if (owner == none || owner == valid)
		return false;
	else
		owner = (owner == black) ? white : black;

	return true;
}

/**
 *	\brief This function returns owner of field.
 *	\return owner of field
 *
 */	
Owner TField::get() const
{
	return owner;
}
