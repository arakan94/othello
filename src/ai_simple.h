#ifndef AI_SIMPLE_INC
#define AI_SIMPLE_INC

/** \file			ai_simple.h
 *  \brief		Header file containing class TAiSimple
 *  \author		David Novák, xnovak1m
 *  \date			18. 4. 2016
 *  \note			Just a draft
 *
 */

using namespace std;

#include "ai.h"

/**
 *	\brief Implementation of AI (picks randomly)
 *
 */
class TAiSimple : public TAi {
public:

	virtual bool pick_field(int &x, int &y, Owner color, TOthelier *othelier) override;
	
	TAiSimple() {}

	virtual ~TAiSimple() {}

private:

};

#endif
