#include <vector>
#include <string>
#include <iostream>

#include "types.h"
#include "position.h"
#include "knights.h"

namespace Epimedes
{
	namespace Knights
	{
		void generateKnightMoves(const Position& pos, std::vector<Move>& moves)
		{
			Color side = pos.getSideToMove();
			BitBoard pieceBoard = side == Color::WHITE ? pos.getBoard(Piece::WHITE_KNIGHT) : pos.getBoard(Piece::BLACK_KNIGHT);
			while(pieceBoard != 0)
			{
				int index = bitScanForward(pieceBoard);
				pieceBoard &= ~(1ULL << index);

				BitBoard knightMoves;
				if (index > 18) knightMoves = Position::KNIGHT_SPAN << (index - 18);
				else knightMoves = Position::KNIGHT_SPAN >> (18 - index);

				File file = toFile(index % 8);
				switch (file)
				{
					case File::FILE_A:
						knightMoves &= ~(Position::fileBoard(File::FILE_G) | Position::fileBoard(File::FILE_H));
						break;
					case File::FILE_B:
						knightMoves &= ~Position::fileBoard(File::FILE_H);
						break;
					case File::FILE_G:
						knightMoves &= ~Position::fileBoard(File::FILE_A);
						break;
					case File::FILE_H:
						knightMoves &= ~(Position::fileBoard(File::FILE_A) | Position::fileBoard(File::FILE_B));
						break;
				}

				knightMoves &= ~pos.getBoard(side);

				while(knightMoves != 0)
				{
					int mIndex = bitScanForward(knightMoves);
					knightMoves &= ~(1ULL << mIndex);

					Move move = (mIndex << 6) + index;
					moves.push_back(move);
				}
			}
		}
	}
}