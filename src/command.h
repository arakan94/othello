#ifndef CMD_INC
#define CMD_INC
/** \file			command.h
 *  \brief		Header file containing type definitons
 *  \author		Roman Ondraszek, xondra43
 *  \date			29. 4. 2016
 */

using namespace std;

#include <string>

/// defines various TCommand types
enum Command{redo, undo, save, load, new_game, exit_game, coords, empty, error};

/**
 *	\brief Commands are used as interface between UI and TGame 
 *	\note Data are public for conviniency
 */
class TCommand {
public:

	/// Creates empty (pass) command, also used for commands without data (undo for example)
	TCommand(Command instruction = empty);

	/// Creates new_game command
	TCommand(Command instruction, bool ai_1_, bool ai_2_, int size_, string diffs_);

	/// Creates load/save command
	TCommand(Command instruction, string file_path);

	/// Creates coord (place a stone) command
	TCommand(Command instruction, int x_, int y_);

	~TCommand() {}
	
	/// type of cmd
	Command type;
	
	/// x-axis coordinate
	int x;
	
	/// y-axis coordinate
	int y;
	
	/// size to be set
	int size;
	
	/// is player 1 ai?
	bool ai_1;
	
	/// is player 2 ai?
	bool ai_2;
	
	/// path to file for load/save; existence of file already checked
	string file_path;
	
	/// selected difficulty of AI (selected from std::map)
	string diffs;

private:

};

#endif
