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
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pieceBoard >> i) & 1) == 0) continue;

				BitBoard knightMoves;
				if (i > 18) knightMoves = Position::KNIGHT_SPAN << (i - 18);
				else knightMoves = Position::KNIGHT_SPAN >> (18 - i);

				File file = toFile(i % 8);
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

				for (int j = 0; j < sizeof(BitBoard) * 8; j++)
				{
					if (((knightMoves >> j) & 1) == 1)
					{
						Move move = (j << 6) + i;
						moves.push_back(move);
					}
				}
			}
		}
	}
}