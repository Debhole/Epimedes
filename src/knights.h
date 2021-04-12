#pragma once
#include <vector>
#include <string>

#include "types.h"
#include "position.h"

namespace Epimedes
{
	namespace Knights
	{
		void generateKnightMoves(const Position& pos, std::vector<Move>& moves);
	}
}