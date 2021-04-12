#pragma once

#include "position.h"

namespace Epimedes
{
	namespace Perft
	{
		void perftDivide(Position pos, int maxDepth);
		long perftSub(const Position& pos, int depth, int maxDepth);
	}
}