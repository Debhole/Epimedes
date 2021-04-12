#pragma once

#include <vector>
#include <string>

#include "types.h"
#include "position.h"

namespace Epimedes
{
	namespace Pawns
	{
		void generatePawnMoves(const Position& pos, std::vector<Move>& moves);

		void generateWhitePawnCaptures(const Position& pos, std::vector<Move>& moves);
		void generateWhitePawnPushes(const Position& pos, std::vector<Move>& moves);
		void generateWhitePawnPromotions(const Position& pos, std::vector<Move>& moves);

		void generateBlackPawnCaptures(const Position& pos, std::vector<Move>& moves);
		void generateBlackPawnPushes(const Position& pos, std::vector<Move>& moves);
		void generateBlackPawnPromotions(const Position& pos, std::vector<Move>& moves);
	}
}