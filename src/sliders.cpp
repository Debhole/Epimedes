#include <vector>
#include <string>
#include <iostream>

#include "sliders.h"
#include "types.h"

namespace Epimedes
{
	namespace Sliders
	{
		void generateSliderMoves(const Position& pos, std::vector<Move>& moves)
		{
			generateHorizontalAndVerticalMoves(pos, moves);
			generateDiagonalAndAntiDiagonalMoves(pos, moves);
		}


		void generateHorizontalAndVerticalMoves(const Position& pos, std::vector<Move>& moves)
		{
			Color side = pos.getSideToMove();
			BitBoard pieceBoard = side == Color::WHITE ? (pos.getBoard(Piece::WHITE_ROOK) | pos.getBoard(Piece::WHITE_QUEEN)) :
				(pos.getBoard(Piece::BLACK_ROOK) | pos.getBoard(Piece::BLACK_QUEEN));

			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pieceBoard >> i) & 1) == 0) 
					continue;

				Rank rank = toRank(i / 8);
				File file = toFile(i % 8);

				BitBoard pPos = 1ULL << i;
				BitBoard horizontalMoves = ((pos.getOccupied() - 2 * pPos) ^ reverse(reverse(pos.getOccupied()) - 2 * reverse(pPos))) & Position::rankBoard(rank);
				BitBoard verticalMoves = (((pos.getOccupied() & Position::fileBoard(file)) - 2 * pPos) ^ reverse(reverse((pos.getOccupied() & Position::fileBoard(file))) - 2 * reverse(pPos))) & Position::fileBoard(file);

				BitBoard possibleMoves = (horizontalMoves | verticalMoves) & ~pos.getBoard(side);

				for (int j = 0; j < sizeof(BitBoard) * 8; j++)
				{
					if (((possibleMoves >> j) & 1) == 1)
					{
						Move move = (j << 6) + i;
						moves.push_back(move);
					}
				}
			}
		}
		void generateDiagonalAndAntiDiagonalMoves(const Position& pos, std::vector<Move>& moves)
		{
			Color side = pos.getSideToMove();
			BitBoard pieceBoard = side == Color::WHITE ? (pos.getBoard(Piece::WHITE_BISHOP) | pos.getBoard(Piece::WHITE_QUEEN)) :
				(pos.getBoard(Piece::BLACK_BISHOP) | pos.getBoard(Piece::BLACK_QUEEN));

			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pieceBoard >> i) & 1) == 0)
					continue;

				Rank rank = toRank(i / 8);
				File file = toFile(i % 8);

				BitBoard pPos = 1ULL << i;
				BitBoard diagonalMoves = (((pos.getOccupied() & Position::diagonalBoard(rank, file)) - 2 * pPos) 
										^ reverse(reverse((pos.getOccupied() & Position::diagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::diagonalBoard(rank, file);
				BitBoard antiDagionalMoves = (((pos.getOccupied() & Position::antiDiagonalBoard(rank, file)) - 2 * pPos)
					^ reverse(reverse((pos.getOccupied() & Position::antiDiagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::antiDiagonalBoard(rank, file);

				BitBoard possibleMoves = (diagonalMoves | antiDagionalMoves) & ~pos.getBoard(side);

				for (int j = 0; j < sizeof(BitBoard) * 8; j++)
				{
					if (((possibleMoves >> j) & 1) == 1)
					{
						Move move = (j << 6) + i;
						moves.push_back(move);
					}
				}
			}
		}
	}
}