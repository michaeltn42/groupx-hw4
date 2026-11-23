#include <iostream>
#include "bowling_frame.h"
#include <cstdlib>
#include <cerrno>
#include <limits>

using namespace std;

int current_game_total(Bowling_Frame * first_frame)
{
	Bowling_Frame * temp = first_frame;
	int game_total = 0;
	while(temp)
	{
		game_total += temp->frame_score();
		temp = temp->next_frame();
	}
	return game_total;	
}

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cout << "Error: Please pass exactly one command-line argument for the number of players." << endl;
		return 1;
	}

	char * end = NULL;
	errno = 0;
	long player_count = strtol(argv[1], &end, 10);
	if (errno != 0 || end == argv[1] || *end != '\0' || player_count <= 0 ||
		player_count > numeric_limits<int>::max())
	{
		cout << "Error: The number of players must be a positive integer." << endl;
		return 1;
	}

	Bowling_Frame ** players = new Bowling_Frame*[player_count];

	for (int player = 0; player < player_count; ++player)
	{
		players[player] = NULL;
		Bowling_Frame * previous = NULL;

		for (int frame_number = 1; frame_number <= 10; ++frame_number)
		{
			Bowling_Frame * frame = new Bowling_Frame(frame_number);
			if (frame_number == 1)
				players[player] = frame;
			if (previous)
			{
				previous->next_frame(frame);
				frame->prev_frame(previous);
			}
			previous = frame;
		}

		Bowling_Frame * frame = players[player];
		while (frame)
		{
			frame->update_scores();
			frame->update_scores();
			if (frame->frame_num() == 10 && (frame->strike() || frame->spare()))
				frame->update_scores();

			frame = frame->next_frame();
		}

		cout << "Player " << player + 1 << endl;
		frame = players[player];
		while (frame)
		{
			cout << "  Frame " << frame->frame_num()
			     << ": Roll 1 = " << frame->score1()
			     << ", Roll 2 = " << frame->score2();
			if (frame->frame_num() == 10)
				cout << ", Roll 3 = " << frame->score3();
			cout << ", Frame Score = " << frame->frame_score() << endl;
			frame = frame->next_frame();
		}
		cout << "  Game Total = " << current_game_total(players[player]) << endl;
	}

	for (int player = 0; player < player_count; ++player)
	{
		Bowling_Frame * frame = players[player];
		while (frame)
		{
			Bowling_Frame * next = frame->next_frame();
			delete frame;
			frame = next;
		}
	}
	delete[] players;

	return 0;
}
