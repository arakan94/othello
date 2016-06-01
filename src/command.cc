/** \file			command.h
 *  \brief		Header file containing type definitons
 *  \author		Roman Ondraszek, xondra43
 *  \date			29. 4. 2016
 *
 */

using namespace std;

#include "command.h"

TCommand::TCommand(Command instruction)
{
	type = instruction;
}

TCommand::TCommand(Command instruction, bool ai_1_, bool ai_2_, int size_, string diffs_)
{
	type = instruction;
	size = size_;
	ai_1 = ai_1_;
	ai_2 = ai_2_;
	diffs = diffs_;
}

TCommand::TCommand(Command instruction, string file_path_)
{
	type = instruction;
	file_path = file_path_;
}

TCommand::TCommand(Command instruction, int x_, int y_)
{
	type = instruction;
	x = x_;
	y = y_;
}
