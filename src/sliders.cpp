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

			while (pieceBoard != 0)
			{
				int index = bitScanForward(pieceBoard);
				pieceBoard &= ~(1ULL << index);

				Rank rank = toRank(index / 8);
				File file = toFile(index % 8);

				BitBoard pPos = 1ULL << index;
				BitBoard horizontalMoves = ((pos.getOccupied() - 2 * pPos) ^ reverse(reverse(pos.getOccupied()) - 2 * reverse(pPos))) & Position::rankBoard(rank);
				BitBoard verticalMoves = (((pos.getOccupied() & Position::fileBoard(file)) - 2 * pPos) ^ reverse(reverse((pos.getOccupied() & Position::fileBoard(file))) - 2 * reverse(pPos))) & Position::fileBoard(file);

				BitBoard possibleMoves = (horizontalMoves | verticalMoves) & ~pos.getBoard(side);

				while (possibleMoves != 0)
				{
					int mIndex = bitScanForward(possibleMoves);
					possibleMoves &= ~(1ULL << mIndex);

					Move move = (mIndex << 6) + index;
					moves.push_back(move);
				}
			}
		}
		void generateDiagonalAndAntiDiagonalMoves(const Position& pos, std::vector<Move>& moves)
		{
			Color side = pos.getSideToMove();
			BitBoard pieceBoard = side == Color::WHITE ? (pos.getBoard(Piece::WHITE_BISHOP) | pos.getBoard(Piece::WHITE_QUEEN)) :
				(pos.getBoard(Piece::BLACK_BISHOP) | pos.getBoard(Piece::BLACK_QUEEN));

			while(pieceBoard != 0)
			{
				int index = bitScanForward(pieceBoard);
				pieceBoard &= ~(1ULL << index);

				Rank rank = toRank(index / 8);
				File file = toFile(index % 8);

				BitBoard pPos = 1ULL << index;
				BitBoard diagonalMoves = (((pos.getOccupied() & Position::diagonalBoard(rank, file)) - 2 * pPos) 
										^ reverse(reverse((pos.getOccupied() & Position::diagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::diagonalBoard(rank, file);
				BitBoard antiDagionalMoves = (((pos.getOccupied() & Position::antiDiagonalBoard(rank, file)) - 2 * pPos)
					^ reverse(reverse((pos.getOccupied() & Position::antiDiagonalBoard(rank, file))) - 2 * reverse(pPos))) & Position::antiDiagonalBoard(rank, file);

				BitBoard possibleMoves = (diagonalMoves | antiDagionalMoves) & ~pos.getBoard(side);

				while(possibleMoves != 0)
				{
					int mIndex = bitScanForward(possibleMoves);
					possibleMoves &= ~(1ULL << mIndex);

					Move move = (mIndex << 6) + index;
					moves.push_back(move);
				}
			}
		}
	}
}