#include <vector>
#include <string>
#include <iostream>

#include "types.h"
#include "position.h"
#include "kings.h"

namespace Epimedes
{
	namespace Kings
	{
		void generateKingMoves(const Position& pos, std::vector<Move>& moves)
		{
			generateKingWalks(pos, moves);
			generateCastles(pos, moves);
		}

		void generateKingWalks(const Position& pos, std::vector<Move>& moves)
		{
			Color side = pos.getSideToMove();
			int index = bitScanForward(side == Color::WHITE ? pos.getBoard(Piece::WHITE_KING) : pos.getBoard(Piece::BLACK_KING));

			BitBoard kingMoves;
			if (index > 9) kingMoves = Position::KING_SPAN << (index - 9);
			else kingMoves = Position::KING_SPAN >> (9 - index);

			File file = toFile(index % 8);
			if (file == File::FILE_A) kingMoves &= ~Position::fileBoard(File::FILE_H);
			else if (file == File::FILE_H) kingMoves &= ~Position::fileBoard(File::FILE_A);

			kingMoves &= ~pos.getBoard(side);

			while (kingMoves != 0)
			{
				int mIndex = bitScanForward(kingMoves);
				kingMoves &= ~(1ULL << mIndex);

				Move move = (mIndex << 6) + index;
				moves.push_back(move);
			}
		}
		void generateCastles(const Position& pos, std::vector<Move>& moves)
		{
			if (pos.getSideToMove() == Color::WHITE)
			{
				if (pos.canCastle(0))
				{
					BitBoard interference = pos.getOccupied() & ((1ULL << Position::INITIAL_ROOK_SQUARES[0]) - 2 * pos.getBoard(Piece::WHITE_KING));
					BitBoard attackOverlaps = ((1ULL << Position::INITIAL_ROOK_SQUARES[0]) - pos.getBoard(Piece::WHITE_KING)) & pos.getAttacked(Color::BLACK);
					if (interference == 0 && attackOverlaps == 0)
					{
						Move move = (6 << 6) + 4 + (2 << 14);
						moves.push_back(move);
					}
				}
				if (pos.canCastle(1))
				{
					BitBoard interference = pos.getOccupied() & (pos.getBoard(Piece::WHITE_KING) - 2ULL);
					BitBoard attackOverlaps = ((pos.getBoard(Piece::WHITE_KING) - 2ULL) << 1) & pos.getAttacked(Color::BLACK);
					if (interference == 0 && attackOverlaps == 0)
					{
						Move move = (2 << 6) + 4 + (2 << 14);
						moves.push_back(move);
					}
				}
			}
			else
			{
				if (pos.canCastle(2))
				{
					BitBoard interference = pos.getOccupied() & ((1ULL << Position::INITIAL_ROOK_SQUARES[2]) - 2 * pos.getBoard(Piece::BLACK_KING));
					BitBoard attackOverlaps = ((1ULL << Position::INITIAL_ROOK_SQUARES[2]) - pos.getBoard(Piece::BLACK_KING)) & pos.getAttacked(Color::WHITE);
					if (interference == 0 && attackOverlaps == 0)
					{
						Move move = (62 << 6) + 60 + (2 << 14);
						moves.push_back(move);
					}
				}
				if (pos.canCastle(3))
				{
					BitBoard interference = pos.getOccupied() & ((pos.getBoard(Piece::BLACK_KING) - 2 * (1ULL << Position::INITIAL_ROOK_SQUARES[3])));
					BitBoard attackOverlaps = (((pos.getBoard(Piece::BLACK_KING) - (2ULL << Position::INITIAL_ROOK_SQUARES[3]))) << 1) & pos.getAttacked(Color::WHITE);
					if (interference == 0 && attackOverlaps == 0)
					{
						Move move = (58 << 6) + 60 + (2 << 14);
						moves.push_back(move);
					}
				}
			}
		}
	}
}