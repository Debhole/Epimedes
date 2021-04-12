#pragma once
#include <vector>
#include <string>

#include "types.h"
#include "position.h"

namespace Epimedes
{
	namespace Sliders
	{
		void generateSliderMoves(const Position& pos, std::vector<Move>& moves);

		void generateHorizontalAndVerticalMoves(const Position& pos, std::vector<Move>& moves);
		void generateDiagonalAndAntiDiagonalMoves(const Position& pos, std::vector<Move>& moves);
	}
}