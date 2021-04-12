#include <vector>
#include <string>
#include <iostream>

#include "pawns.h"
#include "types.h"

namespace Epimedes
{
	namespace Pawns
	{
		void generatePawnMoves(const Position& pos, std::vector<Move>& moves)
		{
			if (pos.getSideToMove() == Color::WHITE)
			{
				generateWhitePawnPushes(pos, moves);
				generateWhitePawnCaptures(pos, moves);
				generateWhitePawnPromotions(pos, moves);
			}
			else
			{
				generateBlackPawnPushes(pos, moves);
				generateBlackPawnCaptures(pos, moves);
				generateBlackPawnPromotions(pos, moves);
			}
		}

		void generateWhitePawnPushes(const Position& pos, std::vector<Move>& moves)
		{
			// Advance 1 square
			BitBoard pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 8) & ~(pos.getOccupied() | Position::rankBoard(Rank::RANK_8));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i - 8);
					moves.push_back(move);
				}
			}

			// Advance 2 squares
			pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 16) & ~(pos.getOccupied() | pos.getOccupied() << 8) & Position::rankBoard(Rank::RANK_4);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i - 16);
					moves.push_back(move);
				}
			}
		}
		void generateWhitePawnCaptures(const Position& pos, std::vector<Move>& moves)
		{
			// Capture right
			BitBoard pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 9) & pos.getBoard(Color::BLACK) & ~(Position::fileBoard(File::FILE_A) | Position::rankBoard(Rank::RANK_8));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i - 9);
					moves.push_back(move);
				}
			}

			// Capture left
			pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 7) & pos.getBoard(Color::BLACK) & ~(Position::fileBoard(File::FILE_H) | Position::rankBoard(Rank::RANK_8));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i - 7);
					moves.push_back(move);
				}
			}

			// En Passant
			if (pos.getEPSquare() != Square::SQUARE_NULL)
			{
				// En Passant right
				pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 9) & (1ULL << numOf(pos.getEPSquare())) & ~Position::fileBoard(File::FILE_A);
				for (int i = 0; i < sizeof(BitBoard) * 8; i++)
				{
					if (((pawnMoves >> i) & 1) == 1)
					{
						Move move = (i << 6) + (i - 9) + (0b01 << 14);
						moves.push_back(move);
					}
				}

				// En Passant left
				pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 7) & (1ULL << numOf(pos.getEPSquare())) & ~Position::fileBoard(File::FILE_H);
				for (int i = 0; i < sizeof(BitBoard) * 8; i++)
				{
					if (((pawnMoves >> i) & 1) == 1)
					{
						Move move = (i << 6) + (i - 7) + (0b01 << 14);
						moves.push_back(move);
					}
				}
			}
		}
		void generateWhitePawnPromotions(const Position& pos, std::vector<Move>& moves)
		{
			// Advance 1 square
			BitBoard pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 8) & ~pos.getOccupied() & Position::rankBoard(Rank::RANK_8);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i - 8) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}

			// Capture right
			pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 9) & pos.getBoard(Color::BLACK) & ~Position::fileBoard(File::FILE_A) & Position::rankBoard(Rank::RANK_8);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i - 9) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}

			// Capture left
			pawnMoves = (pos.getBoard(Piece::WHITE_PAWN) << 7) & pos.getBoard(Color::BLACK) & ~Position::fileBoard(File::FILE_H) & Position::rankBoard(Rank::RANK_8);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i - 7) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}
		}

		void generateBlackPawnPushes(const Position& pos, std::vector<Move>& moves)
		{
			// Advance 1 square
			BitBoard pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 8) & ~(pos.getOccupied() | Position::rankBoard(Rank::RANK_1));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i + 8);
					moves.push_back(move);
				}
			}

			// Advance 2 squares
			pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 16) & ~(pos.getOccupied() | pos.getOccupied() >> 8) & Position::rankBoard(Rank::RANK_5);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i + 16);
					moves.push_back(move);
				}
			}
		}
		void generateBlackPawnCaptures(const Position& pos, std::vector<Move>& moves)
		{
			// Capture right
			BitBoard pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 9) & pos.getBoard(Color::WHITE) & ~(pos.fileBoard(File::FILE_H) | pos.rankBoard(Rank::RANK_1));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i + 9);
					moves.push_back(move);
				}
			}

			// Capture left
			pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 7) & pos.getBoard(Color::WHITE) & ~(pos.fileBoard(File::FILE_A) | pos.rankBoard(Rank::RANK_1));
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					Move move = (i << 6) + (i + 7);
					moves.push_back(move);
				}
			}

			// En Passant
			if (pos.getEPSquare() != Square::SQUARE_NULL)
			{
				//En passant right
				pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 9) & (1ULL << numOf(pos.getEPSquare())) & ~Position::fileBoard(File::FILE_H);
				for (int i = 0; i < sizeof(BitBoard) * 8; i++)
				{
					if (((pawnMoves >> i) & 1) == 1)
					{
						Move move = (i << 6) + (i + 9) + (0b0100 << 12);
						moves.push_back(move);
					}
				}

				//En passant left
				pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 7) & (1ULL << numOf(pos.getEPSquare())) & ~Position::fileBoard(File::FILE_A);
				for (int i = 0; i < sizeof(BitBoard) * 8; i++)
				{
					if (((pawnMoves >> i) & 1) == 1)
					{
						Move move = (i << 6) + (i + 7) + (0b0100 << 12);
						moves.push_back(move);
					}
				}
			}
		}
		void generateBlackPawnPromotions(const Position& pos, std::vector<Move>& moves)
		{
			// Promotion by advancing 1 square
			BitBoard pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 8) & ~pos.getOccupied() & Position::rankBoard(Rank::RANK_1);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i + 8) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}

			// Promotion by capturing right
			pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 9) & pos.getBoard(Color::WHITE) & ~pos.fileBoard(File::FILE_H) & pos.rankBoard(Rank::RANK_1);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i + 9) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}

			// Promotion by capturing left
			pawnMoves = (pos.getBoard(Piece::BLACK_PAWN) >> 7) & pos.getBoard(Color::WHITE) & ~pos.fileBoard(File::FILE_A) & pos.rankBoard(Rank::RANK_1);
			for (int i = 0; i < sizeof(BitBoard) * 8; i++)
			{
				if (((pawnMoves >> i) & 1) == 1)
				{
					for (int j = 0; j < 4; j++)
					{
						Move move = (i << 6) + (i + 7) + (((3 << 2) + j) << 12);
						moves.push_back(move);
					}
				}
			}
		}
	}
}