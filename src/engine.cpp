#include <iostream>
#include <string>
#include <sstream>

#include "engine.h"
#include "position.h"
#include "types.h"
#include "perft.h"

#include "pawns.h"
#include "sliders.h"
#include "knights.h"
#include "kings.h"

namespace Epimedes
{
	const char* startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	const char* kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -";
	const char* legaltesting = "1k2r3/6q1/3K4/8/8/8/8/8 w - - 0 1";

	void Engine::position(Position& pos, std::istream& istream)
	{
		std::string token, fen;
		istream >> token;
		if (token == "print")
		{
			std::cout << pos << std::endl;
		}
		else if (token == "startpos" || token == "startingpos")
		{
			fen = startpos; 
			pos.set(fen);
		}
		else if (token == "kiwipete")
		{
			fen = kiwipete;
			pos.set(fen);
		}
		else if (token == "legaltesting" || token == "legaltest")
		{
			fen = legaltesting;
			pos.set(fen);
		}
		else if (token == "fen")
		{
			while (istream >> token)
			{
				fen += token + " ";
			}
			pos.set(fen);
		}
	}

	void Engine::genMoves(Position& pos, std::istream& istream)
	{
		std::string token;
		istream >> token;

		std::vector<Move> moves;
		if (token == "moves" || token == "all") 
		{
			Pawns::generatePawnMoves(pos, moves);
			Sliders::generateSliderMoves(pos, moves);
			Knights::generateKnightMoves(pos, moves);
			Kings::generateKingMoves(pos, moves);
		}
		else if (token == "pawns" || token == "pawnmoves")
		{
			Pawns::generatePawnMoves(pos, moves);
		}
		else if (token == "sliders" || token == "slidermoves")
		{
			Sliders::generateSliderMoves(pos, moves);
		}
		else if (token == "knights" || token == "knightmoves")
		{
			Knights::generateKnightMoves(pos, moves);
		}
		else if (token == "kings" || token == "kingmoves")
		{
			Kings::generateKingMoves(pos, moves);
		}

		std::cout << "+----------------------+\n";
		std::cout << "|    Possible Moves    |\n";
		std::cout << "+----------------------+";
		for (int i = 0; i < moves.size(); i++)
		{
			if (i % 2 == 0) std::cout << "\n";

			std::cout << i + 1 << ". " << moveToAlgebraic(moves.at(i)) << " ";
		}
		std::cout << "\n+----------------------+\n";
	}

	void Engine::makeMove(Position& pos, std::istream& istream)
	{
		std::string token;
		istream >> token;
		Move inputMove = algebraicToMove(token);

		std::vector<Move> moves;
		Pawns::generatePawnMoves(pos, moves);
		Sliders::generateSliderMoves(pos, moves);
		Knights::generateKnightMoves(pos, moves);
		Kings::generateKingMoves(pos, moves);

		for (const Move& move : moves)
		{
			if (move == inputMove)
			{
				pos.makeMove(move);
				pos.updateBoard();
			}
		}
		std::cout << "\033c" << pos << "\n";
	}

	void Engine::perft(Position position, std::istream& istream)
	{
		char token;
		istream >> token;

		int depth = token - '0';

		Perft::perftDivide(position, depth); 
	}

	void Engine::loop()
	{
		Position pos;
		std::string token, cmd;
		while (true)
		{
			std::getline(std::cin, cmd);

			std::istringstream istream(cmd);

			istream >> std::skipws >> token;

			if (token == "quit" || token == "stop")
			{
				break;
			}
			else if (token == "position") position(pos, istream);
			else if (token == "gen") genMoves(pos, istream);
			else if (token == "move") makeMove(pos, istream);
			else if (token == "perft") perft(pos, istream);
			else if (token == "clear") std::cout << "\033c";
		}
	} //namespace Engine
} //namespace Epimedes