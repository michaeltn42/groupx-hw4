#include "bowling_frame.h"
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

Bowling_Frame::Bowling_Frame(int fnum)
{
	score1_ = 0;
	score2_ = 0;
	score3_ = 0;
	strike_ = false;
	spare_ = false;
	roll_one_ = false;
	roll_two_ = false;
	roll_three_ = false;
	frame_num_ = fnum;
	frame_score_ = 0;

	next_frame_ = NULL;
	prev_frame_ = NULL;

}

Bowling_Frame::~Bowling_Frame()
{
}

Bowling_Frame* Bowling_Frame::next_frame() { return next_frame_; }
void Bowling_Frame::next_frame( Bowling_Frame * new_frame ) { next_frame_ = new_frame; }

Bowling_Frame* Bowling_Frame::prev_frame() { return prev_frame_; }
void Bowling_Frame::prev_frame( Bowling_Frame * new_frame ) { prev_frame_ = new_frame; }

int Bowling_Frame::score1()				{ return score1_; }
void Bowling_Frame::score1( int score )	{ score1_ = score; roll_one_ = true; strike_ = (score == 10); }

int Bowling_Frame::score2()				{ return score2_; }
void Bowling_Frame::score2( int score )	{ score2_ = score; roll_two_ = true; spare_ = (!strike_ && score1_ + score2_ == 10); }

int Bowling_Frame::score3()				{ return score3_; }
void Bowling_Frame::score3( int score )	{ score3_ = score; roll_three_ = true; }

bool Bowling_Frame::strike() 			{ return strike_; }
void Bowling_Frame::strike(bool value )	{ strike_ = value; }

bool Bowling_Frame::spare() 			{ return spare_; }
void Bowling_Frame::spare(bool value )	{ spare_ = value; }

int Bowling_Frame::frame_num()
{
	return frame_num_;
}

int Bowling_Frame::frame_score()
{
	return frame_score_; 
}

void Bowling_Frame::update_scores()
{
	std::uniform_int_distribution<std::mt19937::result_type> dist10(0, 10);

	if (!roll_one_)
	{
		score1_ = dist10(rng);
		roll_one_ = true;

		strike_ = (score1_ == 10);
	}
	else if (frame_num_ == 10)
	{
		if (!roll_two_)
		{
			if (score1_ == 10)
			{
				score2_ = dist10(rng);
			}
			else
			{
				std::uniform_int_distribution<std::mt19937::result_type> distRemaining(0, 10 - score1_);
				score2_ = distRemaining(rng);
			}
			roll_two_ = true;
			spare_ = (!strike_ && score1_ + score2_ == 10);
		}
		else if (!roll_three_ && (strike_ || spare_))
		{
			const int pins_remaining = strike_ && score2_ < 10 ? 10 - score2_ : 10;
			std::uniform_int_distribution<std::mt19937::result_type> distRemaining(0, pins_remaining);
			score3_ = distRemaining(rng);
			roll_three_ = true;
		}
	}
	else if (!strike_ && !roll_two_)
	{
		std::uniform_int_distribution<std::mt19937::result_type> distRemaining(0, 10 - score1_);
		score2_ = distRemaining(rng);
		roll_two_ = true;
		spare_ = (score1_ + score2_ == 10);
	}

	Bowling_Frame * head = this;
	while (head->prev_frame_)
		head = head->prev_frame_;

	Bowling_Frame * frame = head;
	while (frame)
	{
		if (frame->frame_num_ == 10)
		{
			frame->frame_score_ = frame->score1_ + frame->score2_ + frame->score3_;
		}
		else if (frame->strike_)
		{
			Bowling_Frame * next = frame->next_frame_;
			int bonus = 0;
			if (next)
			{
				bonus += next->score1_;
				if (next->strike_)
				{
					Bowling_Frame * afterNext = next->next_frame_;
					if (afterNext)
						bonus += afterNext->score1_;
					else
						bonus += next->score2_;
				}
				else
				{
					bonus += next->score2_;
				}
			}
			frame->frame_score_ = 10 + bonus;
		}
		else if (frame->spare_)
		{
			frame->frame_score_ = 10;
			if (frame->next_frame_)
				frame->frame_score_ += frame->next_frame_->score1_;
		}
		else
		{
			frame->frame_score_ = frame->score1_ + frame->score2_;
		}
		frame = frame->next_frame_;
	}
}


