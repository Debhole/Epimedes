#pragma once
#include <iostream>
#include <sstream>

#include "position.h"

namespace Epimedes
{
	namespace Engine
	{
		void loop();

		void position(Position& pos, std::istream& istream);
		void genMoves(Position& pos, std::istream& istream);
		void makeMove(Position& pos, std::istream& istream);
		void perft(Position, std::istream& istream);
	} //namespace Engine
} //namespace Epimedes