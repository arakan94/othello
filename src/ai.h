#ifndef AI_INC
#define AI_INC

/** \file			ai.h
 *  \brief		Header file containing class TAi
 *  \author		David Novák, xnovak1m
 *  \date			25. 4. 2016
 *
 */

using namespace std;

#include "othelier.h"

/**
 *	\brief Abstract class for game's AI
 *	
 *	Every AI implementation will be a class derived from this class and will implement its methods.
 */
class TAi {
public:
	/**
	 *	\brief Picks one valid field to place a stone on
	 *	\param[out] x x-axis coordinate of placed stone
	 *	\param[out] y y-axis coordinate of placed stone
	 *	\param[in] color player making the turn
	 *	\param[in] othelier pointer to game board, AI will look for valid fields there
	 *	\return is this turn pass or not
	 *	\note method is pure virtual (not implemented in TAi)
	 */
	virtual bool pick_field(int &x, int &y, Owner color, TOthelier *othelier)=0;

	TAi() {}

	virtual ~TAi() {}

private:

};

#endif
