#ifndef ACTION_INC
#define ACTION_INC

/** \file			action.h
 *  \brief		Header file containing class TAction
 *  \author		David Novák, xnovak1m
 *  \date			4. 5. 2016
 *
 */

using namespace std;

#include <list>
#include <fstream>

#include "othelier.h"
#include "lib.h"

/**
 *	\brief Stores one action of TPlayer.
 *	
 *	It can be either pass or turn. TAction can be executed or undone (command design pattern).
 *	TAction also have << and >> operators defined for saving/loading data (<< serializes content of TAction into sequence of characters).
 *
 *	Turn is stored as coordinates of placed stone and list of flipped stones.
 */
class TAction {
public:
	void execute(TOthelier *othelier);
	void undo(TOthelier *othelier);
	
	friend ostream & operator<< (ostream &out, const TAction &a);
	friend istream & operator>> (istream &in, TAction &a);

	/**
	 *	\brief Creates TAction
	 *	\param[in] x_ x-axis coordinate of placed stone
	 *	\param[in] y_ y-axis coordinate of placed stone
	 *	\param[in] owner_ owner of the stone
	 *	\param[in] pass_ is it pass or normal turn?
	 *
	 */	
	TAction(int x_, int y_, Owner owner_, bool pass_ = false): flipped{new list<int>}, x{x_}, y{y_}, owner{owner_}, pass{pass_}
	{}
	
	/// Deletes list of flipped stones
	~TAction()
	{
		delete flipped;
	}

	/// TAction must not be copied - double free of flipped -> forbid copy constructor
  TAction (const TAction& other) = delete;
  
  TAction& operator= (const TAction& other) = delete;
  
  /// move constructor is necessary (TAction is used in THistory as nodes of list)
	TAction (TAction&& other) noexcept : flipped(other.flipped), x(other.x), y(other.y), owner(other.owner), pass(other.pass) // steal pointers when moving
  {
    other.flipped = nullptr;
  }

  TAction& operator= (TAction&& other) noexcept  // move assingment
  {
      flipped = other.flipped;
      x = other.x;
      y = other.y;
      owner = other.owner;
      other.flipped = nullptr;
      pass = other.pass;
      return *this;
  }

	/// List of stones to be flipped
	list<int> *flipped;

private:
	int x, y;
	Owner owner;
	bool pass;
};

#endif
