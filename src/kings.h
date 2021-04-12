#pragma once
#include <vector>
#include <string>

#include "types.h"
#include "position.h"

namespace Epimedes
{
	namespace Kings
	{
		void generateKingMoves(const Position& pos, std::vector<Move>& moves);
		
		void generateKingWalks(const Position& pos, std::vector<Move>& moves);
		void generateCastles(const Position& pos, std::vector<Move>& moves);
	}
}