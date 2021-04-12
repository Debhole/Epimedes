#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "perft.h"
#include "position.h"
#include "pawns.h"
#include "sliders.h"
#include "knights.h"
#include "kings.h"

namespace Epimedes
{
	namespace Perft
	{
		void perftDivide(Position pos, int maxDepth)
		{
			auto start = std::chrono::high_resolution_clock::now();

			std::vector<Move> moves;
			Pawns::generatePawnMoves(pos, moves);
			Sliders::generateSliderMoves(pos, moves);
			Knights::generateKnightMoves(pos, moves);
			Kings::generateKingMoves(pos, moves);

			long totalMoveCount = 0;

			for (const Move& move : moves)
			{
				Position perftPos(pos);
				perftPos.makeMove(move);
				perftPos.updateBoard();
				if (perftPos.isLegal(move))
				{
					long moveCount = perftSub(perftPos, 1, maxDepth);
					totalMoveCount += moveCount;

					std::cout << moveToAlgebraic(move) << ": " << moveCount << "\n";
				}
			}

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = end - start;

			std::cout << totalMoveCount << " moves in " << duration.count() * 1000 << "ms " 
					  << "at " << totalMoveCount / duration.count() / 1000 << "knps\n";
		}

		long perftSub(const Position& pos, int depth, int maxDepth)
		{
			long moveCount = 0;

			if (depth >= maxDepth) return 1;

			std::vector<Move> moves;
			Pawns::generatePawnMoves(pos, moves);
			Sliders::generateSliderMoves(pos, moves);
			Knights::generateKnightMoves(pos, moves);
			Kings::generateKingMoves(pos, moves);

			for (const Move& move : moves)
			{
				Position perftPos(pos);
				perftPos.makeMove(move);
				perftPos.updateBoard();
				if (perftPos.isLegal(move))
				{
					moveCount += perftSub(perftPos, depth + 1, maxDepth);
				}
			}
			return moveCount;
		}
	}
}