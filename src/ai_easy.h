#ifndef AI_EASY_INC
#define AI_EASY_INC

/** \file			ai_easy.h
 *  \brief		Header file containing class TAiEasy
 *  \author		David Novák, xnovak1m
 *  \date			8. 5. 2016
 *
 */

using namespace std;

#include "ai.h"

/**
 *	\brief Implementation of AI (picks by number of flipped stones)
 *
 */
class TAiEasy : public TAi {
public:

	virtual bool pick_field(int &x, int &y, Owner color, TOthelier *othelier) override;
	
	TAiEasy() {}

	virtual ~TAiEasy() {}

private:

};

#endif
